<?php
    //Daniel Gonzalez
    //CPE190
    //$input = $_GET["state"];
    $usr = $_POST["uname"];
    $pass = $_POST["psw"]; 
    if($usr == "cpe190" && $pass == "abc123") {
        header('Location:front-end/website.html');
    }else{
   	header('Location:index.html');
    } 
//header('Location:front-end/website.html');
?>

