--TEST--
pdo_ibm: Trusted Context
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
require_once('fvt.inc');

class Test extends FVTTest {

	public $authID = null;
	public $authPass = null;
	public $tcuser = null;
	public $tcpass = null;
	public $hostname = null;

	public function runTest() {
		$this->getVariables();
		$db = $this->connect();
		$this->createDB($db);

		$this->setup($db, "WITH AUTHENTICATION", "INSERT");

		//Operation with wrong user name and password.
		$tcdb = $this->connectTC(true);
		if($tcdb) {
			if($tcdb->getAttribute(PDO::SQL_ATTR_USE_TRUSTED_CONTEXT)) {
				$userBefore = $tcdb->getAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID);

				//Switching user.
				$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID, "fakeuser");
				$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_PASSWORD, "fakepass");

				$userAfter = $tcdb->getAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID);

				if($userAfter != $userBefore) {
					echo "User has been switched.\n";
					$this->dbOperations($tcdb);
				}
			}
		}

		//Passing password before user while switching user.
		$tcdb = $this->connectTC(true);
		if($tcdb) {
			if($tcdb->getAttribute(PDO::SQL_ATTR_USE_TRUSTED_CONTEXT)) {
				//Switching user.
				try {
					$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_PASSWORD, $this->tcpass);
					$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID, $this->tcuser);
				} catch (PDOException $ex) {
					print_r($tcdb->errorInfo());
				}
			}
		}

		//Normal opeartion.
		$tcdb = $this->connectTC(true);
		if($tcdb) {
			if($tcdb->getAttribute(PDO::SQL_ATTR_USE_TRUSTED_CONTEXT)) {

				$userBefore = $tcdb->getAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID);

				//Switching user.
				$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID, $this->tcuser);
				$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_PASSWORD, $this->tcpass);

				$userAfter = $tcdb->getAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID);

				if($userAfter != $userBefore) {
					echo "User has been switched.\n";
					$this->dbOperations($tcdb);
				}
			}
		}

		$this->setup($db, "WITHOUT AUTHENTICATION", "UPDATE");

		//Normal opeartion.
		$tcdb = $this->connectTC(true);
		if($tcdb) {
			if($tcdb->getAttribute(PDO::SQL_ATTR_USE_TRUSTED_CONTEXT)) {
				$userBefore = $tcdb->getAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID);

				//Switching user.
				$tcdb->setAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID, $this->tcuser);

				$userAfter = $tcdb->getAttribute(PDO::SQL_ATTR_TRUSTED_CONTEXT_USERID);

				if($userAfter != $userBefore) {
					echo "User has been switched.\n";
					$this->dbOperations($tcdb);
				}
			}
		}
		//Write the test here, switch user and do operations.

		$this->dropDB($db);
	}

	public function getVariables() {

		$this->authID = getenv('PDOTEST_AUTHID');
		$this->authPass = getenv('PDOTEST_AUTHPASS');
		$this->tcuser = getenv('PDOTEST_TCUSER');
		$this->tcpass = getenv('PDOTEST_TCPASS');

		if(strstr($this->dsn, "HOSTNAME")) {
			$temp = split("HOSTNAME=", $this->dsn);
			$temp = split(";", $temp[1]);
			$this->hostname = $temp[0];
		} else {
			$this->hostname = "localhost";
		}
	}

	public function setup($db, $authType = "WITH AUTHENTICATION", $grant = "INSERT") {

		//Preparing all the SQL statements.
		$sql_drop_role = "DROP ROLE role_01";
		$sql_drop_trusted_context = "DROP TRUSTED CONTEXT ctx";

		$sql_create_role = "CREATE ROLE role_01";
		$sql_create_trusted_context = "CREATE TRUSTED CONTEXT ctx BASED UPON CONNECTION USING SYSTEM AUTHID ";
		$sql_create_trusted_context .= $this->authID;
		$sql_create_trusted_context .= " ATTRIBUTES (ADDRESS '";
		$sql_create_trusted_context .= $this->hostname;
		$sql_create_trusted_context .= "') DEFAULT ROLE role_01 ENABLE WITH USE FOR ";
		$sql_create_trusted_context .= $this->tcuser . " ";
		$sql_create_trusted_context .= $authType;


		$sql_grant_permission = "GRANT " . $grant . " ON TABLE trusted_table TO ROLE role_01";

		//Executing the SQLs.
		try {
			$db->exec($sql_drop_trusted_context);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_drop_role);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_create_role);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_create_trusted_context);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_grant_permission);
		} catch (PDOException $ex) { }
	}

	public function dropDB($db) {
		//Print contain of database.
		$this->printTable($db);

		//Preparing all the SQL statements.
		$sql_drop_table = "DROP TABLE trusted_table";
		$sql_drop_role = "DROP ROLE role_01";
		$sql_drop_trusted_context = "DROP TRUSTED CONTEXT ctx";

		//Executing the SQLs.
		try {
			$db->exec($sql_drop_table);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_drop_trusted_context);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_drop_role);
		} catch (PDOException $ex) { }
	}

	public function connectTC($useTC=true) {
		$db = new PDO($this->dsn, $this->authID, $this->authPass, array(PDO::SQL_ATTR_USE_TRUSTED_CONTEXT => $useTC));
		$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$db->setAttribute(PDO::ATTR_CASE, PDO::CASE_UPPER);
		$db->setAttribute(PDO::ATTR_STRINGIFY_FETCHES, true);
		return $db;
	}

	public function createDB ($db) {
		$sql_drop_table = "DROP TABLE trusted_table";

		$sql_create_table = "CREATE TABLE trusted_table(i1 int,i2 int)";
		$sql_insert = "INSERT INTO trusted_table (i1, i2) VALUES (?, ?)";

		try {
			$db->exec($sql_drop_table);
		} catch (PDOException $ex) { }

		try {
			$db->exec($sql_create_table);
		} catch (PDOException $ex) { }

		$stmt = $db->prepare( $sql_insert );
		if($stmt) {
			for($i = 1;$i <= 2;$i++) {
				$result = $stmt->execute(array($i * 10, $i * 20));
			}

		}

		$this->printTable($db);
	}

	public function dbOperations ($db) {
		$sql_insert = "INSERT INTO $this->user.trusted_table (i1, i2) VALUES (100, 200)";
		$sql_update = "UPDATE $this->user.trusted_table set i1 = 2000 WHERE i2 = 20";

		try {
			$db->exec($sql_insert);
		} catch (PDOException $ex) {
			print_r($db->errorInfo());
		}

		try {
			$db->exec($sql_update);
		} catch (PDOException $ex) {
			print_r($db->errorInfo());
		}
	}

	public function printTable($db) {
		$sql = "SELECT * FROM trusted_table";
		$stmt = $db->prepare($sql);
		$res = $stmt->execute();
		while ($result = $stmt->fetch()) {
			var_dump($result);
		}
	}
}

