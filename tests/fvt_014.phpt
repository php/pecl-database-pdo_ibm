--TEST--
pdo_ibm: rowCount - insert, delete
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

			$stmt = $this->db->query( "INSERT INTO animals VALUES ( 7 , 'monkey' , 'Evil Monkey' , 10.0 )" );
			print "Num rows affected (Ins): " . $stmt->rowCount() . ", Column count: " . $stmt->columnCount() . "\n";
			$stmt = null;

			$stmt = $this->db->query( "DELETE FROM animals WHERE id=7" );
			print "Num rows affected (Del): " . $stmt->rowCount() . ", Column count: " . $stmt->columnCount() . "\n";
			$stmt = null;
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Num rows affected (Ins): 1, Column count: 0
Num rows affected (Del): 1, Column count: 0
