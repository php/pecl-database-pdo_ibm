--TEST--
pdo_ibm: Count number of affected rows - Delete
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

			$stmt = $this->db->query( "DELETE FROM animals WHERE weight > 10.0" );
			echo "Number of affected rows: " . $stmt->rowCount();
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Number of affected rows: 3
