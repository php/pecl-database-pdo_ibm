/*
  +----------------------------------------------------------------------+
  | (C) Copyright IBM Corporation 2006.                                  |
  +----------------------------------------------------------------------+
  |                                                                      |
  | Licensed under the Apache License, Version 2.0 (the "License"); you  |
  | may not use this file except in compliance with the License. You may |
  | obtain a copy of the License at                                      |
  | http://www.apache.org/licenses/LICENSE-2.0                           |
  |                                                                      |
  | Unless required by applicable law or agreed to in writing, software  |
  | distributed under the License is distributed on an "AS IS" BASIS,    |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      |
  | implied. See the License for the specific language governing         |
  | permissions and limitations under the License.                       |
  +----------------------------------------------------------------------+
  | Authors: Rick McGuire, Dan Scott, Krishna Raman, Kellen Bombardier,  |
  | Ambrish Bhargava                                                     |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "pdo/php_pdo.h"
#include "pdo/php_pdo_driver.h"
#include "php_pdo_ibm.h"
#include "php_pdo_ibm_int.h"
#include "zend_exceptions.h"
#include <stdio.h>

extern struct pdo_stmt_methods ibm_stmt_methods;
extern int ibm_stmt_dtor(pdo_stmt_t *stmt TSRMLS_DC);


/* allocate and initialize the driver_data portion of a PDOStatement object. */
static int dbh_new_stmt_data(pdo_dbh_t* dbh, pdo_stmt_t *stmt TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;

	stmt_handle *stmt_res = (stmt_handle *) emalloc(sizeof(stmt_handle));
	check_allocation(stmt_res, "dbh_new_stmt_data", "Unable to allocate stmt driver data");
	memset(stmt_res, '\0', sizeof(stmt_handle));

	stmt_res->columns = NULL;

	/* attach to the statement */
	stmt->driver_data = stmt_res;
	stmt->supports_placeholders = PDO_PLACEHOLDER_POSITIONAL;
	return TRUE;
}

/* prepare a statement for execution. */
static int dbh_prepare_stmt(pdo_dbh_t *dbh, pdo_stmt_t *stmt, const char *stmt_string, long stmt_len, zval *driver_options TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;
	stmt_handle *stmt_res = (stmt_handle *) stmt->driver_data;
	int rc;
	SQLSMALLINT param_count;
#ifndef PASE
	UCHAR server_info[30];
#else
	unsigned char server_info[30];
#endif
	SQLSMALLINT server_len = 0;

	/* in case we need to convert the statement for positional syntax */
	int converted_len = 0;
	stmt_res->converted_statement = NULL;

	/* clear the current error information to get ready for new execute */
	clear_stmt_error(stmt);

	/*
	 * the statement passed in to us at this point is the raw statement the
	 *  programmer specified.  If the statement is using named parameters
	 * (e.g., ":salary", we can't process this directly.  Fortunately, PDO
	 * has a utility function that will munge the SQL statement into the
	 * form we require and do mappings from named to positional parameters.
	 */

	/* this is necessary...it tells the parser what we require */
	stmt->supports_placeholders = PDO_PLACEHOLDER_POSITIONAL;
	rc = pdo_parse_params(stmt, (char *) stmt_string, stmt_len,
			&stmt_res->converted_statement,
			&converted_len TSRMLS_CC);

	/*
	 * If the query needed reformatting, a new statement string has been
	 * passed back to us.  We're responsible for freeing this when we're done.
	 */
	if (rc == 1) {
		stmt_string = stmt_res->converted_statement;
		stmt_len = converted_len;
	}
	/*
	 * A negative return indicates there was an error.  The error_code
	 * information in the statement contains the reason.
	 */
	else if (rc == -1) {
		/* copy the error information */
		RAISE_IBM_STMT_ERROR(stmt->error_code, "pdo_parse_params", 
			"Invalid SQL statement");
		/* this failed...error cleanup will happen later. */
		return FALSE;
	}

	/* alloc handle and return only if it errors */
	rc = SQLAllocHandle(SQL_HANDLE_STMT, conn_res->hdbc, &(stmt_res->hstmt));
	check_stmt_error(rc, "SQLAllocHandle");

	/* now see if the cursor type has been explicitly specified. */
	stmt_res->cursor_type = pdo_attr_lval(driver_options, PDO_ATTR_CURSOR, 
			PDO_CURSOR_FWDONLY TSRMLS_CC);

	/*
	 * The default is just sequential access.  If something else has been
	 * specified, we need to make this scrollable.
	 */
	if (stmt_res->cursor_type != PDO_CURSOR_FWDONLY) {
		/* set the statement attribute */
#ifdef PASE /* i5 ptr to int (not int) */
		SQLINTEGER vParam = SQL_CURSOR_DYNAMIC;
		rc = SQLSetStmtAttr(stmt_res->hstmt, SQL_ATTR_CURSOR_TYPE, (void *) &vParam, 0);
#else /* not PASE */
		rc = SQLSetStmtAttr(stmt_res->hstmt, SQL_ATTR_CURSOR_TYPE, (void *) SQL_CURSOR_DYNAMIC, 0);
#endif /* not PASE */
		check_stmt_error(rc, "SQLSetStmtAttr");
	}


	/* Prepare the stmt. */
	rc = SQLPrepare((SQLHSTMT) stmt_res->hstmt, (SQLCHAR *) stmt_string, stmt_len);

	/* Check for errors from that prepare */
	check_stmt_error(rc, "SQLPrepare");
	if (rc == SQL_ERROR) {
		stmt_cleanup(stmt TSRMLS_CC);
		return FALSE;
	}

	/* we can get rid of the stmt copy now */
	if (stmt_res->converted_statement != NULL) {
		efree(stmt_res->converted_statement);
		stmt_res->converted_statement = NULL;
	}

	rc = SQLNumResultCols((SQLHSTMT) stmt_res->hstmt, (SQLSMALLINT *) & param_count);
	check_stmt_error(rc, "SQLNumResultCols");

	/* we're responsible for setting the column_count for the PDO driver. */
	stmt->column_count = param_count;

	memset(server_info, 0, sizeof(server_info));
	rc = SQLGetInfo(conn_res->hdbc, SQL_DBMS_VER, (SQLPOINTER)server_info,
			sizeof(server_info), &server_len);
#ifndef PASE /* i5/os release dependent check */
	/* Get the server information:
	 * server_info is in this form:
	 * 0r.01.0000
	 * where r is the major version
	 */
	rc = SQLGetInfo(conn_res->hdbc, SQL_DBMS_VER, &server_info,
			sizeof(server_info), &server_len);
	/* making char numbers into integers eg. "10" --> 10 or "09" --> 9 */
	stmt_res->server_ver = ((server_info[0] - '0')*100) + ((server_info[1] - '0')*10) + (server_info[3] - '0');
#else
	/* PASE 
	 * Get the server information (05040, 06010):
	 * server_info is in this form:
	 * 0r0m0
	 * 01234
	 * where r is the major version
	 * where m is the minor version
	 */
	stmt_res->server_ver =
	   ((server_info[1] - '0')*100)
	   + ((server_info[3] -'0')*10)
	   + (server_info[4] - '0');
#endif /* PASE */
	/*
	 * Attach the methods...we are now live, so errors will no longer immediately
	 * force cleanup of the stmt driver-specific storage.
	 */
	stmt->methods = &ibm_stmt_methods;

	return TRUE;
}

