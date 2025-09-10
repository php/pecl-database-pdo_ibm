/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d973ffba51a2d0ff794d8b86edfdf13aafddb615 */

static zend_class_entry *register_class_Pdo_Ibm(zend_class_entry *class_entry_PDO)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Pdo", "Ibm", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_PDO, ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	zval const_ATTR_INFO_USERID_value;
	ZVAL_LONG(&const_ATTR_INFO_USERID_value, PDO_SQL_ATTR_INFO_USERID);
	zend_string *const_ATTR_INFO_USERID_name = zend_string_init_interned("ATTR_INFO_USERID", sizeof("ATTR_INFO_USERID") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_INFO_USERID_name, &const_ATTR_INFO_USERID_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_INFO_USERID_name);

	zval const_ATTR_INFO_ACCTSTR_value;
	ZVAL_LONG(&const_ATTR_INFO_ACCTSTR_value, PDO_SQL_ATTR_INFO_ACCTSTR);
	zend_string *const_ATTR_INFO_ACCTSTR_name = zend_string_init_interned("ATTR_INFO_ACCTSTR", sizeof("ATTR_INFO_ACCTSTR") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_INFO_ACCTSTR_name, &const_ATTR_INFO_ACCTSTR_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_INFO_ACCTSTR_name);

	zval const_ATTR_INFO_APPLNAME_value;
	ZVAL_LONG(&const_ATTR_INFO_APPLNAME_value, PDO_SQL_ATTR_INFO_APPLNAME);
	zend_string *const_ATTR_INFO_APPLNAME_name = zend_string_init_interned("ATTR_INFO_APPLNAME", sizeof("ATTR_INFO_APPLNAME") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_INFO_APPLNAME_name, &const_ATTR_INFO_APPLNAME_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_INFO_APPLNAME_name);

	zval const_ATTR_INFO_WRKSTNNAME_value;
	ZVAL_LONG(&const_ATTR_INFO_WRKSTNNAME_value, PDO_SQL_ATTR_INFO_WRKSTNNAME);
	zend_string *const_ATTR_INFO_WRKSTNNAME_name = zend_string_init_interned("ATTR_INFO_WRKSTNNAME", sizeof("ATTR_INFO_WRKSTNNAME") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_INFO_WRKSTNNAME_name, &const_ATTR_INFO_WRKSTNNAME_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_INFO_WRKSTNNAME_name);
#if !defined(PASE)

	zval const_ATTR_USE_TRUSTED_CONTEXT_value;
	ZVAL_LONG(&const_ATTR_USE_TRUSTED_CONTEXT_value, PDO_SQL_ATTR_USE_TRUSTED_CONTEXT);
	zend_string *const_ATTR_USE_TRUSTED_CONTEXT_name = zend_string_init_interned("ATTR_USE_TRUSTED_CONTEXT", sizeof("ATTR_USE_TRUSTED_CONTEXT") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_USE_TRUSTED_CONTEXT_name, &const_ATTR_USE_TRUSTED_CONTEXT_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_USE_TRUSTED_CONTEXT_name);

	zval const_ATTR_TRUSTED_CONTEXT_USERID_value;
	ZVAL_LONG(&const_ATTR_TRUSTED_CONTEXT_USERID_value, PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID);
	zend_string *const_ATTR_TRUSTED_CONTEXT_USERID_name = zend_string_init_interned("ATTR_TRUSTED_CONTEXT_USERID", sizeof("ATTR_TRUSTED_CONTEXT_USERID") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_TRUSTED_CONTEXT_USERID_name, &const_ATTR_TRUSTED_CONTEXT_USERID_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_TRUSTED_CONTEXT_USERID_name);

	zval const_ATTR_TRUSTED_CONTEXT_PASSWORD_value;
	ZVAL_LONG(&const_ATTR_TRUSTED_CONTEXT_PASSWORD_value, PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD);
	zend_string *const_ATTR_TRUSTED_CONTEXT_PASSWORD_name = zend_string_init_interned("ATTR_TRUSTED_CONTEXT_PASSWORD", sizeof("ATTR_TRUSTED_CONTEXT_PASSWORD") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_TRUSTED_CONTEXT_PASSWORD_name, &const_ATTR_TRUSTED_CONTEXT_PASSWORD_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_TRUSTED_CONTEXT_PASSWORD_name);
