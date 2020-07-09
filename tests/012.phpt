--TEST--
Timed Receive Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 var_dump( 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq = new PosixMessageQueue($name,PMQ_CREAT|PMQ_RDWR,0600,2,1024);

$start=microtime(true);
var_dump( $pmq->receive(1) );//block until timeout
$duration = microtime(true) - $start;
var_dump( ($duration > 1.0) );
var_dump( ($duration < 1.001) );
var_dump( $pmq->unlink() );
?>
--EXPECT--
bool(false)
bool(true)
bool(true)
bool(true)
