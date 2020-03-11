--TEST--
Test info method
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $pmq = new PMQ($name,"w+");

    $infoA = $pmq->info();

    echo ($infoA['mq_flags'] === 0) . PHP_EOL;
    echo ($infoA['mq_maxmsg'] === 10) . PHP_EOL;
    echo ($infoA['mq_msgsize'] === 8192) . PHP_EOL;
    echo ($infoA['mq_curmsgs'] === 0) . PHP_EOL;

    echo $pmq->send("fsjdfsjdf",0,1) . PHP_EOL;

    $infoB = $pmq->info();

    echo ($infoB['mq_flags'] === 0) . PHP_EOL;
    echo ($infoB['mq_maxmsg'] === 10) . PHP_EOL;
    echo ($infoB['mq_msgsize'] === 8192) . PHP_EOL;
    echo ($infoB['mq_curmsgs'] === 1) . PHP_EOL;

    echo $pmq->close() . PHP_EOL;
    echo $pmq->unlink() . PHP_EOL;

?>
--EXPECT--
1
1
1
1
1
1
1
1
1
1
1
