--TEST--
pdo_ibm: Test error conditions through faulty SQL
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
			$parmno = "200010";
			try {
				$stmt = $this->db->prepare("SELECT empno, lastname, bonus, FROM employee WHERE empno > ?");
				$stmt->execute( array( $parmno ));
				while ($row = $stmt->fetch()) {
					print_r($row);
				}
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
--EXPECTF--
Error code:
42601
Error info:
Array
(
    [0] => 42601
    [1] => -199
    [2] => Keyword WHERE not expected. %s (SQLPrepare[-199] %s
)
