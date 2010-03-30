/*
  +----------------------------------------------------------------------+
  | (C) Copyright IBM Corporation 2006                                   |
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

#ifndef PHP_PDO_IBM_INT_H
#define PHP_PDO_IBM_INT_H


#include "sqlcli1.h"

#define MAX_OPTION_LEN 10
#define MAX_ERR_MSG_LEN (SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 1)
#define CDTIMETYPE 112

#ifndef SQL_XML
#define SQL_XML -370
#endif

/* Maximum length of the name of the DBMS being accessed */
#define MAX_DBMS_IDENTIFIER_NAME 256

/* This is used in last_insert_id.
 We allocate a buffer of size 32 as per recommendations from the CLI IDS team */
#define MAX_IDENTITY_DIGITS 32

#ifndef SQL_ATTR_GET_GENERATED_VALUE
#define SQL_ATTR_GET_GENERATED_VALUE 2583
#endif

#ifndef PASE /* i5/OS no support trusted */
/* Trusted Context has been introduced after DB2 v9 */
#ifndef SQL_ATTR_USE_TRUSTED_CONTEXT
#define SQL_ATTR_USE_TRUSTED_CONTEXT 2561
#define SQL_ATTR_TRUSTED_CONTEXT_USERID 2562
#define SQL_ATTR_TRUSTED_CONTEXT_PASSWORD 2563
#endif /* SQL_ATTR_USE_TRUSTED_CONTEXT */

/* Variables for Trusted Context */
enum {
	PDO_SQL_ATTR_USE_TRUSTED_CONTEXT = SQL_ATTR_USE_TRUSTED_CONTEXT,                /* use to turn on trustext context mode */
	PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID,                                            /* Setting Trusted userID */
	PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD                                           /* Setting password for Trusted User */
};
#endif /* PASE */

/* ISAM Error code Macro */
#ifndef SQL_DIAG_ISAM_ERROR
#define SQL_DIAG_ISAM_ERROR 13
#endif

#define MAX_ISAM_ERROR_MSG_LEN MAX_ERR_MSG_LEN

/* Client information Macros. DB2 for z/OS and OS/390 servers supported max values. */
#define USERID_LEN 16
#define ACCTSTR_LEN 200
#define APPLNAME_LEN 32
#define WRKSTNNAME_LEN 18

/* SQL variable for Client information */
#ifndef SQL_ATTR_INFO_USERID
#define SQL_ATTR_INFO_USERID 1281
#endif

#ifndef SQL_ATTR_INFO_WRKSTNNAME
#define SQL_ATTR_INFO_WRKSTNNAME 1282
#endif

#ifndef SQL_ATTR_INFO_APPLNAME
#define SQL_ATTR_INFO_APPLNAME 1283
#endif

#ifndef SQL_ATTR_INFO_ACCTSTR
#define SQL_ATTR_INFO_ACCTSTR 1284
#endif

/* Variables for Client Info */
enum {
	PDO_SQL_ATTR_INFO_USERID = SQL_ATTR_INFO_USERID,	/* Client UserID */
	PDO_SQL_ATTR_INFO_ACCTSTR,							/* Client Accounting String */
	PDO_SQL_ATTR_INFO_APPLNAME,							/* Client Application Name */
	PDO_SQL_ATTR_INFO_WRKSTNNAME						/* Client Work Station Name */
};

/* This function is called after executing a stmt for recording lastInsertId */
int record_last_insert_id( pdo_stmt_t *stmt, pdo_dbh_t *dbh, SQLHANDLE hstmt TSRMLS_DC);


/* error handling functions and macros. */
void raise_sql_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, SQLHANDLE handle, SQLSMALLINT hType, char *tag, char *file, int line TSRMLS_DC);
void raise_ibm_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, char *state, char *tag, char *message, char *file, int line TSRMLS_DC);
void raise_dbh_error(pdo_dbh_t *dbh, char *tag, char *file, int line TSRMLS_DC);
void raise_stmt_error(pdo_stmt_t *stmt, char *tag, char *file, int line TSRMLS_DC);
void clear_stmt_error(pdo_stmt_t *stmt);
int ibm_stmt_dtor(pdo_stmt_t *stmt TSRMLS_DC);

#define RAISE_DBH_ERROR(tag) raise_dbh_error(dbh, tag, __FILE__, __LINE__ TSRMLS_CC)
#define RAISE_STMT_ERROR(tag) raise_stmt_error(stmt, tag, __FILE__, __LINE__ TSRMLS_CC)
#define RAISE_IBM_STMT_ERROR(state, tag, msg) raise_ibm_error(stmt->dbh, stmt, state, tag, msg, __FILE__, __LINE__ TSRMLS_CC)
#define RAISE_IBM_DBH_ERROR(state, tag, msg) raise_ibm_error(dbh, NULL, state, tag, msg, __FILE__, __LINE__ TSRMLS_CC)

