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


#ifdef PASE /* i5/OS introduced after DB2 v5 (1.3.2) */
/* IBM i generically changed  (remove ifdef PASE) */
#define SQL_IS_INTEGER 0
/* IBM i long is same ordinal, set to fake/unused ordinal (remove ifdef PASE) */
#undef SQL_LONGVARCHAR
#define SQL_LONGVARCHAR -334 
#undef SQL_LONGVARGRAPHIC
#define SQL_LONGVARGRAPHIC -335
#undef SQL_LONGVARBINARY
#define SQL_LONGVARBINARY -336
#undef SQL_WLONGVARCHAR
#define SQL_WLONGVARCHAR -337
/* IBM i support V6R1+, ignore V5R4- (remove ifdef PASE) */
#undef SQL_BINARY
#define  SQL_BINARY          -2
#undef SQL_VARBINARY
#define  SQL_VARBINARY       -3
#undef SQL_C_BINARY
#define  SQL_C_BINARY	SQL_BINARY

#ifndef SQL_ATTR_INFO_USERID
#define SQL_ATTR_INFO_USERID		10103
#endif
#ifndef SQL_ATTR_INFO_WRKSTNNAME
#define SQL_ATTR_INFO_WRKSTNNAME	10104
#endif
#ifndef SQL_ATTR_INFO_APPLNAME
#define SQL_ATTR_INFO_APPLNAME		10105
#endif
#ifndef SQL_ATTR_INFO_ACCTSTR
#define SQL_ATTR_INFO_ACCTSTR		10106
#endif
#ifndef SQL_ATTR_QUERY_TIMEOUT
#define SQL_ATTR_QUERY_TIMEOUT		SQL_QUERY_TIMEOUT
#endif
#ifndef SQL_IS_UINTEGER
#define SQL_IS_UINTEGER				0 
#endif
/* New 'fake' attributes for IBM i explicit use (IBM i or LUW->IBM i) */
#define SQL_ATTR_DBC_LIBL 31666
#define SQL_ATTR_DBC_CURLIB 31667
#endif /* PASE */

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

/* CB 20200826 - This is documented, but not defined by headers in 7.2/7.4 */
#ifndef SQL_ATTR_NON_HEXCCSID
#define SQL_ATTR_NON_HEXCCSID 10203
#endif

/* Variables for Client Info */
#ifndef PASE /* (LUW error)??? */
enum {
	PDO_SQL_ATTR_INFO_USERID = SQL_ATTR_INFO_USERID,	/* Client UserID */
	PDO_SQL_ATTR_INFO_ACCTSTR,							/* Client Accounting String */
	PDO_SQL_ATTR_INFO_APPLNAME,							/* Client Application Name */
	PDO_SQL_ATTR_INFO_WRKSTNNAME						/* Client Work Station Name */
};
#else
enum {
	/* see pdo/php_pdo_driver.h:
	 * this defines the start of the range for driver specific options.
	 * Drivers should define their own attribute constants beginning with this
	 * value. (PDO_ATTR_DRIVER_SPECIFIC = 1000)
	 */
	/* Variables for Client Info */
	PDO_SQL_ATTR_INFO_USERID = PDO_ATTR_DRIVER_SPECIFIC,/* Client UserID */
	PDO_SQL_ATTR_INFO_ACCTSTR,							/* Client Accounting String */
	PDO_SQL_ATTR_INFO_APPLNAME,							/* Client Application Name */
	PDO_SQL_ATTR_INFO_WRKSTNNAME,						/* Client Work Station Name */
	/* i5/OS variables for system naming (1.3.4) */	
	PDO_I5_ATTR_DBC_SYS_NAMING,							/* i5/OS system naming */
	/* i5/OS variables for commit isolation (1.3.4) */	
	PDO_I5_ATTR_COMMIT,									/* i5/OS SQL_ATTR_COMMIT 0 already used */
	PDO_I5_TXN_NO_COMMIT,								/* i5/OS Commitment control is not used. */
	PDO_I5_TXN_READ_UNCOMMITTED,						/* i5/OS Dirty reads, nonrepeatable reads, 
															and phantoms are possible.*/
	PDO_I5_TXN_READ_COMMITTED,							/* i5/OS Dirty reads are not possible. 
															Nonrepeatable reads, 
															and phantoms are possible. */ 
	PDO_I5_TXN_REPEATABLE_READ,							/* i5/OS Dirty reads and nonrepeatable 
 															reads are not possible. 
 															Phantoms are possible. */
	PDO_I5_TXN_SERIALIZABLE,							/* i5/OS Transactions are serializable.
 															Dirty reads, 
 															non-repeatable reads, 
                                							and phantoms are not possible */
	/* i5/OS variables for job sort (special new value via John Broich PTF) (1.3.4) */	
	PDO_I5_ATTR_JOB_SORT,								/* i5/OS SQL_ATTR_JOB_SORT_SEQUENCE (10046) */
    /* i5/OS libl and curlib for system naming (1.3.4) */
    PDO_I5_ATTR_DBC_LIBL,                               /* i5/OS SQL_ATTR_DBC_LIBL (fake) */
    PDO_I5_ATTR_DBC_CURLIB                              /* i5/OS SQL_ATTR_DBC_CURLIB (fake) */
};
#endif /* PASE */



