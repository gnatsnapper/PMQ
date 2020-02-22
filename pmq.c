/* pmq extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_pmq.h"
#include "zend_exceptions.h"

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>


/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

  zend_class_entry *php_pmq_ce; 
  zend_object_handlers *pmq_object_handlers;
   
/* {{{ arginfo
 */

//PMQ Functions
ZEND_BEGIN_ARG_INFO(arginfo_pmq_open, 0)
    ZEND_ARG_INFO(0, name) 
    ZEND_ARG_INFO(0, mode) 
    ZEND_ARG_INFO(0, block) 
    ZEND_ARG_INFO(0, permissions) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_close, 0)
    ZEND_ARG_INFO(0, queue) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_unlink, 0)
    ZEND_ARG_INFO(0, name) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_send, 0)
    ZEND_ARG_INFO(0, queue) 
    ZEND_ARG_INFO(0, message) 
    ZEND_ARG_INFO(0, timeout) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_receive, 0)
    ZEND_ARG_INFO(0, queue) 
    ZEND_ARG_INFO(0, timeout) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_info, 0)
    ZEND_ARG_INFO(0, queue) 
ZEND_END_ARG_INFO()

//PMQ Methods
ZEND_BEGIN_ARG_INFO(arginfo_pmq_class___construct, 0)
    ZEND_ARG_INFO(0, name) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_send, 0)
    ZEND_ARG_INFO(0, message) 
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_receive, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_close, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_unlink, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmq_class_info, 0)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ pmq_functions[]
 */
static const zend_function_entry pmq_functions[] = {
    PHP_FE(pmq_open,		arginfo_pmq_open)
    PHP_FE(pmq_close,		arginfo_pmq_close)
    PHP_FE(pmq_unlink,		arginfo_pmq_unlink)
    PHP_FE(pmq_send,		arginfo_pmq_send)
    PHP_FE(pmq_receive,		arginfo_pmq_receive)
    PHP_FE(pmq_info,		arginfo_pmq_info)
    PHP_FE_END
};
/* }}} */

/* {{{ pmq_methods[]
 */

static const zend_function_entry pmq_methods[] =
{
    PHP_ME(PMQ, __construct,  arginfo_pmq_class___construct, ZEND_ACC_PUBLIC)
    PHP_ME(PMQ, send,         arginfo_pmq_class_send, ZEND_ACC_PUBLIC)
    PHP_ME(PMQ, receive,      arginfo_pmq_class_receive, ZEND_ACC_PUBLIC)
    PHP_ME(PMQ, close,        arginfo_pmq_class_close, ZEND_ACC_PUBLIC)
    PHP_ME(PMQ, unlink,       arginfo_pmq_class_unlink, ZEND_ACC_PUBLIC)
    PHP_ME(PMQ, info,         arginfo_pmq_class_info, ZEND_ACC_PUBLIC)
};

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pmq)
{
    zend_class_entry pmq;
    INIT_CLASS_ENTRY(pmq, "PMQ", pmq_methods);

    php_pmq_ce = zend_register_internal_class_ex(&pmq, NULL);

    return SUCCESS;
}

/* }}} */


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
        struct mq_attr attr = getDefaultAttr();
	php_info_print_table_start();
	php_info_print_table_row(2, "pmq support", "enabled");
	php_info_print_table_row(2, "Default Message Size", (const char*)attr.mq_msgsize);
	php_info_print_table_row(2, "Default Maximum Message in queue", (const char*)attr.mq_maxmsg);
	php_info_print_table_end();
}
/* }}} */


/* {{{ pmq_module_entry
 */

zend_module_entry pmq_module_entry = {
	STANDARD_MODULE_HEADER,
	"pmq",			/* Extension name */
	pmq_functions,		/* zend_function_entry */
	PHP_MINIT(pmq),       	/* PHP_MINIT - Module initialization */
	NULL,			/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(pmq),		/* PHP_RINIT - Request initialization */
	NULL,			/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(pmq),		/* PHP_MINFO - Module info */
	PHP_PMQ_VERSION,	/* Version */
	STANDARD_MODULE_PROPERTIES
};

