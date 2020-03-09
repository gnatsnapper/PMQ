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

    try {
    $pmq = pmq_open($name,"wn",0666,11,8193);
    }
    catch(Exception $e)
    {
        echo $e->getMessage() . PHP_EOL;
    }

    try {
        echo pmq_unlink($name) . PHP_EOL;
    }
    catch(Exception $e)
    {
        echo $e->getMessage() . PHP_EOL;
    }

?>
--EXPECT--
Invalid argument
No such file or directory
