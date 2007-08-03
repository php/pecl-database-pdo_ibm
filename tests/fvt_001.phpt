--TEST--
pdo_ibm: Connect to database
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			print "Attempting to connect..\n";
			$this->connect();
			print "Connection succeeded.\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Attempting to connect..
Connection succeeded.
