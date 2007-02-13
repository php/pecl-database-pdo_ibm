--TEST--
pdo_ibm: Test error conditions
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

			try {
				$sql = "DROP table testFloat";
				$stmt = $this->db->exec($sql);
            } catch( Exception $e ){}

			$sql = "CREATE table testFloat (data FLOAT)";
			$stmt = $this->db->exec($sql);

			$sql = "INSERT INTO testFloat (data) values (0.058290369626395423)";
			$stmt = $this->db->exec($sql);

			$sql = "SELECT data FROM testFloat";
			$stmt = $this->db->query($sql);

			while ( $row = $stmt->fetch() ) {
				print_r($row);
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Array
(
    [DATA] => 5.82903696263954E-002
    [0] => 5.82903696263954E-002
)

