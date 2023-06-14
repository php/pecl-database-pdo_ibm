--TEST--
pdo_ibm: Change fetch modes.
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class animalObj {
		// Define both. The upper-case one is set instead of lower case, as that's what the driver returns to PDO.
		public $id, $breed;
		public $ID, $BREED;
		public function __construct() {
		}
	}
	class Test extends FVTTest {
		public function runTest() {
			$this->connect();
			$this->prepareDB();
			$sql = "SELECT * FROM animals";
			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_NUM);
			$row = $stmt->fetch();
			print "As row column numbers: " . $row[0] . " " . $row[1] . " " . $row[2] . "\n";

			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_ASSOC);
			$row = $stmt->fetch();
			print "As row column names: " . $row["ID"] . "\n" ;

			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_BOTH);
			$row = $stmt->fetch();
			print "As row column numbers: " . $row[0] . " " . $row[1] . " " . $row[2] . "\n";

			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_BOTH);
			$row = $stmt->fetch();
			print "As row column names: " . $row["ID"] . "\n" ;

			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_ASSOC);
			$row = $stmt->fetch();
			print "Rows not available: " .  $row[1] . "\n" ;

			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_COLUMN, 0);
			$row = $stmt->fetch();
			print "As row column number: " .  $row[0] . "\n" ;

			$stmt = $this->db->query($sql);
			$result = $stmt->setFetchMode(PDO::FETCH_COLUMN, 0);
			$row = $stmt->fetch();
			print "Rows not available: " .  $row[1] . "\n" ;

			$sth = $this->db->prepare('SELECT id, breed FROM animals' ); 
			$sth->bindColumn(1, $id);
			$sth->bindColumn(2, $breed);
			$sth->execute();
			
			$result = $sth->setFetchMode(PDO::FETCH_BOUND);
			while( $row = $sth->fetch() ) {
				print "The id is: " . $id . " Breed is: " . $breed . "\n";
				print "Result in row : " .  $row . "\n" ;
			}

			$sth = $this->db->prepare('SELECT id, breed FROM animals WHERE id > 2' );
			$sth->setFetchMode(PDO::FETCH_INTO, new animalObj );
			$sth->execute();
			foreach($sth as $obj) {
				var_dump($obj);
			}

			$stmt = $this->db->prepare('SELECT id, breed FROM animals' );
			$stmt->setFetchMode(PDO::FETCH_CLASS, 'animalObj', array(0));
			$stmt->execute();
			foreach($stmt as $obj) {
				var_dump($obj);
			}

			$stmt = $this->db->prepare('SELECT id, breed FROM animals' );
			$stmt->setFetchMode(PDO::FETCH_OBJ);
			$data = $stmt->execute();
			foreach($stmt as $obj) {
				var_dump($obj);
			}

			$stmt = $this->db->prepare('SELECT id, breed FROM animals' );
			$stmt->setFetchMode(PDO::FETCH_LAZY);
			$data = $stmt->execute();
			try {
				foreach($stmt as $obj) {
					var_dump($obj);
				}
			} catch( Exception $e) {
				print "Error: " . $stmt->errorCode() . "\n";
			}
		}
	}
        
	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
As row column numbers: 0 cat Pook            
As row column names: 0
As row column numbers: 0 cat Pook            
As row column names: 0

%s 1 in %s
Rows not available: 
As row column number: 0

%s 1 in %s
Rows not available: 
The id is: 0 Breed is: cat
Result in row : 1
The id is: 1 Breed is: dog
Result in row : 1
The id is: 2 Breed is: horse
Result in row : 1
The id is: 3 Breed is: gold fish
Result in row : 1
The id is: 4 Breed is: budgerigar
Result in row : 1
The id is: 5 Breed is: goat
Result in row : 1
The id is: 6 Breed is: llama
Result in row : 1
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "3"
  ["BREED"]=>
  string(9) "gold fish"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "4"
  ["BREED"]=>
  string(10) "budgerigar"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "5"
  ["BREED"]=>
  string(4) "goat"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "6"
  ["BREED"]=>
  string(5) "llama"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "0"
  ["BREED"]=>
  string(3) "cat"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "1"
  ["BREED"]=>
  string(3) "dog"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "2"
  ["BREED"]=>
  string(5) "horse"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "3"
  ["BREED"]=>
  string(9) "gold fish"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "4"
  ["BREED"]=>
  string(10) "budgerigar"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "5"
  ["BREED"]=>
  string(4) "goat"
}
object(animalObj)#%d (4) {
  ["id"]=>
  NULL
  ["breed"]=>
  NULL
  ["ID"]=>
  string(1) "6"
  ["BREED"]=>
  string(5) "llama"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "0"
  ["BREED"]=>
  string(3) "cat"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "1"
  ["BREED"]=>
  string(3) "dog"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "2"
  ["BREED"]=>
  string(5) "horse"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "3"
  ["BREED"]=>
  string(9) "gold fish"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "4"
  ["BREED"]=>
  string(10) "budgerigar"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "5"
  ["BREED"]=>
  string(4) "goat"
}
object(stdClass)#%d (2) {
  ["ID"]=>
  string(1) "6"
  ["BREED"]=>
  string(5) "llama"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "0"
  ["BREED"]=>
  string(3) "cat"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "1"
  ["BREED"]=>
  string(3) "dog"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "2"
  ["BREED"]=>
  string(5) "horse"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "3"
  ["BREED"]=>
  string(9) "gold fish"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "4"
  ["BREED"]=>
  string(10) "budgerigar"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "5"
  ["BREED"]=>
  string(4) "goat"
}
object(PDORow)#%d (3) {
  ["queryString"]=>
  string(29) "SELECT id, breed FROM animals"
  ["ID"]=>
  string(1) "6"
  ["BREED"]=>
  string(5) "llama"
}
