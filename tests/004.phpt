--TEST--
Default info Test
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
var_dump( $pmq->info() );
var_dump( $pmq->send("hello") );
var_dump( $pmq->info() );
var_dump( $pmq->receive() );
var_dump( $pmq->info() );
var_dump( $pmq->unlink() );
?>
--EXPECT--
array(4) {
  ["flags"]=>
  int(0)
  ["maxmsg"]=>
  int(10)
  ["msgsize"]=>
  int(8192)
  ["curmsgs"]=>
  int(0)
}
bool(true)
array(4) {
  ["flags"]=>
  int(0)
  ["maxmsg"]=>
  int(10)
  ["msgsize"]=>
  int(8192)
  ["curmsgs"]=>
  int(1)
}
string(5) "hello"
array(4) {
  ["flags"]=>
  int(0)
  ["maxmsg"]=>
  int(10)
  ["msgsize"]=>
  int(8192)
  ["curmsgs"]=>
  int(0)
}
bool(true)
