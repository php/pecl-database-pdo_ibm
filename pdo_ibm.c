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
  | Ambrish Bhargava, Rahul Priyadarshi                                  |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "pdo/php_pdo_driver.h"
#include "php_pdo_ibm.h"
#include "php_pdo_ibm_int.h"

/* If you declare any globals in php_pdo_ibm.h uncomment this:
*/
ZEND_DECLARE_MODULE_GLOBALS(pdo_ibm)

/* True global resources - no need for thread safety here */
static int le_pdo_ibm;
extern pdo_driver_t pdo_ibm_driver;	/* the registration table */


#ifdef PASE /* PASE i5/OS start-up */
/* This routine should be called by the application prior
 * to any other CLI routine to override the ascii ccsid
 * being retrieved from the Qp2RunPase() routine.  If this
 * routine is called after any CLI routine is called in
 * the process it will have no effect.
 */
int SQLOverrideCCSID400(int newCCSID);
#endif /* PASE */

/* {{{ pdo_ibm_deps
 */
#if ZEND_MODULE_API_NO >= 20041225
static zend_module_dep pdo_ibm_deps[] = {
	ZEND_MOD_REQUIRED("pdo")
	{NULL, NULL, NULL}
};
#endif
/* }}} */

/* {{{ pdo_ibm_module_entry
 */
zend_module_entry pdo_ibm_module_entry =
{
#if ZEND_MODULE_API_NO >= 20041225
	STANDARD_MODULE_HEADER_EX, NULL,
	pdo_ibm_deps,
#else
	STANDARD_MODULE_HEADER,
#endif
	"pdo_ibm",
	NULL,
	PHP_MINIT(pdo_ibm),
	PHP_MSHUTDOWN(pdo_ibm),
	PHP_RINIT(pdo_ibm),        /* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pdo_ibm),    /* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pdo_ibm),
	PDO_IBM_VERSION,   /* Replace with version number for your extension */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PDO_IBM
ZEND_GET_MODULE(pdo_ibm)
#endif

/* {{{ PHP_INI
 */
#ifdef PASE /* i5/OS specific functions */
PHP_INI_BEGIN()
	STD_PHP_INI_BOOLEAN("pdo_ibm.i5_override_ccsid", "0", PHP_INI_SYSTEM, OnUpdateLong,
		i5_override_ccsid, zend_pdo_ibm_globals, pdo_ibm_globals)
PHP_INI_END()
#endif /* PASE */


/* {{{ php_pdo_ibm_init_globals
 */
static void php_pdo_ibm_init_globals(zend_pdo_ibm_globals *pdo_ibm_globals)
{
#ifdef PASE /* prior any CLI routine override ascii ccsid */
	if (pdo_ibm_globals->i5_override_ccsid) {
		SQLOverrideCCSID400(pdo_ibm_globals->i5_override_ccsid);
	}
#endif /* PASE */
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pdo_ibm)
{
	ZEND_INIT_MODULE_GLOBALS(pdo_ibm, php_pdo_ibm_init_globals, NULL);
#ifdef PASE
	/* Only PASE has INI entries right now. */
	REGISTER_INI_ENTRIES();
#endif

#ifndef PASE /* i5/OS no support trusted */
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_USE_TRUSTED_CONTEXT", (long) PDO_SQL_ATTR_USE_TRUSTED_CONTEXT);
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_TRUSTED_CONTEXT_USERID", (long) PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID);	
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_TRUSTED_CONTEXT_PASSWORD", (long) PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD);
#else /* PASE i5/OS introduced (1.3.4) */
	REGISTER_PDO_CLASS_CONST_LONG("I5_ATTR_DBC_SYS_NAMING", (long)PDO_I5_ATTR_DBC_SYS_NAMING);
	
	REGISTER_PDO_CLASS_CONST_LONG("I5_ATTR_COMMIT", (long)PDO_I5_ATTR_COMMIT);
	REGISTER_PDO_CLASS_CONST_LONG("I5_TXN_NO_COMMIT", (long)PDO_I5_TXN_NO_COMMIT);
	REGISTER_PDO_CLASS_CONST_LONG("I5_TXN_READ_UNCOMMITTED", (long)PDO_I5_TXN_READ_UNCOMMITTED);
	REGISTER_PDO_CLASS_CONST_LONG("I5_TXN_READ_COMMITTED", (long)PDO_I5_TXN_READ_COMMITTED);
	REGISTER_PDO_CLASS_CONST_LONG("I5_TXN_REPEATABLE_READ", (long)PDO_I5_TXN_REPEATABLE_READ);
	REGISTER_PDO_CLASS_CONST_LONG("I5_TXN_SERIALIZABLE", (long)PDO_I5_TXN_SERIALIZABLE);

	REGISTER_PDO_CLASS_CONST_LONG("I5_ATTR_JOB_SORT", (long)PDO_I5_ATTR_JOB_SORT);
	REGISTER_PDO_CLASS_CONST_LONG("I5_ATTR_DBC_LIBL", (long)PDO_I5_ATTR_DBC_LIBL);
	REGISTER_PDO_CLASS_CONST_LONG("I5_ATTR_DBC_CURLIB", (long)PDO_I5_ATTR_DBC_CURLIB);
#endif /* PASE */

	/* Client information variables */
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_INFO_USERID", (long) PDO_SQL_ATTR_INFO_USERID);
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_INFO_ACCTSTR", (long) PDO_SQL_ATTR_INFO_ACCTSTR);	
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_INFO_APPLNAME", (long) PDO_SQL_ATTR_INFO_APPLNAME);
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_INFO_WRKSTNNAME", (long) PDO_SQL_ATTR_INFO_WRKSTNNAME);
	
	php_pdo_register_driver(&pdo_ibm_driver);
	return TRUE;  
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pdo_ibm)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	 
	php_pdo_unregister_driver(&pdo_ibm_driver);
	return TRUE;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pdo_ibm)
{
	return TRUE;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pdo_ibm)
{
	return TRUE;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pdo_ibm)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pdo_ibm support", "enabled");
	php_info_print_table_row(2, "Module release", PDO_IBM_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* }}} */
