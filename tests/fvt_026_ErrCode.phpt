--TEST--
pdo_ibm: Check error code.
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest {
		public function runTest() {
			$this->connect();
			$this->prepareDB();
			try {
				$err = $this->db->prepare('SELECT skull FROM bones');
				$err->execute();
			} catch (Exception $e) {
				echo "\nPDOStatement::errorCode(): ";
				print $this->db->errorCode();
			}
			try {
				$err = $this->db->prepare('SELECT id FROM animals WHERE bones=100');
				$err->execute();
			} catch (Exception $e) {
				echo "\nPDOStatement::errorCode(): ";
				print $this->db->errorCode();
			}
			try {
				$err = $this->db->prepare('SELECT id, skull FROM animals WHERE id=1');
				$err->execute();
			} catch (Exception $e) {
				echo "\nPDOStatement::errorCode(): ";
				print $this->db->errorCode();
			}
		}
	}
	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTREGEX--
(PDOStatement::errorCode\(\): 42S02
PDOStatement::errorCode\(\): 42S22
PDOStatement::errorCode\(\): 42S22)|(PDOStatement::errorCode\(\): 42S22
PDOStatement::errorCode\(\): IX000
PDOStatement::errorCode\(\): IX000)
