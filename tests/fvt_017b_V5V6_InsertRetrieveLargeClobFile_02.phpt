--TEST--
pdo_ibm: Insert and retrieve a very large clob file.
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

			/*
			 * Create the test table. We use UTF-8 because the
			 * input is UTF-8, but has an invalid character;
			 * converting will cause it to fail (silently, because
			 * SQLParamData errors are not checked as of 20230614).
			 *
			 * XXX: Set i5_override_ccsid=1208 in case?
			 */
			$create = 'CREATE TABLE animals (id INTEGER, my_clob clob ccsid 1208)';
			$result = $this->db->exec( $create );

			$fp = fopen( dirname(__FILE__) . "/large_clob.dat" , "r" );
			//$fp = fopen("large_clob.dat" , "r" ); // Test Tool
			$stmt = $this->db->prepare('insert into animals (id,my_clob) values (:id,:my_clob)');
			print "inserting from file stream\n";
			$stmt->bindValue( ':id' , 0 );
			$stmt->bindParam( ':my_clob' , $fp , PDO::PARAM_LOB );
			$stmt->execute();
			print "succesful\n";

			print "runnign query\n";
			$stmt = $this->db->prepare( 'select id,my_clob from animals' );

			$rs = $stmt->execute();
			$stmt->bindColumn( 'ID' , $id );
			$stmt->bindColumn( 'MY_CLOB' , $clob , PDO::PARAM_LOB );

			while ($stmt->fetch(PDO::FETCH_BOUND)) {
				var_dump( $id );
				//var_dump( $clob );
				$fp = fopen( dirname(__FILE__) . "/large_clob_out.dat" , "w" );
				//$fp = fopen("large_clob_out.dat" , "w" ); // Test Tool
				fwrite($fp , $clob);
				system( "diff " . dirname(__FILE__) . "/large_clob.dat " . dirname(__FILE__) . "/large_clob_out.dat" );
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
done
