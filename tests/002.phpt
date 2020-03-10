--TEST--
Test construct, close, unlink
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $pmq = new PMQ($name,"w+");
    echo $pmq->close().PHP_EOL;
    echo $pmq->unlink().PHP_EOL;

?>
--EXPECT--
1
1
