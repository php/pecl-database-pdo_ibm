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

			// Use class constants in 8.4 or newer, 8.5 will show
			// a deprecation message when using the PDO constants
			$userID = version_compare(PHP_VERSION, '8.4.0') >= 0
				? \PDO\Ibm::ATTR_INFO_USERID : PDO::SQL_ATTR_INFO_USERID;
			$acctStr = version_compare(PHP_VERSION, '8.4.0') >= 0
				? \PDO\Ibm::ATTR_INFO_ACCTSTR : PDO::SQL_ATTR_INFO_ACCTSTR;
			$applName = version_compare(PHP_VERSION, '8.4.0') >= 0
				? \PDO\Ibm::ATTR_INFO_APPLNAME : PDO::SQL_ATTR_INFO_APPLNAME;
			$wrkstnName = version_compare(PHP_VERSION, '8.4.0') >= 0
				? \PDO\Ibm::ATTR_INFO_WRKSTNNAME : PDO::SQL_ATTR_INFO_WRKSTNNAME;
			
			var_dump($this->db->getAttribute($userID));
			var_dump($this->db->getAttribute($acctStr));
			var_dump($this->db->getAttribute($applName));
			// This will default to the hostname of the system nowadays.
			var_dump($this->db->getAttribute($wrkstnName));
		
			$this->db->setAttribute($userID, "MyUser");
			$this->db->setAttribute($acctStr, "MyAccountString");
			$this->db->setAttribute($applName, "MyApp");
			$this->db->setAttribute($wrkstnName, "MyWorkStation");
			
			var_dump($this->db->getAttribute($userID));
			var_dump($this->db->getAttribute($acctStr));
			var_dump($this->db->getAttribute($applName));
			var_dump($this->db->getAttribute($wrkstnName));
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Attempting to connect..
string(0) ""
string(0) ""
string(0) ""
string(%d) "%S"
string(6) "MyUser"
string(15) "MyAccountString"
string(5) "MyApp"
string(13) "MyWorkStation"
