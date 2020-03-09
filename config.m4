dnl config.m4 for extension pmq

PHP_ARG_ENABLE(pmq, whether to enable pmq support,
dnl Make sure that the comment is aligned:
[  --enable-pmq          Enable pmq support], no)

if test "$PHP_PMQ" != "no"; then

    PHP_CHECK_LIBRARY(rt, mq_open, [PHP_ADD_LIBRARY(rt,,PMQ_SHARED_LIBADD)])
    PHP_NEW_EXTENSION(pmq, pmq.c, $ext_shared)
    PHP_SUBST(PMQ_SHARED_LIBADD)


fi
