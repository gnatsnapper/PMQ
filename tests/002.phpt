--TEST--
Default instantiation and unlink Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 print 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq = new PosixMessageQueue($name);
var_dump( $pmq->unlink() );
?>
--EXPECT--
bool(true)
