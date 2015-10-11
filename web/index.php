<?php

if (isset($_POST['domain'])) {
  $domain = $_POST['domain'];
  $miliseconds = round(microtime(true)*1000);
  $hash = md5($miliseconds);
  file_put_contents("table.txt", $domain . " " . $hash . "\n", FILE_APPEND);

  $url = $_SERVER['HTTP_HOST'];
  $prep_conf_file = "service http://$url/addressbook.php\n";
  $prep_conf_file .= "domain " . $domain . "\n";
  $prep_conf_file .= "hash " . $hash . "\n";
  $prep_conf_file .= "interval 300\n";

  $_POST = array();
  
  header("Content-type: text/plain");
  header("Content-Disposition: attachment; filename=conf.txt");
  echo $prep_conf_file;
  exit;
}

?>

<!DOCTYPE html>
<html>

<head>
  <title>EasyDNS</title>
</head>

<body>

  <h1>EasyDNS</h1>

<form action="index.php" method="POST">
  <table>
    <tr>
      <td>
        My domain (max 15 char):
      </td>
      <td>
	  http://<?php echo $_SERVER['HTTP_HOST']; ?>/
        <input type="text" maxlength="15" name="domain" />
      </td>
    </tr>
    <tr>
      <td colspan="2">
        <input type="submit" value="Register">
      </td>
    </tr>
  </table>
</form>

</body>

</html>
