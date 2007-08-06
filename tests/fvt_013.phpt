--TEST--
pdo_ibm: Scrollable cursor; retrieve negative row
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
			$this->prepareDB();

			$stmt = $this->db->prepare( "SELECT * FROM animals" , array(PDO::ATTR_CURSOR, PDO::CURSOR_SCROLL) );
			$stmt->execute();
			var_dump( $stmt->fetchAll() );
			$stmt->execute();
			try{
				$row = $stmt->fetch( PDO::FETCH_BOTH , PDO::FETCH_ORI_ABS , -1 );
				var_dump( $row );
			}catch( PDOException $e ){
				$info = $stmt->errorInfo();
				if( $info[1] == -99999 )
				{
					print "Cannot retrieve negative row\n";
				}
				else
				{
					print $e . "\n";
				}
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
array(7) {
  [0]=>
  array(8) {
    ["ID"]=>
    string(1) "0"
    [0]=>
    string(1) "0"
    ["BREED"]=>
    string(3) "cat"
    [1]=>
    string(3) "cat"
    ["NAME"]=>
    string(16) "Pook            "
    [2]=>
    string(16) "Pook            "
    ["WEIGHT"]=>
    string(4) "3.20"
    [3]=>
    string(4) "3.20"
  }
  [1]=>
  array(8) {
    ["ID"]=>
    string(1) "1"
    [0]=>
    string(1) "1"
    ["BREED"]=>
    string(3) "dog"
    [1]=>
    string(3) "dog"
    ["NAME"]=>
    string(16) "Peaches         "
    [2]=>
    string(16) "Peaches         "
    ["WEIGHT"]=>
    string(5) "12.30"
    [3]=>
    string(5) "12.30"
  }
  [2]=>
  array(8) {
    ["ID"]=>
    string(1) "2"
    [0]=>
    string(1) "2"
    ["BREED"]=>
    string(5) "horse"
    [1]=>
    string(5) "horse"
    ["NAME"]=>
    string(16) "Smarty          "
    [2]=>
    string(16) "Smarty          "
    ["WEIGHT"]=>
    string(6) "350.00"
    [3]=>
    string(6) "350.00"
  }
  [3]=>
  array(8) {
    ["ID"]=>
    string(1) "3"
    [0]=>
    string(1) "3"
    ["BREED"]=>
    string(9) "gold fish"
    [1]=>
    string(9) "gold fish"
    ["NAME"]=>
    string(16) "Bubbles         "
    [2]=>
    string(16) "Bubbles         "
    ["WEIGHT"]=>
    string(4) "0.10"
    [3]=>
    string(4) "0.10"
  }
  [4]=>
  array(8) {
    ["ID"]=>
    string(1) "4"
    [0]=>
    string(1) "4"
    ["BREED"]=>
    string(10) "budgerigar"
    [1]=>
    string(10) "budgerigar"
    ["NAME"]=>
    string(16) "Gizmo           "
    [2]=>
    string(16) "Gizmo           "
    ["WEIGHT"]=>
    string(4) "0.20"
    [3]=>
    string(4) "0.20"
  }
  [5]=>
  array(8) {
    ["ID"]=>
    string(1) "5"
    [0]=>
    string(1) "5"
    ["BREED"]=>
    string(4) "goat"
    [1]=>
    string(4) "goat"
    ["NAME"]=>
    string(16) "Rickety Ride    "
    [2]=>
    string(16) "Rickety Ride    "
    ["WEIGHT"]=>
    string(4) "9.70"
    [3]=>
    string(4) "9.70"
  }
  [6]=>
  array(8) {
    ["ID"]=>
    string(1) "6"
    [0]=>
    string(1) "6"
    ["BREED"]=>
    string(5) "llama"
    [1]=>
    string(5) "llama"
    ["NAME"]=>
    string(16) "Sweater         "
    [2]=>
    string(16) "Sweater         "
    ["WEIGHT"]=>
    string(6) "150.00"
    [3]=>
    string(6) "150.00"
  }
}
Cannot retrieve negative row
