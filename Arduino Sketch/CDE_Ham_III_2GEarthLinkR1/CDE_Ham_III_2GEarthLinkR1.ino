/*  This Sketch was written to support interfacing with
 *  CDE HAM III Antenna rotator systems.
 *  As written it assumes that the control box has no modifications
 *  and is wired per the Owner's manual wiring diagram.
 *  And as such, expects terminal 1 on the control box to be Chasis ground,
 *  and +13VDC will be found between terminals 3 and 7. 
 *  And finally, when the Antenna is rotated to the zero degree heading, 
 *  there will be +13VDC measured from chasis ground to terminal 7.
 *  and then when the antenna is rotated to the 360 degree position, 
 *  the voltage between chasis ground (terminal 1) and terminal 7 will be 0VDC.
 *  It futher assumes that the voltage vs Antenna heading is linear.
 *  
 *  The 1K interface trim pot should be adjusted at the zero degree heading,
 *  and should be adjusted such that the red led on the micro just turns OFF
 */

String Dir = "";
float CurHeading = 0.0;
float HeadingInc = 0.0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int bearing = 0;
int MinSensVal = 38;  // set to value greater than zero if auto zero calibration is desired
int MaxSensVal = 1000;  // set to value greater than zero if auto zero calibration is desired
int LastSensVal = 0;
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
//int ledPin = 6;      // select the pin for the LED



void setup(){
  // Open serial connection.
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // declare the ledPin as an OUTPUT:
  //pinMode(ledPin, OUTPUT);
  HeadingInc = 360.0/(MaxSensVal - MinSensVal);
  Dir.reserve(5); // reserve 5 bytes for the Dir:
}
 
void loop(){
 if ( stringComplete){  
   //Serial.print(inputString);
   //Serial.println(": Hello world");
   bearing = (int)CurHeading;
   //Adjust for "north" being at mid scale
   if (bearing >= 180) bearing = 540 - bearing;
   else bearing = 180 - bearing;
   Serial.println(bearing);
   inputString = "";
   stringComplete = false;
  }
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  if (sensorValue > 1000)   RXLED1;      //Turn RXLED pin on //digitalWrite(ledPin, HIGH); // turn the ledPin on
  else  RXLED0;      //Turn RXLED pin off //digitalWrite(ledPin, LOW); // turn the ledPin OFF
  // now limit moment to moment change to a change of 10 counts
  if (sensorValue >LastSensVal+10) sensorValue = LastSensVal+10;
  if (sensorValue <LastSensVal-10) sensorValue = LastSensVal-10;
  LastSensVal = sensorValue; 
  //auto zero calibration
//  if (sensorValue < MinSensVal){
//    MinSensVal = sensorValue;
//    HeadingInc = 360.0/(MaxSensVal - MinSensVal);
//  }
  if (sensorValue > MaxSensVal & sensorValue<= 1000){
    MaxSensVal = sensorValue;
    HeadingInc = 360.0/(MaxSensVal - MinSensVal);
  }
  //treat this sample to be part of a running 10 sample average
  CurHeading = ((9*CurHeading)+(360.0 - HeadingInc*(sensorValue-MinSensVal)))/10 ;
  delay(50);
  MyserialEvent();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void MyserialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else inputString += inChar;  // add it to the inputString:
  }
}

      
