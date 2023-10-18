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
				// This can return 0/1 on LUW or TRUE/FALSE on i
				echo $r["ID"] . ": " . $r["ENABLED"] . "\n";
			}
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTREGEX--
1: (1|TRUE)
0: (0|FALSE)
