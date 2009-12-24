--TEST--
pdo_ibm: PDOStatement::fetch()
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

			$stmt = $this->db->query( "SELECT id, breed, name, weight FROM animals WHERE id = 0" );
			while( $row = $stmt->fetch( PDO::FETCH_BOTH ) ) {
				$breed = $row[1];
				var_dump( $breed );
				$name = $row["NAME"];
				var_dump( $name );
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
string(3) "cat"
string(16) "Pook            "
