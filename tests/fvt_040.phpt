<?
    function print_usage( $name ){
        print "Usage: php $name (informix|db2|ibm) <file>+\n";
        exit;
    }

    $argv = $_SERVER['argv'];
    $argc = $_SERVER['argc'];

    if( $argc < 3 ){
        print_usage( $argv[0] );
    }


    if( strcasecmp(trim($argv[1]), "informix")==0 ){
        $namespace = "informix";
		$namespaceUP = "INFORMIX";
        $ifDefStr = "DB2";
        $defStr   = "INFORMIX";
    }else if( strcasecmp(trim($argv[1]), "db2")==0 ){
        $namespace = "db2";
		$namespaceUP = "DB2";
        $ifDefStr = "INFORMIX";
        $defStr   = "DB2";
    }else if( strcasecmp(trim($argv[1]), "ibm")==0 ){
        $namespace = "ibm";
                $namespaceUP = "IBM";
        $ifDefStr = "INFORMIX";
        $defStr   = "IBM";
    }else {
        print_usage( $argv[0] );
    }
    $ifdef_toggle = true;

    for( $i=2;$i<$argc;$i++ ){
        $lines = file( $argv[$i] );
        for( $j=0;$j<count($lines);$j++ ){
            $line = trim($lines[$j]);

            if( strcmp($line,"IF_$ifDefStr")==0 ){
                $ifdef_toggle = false;
                continue;
            }
            else if( strcmp($line,"ENDIF_$ifDefStr")==0 ){
                $ifdef_toggle = true;
                continue;
            }else if( 
                strcmp($line,'IF_INFORMIX') == 0 || 
                strcmp($line,'ENDIF_INFORMIX') == 0 || 
                strcmp($line,'IF_DB2') == 0 || 
                strcmp($line,'ENDIF_DB2') == 0 ){
                continue;
            }
                
            if( $ifdef_toggle ){
                $line = $lines[$j];
                $mline = $line;
                $mline = ereg_replace( 'NAMESPACEUP' , $namespaceUP, $mline );
                $mline = ereg_replace( 'NAMESPACE' , $namespace, $mline );
                $mline = ereg_replace( 'PDO_IBM' , 'PDO_' . $defStr, $mline );
                print $mline;
            }
        }
    }
?>