/* debugging routine for printing out failure information. */
static void current_error_state(pdo_dbh_t *dbh)
{
	conn_handle *conn_res = (conn_handle *)dbh->driver_data;
	printf("Handling error %s (%s[%d] at %s:%d)\n",
		conn_res->error_data.err_msg,		/* an associated message */
		conn_res->error_data.failure_name,	/* the routine name */
		conn_res->error_data.sqlcode,		/* native error code of the failure */
		conn_res->error_data.filename,		/* source file of the reported error */
		conn_res->error_data.lineno);		/* location of the reported error */
}

/*
*  NB.  The handle closer is used for PDO dtor purposes, but we also use this
*  for error cleanup if we need to throw an exception while creating the
*  connection.  In that case, the closer is not automatically called by PDO,
*  so we need to force cleanup.
*/
static int ibm_handle_closer( pdo_dbh_t * dbh TSRMLS_DC)
{
	conn_handle *conn_res;

	conn_res = (conn_handle *) dbh->driver_data;

	/*
	 * An error can occur at many stages of setup, so we need to check the
	 * validity of each bit as we unwind.
	 */
	if (conn_res != NULL) {
		/* did we get at least as far as creating the environment? */
		if (conn_res->henv != SQL_NULL_HANDLE) {
			/*
			* If we have a handle for the connection, we have
			* more stuff to clean up
			*/
			if (conn_res->hdbc != SQL_NULL_HANDLE) {
				/*
				* Roll back the transaction if this hasn't been committed yet.
				* There's no point in checking for errors here...
				* PDO won't process any of the failures even if they happen.
				*/
				if (dbh->auto_commit == 0) {
					SQLEndTran(SQL_HANDLE_DBC, (SQLHDBC) conn_res->hdbc, 
							SQL_ROLLBACK);
				}
				SQLDisconnect((SQLHDBC) conn_res->hdbc);
				SQLFreeHandle(SQL_HANDLE_DBC, conn_res->hdbc);
			}
			/* and finally the handle */
			SQLFreeHandle(SQL_HANDLE_ENV, conn_res->henv);
		}
		/* now free the driver data */
		pefree(conn_res, dbh->is_persistent);
		dbh->driver_data = NULL;
	}
	return TRUE;
}

/* prepare a statement for execution. */
static int ibm_handle_preparer(
	pdo_dbh_t *dbh,
	const char *sql,
	long sql_len,
	pdo_stmt_t *stmt,
	zval *driver_options
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *)dbh->driver_data;

	/* allocate new driver_data structure */
	if (dbh_new_stmt_data(dbh, stmt TSRMLS_CC) == TRUE) {
		/* Allocates the stmt handle */
		/* Prepares the statement */
		/* returns the stat_handle back to the calling function */
		return dbh_prepare_stmt(dbh, stmt, sql, sql_len, driver_options TSRMLS_CC);
	}
	return FALSE;
}

/* directly execute an SQL statement. */
static long ibm_handle_doer(
	pdo_dbh_t *dbh,
	const char *sql,
	long sql_len
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;
	SQLHANDLE hstmt;
	SQLINTEGER rowCount;
	/* get a statement handle */
	int rc = SQLAllocHandle(SQL_HANDLE_STMT, conn_res->hdbc, &hstmt);
	check_dbh_error(rc, "SQLAllocHandle");

	rc = SQLExecDirect(hstmt, (SQLCHAR *) sql, sql_len);
	if (rc == SQL_ERROR) {
		/*
		* NB...we raise the error before freeing the handle so that
		* we catch the proper error record.
		*/
		raise_sql_error(dbh, NULL, hstmt, SQL_HANDLE_STMT,
			"SQLExecDirect", __FILE__, __LINE__ TSRMLS_CC);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

		/*
		* Things are a bit overloaded here...we're supposed to return a count
		* of the affected rows, but -1 indicates an error occurred.
		*/
		return -1;
	}

	/*
	* Check if SQL_NO_DATA_FOUND was returned:
	* SQL_NO_DATA_FOUND is returned if the SQL statement is a Searched UPDATE
	* or Searched DELETE and no rows satisfy the search condition.
	*/
	if (rc == SQL_NO_DATA) {
		rowCount = 0;
	} else {
		/* we need to update the number of affected rows. */
		rc = SQLRowCount(hstmt, &rowCount);
		if (rc == SQL_ERROR) {
			/*
			* NB...we raise the error before freeing the handle so that
			* we catch the proper error record.
			*/
			raise_sql_error(dbh, NULL, hstmt, SQL_HANDLE_STMT,
				"SQLRowCount", __FILE__, __LINE__ TSRMLS_CC);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			return -1;
		}
		/*
		* -1 will be retuned if the following:
		* If the last executed statement referenced by the input statement handle
		* was not an UPDATE, INSERT, DELETE, or MERGE statement, or if it did not
		* execute successfully, then the function sets the contents of RowCountPtr to -1.
		*/
		if (rowCount == -1) {
			rowCount = 0;
		}
	}

	/* Set the last inserted id */
	rc = record_last_insert_id( NULL, dbh, hstmt TSRMLS_CC);
	if( rc == FALSE ) {
		return -1;
	}
	/* this is a one-shot deal, so make sure we free the statement handle */
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	return rowCount;
}

