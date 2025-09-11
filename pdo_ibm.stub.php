<?php

/**
 * @generate-class-entries 
 *
 * This driver specific PDO class is only used for PHP 8.4+
 */

namespace Pdo {
	/**
	 * @strict-properties
	 * @not-serializable
	 */
	class Ibm extends \PDO
	{
		/** @cvalue PDO_SQL_ATTR_INFO_USERID */
		public const int ATTR_INFO_USERID = UNKNOWN;

		/** @cvalue PDO_SQL_ATTR_INFO_ACCTSTR */
		public const int ATTR_INFO_ACCTSTR = UNKNOWN;

		/** @cvalue PDO_SQL_ATTR_INFO_APPLNAME */
		public const int ATTR_INFO_APPLNAME = UNKNOWN;

		/** @cvalue PDO_SQL_ATTR_INFO_WRKSTNNAME */
		public const int ATTR_INFO_WRKSTNNAME = UNKNOWN;

#ifndef PASE
		/** @cvalue PDO_SQL_ATTR_USE_TRUSTED_CONTEXT */
		public const int ATTR_USE_TRUSTED_CONTEXT = UNKNOWN;

		/** @cvalue PDO_SQL_ATTR_TRUSTED_CONTEXT_USERID */
		public const int ATTR_TRUSTED_CONTEXT_USERID = UNKNOWN;

		/** @cvalue PDO_SQL_ATTR_TRUSTED_CONTEXT_PASSWORD */
		public const int ATTR_TRUSTED_CONTEXT_PASSWORD = UNKNOWN;
#endif
#ifdef PASE
		/** @cvalue PDO_I5_ATTR_COMMIT */
		public const int ATTR_COMMIT = UNKNOWN;

		/** @cvalue PDO_I5_TXN_NO_COMMIT */
		public const int TXN_NO_COMMIT = UNKNOWN;

		/** @cvalue PDO_I5_TXN_READ_UNCOMMITTED */
		public const int TXN_READ_UNCOMMITTED = UNKNOWN;

		/** @cvalue PDO_I5_TXN_READ_COMMITTED */
		public const int TXN_READ_COMMITTED = UNKNOWN;

		/** @cvalue PDO_I5_TXN_REPEATABLE_READ */
		public const int TXN_REPEATABLE_READ = UNKNOWN;

		/** @cvalue PDO_I5_TXN_SERIALIZABLE */
		public const int TXN_SERIALIZABLE = UNKNOWN;

		/** @cvalue PDO_I5_ATTR_JOB_SORT */
		public const int ATTR_JOB_SORT = UNKNOWN;

		/** @cvalue PDO_I5_ATTR_DBC_LIBL */
		public const int ATTR_DBC_LIBL = UNKNOWN;

		/** @cvalue PDO_I5_ATTR_DBC_CURLIB */
		public const int ATTR_DBC_CURLIB = UNKNOWN;
#endif
	}
}