/* This function is called after executing a stmt for recording lastInsertId */
int record_last_insert_id( pdo_stmt_t *stmt, pdo_dbh_t *dbh, SQLHANDLE hstmt);

/* Cleanup function used across multiple compilation units */
void stmt_cleanup(pdo_stmt_t *stmt);

/* error handling functions and macros. */
void raise_sql_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, SQLHANDLE handle, SQLSMALLINT hType, char *tag, char *file, int line);
void raise_ibm_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, char *state, char *tag, char *message, char *file, int line);
void raise_dbh_error(pdo_dbh_t *dbh, char *tag, char *file, int line);
void raise_stmt_error(pdo_stmt_t *stmt, char *tag, char *file, int line);
void clear_stmt_error(pdo_stmt_t *stmt);
int ibm_stmt_dtor(pdo_stmt_t *stmt);

#define RAISE_DBH_ERROR(tag) raise_dbh_error(dbh, tag, __FILE__, __LINE__)
#define RAISE_STMT_ERROR(tag) raise_stmt_error(stmt, tag, __FILE__, __LINE__)
#define RAISE_IBM_STMT_ERROR(state, tag, msg) raise_ibm_error(stmt->dbh, stmt, state, tag, msg, __FILE__, __LINE__)
#define RAISE_IBM_DBH_ERROR(state, tag, msg) raise_ibm_error(dbh, NULL, state, tag, msg, __FILE__, __LINE__)

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
#ifdef PASE /* ibm i cmd call  helper */
	char * c_i5_pending_libl;   /* chglibl (logic simplicity change) */
	char * c_i5_pending_curlib; /* chgcurlib (logic simplicity change) */
#endif /* PASE */
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

/*
 * PHP 7.4 changes the stream r/w functions to be ssize_t
 * (so you can return errors without (size_t)-1)
 */
#if PHP_MAJOR_VERSION > 7 || (PHP_MAJOR_VERSION == 7 && PHP_MINOR_VERSION == 4)
#define STREAM_RETURN_TYPE ssize_t
#else
#define STREAM_RETURN_TYPE size_t
#endif

/*
 * PHP 8.1 changes many functions to be bool instead of int
 */
#if PHP_MAJOR_VERSION > 8 || (PHP_MAJOR_VERSION == 8 && PHP_MINOR_VERSION == 1)
#define STATUS_RETURN_TYPE bool
#else
#define STATUS_RETURN_TYPE int
#endif

typedef struct _stmt_handle_struct {
	SQLHANDLE hstmt;					/* the statement handle associated with the stmt */
	int executing;						/* an executing state flag for error cleanup */
	/*
	 * PHP 8.1 converts many internal PDO functions to take zend_strings
	 * instead of {char*, size_t}.
	 *
	 * XXX: Why is this variable statement handle scope instead of just
	 * being a local in the statement preparer? It seems it's so that it
	 * can be freed at stmt free, but PDO_ODBC does so in the function...
	 */
#if PHP_MAJOR_VERSION > 8 || (PHP_MAJOR_VERSION == 8 && PHP_MINOR_VERSION == 1)
	zend_string *converted_statement;			/* temporary version of the statement with parameter replacement */
#else
	char *converted_statement;			/* temporary version of the statement with parameter replacement */
#endif
	char *lob_buffer;					/* buffer used for reading in LOB parameters */
	column_data *columns;				/* the column descriptors */
	enum pdo_cursor_type cursor_type;	/* the type of cursor we support. */
	SQLSMALLINT server_ver;				/* the server version */
} stmt_handle;

/* Defines the driver_data structure for caching param data */
typedef struct _param_node {
#ifdef PASE
	int		param_type;			/* Type of param - INP/OUT/INP-OUT/FILE */
#endif /* PASE */
	SQLSMALLINT	data_type;			/* The database data type */
	SQLUINTEGER	param_size;			/* param size */
	SQLSMALLINT	nullable;			/* is Nullable  */
	SQLSMALLINT	scale;				/* Decimal scale */
	SQLSMALLINT	ctype;				/* the optimal C type for transfer */
	SQLINTEGER	transfer_length;	/* the transfer length of the parameter */
	/*
	 * XXX: This should be converted into a general purpose binding buffer
	 * that blits into a new zval like every other extension
	 */
	zval *tmp_binding_buffer;		/* the temporary value used for binding out params if the string is interned */
} param_node;

#endif
