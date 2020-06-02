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
    $create  = "DROP PROCEDURE lotchar\n";
    $result = $this->db->exec( $create );
  } catch( Exception $e ) {}
  try {
    $create  = "CREATE PROCEDURE lotchar(\n";
    $create .= " INOUT V1 VARCHAR(128),\n";
    $create .= " INOUT V2 CHAR(128))\n";
    $create .= " LANGUAGE SQL\n";
    $create .= "BEGIN\n";
    $create .= "SET V1 = V2;\n";
    $create .= "SET V2 = 'TRUE i was replaced with longer string';\n";
    $create .= "END\n";
    $result = $this->db->exec( $create );
    $v1   = "hi from v1";
    $v2   = "hi from v2";
    $stmt = $this->db->prepare('call lotchar(?,?)');
/*
    echo getmypid();
    sleep(30);
*/
    $r1 = $stmt->bindParam(1,$v1, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $r2 = $stmt->bindParam(2,$v2, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $rc = $stmt->execute();
    print $v1."\n";
    print $v2."\n";
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
hi from v2
TRUE i was replaced with longer string%s
done

