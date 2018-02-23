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
  function uc_nochex_apc() {
  
// Get the POST information from Nochex server
$postvars = http_build_query($_POST);

// Set parameters for the email
$to = $_POST['from_email'];

$url = "https://www.nochex.com/apcnet/apc.aspx";

$ch = curl_init ();
curl_setopt ($ch, CURLOPT_URL, $url);
curl_setopt ($ch, CURLOPT_POST, true); 
curl_setopt ($ch, CURLOPT_POSTFIELDS, $postvars);
curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt ($ch, CURLOPT_SSLVERSION, 6);
curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, false);
curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);
$output = curl_exec ($ch);
curl_close ($ch);

// Put the variables in a printable format for the email
$debug = "IP -> " . $_SERVER['REMOTE_ADDR'] ."\r\n\r\nPOST DATA:\r\n"; 
foreach($_POST as $Index => $Value) 
$debug .= "$Index -> $Value\r\n"; 
$debug .= "\r\nRESPONSE:\r\n$output";


//If statement
if (!strstr($output, "AUTHORISED")) {  // searches response to see if AUTHORISED is present if it isn’t a failure message is displayed
    $msg = "APC was not AUTHORISED.\r\n\r\n$debug";  
	
	$amount = $_POST['amount'];
    $transaction_id = $_POST['transaction_id'];
	
	uc_cart_empty(uc_cart_get_id());
    uc_cart_complete_sale($_POST['order_id']);
     
	$comment = t('Nochex transaction ID: @transaction_id', array('@transaction_id' => $transaction_id));
	
	if($_POST['status'] == "Test" || $_POST['status'] == "test"){
	
	uc_order_comment_save($_POST['order_id'], 0, t('This was a test transaction.'), 'admin');
	
	}
	uc_payment_enter($_POST['order_id'], 'nochex', $amount, 1, NULL, $comment);
    uc_order_comment_save($_POST['order_id'], 0, t('APC was not AUTHORISED.'), 'admin');
	
	uc_order_comment_save($_POST['order_id'], 0, t('@amount submitted through Nochex with transaction ID @transaction', array('@amount' => uc_currency_format($amount, FALSE), '@transaction' => $transaction_id)), 'order', 'payment_received', TRUE);
     
} 
else { 
    $msg = "APC was AUTHORISED.\r\n\r\n$debug"; 
	
	$amount = $_POST['amount'];
    $transaction_id = $_POST['transaction_id'];
	
	uc_cart_empty(uc_cart_get_id());
    uc_cart_complete_sale($_POST['order_id']);
     
	$comment = t('Nochex transaction ID: @transaction_id', array('@transaction_id' => $transaction_id));
    	
	
	if($_POST['status'] == "Test" || $_POST['status'] == "test"){
	
	uc_order_comment_save($_POST['order_id'], 0, t('This was a test transaction.'), 'admin');
	
	}
	
	uc_payment_enter($_POST['order_id'], 'nochex', $amount, 1, NULL, $comment);
    uc_order_comment_save($_POST['order_id'], 0, t('APC was AUTHORISED.'), 'admin');
		
	uc_order_comment_save($_POST['order_id'], 0, t('@amount submitted through Nochex with transaction ID @transaction', array('@amount' => uc_currency_format($amount, FALSE), '@transaction' => $transaction_id)), 'order', 'payment_received', TRUE);
      
}

/*   $apcInsert = db_insert('uc_nochex_apc')->fields(array(
        'amount' => $_POST['amount'],
        'apc_response' => $msg,
        'apc_status' => 'payment_received',
        'from_email' => $_POST['from_email'],
        'order_id' => $_POST['order_id'],
        'to_email' => $_POST['to_email'],
        'transaction_date' =>  $_POST['transaction_date'],
        'transaction_id' =>  $_POST['transaction_id'],))->execute();

$msg .= $apcInsert;*/

//Email the response
mail( $to, 'APC', $msg);

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
