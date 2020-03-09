--TEST--
R Exceptions (Procedural)
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
    $pmq1 = pmq_open($name,"r");

    try {

    $pmq2 = pmq_open($name,"rx");

    }

    catch (Exception $e)

    {

    echo $e->getMessage() . PHP_EOL;

    }

    echo pmq_close($pmq1) . PHP_EOL;
    echo pmq_unlink($name) . PHP_EOL;

?>
--EXPECT--
File exists
1
1
