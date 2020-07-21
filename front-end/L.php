<?php

/* 
Daniel Gonzalez
CPE 190 Project
 */
$onoroff = $_GET["state"]; // Declares the request from index.html as a variable

$textfile = "L.txt"; // Declares the name and location of the .txt file
 
$fileLocation = "$textfile";

$fh = fopen($fileLocation, 'w   ') or die("Error"); // Opens up the .txt file for writing and replaces any previous content

$stringToWrite = "$onoroff"; // Write either 1 or 0 depending on request from html

fwrite($fh, $stringToWrite); // Writes it to the .txt file

fclose($fh); 
 
header("Location: website.html"); // Return to frontend (index.html)

?>
