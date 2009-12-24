--TEST--
pdo_ibm: Check error codes after a failed execution
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
			$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

			try {
				/* Drop the test table, in case it exists */
				$drop   = 'DROP TABLE test_error';
				$result = $this->db->exec( $drop );
			} catch( Exception $e ){}

			$this->db->exec("CREATE TABLE test_error (id INTEGER, data VARCHAR(50))");

			echo "Begin\n";
			$this->db->beginTransaction();

			$stmt = $this->db->prepare("INSERT INTO test_error (id, data ) VALUES (?, ?)");

			try {
				echo "Execute\n";
				$res = $stmt->execute(array('a','b'));

				if($res) {
					echo "Commit\n";
					$this->db->commit();
				} else {
					$err = $stmt->errorInfo();
					echo "Execute failed\n";
					echo "$err[0]\n";
					echo "$err[1]\n";
					echo "$err[2]\n";
					$this->db->rollBack();
				}
			} catch(Exception $e) {
				$err = $stmt->errorInfo();
				echo "Exception occured\n";
				echo "$err[0]\n";
				echo "$err[1]\n";
				echo "$err[2]\n";
				$this->db->rollBack();
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Begin
Execute
Exception occured
22018
-420
Character in CAST argument not valid. (SQLExecute[-420] %s
