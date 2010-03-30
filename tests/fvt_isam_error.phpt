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
42S22
Error info:
Array
(
    [0] => 42S22
    [1] => -206
    [2] => [IBM][%s][%s] The specified table (notab) is not in the database. (%s[-206] at %s) ISAM: [IBM][%s][%s] -111 ISAM error:  no record found.
)
