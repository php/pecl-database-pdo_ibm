--TEST--
pdo_ibm: Count number of affected rows - Select
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

			$stmt = $this->db->query( "SELECT name FROM animals WHERE weight < 10.0" );
			print $stmt->rowCount() . "\n";
			echo "Number of rows: " . count( $stmt->fetchAll() ) . "\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
-1
Number of rows: 4

