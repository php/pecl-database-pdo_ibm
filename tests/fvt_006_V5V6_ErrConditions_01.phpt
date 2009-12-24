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
				$stmt1 = $this->db->prepare("SELECT id FROM animals WHERE colnotexist = 1 " ) ;
				print("Error Code 1: ".$this->db->errorCode()."\n");
				print_r($this->db->errorInfo());
				print("\n");
				$stmt2 = $this->db->prepare("SELECT id FROM animals WHERE id = 1 " ) ;
				print("Error Code 2: ".$this->db->errorCode()."\n");
				print_r($this->db->errorInfo());
				print("\n");
			} catch (PDOException $pe) {
				print("Error Code 3: ".$this->db->errorCode()."\n");
				print_r($this->db->errorInfo());
				print("\n");
				$stmt2 = $this->db->prepare("SELECT id FROM animals WHERE id = 1 " ) ;
				print("Error Code 4: ".$this->db->errorCode()."\n");
				print_r($this->db->errorInfo());
				print("\n");
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Error Code 3: 42703
Array
(
    [0] => 42703
    [1] => -206
    [2] => Column COLNOTEXIST not in specified tables. (SQLPrepare[-206] at %s
)

Error Code 4: 00000
Array
(
    [0] => 00000
    [1] => 0
    [2] =>  ((null)[0] at (null):0)
)
