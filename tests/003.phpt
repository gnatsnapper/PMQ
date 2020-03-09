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

    $name = '/testqueue003_'.bin2hex(random_bytes(8));
    $sent_message = bin2hex(random_bytes(8));
    if(is_int($queue = pmq_open($name,'w+')))
    {
        echo 1 . PHP_EOL;
    }

    $s = microtime(TRUE);
    if(!$received_message = pmq_receive($queue,1))
    {
        echo 1 . PHP_EOL;
        $d = (microtime(TRUE) - $s);
        if(($d > 1) && ($d < 1.1))
        {
            echo 1 . PHP_EOL;
        }
    }

    echo pmq_close($queue) . PHP_EOL;
    echo pmq_unlink($name) . PHP_EOL;
?>
--EXPECT--
1
1
1
1
1
