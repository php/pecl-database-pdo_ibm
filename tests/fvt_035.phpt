--TEST--
pdo_ibm: Check return values from exec
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			$this->connect(false);

			try {
				$this->db->exec("DROP TABLE testExec");
			} catch (Exception $e){}

			$sql = "CREATE TABLE testExec (id INTEGER)";
			if ($this->db->exec($sql) === false) {
				echo "Did not work\n";
			} else {
				echo "Worked\n";
			}

			$this->db->exec("INSERT INTO testExec (id) values (1)");

			$sql = "UPDATE testExec SET id = 5 WHERE id = 1";
			if ($this->db->exec($sql) === false) {
				echo "Did not work\n";
			} else {
				echo "Worked\n";
			}

			$sql = "DELETE FROM testExec WHERE id = 1";
			if ($this->db->exec($sql) === false) {
				echo "Did not work\n";
			} else {
				echo "Worked\n";
			}

			$this->db->exec("INSERT INTO testExec (id) values (2)");
			$this->db->exec("INSERT INTO testExec (id) values (3)");
			$this->db->exec("INSERT INTO testExec (id) values (4)");

			$sql = "DELETE FROM testExec";
			$rowCount = $this->db->exec($sql);
			echo "Row count: $rowCount";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Worked
Worked
Worked
Row count: 4
