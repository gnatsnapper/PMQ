--TEST--
RW Exceptions (Procedural)
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
    $pmq = pmq_open($name,"r");

    try {

    echo pmq_send($pmq,$message).PHP_EOL;

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo pmq_close($pmq) . PHP_EOL;
    echo pmq_unlink($name) . PHP_EOL;

?>
--EXPECT--
Bad file descriptor
1
1