/* }}} */

#ifdef COMPILE_DL_PMQ
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(pmq)
#endif

/* {{{ int pmq_open( string name, string mode [, bool blocking = TRUE [, int permission = 666 ]] )
 */
PHP_FUNCTION(pmq_open)
{

     struct mq_attr mqAttr = getDefaultAttr();

     zend_string *name; 
     zend_string *mode; 
     int oflag;
     zend_bool block = 1;
     zend_long permissions = 666;

     ZEND_PARSE_PARAMETERS_START(2, 4)
              Z_PARAM_STR(name)
              Z_PARAM_STR(mode)
              Z_PARAM_OPTIONAL
              Z_PARAM_BOOL(block)
              Z_PARAM_LONG(permissions)
     ZEND_PARSE_PARAMETERS_END();

     oflag = getMode((const char *)ZSTR_VAL(mode));
    
     if(block == 0)
     {
        oflag = oflag + O_NONBLOCK;
     }

     mqd_t queue = mq_open(ZSTR_VAL(name), oflag, (int)permissions, &mqAttr);

     if((int)queue == -1)
     {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
     }

     RETURN_LONG((int) queue)
 
}
/* }}} */

/* {{{ bool pmq_close()
 */
PHP_FUNCTION(pmq_close)
{
    zend_long queue;
    ZEND_PARSE_PARAMETERS_START(1, 1)
              Z_PARAM_LONG(queue)
    ZEND_PARSE_PARAMETERS_END();

    int retval = mq_close((mqd_t)queue); 

    if(retval == -1)
    {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }
    else
    {
        RETURN_TRUE;
    }

}
/* }}} */

/* {{{ bool pmq_unlink()
 */
PHP_FUNCTION(pmq_unlink)
{

    zend_string *name;
    ZEND_PARSE_PARAMETERS_START(1, 1)
             Z_PARAM_STR(name)
    ZEND_PARSE_PARAMETERS_END();

    int retval = mq_unlink((const char *)ZSTR_VAL(name));
    
    if(retval == -1)
    {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }
    else
    {
        RETURN_TRUE;
    }

}
/* }}} */

/* {{{ bool pmq_send( int queue, string message [, int timeout ] )
 */
PHP_FUNCTION(pmq_send)
{

    zend_string *message;
    zend_long queue;
    zend_long timeout = 0;
    int retval;
    ZEND_PARSE_PARAMETERS_START(2, 3)
             Z_PARAM_LONG(queue)
             Z_PARAM_STR(message)
             Z_PARAM_OPTIONAL
             Z_PARAM_LONG(timeout)
    ZEND_PARSE_PARAMETERS_END();

    char *send_ptr = (char *)ZSTR_VAL(message); 
    size_t send_len = ZSTR_LEN(message) + 1; 

    if(timeout > 0)
    {

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec = (time_t)(int)ts.tv_sec + (int)timeout;

    retval = mq_timedsend((mqd_t)queue,send_ptr,send_len,1,&ts); 

        if(retval == -1)
        {
            RETURN_FALSE;
        }

    }
    else
    {

    retval = mq_send((mqd_t)queue,send_ptr,send_len,1); 

    }

    if(retval == -1)
    {

        if(errno == EAGAIN)
        {
        RETURN_FALSE;
        }

        zend_throw_exception(zend_ce_exception, strerror(errno),  0);

    }
    else
    {
        RETURN_TRUE;
    }

}
/* }}} */

/* {{{ string pmq_receive( int queue [, int timeout ] )
 */
