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
				// It seems older Db2 LUW returned 42S22/-206,
				// but the current version seems to instead
				// return 42S02/-204, with a different message.
				// Note that [2] includes a newline as well..
				print_r($this->db->errorInfo());
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTREGEX--
Attempting to connect..
Error code:
42S[02]2
Error info:
Array
\(
    \[0\] => 42S[02]2
    \[1] => -20[46]
    \[2\] => \[IBM\]\[.*\]\[.*\] (The specified table (notab) is not in the database. \(.*\[-206\] at .*\) ISAM: \[IBM\]\[.*\]\[.*\] -111 ISAM error:  no record found.|SQL0204N  "DB2INST1.NOTAB" is an undefined name.  SQLSTATE=42704\n.*)
\)
