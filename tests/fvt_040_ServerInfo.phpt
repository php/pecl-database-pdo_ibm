--TEST--
pdo_ibm: Get the server info.
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
			$result = $this->db->getAttribute(PDO::ATTR_SERVER_INFO);
			echo "Server Info: $result\n";
			if ($result = NULL) {
				echo "Result is NULL...bad\n";
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTREGEX--
(Server Info: DB2.+)|(Server Info: IDS.+)
