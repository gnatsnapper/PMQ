--TEST--
Timed Send Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 print 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq = new PosixMessageQueue($name,PMQ_CREAT|PMQ_WRONLY,0600,2,1024);

var_dump( $pmq->send("hello1") );
var_dump( $pmq->send("hello2") );
$start=microtime(true);
var_dump( $pmq->send("hello3",1,1) );//block until timeout
$duration = microtime(true) - $start;
var_dump( ($duration > 1.0) );
var_dump( ($duration < 1.002) );
var_dump( $pmq->unlink() );
?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
