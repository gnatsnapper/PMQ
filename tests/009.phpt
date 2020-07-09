--TEST--
Non-blocking Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 var_dump( 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq = new PosixMessageQueue($name,PMQ_CREAT|PMQ_WRONLY|PMQ_NONBLOCK,0600,2,1024);

var_dump( $pmq->send("hello1") );
var_dump( $pmq->send("hello2") );
var_dump( $pmq->send("hello3") );//would normally block
var_dump( $pmq->unlink() );
?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(true)
