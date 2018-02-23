<?php
// $Id: uc_nochex_apc.inc,v 1.2 19/04/2013 12:32:00 miveson Exp $
/*
 * @file
 * Process Nochex APC callback
 *
 * @param
 * None
 *
 */
  function uc_nochex_callback() {
  
// Get the POST information from Nochex server
$postvars = http_build_query($_POST);

// Set parameters for the email
$to = $_POST['from_email'];

$url = "https://secure.nochex.com/callback/callback.aspx";
$ch = curl_init ();
curl_setopt ($ch, CURLOPT_URL, $url);
curl_setopt ($ch, CURLOPT_POST, true);
curl_setopt ($ch, CURLOPT_POSTFIELDS, $postvars);
curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, false);
curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);
$output = curl_exec ($ch);
curl_close ($ch);

// Put the variables in a printable format for the email
$debug = "IP -> " . $_SERVER['REMOTE_ADDR'] ."\r\n\r\nPOST DATA:\r\n"; 
foreach($_POST as $Index => $Value) 
$debug .= "$Index -> $Value\r\n"; 
$debug .= "\r\nRESPONSE:\r\n$output";

$secure = "1";
if($_POST["transaction_status"] == "100"){
$testStatus = "Test";
}else{
$testStatus = "Live";
}

	if($_POST["transaction_status"] == "100"){
	
	uc_order_comment_save($_POST['order_id'], 0, t('This was a test transaction.'), 'admin');
	
	}
	
//If statement
if (!strstr($output, "AUTHORISED")) {  // searches response to see if AUTHORISED is present if it isn’t a failure message is displayed
    $msg = "Callback was not AUTHORISED.\r\n\r\n$debug";  
	
	$amount = $_POST['amount'];
    $transaction_id = $_POST['transaction_id'];
	
	uc_cart_empty(uc_cart_get_id());
    uc_cart_complete_sale($_POST['order_id']);
     
	$comment = t('Nochex transaction ID: @transaction_id', array('@transaction_id' => $transaction_id));
	
	uc_payment_enter($_POST['order_id'], 'nochex', $amount, 1, NULL, $comment);
    uc_order_comment_save($_POST['order_id'], 0, t('Callback was not AUTHORISED.'), 'admin');
	
	uc_order_comment_save($_POST['order_id'], 0, t('@amount submitted through Nochex with transaction ID @transaction', array('@amount' => uc_currency_format($amount, FALSE), '@transaction' => $transaction_id)), 'order', 'payment_received', TRUE);
     
} 
else { 
    $msg = "Callback was AUTHORISED.\r\n\r\n$debug"; 
	
	$amount = $_POST['amount'];
    $transaction_id = $_POST['transaction_id'];
	
	uc_cart_empty(uc_cart_get_id());
    uc_cart_complete_sale($_POST['order_id']);
     
	$comment = t('Nochex transaction ID: @transaction_id', array('@transaction_id' => $transaction_id));
    	
	uc_payment_enter($_POST['order_id'], 'nochex', $amount, 1, NULL, $comment);
    uc_order_comment_save($_POST['order_id'], 0, t('Callback was AUTHORISED.'), 'admin');
		
	uc_order_comment_save($_POST['order_id'], 0, t('@amount submitted through Nochex with transaction ID @transaction', array('@amount' => uc_currency_format($amount, FALSE), '@transaction' => $transaction_id)), 'order', 'payment_received', TRUE);
      
}

//Email the response
mail( $to, 'Callback', $msg, "From:james.lugton@nochex.com");

}


function uc_nochex_callback_success($order_id=0, $test='true') {
  if (!($order = uc_order_load($order_id))) {
    drupal_goto('cart');
  }
  $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
  drupal_goto('cart/checkout/complete');
}


function uc_nochex_callback_cancel() {
  unset($_SESSION['cart_order']);

  drupal_set_message(t('Your Nochex payment was cancelled. Please feel free to continue shopping or contact us for assistance.'));

  drupal_goto('cart');
}
