if test "$PHP_PDO" != "no"; then

PHP_ARG_WITH(pdo-ibm, for DB2 driver for PDO,
[  --with-pdo-ibm[=DIR] Include PDO DB2 support, DIR is the base
                            DB2 install directory, defaults to ${DB2DIR:-nothing}.
                            Set the PHP_PDO_IBM_LIB environment variable to set
                            the specific location of the DB2 libraries])

if test "$PHP_PDO_IBM" != "no"; then
  SEARCH_PATH="$PHP_PDO_IBM_LIB $PHP_PDO_IBM $DB2PATH $DB2DIR"

  AC_MSG_CHECKING(Looking for DB2 CLI libraries)
  for i in $SEARCH_PATH ; do
    AC_MSG_CHECKING([     in $i])
    if test -r $i/libdb2.so || test -r $i/libdb2.a || test -r $i/libdb400.a || test -r $i/libdb2.dylib; then
      LIB_DIR="$i/"
      AC_MSG_RESULT(found)
      break
    else
      AC_MSG_RESULT()
    fi
    AC_MSG_CHECKING([     in $i/lib64])
    if test -r $i/lib64/libdb2.so || test -r $i/lib64/libdb2.a || test -r $i/lib64/libdb400.a || test -r $i/lib64/libdb2.dylib ; then
      LIB_DIR="$i/lib64/"
      AC_MSG_RESULT(found)
      break
    else
      AC_MSG_RESULT()
    fi
    AC_MSG_CHECKING([     in $i/lib32])
    if test -r $i/lib32/libdb2.so || test -r $i/lib32/libdb2.a || test -r $i/lib32/libdb400.a || test -r $i/lib32/libdb2.dylib ; then
      LIB_DIR="$i/lib32/"
      AC_MSG_RESULT(found)
      break
    else
      AC_MSG_RESULT()
    fi
    AC_MSG_CHECKING([     in $i/lib])
    if test -r $i/lib/libdb2.so || test -r $i/lib/libdb2.a || test -r $i/lib/libdb400.a || test -r $i/lib/libdb2.dylib ; then
      LIB_DIR="$i/lib/"
      AC_MSG_RESULT(found)
      break
    else
      AC_MSG_RESULT()
    fi
  done

  AC_MSG_CHECKING([for DB2 CLI include files in default path])
  for i in $SEARCH_PATH ; do
    AC_MSG_CHECKING([in $i])
    if test -r "$i/include/sqlcli1.h" ; then
      PDO_IBM_DIR=$i
      AC_MSG_RESULT(found in $i)
      break
    fi
  done

  AC_MSG_CHECKING([for PDO includes])
  if test -f $abs_srcdir/include/php/ext/pdo/php_pdo_driver.h; then
    pdo_inc_path=$abs_srcdir/ext
  elif test -f $abs_srcdir/ext/pdo/php_pdo_driver.h; then
    pdo_inc_path=$abs_srcdir/ext
  elif test -f $prefix/include/php/ext/pdo/php_pdo_driver.h; then
    pdo_inc_path=$prefix/include/php/ext
  else
    AC_MSG_ERROR([Cannot find php_pdo_driver.h.])
  fi
  AC_MSG_RESULT($pdo_inc_path)

  dnl Don't forget to add additional source files here
  php_pdo_ibm_sources_core="pdo_ibm.c ibm_driver.c ibm_statement.c"

  PHP_ADD_INCLUDE($PDO_IBM_DIR/include)
  PHP_ADD_LIBPATH($LIB_DIR, PDO_IBM_SHARED_LIBADD)
  PHP_ADD_LIBRARY(db2, 1, PDO_IBM_SHARED_LIBADD)

  case "$host_alias" in
    *aix*)
      CPPFLAGS="$CPPFLAGS -D__H_LOCALEDEF";;
  esac

  PHP_NEW_EXTENSION(pdo_ibm, $php_pdo_ibm_sources_core, $ext_shared,,-I$pdo_inc_path)

  ifdef([PHP_ADD_EXTENSION_DEP],
  [
    PHP_ADD_EXTENSION_DEP(pdo_ibm, pdo)
  ])

  PHP_SUBST(PDO_IBM_SHARED_LIBADD)

fi

fi
