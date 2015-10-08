<?php

if (!filter_var($_POST['ip'], FILTER_VALIDATE_URL)) {
  exit(1);
} else {
  $ip = $_POST['ip'];
  $hash = $_POST['hash'];
  $users = file("table.txt");

  foreach ($users as $user)
  {
    $e = explode(" ", $user);
    if ($e[1] == $hash) {
      file_put_contents($e[0], $ip);
    }
  }
}

?>