PHP_FUNCTION(pmq_receive)
{
    zend_long queue;
    zend_string *msg;
    char message[8192];
    size_t len = 8192;
    unsigned int *priority;
    zend_long timeout = 0;
    int retval;
    ZEND_PARSE_PARAMETERS_START(1, 2)
            Z_PARAM_LONG(queue)
            Z_PARAM_OPTIONAL
            Z_PARAM_LONG(timeout)
    ZEND_PARSE_PARAMETERS_END();


    if(timeout > 0)
    {

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec = (time_t)(int)ts.tv_sec + (int)timeout;

    retval = mq_timedreceive((mqd_t)queue, message, len, priority,&ts); 

        if(retval == -1)
        {
            RETURN_FALSE;
        }

    }
    else
    {

    retval = mq_receive((mqd_t)queue, message, len, priority);

    }

    if(retval == -1)
    {
        if(errno == EAGAIN)
        {
        RETURN_FALSE;
        }

        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }
        RETURN_STRING(message);
    
}
/* }}} */

/* {{{ array pmq_info( int queue )
 */
PHP_FUNCTION(pmq_info)
{
    zend_long queue;
    //zend_array info;

    struct mq_attr gAttr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
             Z_PARAM_LONG(queue)
    ZEND_PARSE_PARAMETERS_END();

    int retval = mq_getattr((mqd_t)queue, &gAttr);

    if(retval == -1)
    {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }

        array_init(return_value); 
          
        add_assoc_long(return_value, "mq_flags", gAttr.mq_flags); 
        add_assoc_long(return_value, "mq_maxmsg", gAttr.mq_maxmsg); 
        add_assoc_long(return_value, "mq_msgsize", gAttr.mq_msgsize); 
        add_assoc_long(return_value, "mq_curmsgs", gAttr.mq_curmsgs); 

}
/* }}} */


/* {{{ void PMQ::__construct(string $name)
 */
PHP_METHOD(PMQ, __construct)
{
   zend_string *name; 

   ZEND_PARSE_PARAMETERS_START(1, 1)
   Z_PARAM_STR(name)
   ZEND_PARSE_PARAMETERS_END();

   struct mq_attr mqAttr = getDefaultAttr();

   mqd_t queue = mq_open(ZSTR_VAL(name), O_RDWR | O_CREAT, 0777, &mqAttr);

   if((int)queue == -1)
   {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
   }

   php_pmq_init(Z_PHPPMQ_P(getThis()), ZSTR_VAL(name), queue);
   php_pmq_obj *pmq = Z_PHPPMQ_P(getThis());
}
/* }}} */

/* {{{ void PMQ::send(string $message [, int $timeout] )
 */
PHP_METHOD(PMQ,send)
{
   zend_long queue; 
   zend_long timeout; 
   zend_string *message; 
   int retval;
   
   ZEND_PARSE_PARAMETERS_START(1, 2)
                Z_PARAM_STR(message)
                Z_PARAM_OPTIONAL
                Z_PARAM_LONG(timeout)
   ZEND_PARSE_PARAMETERS_END();

   php_pmq_obj *pmq = Z_PHPPMQ_P(getThis());

    if(timeout > 0)
    {

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec = (time_t)(int)ts.tv_sec + (int)timeout;

    retval = mq_timedsend(pmq->queue,ZSTR_VAL(message),ZSTR_LEN(message)+1,1,&ts); 

        if(retval == -1)
        {
            RETURN_FALSE;
        }

    }
    else
    {

        retval = mq_send(pmq->queue,ZSTR_VAL(message),ZSTR_LEN(message)+1,1); 

    }

   if(retval == -1)
   {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
   }
   RETURN_TRUE;
}
/* }}} */

/* {{{ string PMQ::receive([ int $timeout])
 */

PHP_METHOD(PMQ, receive)
{
   zend_long queue; 
   zend_string *msg;
   char message[8192];
   size_t len = 8192;
   int retval;
   unsigned int *priority;
   zend_long timeout; 
   ZEND_PARSE_PARAMETERS_START(0, 1)
       Z_PARAM_OPTIONAL
       Z_PARAM_LONG(timeout)
   ZEND_PARSE_PARAMETERS_END();
   php_pmq_obj *pmq = Z_PHPPMQ_P(getThis());

   if(timeout > 0)
   {

   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   ts.tv_sec = (time_t)(int)ts.tv_sec + (int)timeout;

   retval = mq_timedreceive(pmq->queue, message, len, priority ,&ts); 

        if(retval == -1)
        {
            RETURN_FALSE;
        }

    }
    else
    {

       retval = mq_receive(pmq->queue, message, len, priority); 

    }

   if(retval == -1)
   {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
   }
        RETURN_STRING(message);
}
/* }}} */