/* start a new transaction */
static int ibm_handle_begin( pdo_dbh_t *dbh TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;
#ifndef PASE
	int rc = SQLSetConnectAttr(conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
			(SQLPOINTER) SQL_AUTOCOMMIT_OFF, SQL_NTS);
#else
	SQLINTEGER autocommit = SQL_AUTOCOMMIT_OFF;
	int rc = SQLSetConnectAttr(conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
			(SQLPOINTER) &autocommit, SQL_NTS);
#endif
	check_dbh_error(rc, "SQLSetConnectAttr");
	return TRUE;
}

static int ibm_handle_commit(
	pdo_dbh_t *dbh
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *)dbh->driver_data;

	int rc = SQLEndTran(SQL_HANDLE_DBC, conn_res->hdbc, SQL_COMMIT);
	check_dbh_error(rc, "SQLEndTran");
	if (dbh->auto_commit != 0) {
#ifndef PASE
		rc = SQLSetConnectAttr(conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_NTS);
#else
		SQLINTEGER autocommit = SQL_AUTOCOMMIT_ON;
		rc = SQLSetConnectAttr(conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER) &autocommit, SQL_NTS);
#endif
		check_dbh_error(rc, "SQLSetConnectAttr");
	}
	return TRUE;
}

static int ibm_handle_rollback(
	pdo_dbh_t *dbh
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *)dbh->driver_data;

	int rc = SQLEndTran(SQL_HANDLE_DBC, conn_res->hdbc, SQL_ROLLBACK);
	check_dbh_error(rc, "SQLEndTran");
	if (dbh->auto_commit != 0) {
#ifndef PASE
		rc = SQLSetConnectAttr(conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_NTS);
#else
		SQLINTEGER autocommit = SQL_AUTOCOMMIT_ON;
		rc = SQLSetConnectAttr(conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER) &autocommit, SQL_NTS);
#endif
		check_dbh_error(rc, "SQLSetConnectAttr");
	}
	return TRUE;
}

/* Set the driver attributes. We allow the setting of autocommit */
static int ibm_handle_set_attribute(
	pdo_dbh_t *dbh,
	long attr,
	zval *return_value
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *)dbh->driver_data;
	int rc = 0;

	switch (attr) {
		case PDO_ATTR_AUTOCOMMIT:
			if (dbh->auto_commit != Z_LVAL_P(return_value)) {
				dbh->auto_commit = Z_LVAL_P(return_value);
				if (dbh->auto_commit == TRUE) {
#ifndef PASE
					rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
						(SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_NTS);
#else
					SQLINTEGER autocommit = SQL_AUTOCOMMIT_ON;
					rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
						(SQLPOINTER) &autocommit, SQL_NTS);
#endif
					check_dbh_error(rc, "SQLSetConnectAttr");
				} else {
#ifndef PASE
					rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
						(SQLPOINTER) SQL_AUTOCOMMIT_OFF, SQL_NTS);
#else
					SQLINTEGER autocommit = SQL_AUTOCOMMIT_OFF;
					rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
						(SQLPOINTER) &autocommit, SQL_NTS);
#endif
					check_dbh_error(rc, "SQLSetConnectAttr");
				}
			}
			return TRUE;
			break;
#ifndef PASE /* i5/OS no support trusted */
		case PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID:
			rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_TRUSTED_CONTEXT_USERID,
#if PHP_MAJOR_VERSION >=7
				(SQLPOINTER) Z_STRVAL_P(return_value), 
#else
				(SQLPOINTER) Z_STRVAL_PP(&return_value),
#endif
                                SQL_NTS);  
			check_dbh_error(rc, "SQLSetConnectAttr");
			return TRUE;
			break;

		case PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD:
			rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_TRUSTED_CONTEXT_PASSWORD,
#if PHP_MAJOR_VERSION >=7
				(SQLPOINTER) Z_STRVAL_P(return_value), 
#else
				(SQLPOINTER) Z_STRVAL_PP(&return_value),
#endif
                                SQL_NTS);  
			check_dbh_error(rc, "SQLSetConnectAttr");
			return TRUE;
			break;
#endif

		/* Set Client Info */
		case PDO_SQL_ATTR_INFO_USERID:
			rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_USERID,
#if PHP_MAJOR_VERSION >=7
				(SQLPOINTER) Z_STRVAL_P(return_value), 
#else
				(SQLPOINTER) Z_STRVAL_PP(&return_value),
#endif
                                SQL_NTS);  
			check_dbh_error(rc, "SQLSetConnectAttr");
			return TRUE;
			break;

		case PDO_SQL_ATTR_INFO_ACCTSTR:
			rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_ACCTSTR,
#if PHP_MAJOR_VERSION >=7
				(SQLPOINTER) Z_STRVAL_P(return_value), 
#else
				(SQLPOINTER) Z_STRVAL_PP(&return_value),
