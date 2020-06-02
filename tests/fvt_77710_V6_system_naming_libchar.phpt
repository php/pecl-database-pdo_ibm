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
    $create  = "DROP TABLE liblchar\n";
    print("$create\n");
    $result = $this->db->query( $create );
  } catch( Exception $e ) { }
  try {
    $create  = "CREATE TABLE liblchar(\n";
    $create .= " V1 VARCHAR(128),\n";
    $create .= " V2 CHAR(128),\n";
    $create .= " N1 VARCHAR(128),\n";
    $create .= " N2 CHAR(128),\n";
    $create .= " E1 VARCHAR(128),\n";
    $create .= " E2 CHAR(128))\n";
    print("$create\n");
    $result = $this->db->query( $create );
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
  try {
    $v1   = "hi from v1";
    $v2   = "hi from v2";
    $n1   = null;
    $n2   = null;
    $e1   = "";
    $e2   = "";
	$callme = "insert into liblchar (v1,v2,n1,n2,e1,e2) values (?,?,?,?,?,?)";
    print("$callme\n");
    $stmt = $this->db->prepare($callme);
    $r1 = $stmt->bindParam(1,$v1);
    $r2 = $stmt->bindParam(2,$v2);
    $r3 = $stmt->bindParam(3,$n1);
    $r4 = $stmt->bindParam(4,$n2);
    $r5 = $stmt->bindParam(5,$e1);
    $r6 = $stmt->bindParam(6,$e2);
    $rc = $stmt->execute();
    $v1   = "hi from v1";
    $v2   = "hi from v2";
    $n1   = null;
    $n2   = null;
    $e1   = "";
    $e2   = "";
    $rc = $stmt->execute(array($v1,$v2,$n1,$n2,$e1,$e2));
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
  try {
	$readme = "SELECT * FROM liblchar";
    print("$readme\n");
    $stmt = $this->db->query( $readme );
    while ($res  = $stmt->fetch( PDO::FETCH_BOTH )) {
      $v1 = $res[0];
      $v2 = trim($res[1]);
      $n1 = $res[2];
      $n2 = trim($res[3]);
      $e1 = $res[4];
      $e2 = trim($res[5]);
      print "String contents: ($v1,$v2,$n1,$n2,$e1,$e2)\n";
    }
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }

  try {
	$readme = "SELECT hex(v1),hex(v2),hex(n1),hex(n2),hex(e1),hex(e2) FROM liblchar";
    print("$readme\n");
    $stmt = $this->db->query( $readme );
    while ($res  = $stmt->fetch( PDO::FETCH_BOTH )) {
      $v1 = $res[0].".";
      $v2 = substr($res[1],0,40)."...";
      $n1 = $res[2].".";
      $n2 = substr($res[3],0,40)."...";
      $e1 = $res[4].".";
      $e2 = substr($res[5],0,40)."...";
      print "HEX:\nv1=$v1\nv2=$v2\nn1=$n1\nn2=$n2\ne1=$e1\ne2=$e2\n";
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
String contents: (hi from v1,hi from v2,,,,)
String contents: (hi from v1,hi from v2,,,,)
SELECT hex(v1),hex(v2),hex(n1),hex(n2),hex(e1),hex(e2) FROM liblchar
%s
v1=8889408699969440A5F1.
v2=8889408699969440A5F240404040404040404040...
n1=.
n2=...
e1=.
e2=4040404040404040404040404040404040404040...
%s
v1=8889408699969440A5F1.
v2=8889408699969440A5F240404040404040404040...
n1=.
n2=...
e1=.
e2=4040404040404040404040404040404040404040...


