--TEST--
pdo_ibm: commit with autocommit off
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function __construct()
		{
			parent::__construct();
			// $this->dsn .= ";OptimizeAutoCommit=0;";
		}

		public function runTest()
		{
			$this->connect(false);
			$this->prepareDB();
			$stmt = $this->db->exec( "commit work" );

			$stmt = $this->db->query( "SELECT count(*) FROM animals" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			$rows = $res[0];
			echo $rows."\n";

			$this->db->exec( "DELETE FROM animals" );

			$stmt = $this->db->query( "SELECT count(*) FROM animals" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			$rows = $res[0];
			echo $rows."\n";

			$stmt = $this->db->exec( "commit work" );
			/* $this->db->commit(); */

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
