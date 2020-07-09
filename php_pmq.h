/* pmq extension for PHP */

#ifndef PHP_PMQ_H
# define PHP_PMQ_H

extern zend_module_entry pmq_module_entry;
# define phpext_pmq_ptr &pmq_module_entry

# define PHP_PMQ_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PMQ)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

PHP_FUNCTION(pmq_info);
PHP_METHOD(PosixMessageQueue, __construct);
PHP_METHOD(PosixMessageQueue, send);
PHP_METHOD(PosixMessageQueue, receive);
PHP_METHOD(PosixMessageQueue, unlink);
PHP_METHOD(PosixMessageQueue, info);

PHP_RINIT_FUNCTION(pmq);
PHP_RSHUTDOWN_FUNCTION(pmq);
PHP_MINIT_FUNCTION(pmq);
PHP_MSHUTDOWN_FUNCTION(pmq);
PHP_MINFO_FUNCTION(pmq);

typedef struct _php_pmq_obj php_pmq_obj;

struct _php_pmq_obj {
        char *name;
        mqd_t queue;
        int msgsize;
	zend_object   std;
};

static zend_object *pmq_object_init(zend_class_entry *class_type);
PHPAPI int php_pmq_initialize(php_pmq_obj *pmqobj, char *name, mqd_t queue, int msgsize);
static inline php_pmq_obj *php_pmq_obj_from_obj(zend_object *obj) {
	return (php_pmq_obj*)((char*)(obj) - XtOffsetOf(php_pmq_obj, std));
}


mqd_t defaultQueue(char *name);
mqd_t customQueue(char *name, int flags, mode_t mode, int maxmsg, int maxsize );

#define Z_PHPPMQ_P(zv)  php_pmq_obj_from_obj(Z_OBJ_P((zv)))

#endif	/* PHP_PMQ_H */
