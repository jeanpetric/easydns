<?php

if (!filter_var($_POST['ip'], FILTER_VALIDATE_IP)) {
    exit(1);
} else {
    $ip = $_POST['ip'];
    $domain = $_POST['domain'];
    $hash = $_POST['hash'];
    $fp = fopen('table.txt', 'r');

    while ($line = fgets($fp))
    {
        $line = trim($line);
        $e = explode(" ", $line);
        if (!strcmp($e[0], $domain) && !strcmp($e[1], $hash)) {
            file_put_contents($e[0], $ip);
            break;
        }
    }

    fclose($fp);
}

?>
