<?php
abstract class FVTTest
{
	public $db = null;
	public $dsn = null;
	public $user = null;
	public $pass = null;
	public $libl = null;
	public $curlib = null;

	public function __construct( $_dsn = null , $_user = null , $_pass = null, $_libl = null , $_curlib = null)
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

		/* IBM i settings */
		if( $_libl == null )
		{
			$this->libl = getenv('PDOTEST_LIBL');
		}
		else
		{
			$this->libl = $_libl;
		}

		if( $_curlib == null )
		{
			$this->curlib = getenv('PDOTEST_CURLIB');
		}
		else
		{
			$this->curlib = $_curlib;
		}
	}

	public function connect($autoCommit=true, $useLibl=false, $useIsolation=false)
	{
		$options = array(PDO::ATTR_AUTOCOMMIT=>$autoCommit);

		/* IBM i settings */
		if ($useLibl) {
			$options[PDO::I5_ATTR_DBC_SYS_NAMING] = true;
			if ($this->libl) {
				$options[PDO::I5_ATTR_DBC_LIBL] = $this->libl;
			}
			if ($this->curlib) {
				$options[PDO::I5_ATTR_DBC_CURLIB] = $this->curlib;
			}
			/* Isolation modes
			PDO::I5_ATTR_COMMIT
			PDO::I5_TXN_NO_COMMIT
			PDO::I5_TXN_READ_UNCOMMITTED
			PDO::I5_TXN_READ_COMMITTED 
			PDO::I5_TXN_REPEATABLE_READ
			PDO::I5_TXN_SERIALIZABLE
			*/
			if ($useIsolation !== false) {
				$options[PDO::I5_ATTR_COMMIT] = $useIsolation;
			}
		}
		$this->db = new PDO($this->dsn, $this->user, $this->pass, $options);
		$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$this->db->setAttribute(PDO::ATTR_CASE, PDO::CASE_UPPER);
		$this->db->setAttribute(PDO::ATTR_STRINGIFY_FETCHES, true);
		return $this->db;
	}

	abstract public function runTest();
} 

?>
