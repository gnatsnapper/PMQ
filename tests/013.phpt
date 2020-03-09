--TEST--
Queue Attribute Exceptions (OOP)
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $message  = bin2hex(random_bytes(57));
    $pmq = new PMQ($name,"wn",0666,3,112);

    echo $pmq->send("hi") . PHP_EOL;
    echo $pmq->send("hi") . PHP_EOL;
    echo $pmq->send("hi") . PHP_EOL;
    if (!$pmq->send("hi")) { echo 1 . PHP_EOL; }

    try {

    $pmq->send($message);

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo $pmq->close() . PHP_EOL;
    echo $pmq->unlink() . PHP_EOL;

?>
--EXPECT--
1
1
1
1
Message too long
1
1
