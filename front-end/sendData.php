<?php
    //Daniel Gonzalez
    //CPE190
    $input = $_GET["state"];

    $writer = new XMLWriter();
    $writer->openURI('data.xml');
    $writer->startDocument('1.0','utf-8');   
    $writer->setIndent(4);
    $writer->startElement('controlState');
    $writer->writeElement('nextState', $input);
    $writer->writeElement('OStime', time());
    $writer->endElement();
    $writer->endElement();
    $writer->endDocument();
    $writer->flush();
 
    header('Location: website.html');
?>

