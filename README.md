# PDO_IBM

Interface for PHP to DB2 for z/OS, DB2 for LUW, DB2 for i.

## Prerequisite

CLIDRIVER should be installed in your system.
If not installed Download from the below link.

<a name="downloadCli"></a> [DOWNLOAD CLI DRIVER](https://public.dhe.ibm.com/ibmdl/export/pub/software/data/db2/drivers/odbc_cli/)

PHP should be installed in your system.

## How to install php pdo_ibm extension in Linux/Mac
```
if IBM_DB_HOME and LD_LIBRARY_PATH environment variable not set then set them with installed CLIDRIVER.
(say CLIDRIVER installed at "/home/user/clidriver")

export IBM_DB_HOME=/home/user/clidriver 
export LD_LIBRARY_PATH=/home/user/clidriver/lib

1) pecl install pdo_ibm
        
2) Open the php.ini file in an editor of your choice. Edit the extension entry in the
   php.ini file in the <local_php_directory>/php/lib directory to reference the PHP driver:
       extension=pdo_ibm.so
       
3) Ensure that the PHP driver can access the libdb2.so CLI driver file by
   setting the LD_LIBRARY_PATH variable for Linux and UNIX operating systems
   other than the AIX® operating system. For AIX operating system, you must set LIBPATH variable. 

4) Optional: If the PHP application that is connecting to an IBM database server is running ini
   the HTTP server environment, add the LD_LIBRARY_PATH variable in the httpd.conf file.

```
## How to install php pdo_ibm extension in Windows
```
Set CLIDRIVER\bin path to PATH environment variable.

set PATH=<CLIDRIVER installed path>\bin;%PATH%

1. Download the php_pdo_ibm DLLs for PHP 7.x(7.0, 7.1, 7.2) from below link.
      https://github.com/ibmdb/php_ibm_db2
	  

2. Open the php.ini file in an editor of your choice. Edit the extension entry in the
   php.ini file in the <local_php_directory>\php\lib directory to reference the PHP driver:
	  extension=php_pdo_ibm
```

## How to run sample program

### connect.php:-

```
<?php
        $db = null;
        $dsn = "ibm:<DSN NAME>";
        $user = "<USER>";
        $pass = "<PASSWORD>";

        $pdo = new PDO($dsn, $user, $pass);
        if ($pdo)
           print "Connection Successful.\n";

?>


To run the sample:- php connect.php
```

## Contributing:
```
See CONTRIBUTING.md

The developer sign-off should include the reference to the DCO in defect remarks(example below):
DCO 1.1 Signed-off-by: Random J Developer <random@developer.org>
```
