/* PMQ extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "zend_exceptions.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "php_pmq.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

zend_class_entry *pmq_ce;
static zend_object_handlers pmq_object_handlers;

/* {{{ string pmq_info( string $name )
 */

PHP_FUNCTION(pmq_info)
{
	zend_string *name;
        mqd_t mq;
        struct mq_attr gAttr;
        int retval;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();


    mq = mq_open(ZSTR_VAL(name), 0666);

    if((int)mq == -1)
    {
         zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }

    retval = mq_getattr(mq, &gAttr);

    if(retval == -1)
    {
         zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }

    retval = mq_close(mq);

    if(retval == -1)
    {
         zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }

    array_init(return_value);

    add_assoc_long(return_value, "flags", gAttr.mq_flags);
    add_assoc_long(return_value, "maxmsg", gAttr.mq_maxmsg);
    add_assoc_long(return_value, "msgsize", gAttr.mq_msgsize);
    add_assoc_long(return_value, "curmsgs", gAttr.mq_curmsgs);

}

/* }}}*/

/* {{{ void pmq___construct( string $name )
 */

PHP_METHOD(PMQ,__construct)
{
        zend_string *name;
        zend_long flags = -1;
        zend_long mode = 0600;//PMQ_DEF_MODE;
        zend_long maxmsg = 10;//PMQ_DEF_MAXMSG;
        zend_long msgsize = 8192;//PMQ_DEF_MSGSIZE;

        mqd_t queue;

	ZEND_PARSE_PARAMETERS_START(1, 5)
		Z_PARAM_STR(name)
                Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags)
		Z_PARAM_LONG(mode)
		Z_PARAM_LONG(maxmsg)
		Z_PARAM_LONG(msgsize)
	ZEND_PARSE_PARAMETERS_END();

        if(flags == -1)
        {
            queue = defaultQueue(ZSTR_VAL(name));
        }
        else
        {
            queue = customQueue(ZSTR_VAL(name), flags, mode, (int)maxmsg, (int)msgsize);
        }

	php_pmq_initialize(Z_PHPPMQ_P(ZEND_THIS), ZSTR_VAL(name), queue, (int)msgsize);

}

/* }}} */

/* {{{ bool PMQ::send( string $message [, int $priority [, int $timeout ]] )
 */

PHP_METHOD(PMQ,send)
{
   php_pmq_obj *pmqobj;
   zend_long priority = 1;
   zend_long timeout = 0;
   zend_string *message;

   int retval;

   ZEND_PARSE_PARAMETERS_START(1, 3)
                Z_PARAM_STR(message)
                Z_PARAM_OPTIONAL
                Z_PARAM_LONG(priority)
                Z_PARAM_LONG(timeout)
   ZEND_PARSE_PARAMETERS_END();

   pmqobj = Z_PHPPMQ_P(getThis());

   if(ZSTR_LEN(message) > pmqobj->msgsize)
   {
        zend_throw_exception(zend_ce_exception, "Message too long",  0);
        RETURN_FALSE;
   }

   if(timeout > 0)
   {

       struct timespec ts;
       clock_gettime(CLOCK_REALTIME, &ts);
       ts.tv_sec = (time_t)(int)ts.tv_sec + (int)timeout;

       retval = mq_timedsend(pmqobj->queue,ZSTR_VAL(message),ZSTR_LEN(message)+1,(int)priority,&ts);

   }
   else
   {

       retval = mq_send(pmqobj->queue,ZSTR_VAL(message),ZSTR_LEN(message)+1,(int)priority);

   }

   if(retval == -1)
   {

        if((errno == ETIMEDOUT) || (errno == EAGAIN))
        {
            RETURN_FALSE;
        }
        else
        {
            zend_throw_exception(zend_ce_exception, strerror(errno),  0);
        }

   }

   RETURN_TRUE;

}

/* }}} */

/* {{{ string PMQ::receive([ int $timeout ])
 */