#endif
                                SQL_NTS);  
			check_dbh_error(rc, "SQLSetConnectAttr");
			return TRUE;
			break;
			
		case PDO_SQL_ATTR_INFO_APPLNAME:
			rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_APPLNAME,
#if PHP_MAJOR_VERSION >=7
				(SQLPOINTER) Z_STRVAL_P(return_value), 
#else
				(SQLPOINTER) Z_STRVAL_PP(&return_value),
#endif
                                SQL_NTS);  
			check_dbh_error(rc, "SQLSetConnectAttr");
			return TRUE;
			break;

		case PDO_SQL_ATTR_INFO_WRKSTNNAME:
			rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_WRKSTNNAME,
#if PHP_MAJOR_VERSION >=7
				(SQLPOINTER) Z_STRVAL_P(return_value), 
#else
				(SQLPOINTER) Z_STRVAL_PP(&return_value),
#endif
                                SQL_NTS);  
			check_dbh_error(rc, "SQLSetConnectAttr");
			return TRUE;
			break;
		default:
			return FALSE;
	}
}

/* fetch the last inserted id */
static char *ibm_handle_lastInsertID(pdo_dbh_t * dbh, const char *name, unsigned int *len TSRMLS_DC)
{
	char *last_id = emalloc( MAX_IDENTITY_DIGITS );
	int rc = 0;
	char *sql;
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;
	SQLHANDLE hstmt;
	SQLUINTEGER out_length;
	char server[MAX_DBMS_IDENTIFIER_NAME];

#ifndef PASE /* i5 IDENTITY_VAL_LOCAL is correct */
	rc = SQLGetInfo(conn_res->hdbc, SQL_DBMS_NAME, (SQLPOINTER)server, MAX_DBMS_IDENTIFIER_NAME, NULL);
	check_dbh_error(rc, "SQLGetInfo");
	
	if( strncmp( server, "DB2", 3 ) == 0 )
	{
#endif /* PASE */
		/* get a new statement handle */
		strcpy( last_id, "0" );
		rc = SQLAllocHandle(SQL_HANDLE_STMT, conn_res->hdbc, &hstmt);
		check_dbh_error(rc, "SQLAllocHandle");
		sql = "SELECT IDENTITY_VAL_LOCAL() FROM SYSIBM.SYSDUMMY1";
		rc = SQLExecDirect(hstmt, (SQLCHAR *) sql, strlen(sql));
		if (rc == SQL_ERROR) {
			/*
			* We raise the error before freeing the handle so that
			* we catch the proper error record.
			*/
			raise_sql_error(dbh, NULL, hstmt, SQL_HANDLE_STMT,
				"SQLExecDirect", __FILE__, __LINE__ TSRMLS_CC);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

			return FALSE;
		}

		rc = SQLBindCol(hstmt, 1, SQL_C_CHAR, last_id, MAX_IDENTITY_DIGITS, &out_length);
		if (rc == SQL_ERROR) {
			/*
			* We raise the error before freeing the handle so that
			* we catch the proper error record.
			*/
			raise_sql_error(dbh, NULL, hstmt, SQL_HANDLE_STMT,
				"SQLBindCol", __FILE__, __LINE__ TSRMLS_CC);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

			return FALSE;
		}
		/* go fetch it. */
		rc = SQLFetch(hstmt);
		if (rc == SQL_ERROR) {
			/*
			* We raise the error before freeing the handle so that
			* we catch the proper error record.
			*/
			raise_sql_error(dbh, NULL, hstmt, SQL_HANDLE_STMT,
				"SQLFetch", __FILE__, __LINE__ TSRMLS_CC);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

			return FALSE;
		}
		/* this is a one-shot deal, so make sure we free the statement handle */
		*len = strlen(last_id);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return last_id;
#ifndef PASE /* i5 IDENTITY_VAL_LOCAL is correct */
	}
#endif

	sprintf(last_id, "%d", conn_res->last_insert_id);
	*len = strlen(last_id);
	return last_id;
}

/* fetch the supplemental error material */
static int ibm_handle_fetch_error(
	pdo_dbh_t *dbh,
	pdo_stmt_t *stmt,
	zval *info
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *)dbh->driver_data;
	char suppliment[512];

	if(conn_res->error_data.failure_name == NULL && conn_res->error_data.filename == NULL) {
		conn_res->error_data.filename="(null)";
		conn_res->error_data.failure_name="(null)";
	}
	if(conn_res->error_data.isam_err_msg[0] != '\0') {
		sprintf(suppliment, "%s (%s[%d] at %s:%d) ISAM: %s", 
			conn_res->error_data.err_msg,		/*  an associated message */
			conn_res->error_data.failure_name,	/*  the routine name */
			conn_res->error_data.sqlcode,		/*  native error code of the failure */
			conn_res->error_data.filename,		/*  source file of the reported error */
			conn_res->error_data.lineno,		/*  location of the reported error */
			conn_res->error_data.isam_err_msg);	/*  ISAM Error message */
	} else {
		sprintf(suppliment, "%s (%s[%d] at %s:%d)", 
			conn_res->error_data.err_msg,		/*  an associated message */
			conn_res->error_data.failure_name,	/*  the routine name */
			conn_res->error_data.sqlcode,		/*  native error code of the failure */
			conn_res->error_data.filename,		/*  source file of the reported error */
			conn_res->error_data.lineno);		/*  location of the reported error */
	}
	/*
	 * Now add the error information.  These need to be added
	 * in a specific order
	 */
	add_next_index_long(info, conn_res->error_data.sqlcode);
#if PHP_MAJOR_VERSION >=7
        add_next_index_string(info, suppliment);
#else
	add_next_index_string(info, suppliment, 1);
#endif

	return TRUE;
}