/* check for an SQL error in the context of an 
   PDO method execution. */
#define check_dbh_error(rc, tag)	\
{									\
	if ( rc == SQL_ERROR )			\
	{								\
		RAISE_DBH_ERROR(tag);		\
		return FALSE;				\
	}								\
}									\

/* check for an SQL error in the context of an 
   PDOStatement method execution. */
#define check_stmt_error(rc, tag)	\
				{					\
	if ( rc == SQL_ERROR )			\
	{								\
		RAISE_STMT_ERROR(tag);		\
		return FALSE;				\
	}								\
}									\

/* check an allocation in the context of a PDO object. */
#define check_allocation(ptr, tag, msg)					\
{														\
	if ((ptr) == NULL)									\
	{													\
		RAISE_IBM_DBH_ERROR("HY001", tag, msg);	\
		return FALSE;									\
	}													\
}														\

/* check a storage allocation for a PDOStatement
   object context. */
#define check_stmt_allocation(ptr, tag, msg)				\
{															\
	if ((ptr) == NULL)										\
	{														\
		RAISE_IBM_STMT_ERROR("HY001", tag, msg);	\
		return FALSE;										\
	}														\
}															\


typedef struct _conn_error_data {
	SQLINTEGER sqlcode;							/* native sql error code */
	char *filename;								/* name of the file raising the error */
	int lineno;									/* line number location of the error */
	char *failure_name;							/* the failure tag. */
	SQLCHAR sql_state[8];						/* SQLSTATE code */
	char err_msg[SQL_MAX_MESSAGE_LENGTH + 1];	/* error message associated with failure */
	char isam_err_msg[MAX_ISAM_ERROR_MSG_LEN + 1];	/* ISAM error message */
} conn_error_data;

typedef struct _conn_handle_struct {
	SQLHANDLE henv;				/* handle to the interface environment */
	SQLHANDLE hdbc;				/* the connection handle */
	conn_error_data error_data;	/* error handling information */
	int last_insert_id;			/* the last serial id inserted */
} conn_handle;

/* values used for binding fetched data */
typedef union {
	long l_val;		/* long values -- used for all int values, including bools */
	char *str_val;	/* used for string bindings */
} column_data_value;

/* local descriptor for column data.  These mirror the
   descriptors given back to the PDO driver. */
typedef struct {
	char *name;							/* the column name */
	SQLSMALLINT namelen;				/* length of the column name */
	SQLSMALLINT data_type;				/* the database column type */
	enum pdo_param_type returned_type;	/* our returned parameter type */
	SQLUINTEGER data_size;				/* maximum size of the data  */
	SQLSMALLINT nullable;				/* the nullable flag */
	SQLSMALLINT scale;					/* the scale value */
	SQLUINTEGER out_length;				/* the transfered data length. Filled in by a fetch */
	SQLINTEGER  lob_loc;
	SQLINTEGER  loc_ind;
	SQLINTEGER  loc_type;
	SQLUINTEGER lob_data_length;
	SQLUINTEGER lob_data_offset; /* fetched blob part*/
	char        *lob_data;
	column_data_value data;				/* the transferred data */
} column_data;

/* size of the buffer used to read LOB streams */
#define LOB_BUFFER_SIZE 8192

typedef struct _stmt_handle_struct {
	SQLHANDLE hstmt;					/* the statement handle associated with the stmt */
	int executing;						/* an executing state flag for error cleanup */
	char *converted_statement;			/* temporary version of the statement with parameter replacement */
	char *lob_buffer;					/* buffer used for reading in LOB parameters */
	column_data *columns;				/* the column descriptors */
	enum pdo_cursor_type cursor_type;	/* the type of cursor we support. */
	SQLSMALLINT server_ver;				/* the server version */
} stmt_handle;

/* Defines the driver_data structure for caching param data */
typedef struct _param_node {
	SQLSMALLINT	data_type;			/* The database data type */
	SQLUINTEGER	param_size;			/* param size */
	SQLSMALLINT nullable;			/* is Nullable  */
	SQLSMALLINT	scale;				/* Decimal scale */
	SQLSMALLINT ctype;				/* the optimal C type for transfer */
	SQLINTEGER  transfer_length;	/* the transfer length of the parameter */
} param_node;

#endif
