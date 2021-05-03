[![Build Status](https://travis-ci.org/gnatsnapper/PosixMessageQueue.svg?branch=master)](https://travis-ci.org/gnatsnapper/PosixMessageQueue)

PosixMessageQueue
======

PosixMessageQueue is a php extension for interacting with posix message queues.  Posix message queues are updated alternatives for sysv message queues, i.e. low-latency kernel-based interprocess communication, see examples for typical uses. Tested on PHP7.4 on linux and bsd.

![PosixMessageQueue Example](/img/PosixMessageQueue.gif)

## Install

```
$ phpize
$ ./configure
$ make && make install
```

## Usage

### Create Queue

```php
$pmq = new PosixMessageQueue("/myqueue");
```
PosixMessageQueue with only a queue name will attempt to create a new or join an existing queue with read/write permissions.  As per the standard, queue names must begin with a single forward slash (/) and be less than 255 characters.

PMQ's uses O flags formatting:
* ```PMQ_RDONLY``` - (```O_RDONLY```) read only
* ```PMQ_WRONLY``` - (```O_WRONLY```) write only
* ```PMQ_RDWR``` - (```O_RDWR```) read and write
* ```PMQ_CREAT``` - (```O_CREAT```) create
* ```PMQ_EXCL``` - (```O_EXCL```) exclusive (do not create if file exists)
* ```PMQ_NONBLOCK``` - (```O_NONBLOCK```) non-blocking mode

Queue permissions can be set in umask format, maximum messages, message size (bytes) can also be set, according if the users permissions allow:

```php
$pmq = new PosixMessageQueue("/myqueue", PosixMessageQueue_CREAT | PosixMessageQueue_RDWR, 0666, 10, 8192);
```

### Send Message

```php
$pmq->send("hello");
```

Message priority can be set, the queue is sorted with messages with greater priority being received first:

```php
$pmq->send("gooodbye",3);
$pmq->send("hello",4);
```
In the example above "hello" will be received first".

If the queue is full and blocking is set (```PMQ_NONBLOCK``` not passed) the connection PosixMessageQueue::send will block until there is space in the queue, add a timeout (seconds) to avoid blocking:

```php
$pmq->send("hello",0,5);
```
 PosixMessageQueue::send will return false if the timeout is reached. A value of 0 gives no timeout.

### Receive Message

```php
$pmq->receive();
```

If there are no messages and blocking is set (```PMQ_NONBLOCK``` not passed) on the connection PosixMessageQueue::receive will block until there is a message on the queue, add a timeout (seconds) to avoid blocking:


```php
$pmq->receive(5);
```
 PosixMessageQueue::receive will return false if the timeout is reached.


### Close Queue

```php
$pmq->close();
```

### Delete Queue

```php
$pmq->unlink();
```
Queues not deleted will persist until computer is restarted, so always delete if not needed by another process.

## Troubleshooting

### Bad file descriptor
This occurs where attempting to send to a queue with only read access, or vice versa.

### Function not implemented

If PosixMessageQueue fails with 'Function not implemented' the kernel module may be disabled.
On Linux, compile kernel with:

```bash
CONFIG_POSIX_MQUEUE=Y
```

On FreeBSD:
```bash
kldload mqueuefs
```

