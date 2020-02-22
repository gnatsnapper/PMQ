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
    
    $name = '/testqueue002_'.bin2hex(random_bytes(8));
    $sent_message = bin2hex(random_bytes(8));
    if(is_int($queue = pmq_open($name,'rwc')))
    {
        echo 1 . PHP_EOL;
    }
    if(pmq_send($queue,$sent_message))
    {
        echo 1 . PHP_EOL;
    }
    if($received_message = pmq_receive($queue))
    {
        echo ($sent_message === $received_message) . PHP_EOL;
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
