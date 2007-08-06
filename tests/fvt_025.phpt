--TEST--
pdo_ibm: Get Column meta data.
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest {
		public function runTest() {
			$this->connect();
			$this->prepareDB();
			$sql = "SELECT * FROM animals";
			$stmt = $this->db->query($sql);
			$meta = $stmt->getColumnMeta(0);
			var_dump( $meta );
			$meta = $stmt->getColumnMeta(1);
			var_dump( $meta );
			$meta = $stmt->getColumnMeta(2);
			var_dump( $meta );
			$meta = $stmt->getColumnMeta(3);
			var_dump( $meta );
			try {
				$meta = $stmt->getColumnMeta(6);
				var_dump( $meta );
			} catch( Exception $e ) {
				print "Error: " . $stmt->errorCode() . "\n";
			}
			try{
				$meta = $stmt->getColumnMeta(-1);
				var_dump( $meta );
			} catch( Exception $e ) {
				print "Error: " . $stmt->errorCode() . "\n";
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTREGEX--
array\(8\) \{
  \["scale"\]=>
  int\(0\)
  \["table"\]=>
  string\(7\) ("ANIMALS")|("animals")
  \["native_type"\]=>
  string\(7\) "INTEGER"
  \["flags"\]=>
  array\(3\) {
    \["not_null"\]=>
    bool\(false\)
    \["unsigned"\]=>
    bool\(false\)
    \["auto_increment"\]=>
    bool\(false\)
  \}
  \["name"\]=>
  string\(2\) "ID"
  \["len"\]=>
  int\(11\)
  \["precision"\]=>
  int\(0\)
  \["pdo_type"\]=>
  int\(2\)
\}
array\(8\) \{
  \["scale"\]=>
  int\(0\)
  \["table"\]=>
  string\(7\) ("ANIMALS")|("animals")
  \["native_type"\]=>
  string\(7\) "VARCHAR"
  \["flags"\]=>
  array\(3\) \{
    \["not_null"\]=>
    bool\(false\)
    \["unsigned"\]=>
    bool\(true\)
    \["auto_increment"\]=>
    bool\(false\)
  \}
  \["name"\]=>
  string\(5\) "BREED"
  \["len"\]=>
  int\(32\)
  \["precision"\]=>
  int\(0\)
  \["pdo_type"\]=>
  int\(2\)
\}
array\(8\) \{
  \["scale"\]=>
  int\(0\)
  \["table"\]=>
  string\(7\) ("ANIMALS")|("animals")
  \["native_type"\]=>
  string\(4\) "CHAR"
  \["flags"\]=>
  array\(3\) {
    \["not_null"\]=>
    bool\(false\)
    \["unsigned"\]=>
    bool\(true\)
    \["auto_increment"\]=>
    bool\(false\)
  \}
  \["name"\]=>
  string\(4\) "NAME"
  \["len"\]=>
  int\(16\)
  \["precision"\]=>
  int\(0\)
  \["pdo_type"\]=>
  int\(2\)
\}
array\(8\) \{
  \["scale"\]=>
  int\(2\)
  \["table"\]=>
  string\(7\) ("ANIMALS")|("animals")
  \["native_type"\]=>
  string\(7\) "DECIMAL"
  \["flags"\]=>
  array\(3\) {
    \["not_null"\]=>
    bool\(false\)
    \["unsigned"\]=>
    bool\(false\)
    \["auto_increment"\]=>
    bool\(false\)
  \}
  \["name"\]=>
  string\(6\) "WEIGHT"
  \["len"\]=>
  int\(9\)
  \["precision"\]=>
  int\(2\)
  \["pdo_type"\]=>
  int\(2\)
\}
Error: HY097
Error: 42P10

