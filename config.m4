if test "$PHP_PDO" != "no"; then

PHP_ARG_WITH(pdo-ibm, for DB2 driver for PDO,
[  --with-pdo-ibm[=DIR] Include PDO DB2 support, DIR is the base
                            DB2 install directory, defaults to ${DB2DIR:-nothing}.
                            Set the PHP_PDO_IBM_LIB environment variable to set
                            the specific location of the DB2 libraries])

if test "$PHP_PDO_IBM" != "no"; then
  SEARCH_PATH="$PHP_PDO_IBM_LIB $PHP_PDO_IBM $DB2PATH $DB2DIR"

  dnl Scan the library path for LUW, clidriver, and libdb400 in the usual
  dnl places, also assuming include/ is in the directory too.
  for i in $SEARCH_PATH ; do
    dnl XXX: The messages kinda suck and don't indicate which path
    dnl (combined with AC_MSG_* spew from AC_CHECK_LIB)
    dnl XXX: Macros for this? Can these be merged?

    dnl LUW ships its client libraries in lib64/32 (at least on amd64 linux)
    AC_CHECK_SIZEOF([long])
    AC_MSG_CHECKING([if we're on a 64-bit platform])
    AS_IF([test "$ac_cv_sizeof_long" -eq 4],[
      AC_MSG_RESULT([no])
      PHP_CHECK_LIBRARY(db2, SQLDriverConnect, [
        PHP_ADD_LIBPATH($i/lib32, PDO_IBM_SHARED_LIBADD)
        PHP_ADD_LIBRARY(db2, 1, PDO_IBM_SHARED_LIBADD)
        PHP_ADD_INCLUDE($i/include)
        break
      ], [], "-L$i/lib32" )
    ],[
      AC_MSG_RESULT([yes])
      PHP_CHECK_LIBRARY(db2, SQLDriverConnect, [
        PHP_ADD_LIBPATH($i/lib64, PDO_IBM_SHARED_LIBADD)
        PHP_ADD_LIBRARY(db2, 1, PDO_IBM_SHARED_LIBADD)
        PHP_ADD_INCLUDE($i/include)
        break
      ], [], "-L$i/lib64" )
    ])
    dnl The standalone clidriver package uses lib/
    PHP_CHECK_LIBRARY(db2, SQLDriverConnect, [
      PHP_ADD_LIBPATH($i/lib, PDO_IBM_SHARED_LIBADD)
      PHP_ADD_LIBRARY(db2, 1, PDO_IBM_SHARED_LIBADD)
      PHP_ADD_INCLUDE($i/include)
      break
    ], [
    ], "-L$i/lib" )
    dnl Special cases for PASE
    dnl SG ships a custom libdb400 (with renamed funcs to co-exist w/ ODBC)
    dnl it requires some special handling for headers too
    PHP_CHECK_LIBRARY(db400sg, LDBDriverConnect, [
      PDO_IBM_PASE=yes
      dnl from RPMs libdb400sg-devel and sqlcli-devel
      dnl as IBM i doesn't ship SQL/CLI headers w/ PASE (and RPM's in subdir)
      PHP_ADD_LIBPATH($i/lib, PDO_IBM_SHARED_LIBADD)
      PHP_ADD_LIBRARY(db400sg, 1, PDO_IBM_SHARED_LIBADD)
      PHP_ADD_INCLUDE(/QOpenSys/pkgs/include/cli-sg)
      PHP_ADD_INCLUDE(/QOpenSys/pkgs/include/cli)
      break
    ], [
    ], "-L$i/lib" )
    dnl Probably vanilla libdb400
    dnl XXX: For PASE, libdb400 is likely on the default path
    PHP_CHECK_LIBRARY(db400, SQLDriverConnect, [
      PDO_IBM_PASE=yes
      dnl from RPM sqlcli-devel
      PHP_ADD_LIBPATH($i/lib, PDO_IBM_SHARED_LIBADD)
      PHP_ADD_LIBRARY(db400, 1, PDO_IBM_SHARED_LIBADD)
      PHP_ADD_INCLUDE(/QOpenSys/pkgs/include/cli)
      break
    ], [
    ], "-L$i/lib" )
  done

  PHP_CHECK_PDO_INCLUDES

  dnl Don't forget to add additional source files here
  php_pdo_ibm_sources_core="pdo_ibm.c ibm_driver.c ibm_statement.c"

  dnl Convert the includes to __PASE__ (for IBM-shipped GCC) or use AC_DEFINE
  if test "$PDO_IBM_PASE" = "yes" ; then
    PHP_NEW_EXTENSION(pdo_ibm, $php_pdo_ibm_sources_core, $ext_shared,,-I$pdo_cv_inc_path -DPASE)
  else
    PHP_NEW_EXTENSION(pdo_ibm, $php_pdo_ibm_sources_core, $ext_shared,,-I$pdo_cv_inc_path)
  fi

  PHP_ADD_EXTENSION_DEP(pdo_ibm, pdo)

  PHP_SUBST(PDO_IBM_SHARED_LIBADD)

fi

fi
