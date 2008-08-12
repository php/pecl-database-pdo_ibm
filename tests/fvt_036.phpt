--TEST--
pdo_ibm: Test the setting of different options
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			/* Connect */
			$this->connect();

			/* Set up */
			try {
				$this->db->exec("DROP TABLE test");
			} catch (Exception $e){}
			$this->db->exec("CREATE TABLE test (id INTEGER)");
			$this->db->exec("INSERT INTO test values (1)");
			$this->db->exec("INSERT INTO test values (2)");
			$this->db->exec("INSERT INTO test values (3)");
			$this->db->exec("INSERT INTO test values (4)");
			$this->db->exec("INSERT INTO test values (5)");

			/* Test ATTR_AUTOCOMMIT */
			$this->db->setAttribute(PDO::ATTR_AUTOCOMMIT, false);
			$this->db->beginTransaction();
			$stmt = $this->db->query( "SELECT count(*) FROM test" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			echo $res[0]."\n";
			$this->db->exec( "DELETE FROM test" );
			$stmt = $this->db->query( "SELECT count(*) FROM test" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			echo $res[0]."\n";
			$this->db->rollBack();
			$stmt = $this->db->query( "SELECT count(*) FROM test" );
			$res = $stmt->fetch( PDO::FETCH_NUM );
			echo $res[0]."\n";

			/* Test ATTR_ERRMODE */
			$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			try {
				$this->db->exec("INSERT INTO nontest values (6)");
			} catch (Exception $e) {
				echo "Failed: " . $e->getMessage() . "\n";
			}

			/* Test ATTR_CASE */
			$this->db->setAttribute(PDO::ATTR_CASE, PDO::CASE_UPPER);
			$stmt = $this->db->query( "SELECT id FROM test" );
			$res = $stmt->fetch();
			var_dump( $res );
			$this->db->setAttribute(PDO::ATTR_CASE, PDO::CASE_LOWER);
			$stmt = $this->db->query( "SELECT id FROM test" );
			$res = $stmt->fetch();
			var_dump( $res );

			/* Test ATTR_PERSISTENT */
			$op = array(PDO::ATTR_PERSISTENT => true);
			$pdb = new PDO($this->dsn, $this->user, $this->pass, $op);
			var_dump($pdb);
			$pdb = null;
			$pdb = new PDO($this->dsn, $this->user, $this->pass, $op);
			var_dump($pdb);
			$pdb = null;
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
5
0
5
Failed: %a
array(2) {
  ["ID"]=>
  string(1) "1"
  [0]=>
  string(1) "1"
}
array(2) {
  ["id"]=>
  string(1) "1"
  [0]=>
  string(1) "1"
}
object(PDO)#%d (0) {
}
object(PDO)#%d (0) {
}

