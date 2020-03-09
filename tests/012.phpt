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
    $pmq = pmq_open($name,"wn",0666,3,112);

    echo pmq_send($pmq,"hi") . PHP_EOL;
    echo pmq_send($pmq,"hi") . PHP_EOL;
    echo pmq_send($pmq,"hi") . PHP_EOL;
    if (!pmq_send($pmq,"hi")) { echo 1 . PHP_EOL; }

    try {

    pmq_send($pmq,$message);

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo pmq_close($pmq) . PHP_EOL;
    echo pmq_unlink($name) . PHP_EOL;

?>
--EXPECT--
1
1
1
1
Message too long
1
1
