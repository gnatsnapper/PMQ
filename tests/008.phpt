--TEST--
Msg Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 var_dump( 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq = new PMQ($name,PMQ_CREAT|PMQ_WRONLY,0600,2,1024);
var_dump( $pmq->info() );
var_dump( $pmq->unlink() );
?>
--EXPECT--
array(4) {
  ["flags"]=>
  int(0)
  ["maxmsg"]=>
  int(2)
  ["msgsize"]=>
  int(1024)
  ["curmsgs"]=>
  int(0)
}
bool(true)
