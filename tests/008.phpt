--TEST--
Test writing to a read-only queue
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $message = bin2hex(random_bytes(8));
    $pmq = new PMQ($name,"r");

    try {

    echo $pmq->send($message).PHP_EOL;

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo $pmq->close().PHP_EOL;
    echo $pmq->unlink().PHP_EOL;

?>
--EXPECT--
Bad file descriptor
1
1
