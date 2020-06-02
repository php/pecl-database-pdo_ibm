--TEST--
pdo_ibm: Try more complex in/out stored proc call
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
require_once('fvt.inc');
class Test extends FVTTest {
public function runTest() {
  $this->connect();
  try {
    $create  = "DROP PROCEDURE lotfloat\n";
    $result = $this->db->exec( $create );
  } catch( Exception $e ) {}
  try {
    $create  = "CREATE PROCEDURE lotfloat(\n";
    $create .= " INOUT V1 FLOAT(4),\n";
    $create .= " INOUT V2 FLOAT(8),\n";
    $create .= " INOUT V3 REAL,\n";
    $create .= " INOUT V4 DOUBLE)\n";
    $create .= " LANGUAGE SQL\n";
    $create .= "BEGIN\n";
    $create .= "SET V1 = V1 + 42.42;\n";
    $create .= "SET V2 = V2 + 42.42;\n";
    $create .= "SET V3 = V3 + 43.43;\n";
    $create .= "SET V4 = V4 + 44.44;\n";
    $create .= "END\n";
    $result = $this->db->exec( $create );
    $v1   = 1.1;
    $v2   = 2.2;
    $stmt = $this->db->prepare('call lotfloat(?,?,?,?)');
/*
    echo getmypid();
    sleep(30);
*/
    $r1 = $stmt->bindParam(1,$v1, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $r2 = $stmt->bindParam(2,$v2, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $r3 = $stmt->bindParam(3,$v3, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $r4 = $stmt->bindParam(4,$v4, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $rc = $stmt->execute();
    print round($v1, 2)."\n";
    print round($v2, 2)."\n";
    print round($v3, 2)."\n";
    print round($v4, 2)."\n";
    print "done\n";
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
} /* runTest */
}
$testcase = new Test();
$testcase->runTest();
?>
--EXPECTF--
43.52
44.62
43.43
44.44
done