PHP_METHOD(PMQ, receive)
{
   php_pmq_obj *pmqobj;
   int retval;
   unsigned int *priority;
   zend_long timeout = 0;
   ZEND_PARSE_PARAMETERS_START(0, 1)
       Z_PARAM_OPTIONAL
       Z_PARAM_LONG(timeout)
   ZEND_PARSE_PARAMETERS_END();

   pmqobj = Z_PHPPMQ_P(getThis());
   char message[pmqobj->msgsize];

   if(timeout > 0)
   {

   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   ts.tv_sec = (time_t)(int)ts.tv_sec + (int)timeout;

   retval = mq_timedreceive(pmqobj->queue, message, pmqobj->msgsize, priority ,&ts);

   }
   else
   {

   retval = mq_receive(pmqobj->queue, message, pmqobj->msgsize, priority);

   }

   if(retval == -1)
   {

        if((errno == ETIMEDOUT) || (errno == EAGAIN))
        {
            RETURN_FALSE;
        }
        else
        {
            zend_throw_exception(zend_ce_exception, strerror(errno),  0);
        }

   }

   RETURN_STRING(message);

}

/* }}} */

/* {{{ bool PMQ::close(  )
 */

PHP_METHOD(PMQ,close)
{
        php_pmq_obj     *pmqobj;
        int retval;

	ZEND_PARSE_PARAMETERS_NONE();

        pmqobj = Z_PHPPMQ_P(getThis());

        retval = mq_close(pmqobj->queue);
        if(retval == -1)
        {

            zend_throw_exception(zend_ce_exception, strerror(errno),  0);

        }

	RETURN_TRUE;
}

/* }}}*/

/* {{{ array PMQ::info(  )
 */

PHP_METHOD(PMQ,info)
{
        php_pmq_obj     *pmqobj;
        struct mq_attr gAttr;
        int retval;

	ZEND_PARSE_PARAMETERS_NONE();

        pmqobj = Z_PHPPMQ_P(getThis());

        retval = mq_getattr(pmqobj->queue, &gAttr);

        if(retval == -1)
        {
             zend_throw_exception(zend_ce_exception, strerror(errno),  0);
        }

        array_init(return_value);

        add_assoc_long(return_value, "flags", gAttr.mq_flags);
        add_assoc_long(return_value, "maxmsg", gAttr.mq_maxmsg);
        add_assoc_long(return_value, "msgsize", gAttr.mq_msgsize);
        add_assoc_long(return_value, "curmsgs", gAttr.mq_curmsgs);

}
/* }}}*/

/* {{{ bool PMQ::unlink(  )
 */
PHP_METHOD(PMQ,unlink)
{
        php_pmq_obj     *pmqobj;
        int retval;

	ZEND_PARSE_PARAMETERS_NONE();

        pmqobj = Z_PHPPMQ_P(getThis());

        retval = mq_unlink(pmqobj->name);
        if(retval == -1)
        {

            zend_throw_exception(zend_ce_exception, strerror(errno),  0);

        }

	RETURN_TRUE;
}

/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */

PHP_RINIT_FUNCTION(pmq)
{
#if defined(ZTS) && defined(COMPILE_DL_PMQ)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */

PHP_MINFO_FUNCTION(pmq)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pmq support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */

ZEND_BEGIN_ARG_INFO(arginfo_pmq_info, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_construct, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, mode)
	ZEND_ARG_INFO(0, maxmsg)
	ZEND_ARG_INFO(0, msgsize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_send, 0)
    ZEND_ARG_INFO(0, message)
    ZEND_ARG_INFO(0, priority)
    ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_receive, 0)
    ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_close, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_unlink, 0)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ pmq_functions[]
 */

static const zend_function_entry pmq_functions[] = {
	PHP_FE(pmq_info,		arginfo_pmq_info)
	PHP_FE_END
};

/* }}} */

/* {{{ pmq_methods[]
 */

