--TEST--
Check if pmq is loaded
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 var_dump( 'skip';
}
?>
--FILE--
<?php
 echo 'The extension "pmq" is available';
?>
--EXPECT--
The extension "pmq" is available
