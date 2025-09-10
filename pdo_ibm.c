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

/* The class constants are only useful on 8.4+ */
#if PHP_VERSION_ID >= 80400
#include "pdo_ibm_arginfo.h"
#endif

/* If you declare any globals in php_pdo_ibm.h uncomment this:
*/
#ifdef PASE
ZEND_DECLARE_MODULE_GLOBALS(pdo_ibm)
#endif

/* True global resources - no need for thread safety here */
static int le_pdo_ibm;
extern pdo_driver_t pdo_ibm_driver;	/* the registration table */

static zend_class_entry *pdo_ibm_ce;

#ifdef PASE /* PASE i5/OS start-up */
#include <as400_protos.h>
#endif /* PASE */

/* {{{ pdo_ibm_deps
 */
static zend_module_dep pdo_ibm_deps[] = {
	ZEND_MOD_REQUIRED("pdo")
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ pdo_ibm_module_entry
 */
zend_module_entry pdo_ibm_module_entry =
{
	STANDARD_MODULE_HEADER_EX, NULL,
	pdo_ibm_deps,
	"pdo_ibm",
	NULL,
	PHP_MINIT(pdo_ibm),
	PHP_MSHUTDOWN(pdo_ibm),
	PHP_RINIT(pdo_ibm),
	PHP_RSHUTDOWN(pdo_ibm),
	PHP_MINFO(pdo_ibm),
	PDO_IBM_VERSION,
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
	STD_PHP_INI_ENTRY("pdo_ibm.i5_override_ccsid", "0", PHP_INI_SYSTEM, OnUpdateLong,
		i5_override_ccsid, zend_pdo_ibm_globals, pdo_ibm_globals)
	STD_PHP_INI_ENTRY("pdo_ibm.i5_dbcs_alloc", "0", PHP_INI_SYSTEM, OnUpdateLong,
		i5_dbcs_alloc, zend_pdo_ibm_globals, pdo_ibm_globals)
PHP_INI_END()
#endif /* PASE */


#ifdef PASE /* prior any CLI routine override ascii ccsid */
/* {{{ php_pdo_ibm_init_globals
 */
static void php_pdo_ibm_init_globals(zend_pdo_ibm_globals *pdo_ibm_globals)
{
	if (pdo_ibm_globals->i5_override_ccsid) {
		/* This routine should be called by the application prior
		 * to any other CLI routine to override the ascii ccsid
		 * being retrieved from the Qp2RunPase() routine.  If this
		 * routine is called after any CLI routine is called in
		 * the process it will have no effect.
		 */
		SQLOverrideCCSID400(pdo_ibm_globals->i5_override_ccsid);
	}
}
/* }}} */
#endif /* PASE */

/* PDO_IBM used both SQL_ and I5_ as prefixes for constants */
#if PHP_VERSION_ID >= 80500
#define REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85(base_name, value) \
		REGISTER_PDO_CLASS_CONST_LONG_DEPRECATED_ALIAS_85(base_name, "SQL_", "Pdo\\Ibm::", value)
#define REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85(base_name, value) \
		REGISTER_PDO_CLASS_CONST_LONG_DEPRECATED_ALIAS_85(base_name, "I5_", "Pdo\\Ibm::", value)
#else
#define REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85(base_name, value) \
		REGISTER_PDO_CLASS_CONST_LONG("SQL_" base_name, value)
#define REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85(base_name, value) \
		REGISTER_PDO_CLASS_CONST_LONG("I5_" base_name, value)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pdo_ibm)
{
#ifdef PASE
	/* Only PASE has INI entries right now. */
	ZEND_INIT_MODULE_GLOBALS(pdo_ibm, php_pdo_ibm_init_globals, NULL);
	REGISTER_INI_ENTRIES();
#endif

	if (FAILURE == php_pdo_register_driver(&pdo_ibm_driver)) {
		return FAILURE;
	}

#ifndef PASE /* i5/OS no support trusted */
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_USE_TRUSTED_CONTEXT", (long) PDO_SQL_ATTR_USE_TRUSTED_CONTEXT);
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_TRUSTED_CONTEXT_USERID", (long) PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID);
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_TRUSTED_CONTEXT_PASSWORD", (long) PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD);
#else /* PASE i5/OS introduced (1.3.4) */
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_DBC_SYS_NAMING", (long)PDO_I5_ATTR_DBC_SYS_NAMING);
	
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_COMMIT", (long)PDO_I5_ATTR_COMMIT);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("TXN_NO_COMMIT", (long)PDO_I5_TXN_NO_COMMIT);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("TXN_READ_UNCOMMITTED", (long)PDO_I5_TXN_READ_UNCOMMITTED);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("TXN_READ_COMMITTED", (long)PDO_I5_TXN_READ_COMMITTED);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("TXN_REPEATABLE_READ", (long)PDO_I5_TXN_REPEATABLE_READ);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("TXN_SERIALIZABLE", (long)PDO_I5_TXN_SERIALIZABLE);

	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_JOB_SORT", (long)PDO_I5_ATTR_JOB_SORT);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_DBC_LIBL", (long)PDO_I5_ATTR_DBC_LIBL);
	REGISTER_PDO_IBM_I5_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_DBC_CURLIB", (long)PDO_I5_ATTR_DBC_CURLIB);
#endif /* PASE */

	/* Client information variables */
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_INFO_USERID", (long) PDO_SQL_ATTR_INFO_USERID);
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_INFO_ACCTSTR", (long) PDO_SQL_ATTR_INFO_ACCTSTR);
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_INFO_APPLNAME", (long) PDO_SQL_ATTR_INFO_APPLNAME);
	REGISTER_PDO_IBM_CLASS_CONST_LONG_DEPRECATED_ALIAS_85("ATTR_INFO_WRKSTNNAME", (long) PDO_SQL_ATTR_INFO_WRKSTNNAME);

#if PHP_VERSION_ID >= 80400
	pdo_ibm_ce = register_class_Pdo_Ibm(pdo_dbh_ce);
	pdo_ibm_ce->create_object = pdo_dbh_new;

	return php_pdo_register_driver_specific_ce(&pdo_ibm_driver, pdo_ibm_ce);
#else
	return TRUE;
#endif
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

#ifdef PASE
	DISPLAY_INI_ENTRIES();
#endif
}
/* }}} */

/* }}} */
