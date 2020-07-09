--TEST--
Check if pmq is loaded
--SKIPIF--
<?php
if (!extension_loaded('pmq')) {
	 print 'skip';
}
?>
--FILE--
<?php
 echo 'The extension "pmq" is available';
?>
--EXPECT--
The extension "pmq" is available
