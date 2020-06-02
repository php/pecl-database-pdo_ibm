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
    $create  = "DROP PROCEDURE lotint\n";
    $result = $this->db->exec( $create );
  } catch( Exception $e ) {}
  try {
    $create  = "CREATE PROCEDURE lotint(\n";
    $create .= " INOUT V1 SMALLINT,\n";
    $create .= " INOUT V2 INTEGER,\n";
    $create .= " INOUT V3 BIGINT)\n";
    $create .= " LANGUAGE SQL\n";
    $create .= "BEGIN\n";
    $create .= "SET V1 = V1 + 42;\n";
    $create .= "SET V2 = V2 + 42;\n";
    $create .= "SET V3 = V3 + 42;\n";
    $create .= "END\n";
    $result = $this->db->exec( $create );
    $v1   = 1;
    $v2   = 2;
    $v3   = 3;
    $stmt = $this->db->prepare('call lotint(?,?,?)');
/*
    echo getmypid();
    sleep(30);
*/
    $r1 = $stmt->bindParam(1,$v1, PDO::PARAM_INT|PDO::PARAM_INPUT_OUTPUT);
    $r2 = $stmt->bindParam(2,$v2, PDO::PARAM_INT|PDO::PARAM_INPUT_OUTPUT);
    $r3 = $stmt->bindParam(3,$v3, PDO::PARAM_INT|PDO::PARAM_INPUT_OUTPUT);
    $rc = $stmt->execute();
    print $v1."\n";
    print $v2."\n";
    print $v3."\n";
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
43
44
45
done
