/* pmq extension for PHP */

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
        zend_long mode = 0666;//PMQ_DEF_MODE;
        zend_long maxmsg = 10;//PMQ_DEF_MAXMSG;
        zend_long msgsize = 1024;//PMQ_DEF_MSGSIZE;
        struct mq_attr mqAttr;
        int oflag;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

        mqAttr.mq_maxmsg = (int)maxmsg;
        mqAttr.mq_msgsize = (int)msgsize;
        oflag = O_CREAT | O_RDWR;

        mqd_t queue = mq_open(ZSTR_VAL(name), oflag, (mode_t)mode, &mqAttr);

       if((int)queue == -1)
       {

            zend_throw_exception(zend_ce_exception, strerror(errno),  0);

       }



	php_pmq_initialize(Z_PHPPMQ_P(ZEND_THIS), ZSTR_VAL(name), queue);
}
/* }}} */

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
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_info, 0)
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
	PHP_ME(PMQ, info,    arginfo_pmq_class_info, ZEND_ACC_PUBLIC)
	PHP_ME(PMQ, unlink,    arginfo_pmq_class_unlink, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/* {{{ pmq_module_entry
 */
zend_module_entry pmq_module_entry = {
	STANDARD_MODULE_HEADER,
	"pmq",	/* Extension name */
        pmq_functions,			/* zend_function_entry */
	PHP_MINIT(pmq),			/* PHP_MINIT - Module initialization */
	NULL,					/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(pmq),			/* PHP_RINIT - Request initialization */
	NULL,					/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(pmq),			/* PHP_MINFO - Module info */
	PHP_PMQ_VERSION,/* Version */
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
//	pmq_object_handlers_pmq.offset = XtOffsetOf(php_pmq_obj, std);
//	pmq_object_handlers_pmq.free_obj = pmq_object_free_storage;
//	pmq_object_handlers_pmq.clone_obj = pmq_object_clone;
//	pmq_object_handlers_pmq.dtor_obj = pmq_object_destroy;
//	pmq_object_handlers_pmq.compare_objects = pmq_object_compare;
//	pmq_object_handlers_pmq.get_properties_for = pmq_object_get_properties_for;
//	pmq_object_handlers_pmq.get_gc = pmq_object_get_gc;

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
PHPAPI int php_pmq_initialize(php_pmq_obj *pmqobj, /*const*/ char *name, mqd_t queue)
{
        pmqobj->name = name;
        pmqobj->queue = queue;

	return 1;
}
/* }}} */

