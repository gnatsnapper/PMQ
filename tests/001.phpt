--TEST--
Open Message Queue
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue001_'.bin2hex(random_bytes(8));
    if(is_int($queue = pmq_open($name,'w+',FALSE)))
    {
        echo 1 . PHP_EOL;
    }
    echo pmq_close($queue) . PHP_EOL;
    echo pmq_unlink($name) . PHP_EOL;
?>
--EXPECT--
1
1
1
