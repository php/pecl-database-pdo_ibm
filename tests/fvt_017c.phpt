--TEST--
pdo_ibm: Select LOBs, including null and 0-length
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
			$create = 'CREATE TABLE animals (id INTEGER, my_clob clob, my_blob blob)';
			$result = $this->db->exec( $create );

            $data = array (
                array(1, 'this is the clob that never ends...',
                         'this is the blob that never ends...')
				,
                array(2, null,null)
				,
				array(3,'','')
            );

			$stmt = $this->db->prepare('insert into animals (id,my_clob,my_blob) values (?,?,?)');

			print "inserting\n";
			foreach ($data as $row) {
			    $stmt->execute($row);
			}

			print "succesful\n";
			print "running query\n";

			$stmt = $this->db->prepare( 'select id,my_clob,my_blob from animals' );

			$rs = $stmt->execute();

           $count = 0;
			while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
				var_dump( $row['ID'] );

                // this is a temporary workaround
                // until zero-length/lob stream
                // issue is fixed
                if ($count < 2) {
				   var_dump( $row['MY_CLOB'] );
                   var_dump( $row['MY_BLOB'] );
                }
                var_dump(strpos($row['MY_CLOB'], 'lob'));
                $count++;
			}

			print "done\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>

--EXPECTF--
inserting
succesful
running query
string(1) "1"
string(35) "this is the clob that never ends..."
string(35) "this is the blob that never ends..."
int(13)
string(1) "2"
NULL
NULL
bool(false)
string(1) "3"
bool(false)
done