/* {{{ bool PMQ::close()
 */
PHP_METHOD(PMQ,close)
{

   ZEND_PARSE_PARAMETERS_NONE();

   php_pmq_obj *pmq = Z_PHPPMQ_P(getThis());
   int retval = mq_close(pmq->queue); 

   if(retval == -1)
   {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
   }

   RETURN_TRUE;

}
/* }}} */

/* {{{ bool PMQ::unlink()
 */
PHP_METHOD(PMQ,unlink)
{

   ZEND_PARSE_PARAMETERS_NONE();

   php_pmq_obj *pmq = Z_PHPPMQ_P(getThis());
   int retval = mq_unlink(pmq->name); 

   if(retval == -1)
   {
        zend_throw_exception(zend_ce_exception, strerror(errno),  0);
   }

   RETURN_TRUE;

}

/* }}} */

/* {{{ array PMQ::info()
 */
PHP_METHOD(PMQ,info)
{

    ZEND_PARSE_PARAMETERS_NONE();

    php_pmq_obj *pmq = Z_PHPPMQ_P(getThis());
    struct mq_attr gAttr;
    int retval = mq_getattr(pmq->queue, &gAttr);
   
    if(retval == -1)
    {
         zend_throw_exception(zend_ce_exception, strerror(errno),  0);
    }

    array_init(return_value); 
              
    add_assoc_long(return_value, "mq_flags", gAttr.mq_flags); 
    add_assoc_long(return_value, "mq_maxmsg", gAttr.mq_maxmsg); 
    add_assoc_long(return_value, "mq_msgsize", gAttr.mq_msgsize); 
    add_assoc_long(return_value, "mq_curmsgs", gAttr.mq_curmsgs); 

}
/* }}} */

/* {{{ Internal functions
 */
PHPAPI int php_pmq_init(php_pmq_obj *pmqobj, char *name, mqd_t queue )
{
        pmqobj->queue = queue;
        pmqobj->name = name;

	return 1;
}

int getMode(const char *input)
{
    int i = 0, mode = 0, read = 0, write = 0, create = 0, exec = 0;

    for(i=0;i<strlen(input);i++)
    {

        if(input[i] == 'r')
        {
            read = O_RDONLY;
        }
        else if(input[i] == 'w')
        {
            write = O_WRONLY;
        }
        else if(input[i] == 'c')
        {
            create = O_CREAT;
        }
        else if(input[i] =='e')
        {
            exec = O_EXCL;
        }

    }

    if((read == O_RDONLY) && (write == O_WRONLY))
    {

        read = 0; 
        write = O_RDWR;

    }

    mode = read + write + create + exec;

    return mode;
}

struct mq_attr getDefaultAttr() {

char * msg_def_file = "/proc/sys/fs/mqueue/msg_default";
char * msgsize_def_file = "/proc/sys/fs/mqueue/msgsize_default";
struct mq_attr mqAttr;

FILE * m = fopen (msg_def_file, "r");
size_t msz = 0;
char * mlin = 0;

do {
   ssize_t mlsz = getline(&mlin, &msz, m);
   if (mlsz<0) break;
   mqAttr.mq_maxmsg = atoi(mlin);
} while (!feof(m));

fclose (m);

FILE * s = fopen (msgsize_def_file, "r");
size_t ssz = 0;
char * slin = 0;

do {
   ssize_t slsz = getline(&slin, &ssz, s);
   if (slsz<0) break;
   mqAttr.mq_msgsize = atoi(slin);
} while (!feof(s));

fclose (s);

return mqAttr;

}

/* }}} */
 


