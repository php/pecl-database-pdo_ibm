--TEST--
pdo_ibm: Handle empty parameter (GH-12)
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

			$stmt = $this->db->prepare( "SELECT * FROM animals WHERE name = ?" );
			$stmt->bindValue(1, "");
			$stmt->execute();
			print $stmt->rowCount() . "\n";
			echo "Number of rows: " . count( $stmt->fetchAll() ) . "\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
-1
Number of rows: 0

