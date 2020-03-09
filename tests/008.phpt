--TEST--
W Exceptions (Procedural)
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $message = bin2hex(random_bytes(4097));
    $pmq = pmq_open($name,"w");

    try {

    echo pmq_send($pmq,$message).PHP_EOL;

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    try {

    echo pmq_receive($pmq).PHP_EOL;

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo pmq_close($pmq) . PHP_EOL;
    echo pmq_unlink($name) . PHP_EOL;

?>
--EXPECT--
Message too long
Bad file descriptor
1
1
