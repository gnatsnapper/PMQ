#PMQ
======

PMQ is a php extension for interacting with posix message queues.  Posix message queues are updated alternatives for sysv message queues, i.e. low-latency kernel-based interprocess communication, see examples for typical uses.

## Install

```
$ phpize
$ ./configure
$ make && make install
```

## Procedural Functions
### Create Queue

```php
$mq = pmq_open("/myqueue","w+");
```
To follow posix message queue convention the queue name must begin with a forward slash, contain no further slashes and be less 255 characters.

Flags passed are inspired fopen's format:
* r - read only
* w - write only
* r+/w+ - read and write
* x - exclusive (do not create if file exists)
* e - close on exec
* n - non-blocking mode

Queue permissions can be set in umask format, maximum messages in queue, message size (bytes) can also be set, if the users permissions allow:
```php
$mq = pmq_open("/myqueue","w+",0666,10,8192);
```

### Send Message

```php
pmq_send($mq,"hello");
```

If the queue is full and blocking is set on the connection pmq_send will block until there is space on the queue, add a timeout (seconds) to avoid blocking:

```php
pmq_send($mq,"hello",5);
```
The funcion pmq_send will return false if the timeout is reached.

### Receive Message

```php
echo pmq_receive($mq);
```
If there are no messages and blocking is set on the connection pmq_receive will block until there are messages on the queue, add a timeout (seconds) to avoid blocking:

```php
pmq_receive($mq,5);
```
The function pmq_receive will return false if the timeout is reached.

### Close Queue

```php
echo pmq_close($mq);
```

### Delete Queue

```php
echo pmq_unlink($mq);
```

## Object Orientated
### Create Queue

```php
$pmq = new PMQ("/myqueue","w+");
```
Flags passed are inspired fopen's format:
* r - read only
* w - write only
* r+/w+ - read and write
* x - exclusive (do not create if file exists)
* e - close on exec
* n - non-blocking mode


Queue permissions can be set in umask format, maximum messages, message size (bytes) can also be set, according if the users permissions allow:

```php
$pmq = new PMQ("/myqueue","w+",0666,10,8192);
```

### Send Message

```php
$pmq->send("hello");
```

If the queue is full and blocking is set on the connection PMQ::send will block until there is space on the queue, add a timeout (seconds) to avoid blocking:

```php
$pmq->send("hello",5);
```
PMQ::send will return false if the timeout is reached.

### Receive Message

```php
$pmq->receive();
```

If there are no messages and blocking is set on the connection PMQ::receive will block until there are message on the queue, add a timeout (seconds) to avoid blocking:

```php
$pmq->receive(5);
```
PMQ::receive will return false if the timeout is reached.


### Close Queue

```php
$pmq->close();
```

### Delete Queue

```php
$pmq->unlink();
```

## Troubleshooting
###Bad file descriptor
This occurs where attempting to send to a queue with only read access, or vice versa.

###Function not implemented
If PMQ fails with 'Function not implemented' the kernel module may be disabled.
On Linux, compile with:

```bash
CONFIG_POSIX_MQUEUE=Y
```

On FreeBSD:
```bash
kldload mqueuefs
```

