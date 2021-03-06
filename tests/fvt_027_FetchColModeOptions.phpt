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
			try {
				while( $value = $stmt->fetchColumn( -1 ) ) {
					print "The column value is: " . $value . "\n";
				}
			} catch( ValueError $e ) {
				/*
				 * PHP 8 makes PDO catch negative column
				 * references, so it'll never reach the driver.
				 * It won't mutate the SQL error, so it'll just
				 * return the same value it had last time. As
				 * such, just consider getting a ValueError
				 * good enough for this part of the test.
				 */
				print "Negative index expected to fail\n";
			} catch (Exception $e) {
				print "Negative index expected to fail\n";
			}

			$stmt = $this->db->prepare($sql);
			$stmt->execute();
			try {
				while( $value = $stmt->fetchColumn( 7 ) ) {
					print "The column value is: " . $value . "\n";
				}
			} catch( ValueError $e ) {
				/* As above, so below (but for OOB) */
				print "Out of bounds index expected to fail\n";
			} catch (Exception $e) {
				print "Out of bounds index expected to fail\n";
			}
		}
	}
	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
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
Negative index expected to fail
Out of bounds index expected to fail
