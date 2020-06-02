--TEST--
pdo_ibm: Try system naming *LIBL
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
require_once('fvt400.inc');
class Test extends FVTTest {
public function runTest() {
  //echo getmypid();
  //sleep(30);

  /* see README400.txt */
  $this->connect(true,true,PDO::I5_TXN_NO_COMMIT);

/* This is odd, yes?
  try {
    $libl  = "select current path from sysibm.sysdummy1";
    $stmt = $this->db->query( $libl );
    print("$libl\n");
    while( $row = $stmt->fetch( PDO::FETCH_NUM ) ) {
      print($row[0]);
      print("\n");
    }
  } catch( Exception $e ) { }
*/

  try {
    $libl  = "select current schema from sysibm.sysdummy1";
    $stmt = $this->db->query( $libl );
    print("$libl\n");
    while( $row = $stmt->fetch( PDO::FETCH_NUM ) ) {
      print($row[0]);
      print("\n");
    }
  } catch( Exception $e ) { }

  try {
    $create  = "DROP TABLE libint\n";
    print("$create\n");
    $result = $this->db->query( $create );
  } catch( Exception $e ) { }
  try {
    $create  = "CREATE TABLE libint(\n";
    $create .= " V1 SMALLINT,\n";
    $create .= " V2 INTEGER,\n";
    $create .= " V3 BIGINT,\n";
    $create .= " N1 SMALLINT,\n";
    $create .= " N2 INTEGER,\n";
    $create .= " N3 BIGINT,\n";
    $create .= " E1 SMALLINT,\n";
    $create .= " E2 INTEGER,\n";
    $create .= " E3 BIGINT)\n";
    print("$create\n");
    $result = $this->db->query( $create );
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
  try {
    $v1   = 1;
    $v2   = 2;
    $v3   = 3;
    $n1   = null;
    $n2   = null;
    $n3   = null;
    $e1   = "";
    $e2   = "";
    $e3   = "";
	$callme = "insert into libint (v1,v2,v3,n1,n2,n3,e1,e2,e3) values (?,?,?,?,?,?,?,?,?)";
    print("$callme\n");
    $stmt = $this->db->prepare($callme);
    $r1 = $stmt->bindParam(1,$v1);
    $r2 = $stmt->bindParam(2,$v2);
    $r3 = $stmt->bindParam(3,$v3);
    $r4 = $stmt->bindParam(4,$n1);
    $r5 = $stmt->bindParam(5,$n2);
    $r6 = $stmt->bindParam(6,$n3);
    $r7 = $stmt->bindParam(7,$e1);
    $r8 = $stmt->bindParam(8,$e2);
    $r9 = $stmt->bindParam(9,$e3);
    $rc = $stmt->execute();
    $v1   = 1;
    $v2   = 2;
    $v3   = 3;
    $n1   = null;
    $n2   = null;
    $n3   = null;
    $e1   = "";
    $e2   = "";
    $e3   = "";
    $rc = $stmt->execute(array($v1,$v2,$v3,$n1,$n2,$n3,$e1,$e2,$e3));
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
  try {
	$readme = "SELECT * FROM libint";
    print("$readme\n");
    $stmt = $this->db->query( $readme );
    while ($res  = $stmt->fetch( PDO::FETCH_BOTH )) {
      $v1 = $res[0];
      $v2 = $res[1];
      $v3 = $res[2];
      $n1 = $res[3];
      $n2 = $res[4];
      $n3 = $res[5];
      $e1 = $res[6];
      $e2 = $res[7];
      $e3 = $res[8];
      print "Int contents: ($v1,$v2,$v3,$n1,$n2,$n3,$e1,$e2,$e3)\n";
    }
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
%s
*LIBL
%s
Int contents: (1,2,3,,,,,,)
Int contents: (1,2,3,,,,,,)

