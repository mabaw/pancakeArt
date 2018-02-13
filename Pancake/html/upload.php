<?php
ini_set('display_errors', 1);
error_reporting(~0);
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


//echo "hello";

$target_dir = "";
$count_pancakes = ("counter.txt");
$last_count = file($count_pancakes);

//$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$target_file = $target_dir . basename("$last_count[0]".".jpg");
//echo "target_file = ".$target_file."<br>";
$uploadOk = 1;
$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
    if($check !== false) {
        //echo "File is an image - " . $check["mime"] . ".";
        $uploadOk = 1;
    } else {
        echo "File is not an image.";
        $uploadOk = 0;
    }
}
// Check if file already exists
if (file_exists($target_file)) {
    echo "Sorry, file already exists.";
    $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
    echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
    $uploadOk = 0;
}

//echo "aaaaa";
// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
     if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        //echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded to".$target_file;
        
        //echo " <br>$last_count[0]"."_out.jpg <br>";
        $out = "$last_count[0]"."_out.jpg";
        to_pancake($target_file,$out,'haarcascade_frontalface_alt.xml');
        $im = LoadJpeg($out);
        header('Content-Type: image/jpeg');
        imagejpeg($im);
        imagedestroy($im);

        $fp = fopen($count_pancakes  , "w");
        $last_count[0]++;
        fputs($fp , "$last_count[0]");
        fclose($fp);
    } else {

        echo "The file ". basename( $_FILES["fileToUpload"]["name"]);
        echo $target_file;
        echo "Sorry, there was an error uploading your file.";
    }
}
?>