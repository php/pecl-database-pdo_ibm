To run on IBM i:
- fvt.inc - no *LIBL (no system naming)
- fvt400.inc - enable *LIBL (system naming)

0) set-up libraries
strsql
create schema DB2
create schema DB2LIBL


1) set env vars:
PDOTEST_DSN=ibm:*LOCAL
PDOTEST_USER=DB2
PDOTEST_PASS=(password)
PDOTEST_LIBL=DB2LIBL QTEMP
PDOTEST_CURLIB=DB2LIBL

2) run tests
> cd tests
> pear run-tests *.phpt

Note: 
- library DB2 is example user profile for non-*LIBL tests (SQL naming).
- library DB2LIBL is example for *LIBL tests (system naming).
- isolation PDO::I5_TXN_READ_UNCOMMITTED 'create schema lib' sets journaling required (default IBM i)
- isolation PDO::I5_TXN_NO_COMMIT works with CRTLIB (no journaling)
