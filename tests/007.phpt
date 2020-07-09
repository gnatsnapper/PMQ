--TEST--
Mode Test
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 var_dump( 'skip';
}
?>
--FILE--
<?php
$name = '/testqueue' . bin2hex( random_bytes(8) );
$pmq1 = new PosixMessageQueue($name,PMQ_CREAT|PMQ_WRONLY,0);
try { var_dump( $pmq2 = new PosixMessageQueue($name,PMQ_CREAT|PMQ_WRONLY,0) ); }
catch (Exception $e)
    {
        var_dump( $e->getMessage() );
    }
var_dump( $pmq1->unlink() );
?>
--EXPECT--
string(17) "Permission denied"
bool(true)
