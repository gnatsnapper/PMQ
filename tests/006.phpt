--TEST--
Read/Write Only Flag Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 print 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq1 = new PMQ($name,PMQ_CREAT|PMQ_WRONLY);
$pmq2 = new PMQ($name,PMQ_RDONLY);
var_dump( $pmq1->send("hello") );
var_dump( $pmq2->receive() );

try { var_dump( $pmq2->send("goodbye") ); }
catch (Exception $e)
    {
        var_dump( $e->getMessage() );
    }
var_dump( $pmq1->send("hello") );
try { var_dump( $pmq1->receive() ); }
catch (Exception $e)
    {
        var_dump( $e->getMessage() );
    }
var_dump( $pmq1->unlink() );
?>
--EXPECT--
bool(true)
string(5) "hello"
string(19) "Bad file descriptor"
bool(true)
string(19) "Bad file descriptor"
bool(true)
