<?php

/* 
Daniel Gonzalez
CPE 190 Project
 */

echo "<h2>Test</h2>";


$input = $_GET["state"]; // Declares the request from index.html as a variable

$textfile = "input.txt"; // Declares the name and location of the .txt file
 
$fileLocation = "$textfile";

$fh = fopen($fileLocation, 'w   ') or die("Error"); // Opens up the .txt file for writing and replaces any previous content

$stringToWrite = "$input"; // Write either 1 or 0 depending on request from html

fwrite($fh, $stringToWrite); // Writes it to the .txt file

fclose($fh); 
 
header("Location: website.html"); // Return to frontend (index.html)

?>
