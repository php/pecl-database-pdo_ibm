--TEST--
pdo_ibm: Client Info 
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			print "Attempting to connect..\n";
			$this->connect();
			
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_USERID));
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_ACCTSTR));
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_APPLNAME));
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_WRKSTNNAME));
		
			$this->db->setAttribute(PDO::SQL_ATTR_INFO_USERID, "MyUser");
			$this->db->setAttribute(PDO::SQL_ATTR_INFO_ACCTSTR, "MyAccountString");
			$this->db->setAttribute(PDO::SQL_ATTR_INFO_APPLNAME, "MyApp");
			$this->db->setAttribute(PDO::SQL_ATTR_INFO_WRKSTNNAME, "MyWorkStation");
			
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_USERID));
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_ACCTSTR));
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_APPLNAME));
			var_dump($this->db->getAttribute(PDO::SQL_ATTR_INFO_WRKSTNNAME));
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Attempting to connect..
string(0) ""
string(0) ""
string(0) ""
string(0) ""
string(6) "MyUser"
string(15) "MyAccountString"
string(5) "MyApp"
string(13) "MyWorkStation"