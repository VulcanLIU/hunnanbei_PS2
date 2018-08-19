/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


#include "PS2X_lib.h" //for v1.6

/******************************************************************
* set pins connected to PS2 controller:
*   - 1e column: original
*   - 2e colmun: Stef?
* replace pin numbers by the ones you use
******************************************************************/
#define PS2_DAT        13  //14
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

/******************************************************************
* select modes of PS2 controller:
*   - pressures = analog reading of push-butttons
*   - rumble    = motor rumbling
* uncomment 1 of the lines for each mode selection
******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){
	
	Serial.begin(57600);
	
	delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
	
	//CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
	
	//setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
	error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
	
	if(error == 0){
		Serial.print("Found Controller, configured successful ");
		Serial.print("pressures = ");
		if (pressures)
		Serial.println("true ");
		else
		Serial.println("false");
		Serial.print("rumble = ");
		if (rumble)
		Serial.println("true)");
		else
		Serial.println("false");
		Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
		Serial.println("holding L1 or R1 will print out the analog stick values.");
		Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
	}
	else if(error == 1)
	Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
	
	else if(error == 2)
	Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

	else if(error == 3)
	Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
	
	//  Serial.print(ps2x.Analog(1), HEX);
	
	type = ps2x.readType();
	switch(type) {
		case 0:
		Serial.print("Unknown Controller type found ");
		break;
		case 1:
		Serial.print("DualShock Controller found ");
		break;
		case 2:
		Serial.print("GuitarHero Controller found ");
		break;
		case 3:
		Serial.print("Wireless Sony DualShock Controller found ");
		break;
	}
}

void loop() {
	/* You must Read Gamepad to get new values and set vibration values
	ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
	if you don't enable the rumble, use ps2x.read_gamepad(); with no values
	You should call this at least once a second
	*/
	ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

	vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
	if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
		if(ps2x.Button(PSB_L3))
		Serial.println("L3");
		if(ps2x.Button(PSB_R3))
		Serial.println("R3");
		if(ps2x.Button(PSB_L2))
		Serial.println("L2");
		if(ps2x.Button(PSB_R2))
		Serial.println("R2");
		if(ps2x.Button(PSB_TRIANGLE))
		Serial.println("T");
	}

	if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
	Serial.println("C");
	if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
	Serial.println("X");
	if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
	Serial.println("S");
	
	Serial.print("Stick Values:");
	Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
	Serial.print(",");
	Serial.print(ps2x.Analog(PSS_LX), DEC);
	Serial.print(",");
	Serial.print(ps2x.Analog(PSS_RY), DEC);
	Serial.print(",");
	Serial.println(ps2x.Analog(PSS_RX), DEC);
	delay(50);
}