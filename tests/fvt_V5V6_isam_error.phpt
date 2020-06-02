--TEST--
pdo_ibm: ISAM Error
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			print "Attempting to connect..\n";
			$this->connect();
			
			$sql = "SELECT count(*) FROM notab";
			
			try {
				$stmt = $this->db->query($sql);			
			} catch (PDOException $e) {
				echo "Error code:\n";
				print_r($this->db->errorCode());
				echo "\n";
				echo "Error info:\n";
				print_r($this->db->errorInfo());
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Attempting to connect..
Error code:
42704
Error info:
Array
(
    [0] => 42704
    [1] => -204
    [2] => NOTAB in DB2 type *FILE not found. (SQLPrepare[-204] %s
)

