--TEST--
pdo_ibm: commit
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
			$stmt = $this->db->query( "SELECT count(*) FROM animals" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			$rows = $res[0];
			echo $rows."\n";

			$this->db->beginTransaction();
			$this->db->exec( "DELETE FROM animals" );

			$stmt = $this->db->query( "SELECT count(*) FROM animals" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			$rows = $res[0];
			echo $rows."\n";

			$this->db->commit();

			$stmt = $this->db->query( "SELECT count(*) FROM animals" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			$rows = $res[0];
			echo $rows."\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
7
0
0