/* quotes an SQL statement */
static int ibm_handle_quoter(
	pdo_dbh_t *dbh,
	const char *unq,
	int unq_len,
	char **q,
	int *q_len,
	enum pdo_param_type paramtype
	TSRMLS_DC)
{
	char *sql;
	int new_length, i, j;

        int len;
	if(!unq)  {
		return FALSE;
	}

	/* allocate twice the source length first (worst case) */
	sql = (char*)emalloc(((unq_len*2)+3)*sizeof(char));

	/* set the first quote */
	sql[0] = '\'';

	j = 1;
	for (i = 0; i < unq_len; i++) {
		switch (unq[i]) {
			case '\'':
				sql[j++] = '\'';
				sql[j++] = '\'';
				break;
			default:
				sql[j++] = unq[i];
				break;
		}
	}

	/* set the last quote and null terminating character */
	sql[j++] = '\'';
	sql[j++] = '\0';

	/* copy over final string and free the memory used */
	*q = (char*)emalloc(((unq_len*2)+3)*sizeof(char));
	strcpy(*q, sql);
	*q_len = strlen(sql);
	efree(sql);
	return TRUE;
}


/* Get the driver attributes. We return the autocommit and version information. */
static int ibm_handle_get_attribute(
	pdo_dbh_t *dbh,
	long attr,
	zval *return_value
	TSRMLS_DC)
{
	char value[MAX_DBMS_IDENTIFIER_NAME];
	int rc;
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;
	SQLINTEGER tc_flag;

	char info_user_id[USERID_LEN];
	char info_acctstr[ACCTSTR_LEN];
	char info_appl_name[APPLNAME_LEN];
	char info_wrkstn_name[WRKSTNNAME_LEN];
	int length;

#ifdef PASE /* i5/os release dependent check */
	unsigned char server_info[30];
	SQLSMALLINT server_len = 0;
#endif /* PASE */

	switch (attr) {
		case PDO_ATTR_CLIENT_VERSION:
#if PHP_MAJOR_VERSION >= 7
                        ZVAL_STRING(return_value, PDO_IBM_VERSION);
#else 
			ZVAL_STRING(return_value, PDO_IBM_VERSION, 1);
#endif
			return TRUE;

		case PDO_ATTR_AUTOCOMMIT:
			ZVAL_BOOL(return_value, dbh->auto_commit);
			return TRUE;

		case PDO_ATTR_SERVER_INFO:
			rc = SQLGetInfo(conn_res->hdbc, SQL_DBMS_NAME, 
					(SQLPOINTER)value, MAX_DBMS_IDENTIFIER_NAME, NULL);
			check_dbh_error(rc, "SQLGetInfo");
#if PHP_MAJOR_VERSION >= 7
                        ZVAL_STRING(return_value, value);
#else
			ZVAL_STRING(return_value, value, 1);
#endif
			return TRUE;

#ifdef PASE /* i5/os release dependent check */
		case PDO_ATTR_SERVER_VERSION:
			rc = SQLGetInfo(conn_res->hdbc, SQL_DBMS_VER,
					(SQLPOINTER)server_info, sizeof(server_info), &server_len);
			check_dbh_error(rc, "SQLGetInfo");
#if PHP_MAJOR_VERSION >= 7
			ZVAL_STRING(return_value, server_info);
#else
			ZVAL_STRING(return_value, server_info, 1);
#endif
			return TRUE;
#endif /* PASE */

#ifndef PASE /* i5/OS no support trusted */
		case PDO_SQL_ATTR_USE_TRUSTED_CONTEXT:
			rc = SQLGetConnectAttr(conn_res->hdbc, SQL_ATTR_USE_TRUSTED_CONTEXT, 
					(SQLPOINTER) &tc_flag, 0, NULL);
			check_dbh_error(rc, "SQLGetInfo");
			if(tc_flag == SQL_TRUE) {
				ZVAL_BOOL(return_value, tc_flag);
				return TRUE;
			}
			
		case PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID:
			rc = SQLGetConnectAttr(conn_res->hdbc, SQL_ATTR_TRUSTED_CONTEXT_USERID, 
					(SQLPOINTER)value, MAX_DBMS_IDENTIFIER_NAME, NULL);
			check_dbh_error(rc, "SQLGetInfo");
#if PHP_MAJOR_VERSION >= 7
			ZVAL_STRING(return_value, value);
#else
			ZVAL_STRING(return_value, value, 1);
#endif
			return TRUE;
#endif

		/* Get Client Info */
		case PDO_SQL_ATTR_INFO_USERID:
			rc = SQLGetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_USERID, 
					(SQLPOINTER) info_user_id, USERID_LEN, &length);
			check_dbh_error(rc, "SQLGetInfo");
			if(length < USERID_LEN) {
				info_user_id[length] = '\0';
			}
#if PHP_MAJOR_VERSION >= 7
			ZVAL_STRING(return_value, info_user_id);
#else
			ZVAL_STRING(return_value, info_user_id, 1);
#endif
			return TRUE;
			
		case PDO_SQL_ATTR_INFO_ACCTSTR:
			rc = SQLGetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_ACCTSTR, 
					(SQLPOINTER) info_acctstr, ACCTSTR_LEN, &length);
			check_dbh_error(rc, "SQLGetInfo");
			if(length < ACCTSTR_LEN) {
				info_acctstr[length] = '\0';
			}
#if PHP_MAJOR_VERSION >= 7
			ZVAL_STRING(return_value, info_acctstr);
#else
			ZVAL_STRING(return_value, info_acctstr, 1);
#endif
			return TRUE;
			
		case PDO_SQL_ATTR_INFO_APPLNAME:
			rc = SQLGetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_APPLNAME, 
					(SQLPOINTER) info_appl_name, APPLNAME_LEN, &length);
			check_dbh_error(rc, "SQLGetInfo");
			if(length < APPLNAME_LEN) {
				info_appl_name[length] = '\0';
			}
#if PHP_MAJOR_VERSION >= 7
			ZVAL_STRING(return_value, info_appl_name);
#else
			ZVAL_STRING(return_value, info_appl_name, 1);
