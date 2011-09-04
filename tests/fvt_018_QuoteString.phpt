--TEST--
pdo_ibm: Quote a string.
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
			print "Connection succeeded.\n";
			$string = 'Nice';
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = 'Naughty \' string';
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = "Co'mpl''ex \"st'\"ring";
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = "''''"; 
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = "";
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = NULL;
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = "'";
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
			$string = "'quoted'";
			print "Unquoted string: $string\n";
			print "Quoted string: " . $this->db->quote($string) . "\n";
		}
	}

	$testcase = new Test();
	$testcase->runTest();
?>
--EXPECT--
Connection succeeded.
Unquoted string: Nice
Quoted string: 'Nice'
Unquoted string: Naughty ' string
Quoted string: 'Naughty '' string'
Unquoted string: Co'mpl''ex "st'"ring
Quoted string: 'Co''mpl''''ex "st''"ring'
Unquoted string: ''''
Quoted string: ''''''''''
Unquoted string: 
Quoted string: ''
Unquoted string: 
Quoted string: ''
Unquoted string: '
Quoted string: ''''
Unquoted string: 'quoted'
Quoted string: '''quoted'''
