--TEST--
pdo_ibm: Testing fetchColumn with different modes and options
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest {
		public function runTest() {
			$this->connect();
			$this->prepareDB();
			$sql = "SELECT * FROM animals WHERE id > 0";

			$stmt = $this->db->prepare($sql);
			$stmt->execute();
			while( $value = $stmt->fetchColumn() ) {
				print "The column value is: " . $value . "\n";
			}

			$stmt = $this->db->prepare($sql);
			$stmt->execute();
			while( $value = $stmt->fetchColumn( 1 ) ) {
				print "The column value is: " . $value . "\n";
			}

			$stmt = $this->db->prepare($sql);
			$stmt->execute();
			while( $value = $stmt->fetchColumn( -1 ) ) {
				print "The column value is: " . $value . "\n";
			}

			$stmt = $this->db->prepare($sql);
			$stmt->execute();
			while( $value = $stmt->fetchColumn( 7 ) ) {
				print "The column value is: " . $value . "\n";
			}
		}
	}
	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
The column value is: 1
The column value is: 2
The column value is: 3
The column value is: 4
The column value is: 5
The column value is: 6
The column value is: dog
The column value is: horse
The column value is: gold fish
The column value is: budgerigar
The column value is: goat
The column value is: llama
