--TEST--
pdo_ibm: Check error condition when given null connection parameters
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			try {
				$my_null = NULL;
				$new_conn = new PDO($my_null, $this->user, $this->pass);
			} catch(Exception $e) {
				echo "Connection Failed\n";
				echo $e->getMessage() . "\n\n";
			}

			try {
				$my_null = "    ";
				$new_conn = new PDO($this->dsn, $my_null, $this->pass);
			} catch(Exception $e) {
				echo "Connection Failed\n";
				echo $e->getMessage() . "\n";
			}

			try {
				$my_null = "    ";
				$new_conn = new PDO($this->dsn, $this->user, $my_null);
			} catch(Exception $e) {
				echo "Connection Failed\n";
				echo $e->getMessage();
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Connection Failed
invalid data source name

Connection Failed
SQLSTATE=08001, SQLConnect: -30082 %s
Connection Failed
SQLSTATE=08001, SQLConnect: -30082 %s
