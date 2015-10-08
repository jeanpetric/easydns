<?php

if (!filter_var($_POST['url'], FILTER_VALIDATE_URL)) {
  exit(1);
} else {
  file_put_contents('jeanpetric', $_POST['url']);
}

?>
