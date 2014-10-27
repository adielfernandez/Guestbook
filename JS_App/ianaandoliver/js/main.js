

// when window loads call the setup method
$(window).on("load", setup);
$(window).on("resize", fixSizes);


// Spacebrew Object
var sb
	, app_name = "Wedding Guest "
	;

//connection checking
var lastCheckTime = 0;
var connected = false;
var sent = false; 

/**
* setup Function that connect to spacebrew and creates a listener for clicks of the submit button.
*/
function setup (){
	var random_id = "0000" + Math.floor(Math.random() * 10000);

	app_name = app_name + random_id.substring(random_id.length-4);

	// setup spacebrew
	sb = new Spacebrew.Client();  // create spacebrew client object

	sb.name(app_name);
	sb.description("Sends a message string to OF app"); // set the app description
	sb.server = "54.68.243.245";

    // create the spacebrew subscription channels
	sb.addPublish("userMessage", "string", "");	// create the publication feed
	sb.addSubscribe("connection", "boolean");		// create the subscription feed

	// configure the publication and subscription feeds
	sb.onBooleanMessage = onBooleanMessage;
	sb.onOpen = onOpen;

	// connect to spacbrew
	sb.connect();  

	// listen to button clicks
	$("#sendButton").on("mousedown", onMouseDown);


	//force sizes to be screen appropriate
	fixSizes();


}

var interval = setInterval(checkConnection, 100);

function checkConnection(){

	var d = new Date();

	//if its been too long since we've received a message
	//then we're disconnected, flip the boolean to false
	//otherwise we are connected and its true
	if(d.getTime() - lastCheckTime > 1000){
		connected = false;
	} else {
		connected = true;
	}


	if(sent === false){
		if(connected){
			$('#sendButton').show();
			$('#waitMessage').hide();

			// console.log("not connected - hide button");

		} else {
			$('#sendButton').hide();
			$('#waitMessage').show();
			// console.log("not connected - hide button");

		}
	}






}


function fixSizes() {

	console.log("Inner width: " + window.innerWidth);

	//Force resize on everything:
	var headerImage = document.getElementById('headerImg');
	headerImg.style.height = "auto";
	headerImg.style.width = window.innerWidth + "px";

	var bylineImage = document.getElementById('bylineImg');
	bylineImage.style.height = "auto";
	bylineImage.style.width = window.innerWidth + "px";

	var bikeImage = document.getElementById('bikeImg');
	bikeImage.style.height = "auto";
	bikeImage.style.width = window.innerWidth + "px";

	var fontScale = 0.05;

	//fix field titles
	$('.fieldTitle').css("font-size", window.innerWidth * fontScale + "px");
	
	//and input wrapper
	$('#inputs').css("width", window.innerWidth * 0.9 + "px");
	// $('#inputs').css("width", window.innerWidth + "px");
	// $('#inputs').css("width", window.innerWidth + "px");
	
	var $messageBox = $('#messageBox');
	$messageBox.css("font-size", window.innerWidth * fontScale + "px");
	$messageBox.css("height",(window.innerWidth * 0.15) + "px");

	var $fromBox = $('#fromBox');
	 $fromBox.css("font-size", window.innerWidth * fontScale + "px");
	$fromBox.css("height", window.innerWidth * (fontScale * 1.5) + "px");


	var $sendButton = $('#sendButton');
	$sendButton.css("height", (window.innerWidth * 0.13) + "px");
	$sendButton.css("width", (window.innerWidth * 0.32) + "px");
	$sendButton.css("font-size", (window.innerWidth * 0.07) + "px");

	var $waitMessage = $('#waitMessage');
	$waitMessage.css("height", (window.innerWidth * 0.15) + "px");
	$waitMessage.css("width", (window.innerWidth * 0.50) + "px");
	$waitMessage.css("font-size", (window.innerWidth * 0.05) + "px");

}


/**
 * Function that is called when Spacebrew connection is established
 */
function onOpen() {
	var message = "Connected to <strong>" + sb.name() + "</strong>. ";
	if (sb.name() === app_name) {
		message += ""
	}
	$("#name").html( message );
	console.log(message);
}

/**
* onMouseDown Function that is called when the submit button is pressed. It reads the
*     text in the input box, and then sends it to spacebrew. It accepts a mouse event
*     object, though we don't use it in this example.
*/
function onMouseDown (evt){
	var userMessage = $("#messageBox").val();
	var userName = $("#fromBox").val();

	var outString = userMessage + "***" + userName;
	// console.log(outString);   

	if (userMessage !== "" && userName !== "") {               // if input box is not blank
		console.log("Sending message " + outString); 
		sb.send("userMessage", "string", outString);   // send string to spacebrew
		$("#messageBox").val("");                   
		$("#fromBox").val("");                   
		$("#status").text(outString); // display the sent message in the browser    

		//then change the input button to say thank you
		$('#sendButton').text("Thank you!");
		$('#sendButton').css("width", (window.innerWidth * 0.38) + "px");
		$('#sendButton').css("background-color", "#69ad4e");
		$('#message').hide(500);
		$('#from').hide(500);

		sent = true;
	}
}


/**
 * onStringMessage Function that is called whenever new spacebrew string messages are received.
 *          It accepts two parameters:
 * @param  {String} name    Holds name of the subscription feed channel
 * @param  {String} value 	Holds value received from the subscription feed
 */


function onBooleanMessage( name, value ){
	var d = new Date();

	//update the time that we got the last message from the OF app
	lastCheckTime = d.getTime();






}








