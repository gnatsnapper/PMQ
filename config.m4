dnl config.m4 for extension pmq

PHP_ARG_ENABLE(pmq, whether to enable pmq support,
dnl Make sure that the comment is aligned:
[  --enable-pmq          Enable pmq support], no)

if test "$PHP_PMQ" != "no"; then

    LDFLAGS="$LDFLAGS -lrt"
    PHP_NEW_EXTENSION(pmq, pmq.c, $ext_shared)

fi