#endif
			return TRUE;
			
		case PDO_SQL_ATTR_INFO_WRKSTNNAME:
			rc = SQLGetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_INFO_WRKSTNNAME, 
					(SQLPOINTER) info_wrkstn_name, WRKSTNNAME_LEN, &length);
			check_dbh_error(rc, "SQLGetInfo");
			if(length < WRKSTNNAME_LEN) {
				info_wrkstn_name[length] = '\0';
			}
#if PHP_MAJOR_VERSION >= 7
			ZVAL_STRING(return_value, info_wrkstn_name);
#else
			ZVAL_STRING(return_value, info_wrkstn_name, 1);
#endif
			return TRUE;

	}
	return FALSE;
}

#ifndef PASE
static int ibm_handle_check_liveness(
	pdo_dbh_t *dbh
	TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;

	SQLINTEGER dead_flag;
	SQLINTEGER length;

	int rc = SQLGetConnectAttr(conn_res->hdbc, SQL_ATTR_CONNECTION_DEAD,
			(SQLPOINTER) & dead_flag, sizeof(dead_flag), &length);
	/* this will qualify as a failed liveness check */
	if (rc == SQL_ERROR) {
		RAISE_DBH_ERROR("SQLGetConnectAttr");
		return FAILURE;
	}
	/* return the state from the query */
	return dead_flag == SQL_CD_FALSE ? SUCCESS : FAILURE;

}
#endif

static struct pdo_dbh_methods ibm_dbh_methods = {
	ibm_handle_closer,
	ibm_handle_preparer,
	ibm_handle_doer,
	ibm_handle_quoter,		
	ibm_handle_begin,
	ibm_handle_commit,
	ibm_handle_rollback,
	ibm_handle_set_attribute,
	ibm_handle_lastInsertID,
	ibm_handle_fetch_error,
	ibm_handle_get_attribute,
#ifndef PASE
	ibm_handle_check_liveness,/* check_liveness  */
#else
	NULL,				/* check_liveness  */
#endif
	NULL				/* get_driver_methods */
};

/* handle the business of creating a connection. */
static int dbh_connect(pdo_dbh_t *dbh, zval *driver_options TSRMLS_DC)
{
	int rc = 0;
	int dsn_length = 0;
	char *new_dsn = NULL;
	SQLSMALLINT d_length = 0, u_length = 0, p_length = 0;
	struct sqlca    sqlca;
	struct sqlca    *pSQLCA = &sqlca;
#ifdef PASE /* i5/OS incompatible v6 change */
	char buffer11[11];
	long attr = SQL_TRUE;
#endif /* PASE */
	/*
	* Allocate our driver data control block.  If this is a persistent
	* connection, we need to allocate this from persistent storage.
	*/
	conn_handle *conn_res = (conn_handle *) pemalloc(sizeof(conn_handle), dbh->is_persistent);
	check_allocation(conn_res, "dbh_connect", "Unable to allocate driver data");

	/* clear, and hook up to the PDO data structure. */
	memset((void *) conn_res, '\0', sizeof(conn_handle));
	dbh->driver_data = conn_res;

#ifndef PASE /* i5/OS difference */
	/* we need an environment to use for a base */
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &conn_res->henv);
	check_dbh_error(rc, "SQLAllocHandle");
#else /* PASE */
	{
		rc = SQLAllocEnv(&(conn_res->henv));
		if ( rc == SQL_ERROR ) {
			check_dbh_error(rc, "SQLAllocHandle");
		}
	}
#endif /* PASE */

#ifndef PASE
	/* and we're using the OBDC version 3 style interface */
	rc = SQLSetEnvAttr((SQLHENV)conn_res->henv, SQL_ATTR_ODBC_VERSION,
			(void *) SQL_OV_ODBC3, 0);
	check_dbh_error(rc, "SQLSetEnvAttr");
#else
	/* if (dbh->username != NULL) -- always server mode to avoid ini file setting */
	{
		attr = SQL_TRUE;
		SQLSetEnvAttr((SQLHENV)conn_res->henv, SQL_ATTR_SERVER_MODE, &attr, 0);
	}
#endif

	/* now an actual connection handle */
	rc = SQLAllocHandle(SQL_HANDLE_DBC, conn_res->henv, &(conn_res->hdbc));
	check_dbh_error(rc, "SQLAllocHandle");

	/* if we're in auto commit mode, set the connection attribute. */
#ifndef PASE
	if (dbh->auto_commit != 0) {
		rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_NTS);
		check_dbh_error(rc, "SQLSetConnectAttr");
	} else {
		rc = SQLSetConnectAttr((SQLHDBC) conn_res->hdbc, SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER) SQL_AUTOCOMMIT_OFF, SQL_NTS);
		check_dbh_error(rc, "SQLSetConnectAttr");
	}
#else
	{
		SQLINTEGER auto_commit;
		auto_commit = dbh->auto_commit != 0 ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF;
		rc = SQLSetConnectAttr((SQLHDBC)conn_res->hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)(&auto_commit), SQL_NTS);
	}
#endif

