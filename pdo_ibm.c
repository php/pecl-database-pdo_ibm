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

/* {{{ pdo_ibm_functions[]
 *
 * Every user visible function must have an entry in pdo_ibm_functions[].
 */
zend_function_entry pdo_ibm_functions[] =
{
	PHP_FE(confirm_pdo_ibm_compiled, NULL)	/* For testing, remove later. */
	{
		NULL, NULL, NULL
	}	/* Must be the last line in pdo_ibm_functions[] */
};
/* }}} */

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
	pdo_ibm_functions,
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
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("pdo_ibm.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pdo_ibm_globals, pdo_ibm_globals)
	STD_PHP_INI_ENTRY("pdo_ibm.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pdo_ibm_globals, pdo_ibm_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_pdo_ibm_init_globals
 */
static void php_pdo_ibm_init_globals(zend_pdo_ibm_globals *pdo_ibm_globals)
{
        pdo_ibm_globals->is_i5os_classic = 1;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pdo_ibm)
{
	ZEND_INIT_MODULE_GLOBALS(pdo_ibm, php_pdo_ibm_init_globals, NULL);
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

#ifndef PASE /* i5/OS no support trusted */
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_USE_TRUSTED_CONTEXT", (long) PDO_SQL_ATTR_USE_TRUSTED_CONTEXT);
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_TRUSTED_CONTEXT_USERID", (long) PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID);	
	REGISTER_PDO_CLASS_CONST_LONG("SQL_ATTR_TRUSTED_CONTEXT_PASSWORD", (long) PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD);
#endif

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


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_pdo_ibm_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_pdo_ibm_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char string[256];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FALSE)
	{
		return;
	}

	len = sprintf(string, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "pdo_ibm", arg);
#if PHP_MAJOR_VERSION >= 7
        RETURN_STRINGL(string, len);
#else
	RETURN_STRINGL(string, len, 1);
#endif
}
/* }}} */