static const zend_function_entry pmq_methods[] = {
        PHP_ME(PMQ, __construct,	arginfo_pmq_class_construct, ZEND_ACC_PUBLIC)
	PHP_ME(PMQ, send,               arginfo_pmq_class_send, ZEND_ACC_PUBLIC)
        PHP_ME(PMQ, receive,            arginfo_pmq_class_receive, ZEND_ACC_PUBLIC)
	PHP_ME(PMQ, info,               arginfo_pmq_class_info, ZEND_ACC_PUBLIC)
	PHP_ME(PMQ, close,              arginfo_pmq_class_close, ZEND_ACC_PUBLIC)
	PHP_ME(PMQ, unlink,             arginfo_pmq_class_unlink, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */

/* {{{ pmq_module_entry
 */

zend_module_entry pmq_module_entry = {
	STANDARD_MODULE_HEADER,
	"pmq",                    /* Extension name */
        pmq_functions,			/* zend_function_entry */
	PHP_MINIT(pmq),			/* PHP_MINIT - Module initialization */
	NULL,				/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(pmq),			/* PHP_RINIT - Request initialization */
	NULL,				/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(pmq),			/* PHP_MINFO - Module info */
	PHP_PMQ_VERSION,                /* Version */
	STANDARD_MODULE_PROPERTIES
};

/* }}} */

#ifdef COMPILE_DL_PMQ
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(pmq)
#endif

/* {{{ PHP_MINIT_FUNCTION(pmq)
 */

PHP_MINIT_FUNCTION(pmq)
{
        zend_class_entry ce_pmq;

        INIT_CLASS_ENTRY(ce_pmq, "PMQ", pmq_methods);
	ce_pmq.create_object = pmq_object_init;
	pmq_ce = zend_register_internal_class_ex(&ce_pmq, NULL);
	memcpy(&pmq_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));

        REGISTER_LONG_CONSTANT("PMQ_CREAT", O_CREAT, CONST_CS | CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("PMQ_EXCL", O_EXCL, CONST_CS | CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("PMQ_RDONLY", O_RDONLY, CONST_CS | CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("PMQ_WRONLY", O_WRONLY, CONST_CS | CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("PMQ_RDWR", O_RDWR, CONST_CS | CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("PMQ_NONBLOCK", O_NONBLOCK, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;

}

/* }}} */

/* {{{ pmq_object_init(ce)
 */
static zend_object *pmq_object_init(zend_class_entry *ce) /* {{{ */
{
	php_pmq_obj *intern = zend_object_alloc(sizeof(php_pmq_obj), ce);

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &pmq_object_handlers;

	return &intern->std;
}
/* }}} */

/* {{{ php_pmq_initialize(*pmqobj, *name)
 */

PHPAPI int php_pmq_initialize(php_pmq_obj *pmqobj, /*const*/ char *name, mqd_t queue, int msgsize)
{
        pmqobj->name = name;
        pmqobj->queue = queue;
        pmqobj->msgsize = msgsize;

	return 1;
}

/* }}} */

/* {{{ mqd_t defaultQueue(char *name)
 */
mqd_t defaultQueue(char *name)
{

    mqd_t queue;
    struct mq_attr mqAttr;
    mqAttr.mq_maxmsg = 10;    //Linux and FreeBSD defaults
    mqAttr.mq_msgsize = 8192; //Linux and FreeBSD defaults
    mode_t mode = 0600;

    if((int)(queue = mq_open(name, O_EXCL | O_CREAT | O_RDWR, mode, &mqAttr)) == -1)
    {

        if(errno == EEXIST)
        {

            if((int)(queue = mq_open(name, O_RDWR)) == -1)
            {
                zend_throw_exception(zend_ce_exception, strerror(errno),  0);
            }

        }
        else
        {
            zend_throw_exception(zend_ce_exception, strerror(errno),  0);
        }

    }

    return queue;

}

/* }}} */

/* {{{ mqd_t customQueue(char *name, int flags, mode_t mode, int maxmsg, int maxsize )
 */
mqd_t customQueue(char *name, int flags, mode_t mode, int maxmsg, int maxsize )
{
    mqd_t queue;
    struct mq_attr mqAttr;
    mqAttr.mq_maxmsg = maxmsg;
    mqAttr.mq_msgsize = maxsize;

    if((int)(queue = mq_open(name, flags, mode, &mqAttr)) == -1)
    {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }

    return queue;

}

/* }}} */