#ifndef PASE /* i5/OS no support trusted */
	/*
	* Checking if trusted context attribute is eabled or not.
	* Setting Trusted Context attribute before making connection, if enabled.
	*/

	if (driver_options != NULL) {
		int i = 0;
		ulong num_idx;
		char *opt_key;
#if PHP_MAJOR_VERSION >= 7
		zend_long option_num = 0;
		zval *data;
#else
		zval **data;
		long option_num = 0;
#endif
		char *option_str = NULL;

		int numOpts = zend_hash_num_elements(Z_ARRVAL_P(driver_options));
#if PHP_MAJOR_VERSION >= 7
                
                        ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(driver_options), num_idx, opt_key, data) {
                                if (opt_key) {
                                        continue;
                                }
#else
		zend_hash_internal_pointer_reset(Z_ARRVAL_P(driver_options));
		for ( i = 0; i < numOpts; i++) {
			zend_hash_get_current_key(Z_ARRVAL_P(driver_options), &opt_key, &num_idx, 1);
			zend_hash_get_current_data(Z_ARRVAL_P(driver_options), (void**)&data);
#endif			
		
#if PHP_MAJOR_VERSION >= 7
                        if (Z_TYPE_P(data) == IS_STRING) {
#else	
			if (Z_TYPE_PP(data) == IS_STRING) {
#endif
#if PHP_MAJOR_VERSION >= 7
               	     option_str = Z_STRVAL_P(data);
#else
	             option_str = Z_STRVAL_PP(data);
#endif
			} else {
#if PHP_MAJOR_VERSION >= 7
				option_num = Z_LVAL_P(data);
#else
				option_num = Z_LVAL_PP(data);
#endif
			}

			if(num_idx == PDO_SQL_ATTR_USE_TRUSTED_CONTEXT) {
				if (option_num == SQL_TRUE) {
					rc = SQLSetConnectAttr((SQLHDBC)conn_res->hdbc, SQL_ATTR_USE_TRUSTED_CONTEXT, SQL_TRUE, SQL_IS_INTEGER);
					check_dbh_error(rc, "SQLSetConnectAttr");
					break;
				}
			}
#if PHP_MAJOR_VERSION >=7
                } ZEND_HASH_FOREACH_END();
#else
			zend_hash_move_forward(Z_ARRVAL_P(driver_options));
			continue;
		}
#endif
	}
#endif
		
	/*
	* NB:  We don't have any specific driver options we support at this time, so
	* we don't need to do any option parsing. If the string contains a =, then
	* we need to use SQLDriverConnect to make the connection.  This may require
	* reform  var_dump($rows);atting the DSN string to include a userid and
	* password.
	*/
#ifndef PASE
	if (strchr(dbh->data_source, '=') != NULL) {
		/* first check to see if we have a user name */
		if (dbh->username != NULL && dbh->password != NULL) {
			/*
			* Ok, one was given...however, the DSN may already contain UID
			* information, so check first.
			*/
			if (strstr(dbh->data_source, ";uid=") == NULL
					&& strstr(dbh->data_source, ";UID=") == NULL) {
				/* Make sure each of the connection parameters is not NULL */
				d_length = strlen(dbh->data_source);
				u_length = strlen(dbh->username);
				p_length = strlen(dbh->password);
				dsn_length = d_length + u_length + p_length + sizeof(";UID=;PWD=;") + 1;
				new_dsn = pemalloc(dsn_length, dbh->is_persistent);
				check_allocation(new_dsn, "dbh_connect", "unable to allocate DSN string");
				sprintf(new_dsn, "%s;UID=%s;PWD=%s;", dbh->data_source,
						dbh->username, dbh->password);
				if (dbh->data_source) {
					pefree((void *) dbh->data_source, dbh->is_persistent);
				}
				/* now replace the DSN with a properly formatted one. */
				dbh->data_source = new_dsn;
			}

		}

		/* and finally try to connect */
		rc = SQLDriverConnect((SQLHDBC) conn_res->hdbc, (SQLHWND) NULL,
				(SQLCHAR *) dbh->data_source, SQL_NTS, NULL,
				0, NULL, SQL_DRIVER_NOPROMPT);
		check_dbh_error(rc, "SQLDriverConnect");
	} else 
#endif
	{
		/* Make sure each of the connection parameters is not NULL */
		if (dbh->data_source) {
			d_length = strlen(dbh->data_source);
		}
		if (dbh->username) {
			u_length = strlen(dbh->username);
		}
		if (dbh->password) {
			p_length = strlen(dbh->password);
		}
		/*
		* No connection options specified, we can just connect with the name,
		*  userid, and password as given.
		*/
		rc = SQLConnect((SQLHDBC) conn_res->hdbc, (SQLCHAR *) dbh->data_source,
			(SQLSMALLINT) d_length,
			(SQLCHAR *) dbh->username,
			(SQLSMALLINT) u_length,
			(SQLCHAR *)dbh->password,
			(SQLSMALLINT) p_length);
		check_dbh_error(rc, "SQLConnect");
#ifdef PASE /* i5/OS incompatible v6+ change */
		memset(buffer11, 0, sizeof(buffer11));
		rc = SQLGetInfo(conn_res->hdbc, SQL_DBMS_VER, (SQLPOINTER)buffer11, sizeof(buffer11), NULL);
		if (buffer11[0]=='0' && buffer11[1]=='5') PDO_IBM_G(is_i5os_classic) = 1;
		else PDO_IBM_G(is_i5os_classic) = 0;
#endif /* PASE */
	}
	rc = SQLGetSQLCA((SQLHENV) conn_res->henv, (SQLHDBC) conn_res->hdbc, SQL_NULL_HSTMT, pSQLCA);
         check_dbh_error(rc, "SQLGetSQLCA");
	conn_res->expansion_factor = pSQLCA->sqlerrd[1];	
	


	/* set the desired case to be upper */
	dbh->desired_case = PDO_CASE_UPPER;

	/* this is now live!  all error handling goes through normal mechanisms. */
	dbh->methods = &ibm_dbh_methods;
	dbh->alloc_own_columns = 1;
	return TRUE;
}


/*
* Main routine called to create a connection.  The dbh structure is
* allocated for us, and we attached a driver-specific control block
* to the PDO allocated one,
*/
static int ibm_handle_factory(
	pdo_dbh_t *dbh,
	zval *driver_options
	TSRMLS_DC)
{
	/* go do the connection */
	return dbh_connect(dbh, driver_options TSRMLS_CC);
}

pdo_driver_t pdo_ibm_driver =
{
	PDO_DRIVER_HEADER(ibm),
	ibm_handle_factory
};

