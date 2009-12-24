--TEST--
pdo_ibm: Insert and retrieve a very large file.
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
				$drop = 'DROP TABLE animals';
				$result = $this->db->exec( $drop );
			} catch( Exception $e ){}

			/* Create the test table */
			$create = 'CREATE TABLE animals (id INTEGER, my_blob blob)';
			$result = $this->db->exec( $create );

			$fp = fopen( dirname(__FILE__) . "/large_blob.dat" , "rb" );
			//$fp = fopen("large_blob.dat" , "rb" ); // Test Tool
			$stmt = $this->db->prepare('insert into animals (id,my_blob) values (:id,:my_blob)');
			print "inserting from file stream\n";
			$stmt->bindValue( ':id' , 0 );
			$stmt->bindParam( ':my_blob' , $fp , PDO::PARAM_LOB );
			$stmt->execute();
			print "succesful\n";

			print "runnign query\n";
			$stmt = $this->db->prepare( 'select id,my_blob from animals' );


			$rs = $stmt->execute(); // Must execute before bind by column name
			$stmt->bindColumn( 'ID' , $id );
			$stmt->bindColumn( 'MY_BLOB' , $blob , PDO::PARAM_LOB );
			while ($stmt->fetch(PDO::FETCH_BOUND)){
				var_dump( $id );
				var_dump( $blob );
				$fp = fopen( dirname(__FILE__) . "/large_blob_out.dat" , "wb" );
				//$fp = fopen("large_blob_out.dat" , "wb" ); // Test Tool
				//echo "datalength: " . stream_copy_to_stream( $blob , $fp ) . "\n";
				echo "datalength: " . fwrite($fp, $blob) . "\n";
				system( "diff large_blob.dat large_blob_out.dat" );
			}
			print "done\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
inserting from file stream
succesful
runnign query
string(1) "0"
string(4966) %s
done
