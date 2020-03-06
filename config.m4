dnl config.m4 for extension pmq

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(pmq, for pmq support,
dnl Make sure that the comment is aligned:
dnl [  --with-pmq             Include pmq support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(pmq, whether to enable pmq support,
dnl Make sure that the comment is aligned:
[  --enable-pmq          Enable pmq support], no)

if test "$PHP_PMQ" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, PMQ_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-pmq -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/pmq.h"  # you most likely want to change this
  dnl if test -r $PHP_PMQ/$SEARCH_FOR; then # path given as parameter
  dnl   PMQ_DIR=$PHP_PMQ
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for pmq files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PMQ_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PMQ_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the pmq distribution])
  dnl fi

  dnl # --with-pmq -> add include path
  dnl PHP_ADD_INCLUDE($PMQ_DIR/include)

  dnl # --with-pmq -> check for lib and symbol presence
  dnl LIBNAME=PMQ # you may want to change this
  dnl LIBSYMBOL=PMQ # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PMQ_DIR/$PHP_LIBDIR, PMQ_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PMQLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong pmq lib version or lib not found])
  dnl ],[
  dnl   -L$PMQ_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PMQ_SHARED_LIBADD)

  dnl PHP_CHECK_LIBRARY(rt,mq_open,[PHP_ADD_LIBRARY(rt,,PMQ_EXTRA_LIBADD)])
  dnl PHP_SUBST(PMQ_EXTRA_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_PMQ, 1, [ Have pmq support ])

  PHP_NEW_EXTENSION(pmq, pmq.c, $ext_shared)
fi
