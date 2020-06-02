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
    $create  = "DROP TABLE libdec\n";
    print("$create\n");
    $result = $this->db->query( $create );
  } catch( Exception $e ) { }
  try {
    $create  = "CREATE TABLE libdec(\n";
    $create .= " V1 DECIMAL(12,2),\n";
    $create .= " V2 NUMERIC(12,2),\n";
    $create .= " V3 DECIMAL(8,4),\n";
    $create .= " V4 NUMERIC(8,4),\n";
    $create .= " N1 DECIMAL(12,2),\n";
    $create .= " N2 NUMERIC(12,2),\n";
    $create .= " N3 DECIMAL(8,4),\n";
    $create .= " N4 NUMERIC(8,4),\n";
    $create .= " E1 DECIMAL(12,2),\n";
    $create .= " E2 NUMERIC(12,2),\n";
    $create .= " E3 DECIMAL(8,4),\n";
    $create .= " E4 NUMERIC(8,4))\n";
    print("$create\n");
    $result = $this->db->query( $create );
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
  try {
    $v1   = 1.1;
    $v2   = 2.2;
    $v3   = 3.3;
    $v4   = 4.4;
    $n1   = null;
    $n2   = null;
    $n3   = null;
    $n4   = null;
    $e1   = "";
    $e2   = "";
    $e3   = "";
    $e4   = "";
	$callme = "insert into libdec (v1,v2,v3,v4,n1,n2,n3,n4,e1,e2,e3,e4) values (?,?,?,?,?,?,?,?,?,?,?,?)";
    print("$callme\n");
    $stmt = $this->db->prepare($callme);
    $r1  = $stmt->bindParam(1,$v1);
    $r2  = $stmt->bindParam(2,$v2);
    $r3  = $stmt->bindParam(3,$v3);
    $r4  = $stmt->bindParam(4,$v4);
    $r5  = $stmt->bindParam(5,$n1);
    $r6  = $stmt->bindParam(6,$n2);
    $r7  = $stmt->bindParam(7,$n3);
    $r8  = $stmt->bindParam(8,$n4);
    $r9  = $stmt->bindParam(9,$e1);
    $r10 = $stmt->bindParam(10,$e2);
    $r11 = $stmt->bindParam(11,$e3);
    $r12 = $stmt->bindParam(12,$e4);
    $rc = $stmt->execute();
    $v1   = 1.1;
    $v2   = 2.2;
    $v3   = 3.3;
    $v4   = 4.4;
    $n1   = null;
    $n2   = null;
    $n3   = null;
    $n4   = null;
    $e1   = "";
    $e2   = "";
    $e3   = "";
    $e4   = "";
    $rc = $stmt->execute(array($v1,$v2,$v3,$v4,$n1,$n2,$n3,$n4,$e1,$e2,$e3,$e4));
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
  try {
	$readme = "SELECT * FROM libdec";
    print("$readme\n");
    $stmt = $this->db->query( $readme );
    while ($res  = $stmt->fetch( PDO::FETCH_BOTH )) {
      $v1 = $res[0];
      $v2 = $res[1];
      $v3 = $res[2];
      $v4 = $res[3];
      $n1 = $res[4];
      $n2 = $res[5];
      $n3 = $res[6];
      $n4 = $res[7];
      $e1 = $res[8];
      $e2 = $res[9];
      $e3 = $res[10];
      $e4 = $res[11];
      print "Dec contents: ($v1,$v2,$v3,$v4,$n1,$n2,$n3,$n4,$e1,$e2,$e3,$e4)\n";
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
Dec contents: (1.10,2.20,3.3000,4.4000,,,,,,,,)
Dec contents: (1.10,2.20,3.3000,4.4000,,,,,,,,)