/* common error handling path for final disposition of an error.*/
static void process_pdo_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt TSRMLS_DC)
{
/*  current_error_state(dbh);*/

	conn_handle *conn_res = (conn_handle *)dbh->driver_data;
	strcpy(dbh->error_code, conn_res->error_data.sql_state);
	if (stmt != NULL) {
		/* what this a error in the stmt constructor? */
		if (stmt->methods == NULL) {
			/* make sure we do any required cleanup. */
			ibm_stmt_dtor(stmt TSRMLS_CC);
		}
		strcpy(stmt->error_code, conn_res->error_data.sql_state);
	}

	/*
	* if we got an error very early, we need to throw an exception rather than
	* use the PDO error reporting.
	*/

	if (dbh->methods == NULL) {
		if(conn_res->error_data.isam_err_msg != '\0') {
			zend_throw_exception_ex(php_pdo_get_exception(), 0 TSRMLS_CC,
				"SQLSTATE=%s, %s: %d %s",
				conn_res->error_data.sql_state,
				conn_res->error_data.failure_name,
				conn_res->error_data.sqlcode,
				conn_res->error_data.err_msg);
		} else {
			zend_throw_exception_ex(php_pdo_get_exception(), 0 TSRMLS_CC,
				"SQLSTATE=%s, %s: %d %s ISAM: %s",
				conn_res->error_data.sql_state,
				conn_res->error_data.failure_name,
				conn_res->error_data.sqlcode,
				conn_res->error_data.err_msg,
				conn_res->error_data.isam_err_msg);
		}
		ibm_handle_closer(dbh TSRMLS_CC);
	}
}

/*
* Handle an error return from an SQL call.  The error information from the
* call is saved in our error record.
*/
void raise_sql_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, SQLHANDLE handle,
	SQLSMALLINT hType, char *tag, char *file, int line TSRMLS_DC)
{
	int rc;
	SQLSMALLINT length;
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;

	conn_res->error_data.failure_name = tag;
	conn_res->error_data.filename = file;
	conn_res->error_data.lineno = line;

	SQLGetDiagRec(hType, handle, 1, (SQLCHAR *) & (conn_res->error_data.sql_state),
		&(conn_res->error_data.sqlcode),
		(SQLCHAR *) & (conn_res->error_data.err_msg),
		SQL_MAX_MESSAGE_LENGTH, &length);
	/* the error message is not returned null terminated. */
	conn_res->error_data.err_msg[length] = '\0';

	if(hType == SQL_HANDLE_STMT) {
		/* This is the actual call that returns the ISAM error */
		rc = SQLGetDiagField(SQL_HANDLE_STMT, handle, 1, SQL_DIAG_ISAM_ERROR,
				(SQLCHAR *) & (conn_res->error_data.isam_err_msg), MAX_ISAM_ERROR_MSG_LEN, &length);
		conn_res->error_data.isam_err_msg[length] = '\0';
	}

	/* now go tell PDO about this problem */
	process_pdo_error(dbh, stmt TSRMLS_CC);
}

/*
* Raise a driver-detected error.  This is a faked-SQL type error, using a
* provided sqlstate and message info.
*/
void raise_ibm_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, char *state, char *tag,
	char *message, char *file, int line TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;

	conn_res->error_data.failure_name = tag;
	conn_res->error_data.filename = file;
	conn_res->error_data.lineno = line;
	strcpy(conn_res->error_data.err_msg, message);

	strcpy(conn_res->error_data.sql_state, state);
	conn_res->error_data.sqlcode = 1;	/* just give a non-zero code state. */
	/* now go tell PDO about this problem */
	process_pdo_error(dbh, stmt TSRMLS_CC);
}

/*
* Raise an error in a connection context.  This ensures we use the
* connection handle for retrieving error information.
*/
void raise_dbh_error(pdo_dbh_t *dbh, char *tag, char *file, int line TSRMLS_DC)
{
	conn_handle *conn_res = (conn_handle *) dbh->driver_data;
	raise_sql_error(dbh, NULL, conn_res->hdbc, SQL_HANDLE_DBC, tag, file,
			line TSRMLS_CC);
}

/*
* Raise an error in a statement context.  This ensures we use the correct
* handle for retrieving the diag record, as well as forcing stmt-related
* cleanup.
*/
void raise_stmt_error(pdo_stmt_t *stmt, char *tag, char *file, int line TSRMLS_DC)
{
	stmt_handle *stmt_res = (stmt_handle *) stmt->driver_data;

	/* if we're in the middle of execution when an error was detected, make sure we cancel */
	if (stmt_res->executing) {
		/*  raise the error */
		raise_sql_error(stmt->dbh, stmt, stmt_res->hstmt, SQL_HANDLE_STMT, tag, file, line TSRMLS_CC);
		/*  cancel the statement */
		SQLCancel(stmt_res->hstmt);
		/*  make sure we release execution-related storage. */
		if (stmt_res->lob_buffer != NULL) {
			efree(stmt_res->lob_buffer);
			stmt_res->lob_buffer = NULL;
		}
		if (stmt_res->converted_statement != NULL) {
			efree(stmt_res->converted_statement);
			stmt_res->converted_statement = NULL;
		}
		stmt_res->executing = 0;
	} else {
		/*  raise the error */
		raise_sql_error(stmt->dbh, stmt, stmt_res->hstmt, SQL_HANDLE_STMT, tag, file, line TSRMLS_CC);
	}
}

/*
* Clears the error information
*/
void clear_stmt_error(pdo_stmt_t *stmt)
{
	conn_handle *conn_res = (conn_handle *) stmt->dbh->driver_data;

	conn_res->error_data.sqlcode			= 0;
	conn_res->error_data.filename			= NULL;
	conn_res->error_data.lineno				= 0;
	conn_res->error_data.failure_name		= NULL;
	conn_res->error_data.sql_state[0]		= '\0';
	conn_res->error_data.err_msg[0]			= '\0';
	conn_res->error_data.isam_err_msg[0]	= '\0';
}
