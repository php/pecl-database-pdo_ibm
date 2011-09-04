<?php
	abstract class FVTTest
	{
		public $db = null;
		public $dsn = null;
		public $user = null;
		public $pass = null;

		public function __construct( $_dsn = null , $_user = null , $_pass = null )
		{
			if( $_dsn == null )
			{
				$this->dsn = getenv('PDOTEST_DSN');
			}
			else
			{
				$this->dsn = $_dsn;
			}

			if( $_user == null )
			{
				$this->user = getenv('PDOTEST_USER');
			}
			else
			{
				$this->user = $_user;
			}

			if( $_pass == null )
			{
				$this->pass = getenv('PDOTEST_PASS');
			}
			else
			{
				$this->pass = $_pass;
			}
		}

		public function connect($autoCommit=true)
		{
			$this->db = new PDO($this->dsn, $this->user, $this->pass, array( PDO::ATTR_AUTOCOMMIT => $autoCommit ));
			$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			$this->db->setAttribute(PDO::ATTR_CASE, PDO::CASE_UPPER);
			$this->db->setAttribute(PDO::ATTR_STRINGIFY_FETCHES, true);
			return $this->db;
		}
        
		public function prepareDB()
		{
			try {
				/* Drop the test table, in case it exists */
				$drop = 'DROP TABLE animals';
				$result = $this->db->exec( $drop );
			} catch( Exception $e ){}

			/* Create the test table */
			$create = 'CREATE TABLE animals (id INTEGER, breed VARCHAR(32), name CHAR(16), weight DECIMAL(7,2))';
			$result = $this->db->exec( $create );

			/* Populate the test table */
			$animals = array(
					array(0, 'cat', 'Pook', 3.2),
					array(1, 'dog', 'Peaches', 12.3),
					array(2, 'horse', 'Smarty', 350.0),
					array(3, 'gold fish', 'Bubbles', 0.1),
					array(4, 'budgerigar', 'Gizmo', 0.2),
					array(5, 'goat', 'Rickety Ride', 9.7),
					array(6, 'llama', 'Sweater', 150)
			);

			$insert = 'INSERT INTO animals (id, breed, name, weight) VALUES (?, ?, ?, ?)';
			$stmt = $this->db->prepare( $insert );
			if ($stmt)
			{
				foreach ($animals as $animal)
				{
					$result = $stmt->execute($animal);
				}
			}
		}

		public function allowViperXML() {
			$server_info = $this->db->getAttribute( PDO::ATTR_SERVER_INFO );
			// DB2 LUW returns "DB2" (maybe?)
			// i5/OS DB2 returns "AS." or "QSQ" (AS400?)
			if(strncmp($server_info,"DB2",3)==0) return True; 
      			return False;
		}

		abstract public function runTest();
	} 

// added for select test running
// based on machine type and release
class TestSkip extends FVTTest {
	public function runTest() {
		$this->connect();
		try {
			$dbms_name = $this->db->getAttribute(PDO::ATTR_SERVER_INFO);
			if ($dbms_name == "QSQ" || $dbms_name == "AS") {
				$dbms_ver = $this->db->getAttribute(PDO::ATTR_SERVER_VERSION);
				if ($dbms_ver > "06000") {
					$rel = "/V6/";
				}
				else {
					$rel = "/V5/";
				}
				// PASE test_xxx_V5V6.phpt
				if (preg_match("/V5|V6/", $_SERVER["PHP_SELF"])) {
					if (!preg_match($rel, $_SERVER["PHP_SELF"])) {
						die('skip');
					}
				}
				// check the skipif.PASE
				else {
					$result = file_get_contents("skipif.PASE");
					for ($line = strtok($result, "\n"); $line; $line = strtok("\n")) {
						$pos = strpos($line, "PASE");
						if ($pos && strpos($line, "|$rel|")){
							$check = substr($line, 1, $pos-6);
							if (strpos($_SERVER["PHP_SELF"], $check)) {
								die('skip');
							}
						}
					}
				}
			}
			else {
				if (preg_match("/V5|V6/", $_SERVER["PHP_SELF"])) die('skip');
			}
		} catch (PDOException $pe) {  
			if (preg_match("/V5|V6/", $_SERVER["PHP_SELF"])) die('skip');
		}
	}
}
$testcase = new TestSkip();
$testcase->runTest();

?>
