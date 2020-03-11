--TEST--
Test pmq_info();
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
?>
--FILE--
<?php
print_r(pmq_info());
?>
--EXPECT--
Array
(
    [mq_maxmsg] => 10
    [mq_msgsize] => 8192
)

