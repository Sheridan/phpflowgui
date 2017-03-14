<?php
//error_reporting(E_ALL);
$base_name=$_GET['i'];
$overlay_name=$_GET['o'];
$base = imagecreatefrompng("img/{$base_name}.png");
$overlay = imagecreatefrompng("img/overlays/{$overlay_name}.png");
imagecopyresampled($base, $overlay, 0, 0, 0, 0, 16, 16, 16, 16);
imagedestroy($overlay);
header("Content-type: image/png; name=\"{$base_name}_{$overlay_name}.png\"");
imagepng($base);
imagedestroy($base);
?>
