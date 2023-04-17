<?php
// $Id: uc_nochex_apc.inc,v 1.3 07/03/2018 12:32:00 miveson Exp $
/*
 * @file
 * Process Nochex APC
 *
 * @param
 * None
 *
 */
function uc_nochex_apc() {
  
function http_post($server, $port, $url, $vars) { 
    // get urlencoded vesion of $vars array 
    $urlencoded = ""; 
    foreach ($vars as $Index => $Value) // loop round variables and encode them to be used in query
    $urlencoded .= urlencode($Index ) . "=" . urlencode($Value) . "&"; 
    $urlencoded = substr($urlencoded,0,-1);   // returns portion of string, everything but last character

    $headers1 = "POST $url HTTP/1.0\r\n"  // headers to be sent to the server
    . "Content-Type: application/x-www-form-urlencoded\r\n" 
	. "Host: secure.nochex.com\r\n"
    . "Content-Length: ". strlen($urlencoded) . "\r\n\r\n";  // length of the string
  
    $fp = fsockopen($server, $port, $errno, $errstr, 10);  // returns file pointer
    if (!$fp) return "ERROR: fsockopen failed.\r\nError no: $errno - $errstr";  // if cannot open socket then display error message

    fputs($fp, $headers1);  //writes to file pointer
    fputs($fp, $urlencoded);  
  
    $ret = ""; 
    while (!feof($fp)) $ret .= fgets($fp, 1024); // while it’s not the end of the file it will loop 
    fclose($fp);  // closes the connection
    return $ret; // array 
    } 

/* Do a post to the following url and receive a response.*/
$response = http_post("ssl://secure.nochex.com", 430, "/apc/apc.aspx", $_POST); 

/* Debug */
$debug = "IP -> " . $_SERVER['REMOTE_ADDR'] ."\r\n\r\nPOST DATA:\r\n"; 
foreach($_POST as $Index => $Value) 
$debug .= "$Index -> $Value\r\n"; 
$debug .= "\r\nRESPONSE:\r\n$response";

/* variables */
$amount = $_POST['amount'];
$transaction_id = $_POST['transaction_id'];
$comment = t('Nochex transaction ID: @transaction_id', array('@transaction_id' => $transaction_id));


// Checks the response to see if AUTHORISED is present if it isn’t a failure message is displayed
if ($response == "AUTHORISED") {  
    $msg = "APC was not AUTHORISED.\r\n\r\n$debug";  
		       
} else { 
    $msg = "APC was AUTHORISED.";      
    	 
}

// Adds the returned values and updates the payment details for a particular order.
	uc_payment_enter($_POST['order_id'], 'nochex', $amount, $_POST['custom'], NULL, $comment);
	
// Checks to see if the order / payment was a test, and if it is then update the order with a comment.	
if($_POST['status'] == "Test" || $_POST['status'] == "test"){
	
	uc_order_comment_save($_POST['order_id'], 0, t('This was a test transaction.'), 'admin');
	
}
/* Save order and update comments*/
	uc_order_comment_save($_POST['order_id'], 0, $msg, 'admin');
	uc_order_comment_save($_POST['order_id'], 0, t('@amount submitted through Nochex with transaction ID @transaction', array('@amount' => uc_currency_format($amount, FALSE), '@transaction' => $transaction_id)), 'order', 'payment_received', FALSE);

}


function uc_nochex_success($order_id=0, $test='true') {
  if (!($order = uc_order_load($order_id))) {
    drupal_goto('cart');
  }
  $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
  drupal_goto('cart/checkout/complete');
}


function uc_nochex_cancel() {
  unset($_SESSION['cart_order']);
  drupal_set_message(t('Your Nochex payment was cancelled. Please feel free to continue shopping or contact us for assistance.'));
  drupal_goto('cart');
}
