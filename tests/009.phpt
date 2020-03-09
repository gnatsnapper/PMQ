--TEST--
W Exceptions (OOP)
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
    $pmq = new PMQ($name,"w");

    try {

    echo $pmq->send($message).PHP_EOL;

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    try {

    echo $pmq->receive().PHP_EOL;

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }


    echo $pmq->close().PHP_EOL;
    echo $pmq->unlink().PHP_EOL;

?>
--EXPECT--
Message too long
Bad file descriptor
1
1
