--TEST--
Priority Send Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 print 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq = new PMQ($name,PMQ_CREAT|PMQ_RDWR,0600,5,1024);
$start = time();
var_dump( $pmq->send("hello1", 1) );
var_dump( $pmq->send("hello2", 3) );
var_dump( $pmq->send("hello3", 2) );
var_dump( $pmq->receive( ) );
var_dump( $pmq->receive( ) );
var_dump( $pmq->receive( ) );
var_dump( $pmq->unlink() );
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
string(6) "hello2"
string(6) "hello3"
string(6) "hello1"
bool(true)
