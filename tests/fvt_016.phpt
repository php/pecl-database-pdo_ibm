--TEST--
pdo_ibm: Insert integer by binding an empty string, a NULL, and an integer string to column
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest
	{
		public function runTest()
		{
			$this->connect();

			try {
				/* Drop the test table, in case it exists */
				$drop   = 'DROP TABLE animals';
				$result = $this->db->exec( $drop );
			} catch( Exception $e ){}

			/* Create the test table */
			$create = 'CREATE TABLE animals (id INTEGER)';
			$result = $this->db->exec( $create );

			$null          = NULL;
			$empty_string0 = "";
			$empty_string1 = "";
			$int_string    = "0";

			$sql  = "INSERT INTO animals VALUES ( :mynull0 ) ";
			$stmt = $this->db->prepare ( $sql );
			$stmt->bindParam ( ":mynull0" , $null );
			$stmt->execute();
			$stmt = $this->db->query( "SELECT * FROM animals" );
			$res  = $stmt->fetch( PDO::FETCH_BOTH );
			$rows = $res[0];
			print "Null contents: $rows\n";

			$delete = 'DELETE FROM animals';
			$result = $this->db->exec( $delete );

			$sql  = "INSERT INTO animals VALUES ( :mynull1 ) ";
			$stmt = $this->db->prepare ( $sql );
			$stmt->bindParam ( ":mynull1" , $null, PDO::PARAM_INT );
			$stmt->execute();
			$stmt = $this->db->query( "SELECT * FROM animals" );
			$res  = $stmt->fetch( PDO::FETCH_BOTH );
			$rows = $res[0];
			print "Null contents with int specified: $rows\n";

			$delete = 'DELETE FROM animals';
			$result = $this->db->exec( $delete );

			$sql  = "INSERT INTO animals VALUES ( :myemptystring0 ) ";
			$stmt = $this->db->prepare ( $sql );
			$stmt->bindParam ( ":myemptystring0" , $empty_string0);
			$stmt->execute();
			$stmt = $this->db->query( "SELECT * FROM animals" );
			$res  = $stmt->fetch( PDO::FETCH_BOTH );
			$rows = $res[0];
			print "Empty string contents: $rows\n";

			$delete = 'DELETE FROM animals';
			$result = $this->db->exec( $delete );

			$sql  = "INSERT INTO animals VALUES ( :myemptystring1 ) ";
			$stmt = $this->db->prepare ( $sql );
			$stmt->bindParam ( ":myemptystring1" , $empty_string1, PDO::PARAM_INT );
			$stmt->execute();
			$stmt = $this->db->query( "SELECT * FROM animals" );
			$res  = $stmt->fetch( PDO::FETCH_BOTH );
			$rows = $res[0];
			print "Empty string contents with int specified: $rows\n";

			$delete = 'DELETE FROM animals';
			$result = $this->db->exec( $delete );

			$sql  = "INSERT INTO animals VALUES ( :myintstring0 ) ";
			$stmt = $this->db->prepare ( $sql );
			$stmt->bindParam ( ":myintstring0" , $int_string );
			$stmt->execute();
			$stmt = $this->db->query( "SELECT * FROM animals" );
			$res  = $stmt->fetch( PDO::FETCH_BOTH );
			$rows = $res[0];
			print "Int string contents: $rows\n";

			$delete = 'DELETE FROM animals';
			$result = $this->db->exec( $delete );

			$sql  = "INSERT INTO animals VALUES ( :myintstring1 ) ";
			$stmt = $this->db->prepare ( $sql );
			$stmt->bindParam ( ":myintstring1" , $int_string, PDO::PARAM_INT );
			$stmt->execute();
			$stmt = $this->db->query( "SELECT * FROM animals" );
			$res  = $stmt->fetch( PDO::FETCH_BOTH );
			$rows = $res[0];
			print "Int string contents with int specified: $rows\n";

			print "done\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Null contents: 
Null contents with int specified: 
Empty string contents: 0
Empty string contents with int specified: 
Int string contents: 0
Int string contents with int specified: 0
done
