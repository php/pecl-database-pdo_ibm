--TEST--
pdo_ibm: Boolean data type
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			// XXX: Skip on older LUW/i
			$this->connect();

			// Tests insert and select
			try {
				$this->db->exec("drop table booltest");
			} catch (Exception $e) {}
			$this->db->exec("create table booltest (id integer not null, enabled boolean, primary key(id))");
			
			$s = $this->db->prepare("insert into booltest (id, enabled) values (1, ?)");
			$x = true;
			$s->bindParam(1, $x, PDO::PARAM_BOOL);
			$r = $s->execute();;
			
			$s = $this->db->prepare("insert into booltest (id, enabled) values (2, ?)");
			$x = false;
			$s->bindParam(1, $x, PDO::PARAM_BOOL);
			$r = $s->execute();
			
			$s = $this->db->prepare("select * from booltest");
			$s->execute();
			while ($r = $s->fetch(PDO::FETCH_ASSOC)) {
				var_dump($r);
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
array(2) {
  ["ID"]=>
  string(1) "1"
  ["ENABLED"]=>
  string(4) "TRUE"
}
array(2) {
  ["ID"]=>
  string(1) "2"
  ["ENABLED"]=>
  string(5) "FALSE"
}
