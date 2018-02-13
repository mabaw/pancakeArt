<?php
function LoadJpeg($imgname)
{
 $im = @imagecreatefromjpeg($imgname); /* Attempt to open */
 if (!$im) { /* See if it failed */
 $im  = imagecreate(150, 30); /* Create a blank image */
 $bgc = imagecolorallocate($im, 255, 255, 255);
 $tc  = imagecolorallocate($im, 0, 0, 0);
 imagefilledrectangle($im, 0, 0, 150, 30, $bgc);
 /* Output an errmsg */
 imagestring($im, 1, 5, 5, "Error loading $imgname", $tc);
 }
 return $im;
}


to_pancake($_GET['file'],$_GET['outfile'],'haarcascade_frontalface_alt.xml');

$im = LoadJpeg($_GET['outfile']);
header('Content-Type: image/jpeg');
imagejpeg($im);
imagedestroy($im);

?>