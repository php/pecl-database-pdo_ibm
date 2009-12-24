--TEST--
pdo_ibm: Test the execution of a basic stored procedure
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
	require_once('fvt.inc');
	class Test extends FVTTest {
		public function runTest() {
			$this->connect();
			$this->prepareDB();
			try{
				$result = $this->db->exec("DROP PROCEDURE SP_Example");
			} catch( Exception $e ){}

			$server_info = $this->db->getAttribute(PDO::ATTR_SERVER_INFO);
			$create = "";
			if( strncmp( $server_info, "DB2", 3 ) == 0 
			||  strncmp( $server_info, "AS", 2 ) == 0
			||  strncmp( $server_info, "QSQ", 3 ) == 0 )
			{
$create = <<<ENDPROC
  CREATE PROCEDURE SP_Example ()
  RESULT SETS 3
  LANGUAGE SQL
  BEGIN
    DECLARE c1 CURSOR WITH RETURN FOR
      SELECT name, id
      FROM animals
      ORDER BY name;

    OPEN c1;
  END
ENDPROC;
			}
			else if( strncmp( $server_info, "IDS", 3 ) == 0 )
			{
$create = <<<ENDPROC2
CREATE PROCEDURE SP_Example ()
 RETURNING char(16), int;

  DEFINE animalName char(16);
  DEFINE intvalue int;

  FOREACH
    SELECT name, id
    INTO animalName, intvalue
    FROM animals
    ORDER BY name
    RETURN animalName,intvalue;
  END FOREACH;

  END PROCEDURE
ENDPROC2;
			}
			$result = $this->db->exec($create);

			/* EXECUTE THE STORED PROCEDURE */
			$stmt = $this->db->query('CALL SP_Example()');

			/* FIRST RESULT SET */
			while ($row = $stmt->fetch()) {
				var_dump($row);
			}

		}
	}
	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECTREGEX--
(array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Bubbles         "
  \[0\]=>
  string\(16\) "Bubbles         "
  \["ID"\]=>
  string\(1\) "3"
  \[1\]=>
  string\(1\) "3"
\}
array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Gizmo           "
  \[0\]=>
  string\(16\) "Gizmo           "
  \["ID"\]=>
  string\(1\) "4"
  \[1\]=>
  string\(1\) "4"
\}
array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Peaches         "
  \[0\]=>
  string\(16\) "Peaches         "
  \["ID"\]=>
  string\(1\) "1"
  \[1\]=>
  string\(1\) "1"
\}
array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Pook            "
  \[0\]=>
  string\(16\) "Pook            "
  \["ID"\]=>
  string\(1\) "0"
  \[1\]=>
  string\(1\) "0"
\}
array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Rickety Ride    "
  \[0\]=>
  string\(16\) "Rickety Ride    "
  \["ID"\]=>
  string\(1\) "5"
  \[1\]=>
  string\(1\) "5"
\}
array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Smarty          "
  \[0\]=>
  string\(16\) "Smarty          "
  \["ID"\]=>
  string\(1\) "2"
  \[1\]=>
  string\(1\) "2"
\}
array\(4\) \{
  \["NAME"\]=>
  string\(16\) "Sweater         "
  \[0\]=>
  string\(16\) "Sweater         "
  \["ID"\]=>
  string\(1\) "6"
  \[1\]=>
  string\(1\) "6"
\})|(array\(3\) \{
  \[1\]=>
  string\(16\) "Bubbles         "
  \[2\]=>
  string\(1\) "3"
  \[3\]=>
  string\(1\) "3"
\})
