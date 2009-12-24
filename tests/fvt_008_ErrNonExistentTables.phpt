--TEST--
pdo_ibm: Test error conditions through non-existent tables
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
			$sql = "CREATE TABLE testError(" .
				"id INTEGER," .
				"data VARCHAR(50)," .
				"attachment VARCHAR(50)," .
				"about VARCHAR(50))";

			try {
				$stmt = $this->db->prepare($sql);
				$stmt->execute();
			} catch (PDOException $pe) {
				echo $pe->getMessage();
			}

			$this->db->exec("DROP TABLE testError");
			$sql = "SELECT id FROM FINAL TABLE(INSERT INTO testError(data,about,attachment)values(?,?,?))";

			try {
				$stmt = $this->db->prepare($sql);
				$stmt->execute();
			}	catch (PDOException $pe) {
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
--EXPECTREGEX--
(Error code:
42S02
Error info:
Array
\(
    \[0\] => 42S02
    \[1\] => -204
    \[2\] => \[IBM\]\[CLI Driver\]\[.+\] SQL0204N  ".+" is an undefined name\.  SQLSTATE=42704
 \(.+\[-204\] at .+\)
\))|(Error code:
42S22
Error info:
Array
\(
    \[0\] => 42S22
    \[1\] => -206
    \[2\] => \[IBM\]\[CLI Driver\]\[.+\] The specified table \(informix\.testerror\) is not in the database\. \(.+\[-206\] at .+\)
\))

