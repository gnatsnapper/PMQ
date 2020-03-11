--TEST--
Test opening a queue with exclusive
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $pmq1 = new PMQ($name,"r");

    try {

    $pmq2 = new PMQ($name,"rx");

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo $pmq1->close() . PHP_EOL;
    echo $pmq1->unlink() . PHP_EOL;

?>
--EXPECT--
File exists
1
1
