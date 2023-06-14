--TEST--
pdo_ibm: Make sure SQLParamData fails for invalid inputs for data-on-exec (IBM i)
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
			 * make sure the test table has encoding that will fail conversion
			 */
			$create = 'CREATE TABLE animals (id INTEGER, my_clob clob ccsid 37)';
			$result = $this->db->exec( $create );

			$fp = fopen( dirname(__FILE__) . "/large_clob.dat" , "r" );
			//$fp = fopen("large_clob.dat" , "r" ); // Test Tool
			$stmt = $this->db->prepare('insert into animals (id,my_clob) values (:id,:my_clob)');
			$stmt->bindValue( ':id' , 0 );
			$stmt->bindParam( ':my_clob' , $fp , PDO::PARAM_LOB );
			$stmt->execute();
			// XXX: This error is i specific; I'd like to have an equivalent test for LUW.
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTF--
Fatal error: Uncaught PDOException: SQLSTATE[22504]: <<Unknown error>>: -191 Mixed data or UTF-8 data not properly formed. (SQLParamData[-191] at %s:%d) in %s:%d
Stack trace:
#0 %s(%d): PDOStatement->execute()
#1 %s(%d): Test->runTest()
#2 {main}
  thrown in %s on line %d
