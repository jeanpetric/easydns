<?php

if (isset($_POST['domain'])) {
  $domain = $_POST['domain'];
  $miliseconds = round(microtime(true)*1000);
  $hash = md5($miliseconds);
  file_put_contents("table.txt", $domain . " " . $hash . "\n", FILE_APPEND);

  echo "Your domain: $domain <br>";
  echo "Your hash: $hash <br><br>";

  $_POST = array();
  
  echo "Success!";
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
        http://petric.esy.es/
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
