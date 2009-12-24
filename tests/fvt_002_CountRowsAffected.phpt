--TEST--
pdo_ibm: Count number of affected rows
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
			$this->prepareDB();
			$sql = 'UPDATE animals SET id = 9';
			$res = $this->db->exec($sql);
			print "Number of affected rows: " . $res;
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Number of affected rows: 7
