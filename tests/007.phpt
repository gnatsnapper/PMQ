--TEST--
Test send and receive (e+)
--SKIPIF--
<?php
//if (!extension_loaded('pmq')) {
	echo 'skip';
//}
?>
--FILE--
<?php

    $name = '/testqueue'.bin2hex(random_bytes(8));
    $message = "helloeooopoopoop";
    $pmq = new PMQ($name,"e+");
//    echo $pmq->send($message).PHP_EOL;
//    echo ($pmq->receive() === $message) . PHP_EOL;
//    echo $pmq->close().PHP_EOL;
    echo $pmq->unlink().PHP_EOL;

?>
--EXPECT--
1
1
1
1