#endif
#if defined(PASE)

	zval const_ATTR_COMMIT_value;
	ZVAL_LONG(&const_ATTR_COMMIT_value, PDO_I5_ATTR_COMMIT);
	zend_string *const_ATTR_COMMIT_name = zend_string_init_interned("ATTR_COMMIT", sizeof("ATTR_COMMIT") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_COMMIT_name, &const_ATTR_COMMIT_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_COMMIT_name);

	zval const_TXN_NO_COMMIT_value;
	ZVAL_LONG(&const_TXN_NO_COMMIT_value, PDO_I5_TXN_NO_COMMIT);
	zend_string *const_TXN_NO_COMMIT_name = zend_string_init_interned("TXN_NO_COMMIT", sizeof("TXN_NO_COMMIT") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_TXN_NO_COMMIT_name, &const_TXN_NO_COMMIT_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_TXN_NO_COMMIT_name);

	zval const_TXN_READ_UNCOMMITTED_value;
	ZVAL_LONG(&const_TXN_READ_UNCOMMITTED_value, PDO_I5_TXN_READ_UNCOMMITTED);
	zend_string *const_TXN_READ_UNCOMMITTED_name = zend_string_init_interned("TXN_READ_UNCOMMITTED", sizeof("TXN_READ_UNCOMMITTED") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_TXN_READ_UNCOMMITTED_name, &const_TXN_READ_UNCOMMITTED_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_TXN_READ_UNCOMMITTED_name);

	zval const_TXN_READ_COMMITTED_value;
	ZVAL_LONG(&const_TXN_READ_COMMITTED_value, PDO_I5_TXN_READ_COMMITTED);
	zend_string *const_TXN_READ_COMMITTED_name = zend_string_init_interned("TXN_READ_COMMITTED", sizeof("TXN_READ_COMMITTED") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_TXN_READ_COMMITTED_name, &const_TXN_READ_COMMITTED_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_TXN_READ_COMMITTED_name);

	zval const_TXN_REPEATABLE_READ_value;
	ZVAL_LONG(&const_TXN_REPEATABLE_READ_value, PDO_I5_TXN_REPEATABLE_READ);
	zend_string *const_TXN_REPEATABLE_READ_name = zend_string_init_interned("TXN_REPEATABLE_READ", sizeof("TXN_REPEATABLE_READ") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_TXN_REPEATABLE_READ_name, &const_TXN_REPEATABLE_READ_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_TXN_REPEATABLE_READ_name);

	zval const_TXN_SERIALIZABLE_value;
	ZVAL_LONG(&const_TXN_SERIALIZABLE_value, PDO_I5_TXN_SERIALIZABLE);
	zend_string *const_TXN_SERIALIZABLE_name = zend_string_init_interned("TXN_SERIALIZABLE", sizeof("TXN_SERIALIZABLE") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_TXN_SERIALIZABLE_name, &const_TXN_SERIALIZABLE_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_TXN_SERIALIZABLE_name);

	zval const_ATTR_JOB_SORT_value;
	ZVAL_LONG(&const_ATTR_JOB_SORT_value, PDO_I5_ATTR_JOB_SORT);
	zend_string *const_ATTR_JOB_SORT_name = zend_string_init_interned("ATTR_JOB_SORT", sizeof("ATTR_JOB_SORT") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_JOB_SORT_name, &const_ATTR_JOB_SORT_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_JOB_SORT_name);

	zval const_ATTR_DBC_LIBL_value;
	ZVAL_LONG(&const_ATTR_DBC_LIBL_value, PDO_I5_ATTR_DBC_LIBL);
	zend_string *const_ATTR_DBC_LIBL_name = zend_string_init_interned("ATTR_DBC_LIBL", sizeof("ATTR_DBC_LIBL") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_DBC_LIBL_name, &const_ATTR_DBC_LIBL_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_DBC_LIBL_name);

	zval const_ATTR_DBC_CURLIB_value;
	ZVAL_LONG(&const_ATTR_DBC_CURLIB_value, PDO_I5_ATTR_DBC_CURLIB);
	zend_string *const_ATTR_DBC_CURLIB_name = zend_string_init_interned("ATTR_DBC_CURLIB", sizeof("ATTR_DBC_CURLIB") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_ATTR_DBC_CURLIB_name, &const_ATTR_DBC_CURLIB_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_ATTR_DBC_CURLIB_name);
#endif

	return class_entry;
}
