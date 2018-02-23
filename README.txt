
-- SUMMARY --

The Nochex payment module provides Ubercart with a Nochex payment facility with
Automatic Payment Confirmation.

Nochex is a UK based online payment company, specialising in providing smaller 
online businesses with simple, accessible, easy to use, online payment 
services.

For further information on Nochex visit:
  http://www.nochex.com

For a full description of the module, visit the project page:
  http://drupal.org/project/xxxxxxx

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/xxxxxxxx


-- REQUIREMENTS --

Ubercart module
Ubercart payment module
PHP Curl extension library
A Nochex account


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure settings in 
Home › Administer › Store administration › Configuration › Payment settings


  Nochex settings:

  - Nochex email address:
  
    The Nochex merchant id required to identify your account.  
    
    The merchant id is the email address you use with your Nochex 
    account. 
    
    If you do not already have an account, go online to www.nochex.com and
    apply for the account that best suits your requirements.

  - Payment method title:

    The text that will appear on the Upcart checkout screen against the Nochex
    payment method option.
	
  - Order review submit button text:
  
    Provide Nochex specific text for the submit button on the Ubercart order
    review page.
    
  - Nochex URL:
  
    The URL used for posting information to Nochex. 
    
    Currently https://secure.nochex.com/.
    
  - APC authentication URL:
  
    The URL to confirm receipt of information from Nochex.
    
    Currently https://www.nochex.com/nochex.dll/apc/apc.
    
    The Responder URL. Nochex will post a confirmation to the Responder URL you
    specified which includes the information about the transaction as well as 
    the security key.
    
  - Debug APC callback:
  
    When checked, the Nochex APC data will be written to the log. 
    
  - Enable test mode:
  
    Checking this allows you to process payments through Nochex without any
    money being taken. Make sure you only use this in a test environment
    otherwise you won't get paid.
	
 - Hide Billing Address:
  
   Hide billing address on your Nochex payment page, so customers can't edit it. 
   If this value is set, the billing address passed to Nochex will be set to the delivery address.
    
  - Show Postage:
  
    Show postage separately from the total amount that is shown on the Nochex payment page.
    
 - Detailed Product Information:
  
    Display your product details in a structured format on your Nochex payment page.
    
  - Site base URL for test callback: 
  
    This will normally be the URL of your site root. If however your site is
    running under localhost you will need to enter an externally accessable URL
    to your site root here.
    
-- TESTING APC

  - There is a page used for testing the APC on the Nochex site. 
    It will allow you to see if your server is sending and receiving
    the correct information : https://www.nochex.com/nochex.dll/apc/testapc


-- CONTACT --

Current maintainers:
* Bob Scott - http://drupal.org/user/551442

This project was originally based on code from this thead in the Ubercart 
forum: http://www.ubercart.org/forum/support/3973/nochex_integration)