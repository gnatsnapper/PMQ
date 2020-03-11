--TEST--
Test setting message priority
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $messageA = "Message A";
    $messageB = "Message B";
    $messageC = "Message C";
    $pmq = new PMQ($name,"w+");
    echo $pmq->send($messageC,0,1).PHP_EOL;
    echo $pmq->send($messageB,0,2).PHP_EOL;
    echo $pmq->send($messageA,0,3).PHP_EOL;
    echo ($pmq->receive() === $messageA) . PHP_EOL;
    echo ($pmq->receive() === $messageB) . PHP_EOL;
    echo ($pmq->receive() === $messageC) . PHP_EOL;
    echo $pmq->close().PHP_EOL;
    echo $pmq->unlink().PHP_EOL;

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
