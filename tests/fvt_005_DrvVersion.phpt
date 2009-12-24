--TEST--
pdo_ibm: Get the driver version
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			$this->connect();

			$result = $this->db->getAttribute(PDO::ATTR_CLIENT_VERSION);
			echo "Version is: $result\n";
			if ($result = NULL) {
				echo "Result is NULL...bad\n";
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Version is: %d.%d.%d