$testcase = new Test();
$testcase->runTest();
?>
--EXPECTF--
array(4) {
  ["I1"]=>
  string(2) "10"
  [0]=>
  string(2) "10"
  ["I2"]=>
  string(2) "20"
  [1]=>
  string(2) "20"
}
array(4) {
  ["I1"]=>
  string(2) "20"
  [0]=>
  string(2) "20"
  ["I2"]=>
  string(2) "40"
  [1]=>
  string(2) "40"
}
User has been switched.
Array
(
    [0] => 08001
    [1] => -30082
    [2] => [%s][%s][%s] SQL30082N  Security processing failed with reason "24" ("USERNAME AND/OR PASSWORD INVALID").  SQLSTATE=08001
 (SQLExecDirect[-30082] at %s:%d)
)
Array
(
    [0] => 40003
    [1] => -900
    [2] => [%s][%s][%s] SQL0900N  The application state is in error.  A database connection does not exist.  SQLSTATE=08003
 (SQLExecDirect[-900] at %s:%d)
)
Array
(
    [0] => HY010
    [1] => -99999
    [2] => [%s][%s] CLI0198E  Missing trusted context userid. SQLSTATE=HY010 (SQLSetConnectAttr[-99999] at %s:%d)
)
User has been switched.
Array
(
    [0] => 42501
    [1] => -551
    [2] => [%s][%s][%s] SQL0551N  "%s" does not have the privilege to perform operation "UPDATE" on object "%s.TRUSTED_TABLE".  SQLSTATE=42501
 (SQLExecDirect[-551] at %s:%d)
)
User has been switched.
Array
(
    [0] => 42501
    [1] => -551
    [2] => [%s][%s][%s] SQL0551N  "%s" does not have the privilege to perform operation "INSERT" on object "%s.TRUSTED_TABLE".  SQLSTATE=42501
 (SQLExecDirect[-551] at %s:%d)
)
array(4) {
  ["I1"]=>
  string(4) "2000"
  [0]=>
  string(4) "2000"
  ["I2"]=>
  string(2) "20"
  [1]=>
  string(2) "20"
}
array(4) {
  ["I1"]=>
  string(2) "20"
  [0]=>
  string(2) "20"
  ["I2"]=>
  string(2) "40"
  [1]=>
  string(2) "40"
}
array(4) {
  ["I1"]=>
  string(3) "100"
  [0]=>
  string(3) "100"
  ["I2"]=>
  string(3) "200"
  [1]=>
  string(3) "200"
}
