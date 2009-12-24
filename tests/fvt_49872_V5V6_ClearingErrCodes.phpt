--TEST--
pdo_ibm: Check the clearing of error codes
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
				/* Drop the test table, in case it exists */
				$drop   = 'DROP TABLE animals';
				$result = $this->db->exec( $drop );
			} catch( Exception $e ){}

			/* Create the test table */
			$create = 'CREATE TABLE animals (id INTEGER)';
			$result = $this->db->exec( $create );

			$sql = "selec id from animals ";
			try {
				$stmt = $this->db->query($sql);
			} catch ( Exception $e) {}

			$sql = "select id from animals ";
			$stmt = $this->db->query($sql);
			print_r($this->db->errorInfo());

			$res = $stmt->fetch();
			print_r($res);
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Array
(
    [0] => 00000
    [1] => 0
    [2] =>  ((null)[0] at (null):0)
)
