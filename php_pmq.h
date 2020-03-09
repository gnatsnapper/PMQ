/* pmq extension for PHP */

#ifndef PHP_PMQ_H
# define PHP_PMQ_H

# define PHP_PMQ_VERSION "0.1.1"

#include <mqueue.h>

extern zend_module_entry pmq_module_entry;
# define phpext_pmq_ptr &pmq_module_entry

PHP_FUNCTION(pmq_open);
PHP_FUNCTION(pmq_close);
PHP_FUNCTION(pmq_unlink);
PHP_FUNCTION(pmq_send);
PHP_FUNCTION(pmq_receive);
PHP_FUNCTION(pmq_info);
PHP_METHOD(PMQ, __construct);
PHP_METHOD(PMQ,send);
PHP_METHOD(PMQ,receive);
PHP_METHOD(PMQ,close);
PHP_METHOD(PMQ,unlink);
PHP_METHOD(PMQ,info);

typedef struct _php_pmq_obj php_pmq_obj;

struct _php_pmq_obj {
	mqd_t queue;
	char *name;
	zend_object std;
};

PHPAPI int php_pmq_init(php_pmq_obj *pmqobj, char *name, mqd_t queue );
static inline php_pmq_obj *php_pmq_obj_from_obj(zend_object *obj);
static inline php_pmq_obj *php_pmq_obj_from_obj(zend_object *obj) {
	return (php_pmq_obj*)((char*)(obj) - XtOffsetOf(php_pmq_obj, std));
}

#define Z_PHPPMQ_P(zv)  php_pmq_obj_from_obj(Z_OBJ_P((zv)))


# if defined(ZTS) && defined(COMPILE_DL_PMQ)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

//Internal functions
int getFlags(const char *input);

#endif	/* PHP_PMQ_H */

