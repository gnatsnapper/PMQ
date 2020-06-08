--TEST--
Fork Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	echo 'skip';
}
if (!extension_loaded('pcntl')) {
	echo 'skip';
}

?>
--FILE--
<?php
$name = '/'.bin2hex(random_bytes(8));
$pmq = new PMQ($name);

    $pid = pcntl_fork();
    if ($pid == -1) {
     echo 'skip';
} else if ($pid) {

    pcntl_wait($status);

    var_dump( $pmq->receive() ) ;

    var_dump( $pmq->unlink() ) ;



} else {

    var_dump( $pmq->send("Hi Mum!") );
    exit(0);

}



?>
--EXPECT--
bool(true)
string(7) "Hi Mum!"
bool(true)
