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
    $stmt = $this->db->prepare('call xmlservice.iplug512K(?,?,?,?)');
    $ipc    = "na";
    $ctl    = "*here";
    $xmlIn  = $this->ZZARRAY2();
    $xmlOut = "";
/*
    echo getmypid();
    sleep(30);
*/
    $r1 = $stmt->bindParam(1,$ipc, PDO::PARAM_STR);
    $r2 = $stmt->bindParam(2,$ctl, PDO::PARAM_STR);
    $r3 = $stmt->bindParam(3,$xmlIn, PDO::PARAM_STR);
    $r4 = $stmt->bindParam(4,$xmlOut, PDO::PARAM_STR|PDO::PARAM_INPUT_OUTPUT);
    $rc = $stmt->execute();
    print $xmlOut;
    print "\ndone\n";
  } catch( Exception $e ) { 
    $err = $this->db->errorInfo();
    $cod = $this->db->errorCode();
    echo("error ".$cod." ".$err[0]." ".$err[1]." ".$err[2]);
  }
}
public function ZZARRAY2() {
// XXX: This shouldn't hardcode the xmlservice library.
// But it does use test programs not in the system xmlservice distribution...
$clob = <<<ENDPROC
<?xml version='1.0'?>
<script>
<cmd comment='addlible'>ADDLIBLE LIB(XMLSERVICE) POSITION(*FIRST)</cmd>
<pgm name='ZZSRV' lib='XMLSERVICE' func='ZZARRAY2'>
 <parm comment='search this name'>
  <data var='myName' type='10A'>Ranger</data>
 </parm>
 <parm comment='max allowed return'>
  <data var='myMax' type='10i0'>5</data>
 </parm>
 <parm comment='actual count returned'>
  <data var='myCount' type='10i0' enddo='mycount'>0</data>
 </parm>
 <parm comment='array return'>
  <ds var='dcRec_t' dim='999' dou='mycount'>
    <data var='dcMyName' type='10A'>na</data>
    <data var='dcMyJob' type='4096A'>na</data>
    <data var='dcMyRank' type='10i0'>0</data>
    <data var='dcMyPay' type='12p2'>0.0</data>
  </ds>
 </parm>
</pgm>
</script>
ENDPROC;
return $clob;
}
}

$testcase = new Test();
$testcase->runTest();
?>
--EXPECTF--
<?xml version='1.0'?>
<script>
<cmd comment='addlible'><success>%s</success>
</cmd>
<pgm name='%s' lib='%s' func='%s'>
<parm comment='search this name'>
<data var='myName' type='10A'>Ranger</data>
</parm>
<parm comment='max allowed return'>
<data var='myMax' type='10i0'>5</data>
</parm>
<parm comment='actual count returned'>
<data var='myCount' type='10i0' enddo='mycount'>5</data>
</parm>
<parm comment='array return'>
<ds var='dcRec_t' dim='999' dou='mycount'>
<data var='dcMyName' type='10A'>Ranger1</data>
<data var='dcMyJob' type='4096A'>Test 101</data>
<data var='dcMyRank' type='10i0'>11</data>
<data var='dcMyPay' type='12p2'>13.42</data>
</ds>
<ds var='dcRec_t' dim='999' dou='mycount'>
<data var='dcMyName' type='10A'>Ranger2</data>
<data var='dcMyJob' type='4096A'>Test 102</data>
<data var='dcMyRank' type='10i0'>12</data>
<data var='dcMyPay' type='12p2'>26.84</data>
</ds>
<ds var='dcRec_t' dim='999' dou='mycount'>
<data var='dcMyName' type='10A'>Ranger3</data>
<data var='dcMyJob' type='4096A'>Test 103</data>
<data var='dcMyRank' type='10i0'>13</data>
<data var='dcMyPay' type='12p2'>40.26</data>
</ds>
<ds var='dcRec_t' dim='999' dou='mycount'>
<data var='dcMyName' type='10A'>Ranger4</data>
<data var='dcMyJob' type='4096A'>Test 104</data>
<data var='dcMyRank' type='10i0'>14</data>
<data var='dcMyPay' type='12p2'>53.68</data>
</ds>
<ds var='dcRec_t' dim='999' dou='mycount'>
<data var='dcMyName' type='10A'>Ranger5</data>
<data var='dcMyJob' type='4096A'>Test 105</data>
<data var='dcMyRank' type='10i0'>15</data>
<data var='dcMyPay' type='12p2'>67.10</data>
</ds>
</parm>
<success><![CDATA[+++ success %s %s %s ]]></success>
</pgm>
</script>
done

