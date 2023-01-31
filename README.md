# PDO_IBM

Interface for PHP to DB2 for z/OS, DB2 for LUW, DB2 for i.

## Pre-requisites

The minimum PHP version supported by driver is PHP 7.3 and the latest version supported is PHP 8.2.

## IBM i users

When running on IBM i, `PDO_IBM` doesn't link with the Db2 LUW client library,
but instead with libdb400, which provides a PASE wrapper for SQL/CLI. The
differences between SQL/CLI in IBM i and the LUW driver are wrapped for you.
You don't need Db2 Connect on IBM i as a result.

To install, make sure you have the new Yum-based OSS environment. Install PHP,
plus any dependencies like so:

```shell
yum install sqlcli-devel gcc make-gnu
```

Tony Cairns' [replacement libdb400](https://bitbucket.org/litmis/db2sock/src/master/db2/)
is not yet tested, but may be desirable due to its greater debugging features.

IBM i users should read `tests/README400.txt` in order to set up prequisites
for unit tests.

## LUW/z/Db2 Connect users

CLIDRIVER should be installed in your system.
If not installed Download from the below link.

<a name="downloadCli"></a> [DOWNLOAD CLI DRIVER](https://public.dhe.ibm.com/ibmdl/export/pub/software/data/db2/drivers/odbc_cli/)

PHP should be installed in your system.

## How to install php pdo_ibm extension in Linux/Mac

If `IBM_DB_HOME` and `LD_LIBRARY_PATH` environment variables are not set, then set them with installed CLIDRIVER.
(say CLIDRIVER installed at `/home/user/clidriver`)

```shell
export IBM_DB_HOME=/home/user/clidriver 
export LD_LIBRARY_PATH="${IBM_DB_HOME}/lib"
```

1. Install this extension:

   ```shell
   pecl install pdo_ibm
   ```
        
2. Open the `php.ini` file in an editor of your choice. Edit the extension entry in the
   `php.ini` file in the `<local_php_directory>/php/lib` directory to reference the PHP driver:

   ```ini
   extension=pdo_ibm.so
   ```
       
3. Ensure that the PHP driver can access the `libdb2.so` CLI driver file by
   setting the `LD_LIBRARY_PATH` variable for Linux and UNIX operating systems
   other than the AIX® operating system. For AIX operating system, you must set `LIBPATH` variable.

4. Optionally, if the PHP application that is connecting to an IBM database server is running in
   the HTTP server environment, add the `LD_LIBRARY_PATH` variable in the `httpd.conf` file.

## Prebuilt binaries for Windows

1. Add the `CLIDRIVER\bin` path to the `PATH` environment variable like so (for a batch file):

   ```shell
   set PATH=<CLIDRIVER installed path>\bin;%PATH%
   ```

2. Download the DLLs for PHP 7.x and 8.x from [the ibmdb repository](https://github.com/ibmdb/php_ibm_db2).
   Select the build for the PHP that matches the version, architecture, and thread model.

3. Open the `php.ini` file in an editor of your choice. Edit the extension entry in the
   `php.ini` file in the `<local_php_directory>\php\lib` directory to reference the driver:

   ```ini
   extension=php_pdo_ibm
   ```

## How to run sample program

Create a `connect.php` script with the following content:

```php
<?php

$dsn = 'ibm:<DSN NAME>';
$user = '<USER>';
$pass = '<PASSWORD>';

$pdo = new \PDO($dsn, $user, $pass);
```

To run the sample:

```
php connect.php
```

## Contributing:

The developer sign-off should include the reference to the DCO in defect remarks, like in this example:

```
DCO 1.1 Signed-off-by: Random J Developer <random@developer.org>
```
