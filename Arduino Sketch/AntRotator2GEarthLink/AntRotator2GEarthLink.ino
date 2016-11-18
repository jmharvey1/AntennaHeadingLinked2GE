#define FullCircleCnt 3780 //3780
String Dir = "";
bool AllOff = false;
bool Running = false;
float CurHeading = 0.0;
float HeadingInc = 0.0;
float RunOn = 2.0; // Number Degrees Antenna continues to rotate after power is cur
unsigned long Start; 
unsigned long period;
int cnt =0;
int cntU =0;
int cntD =0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int bearing = 0;

void setup(){
// Open serial connection.
Serial.begin(9600);
 // reserve 200 bytes for the inputString:
  inputString.reserve(200);
   pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  HeadingInc = 360.0/FullCircleCnt;
  Dir.reserve(5); // reserve 5 bytes for the Dir:
}
 
void loop(){
 if ( stringComplete){  
  //Serial.print(inputString);
  //Serial.println(": Hello world");
  bearing = (int)CurHeading;
  Serial.println(bearing);
  inputString = "";
  stringComplete = false;
  //bearing +=2;
  //if (bearing == 360) bearing = 0;
 }
 //delay(10); // ms
 int Pin14Val = digitalRead(14);
  int Pin15Val = digitalRead(15);
  if (Pin14Val & Pin15Val){
    if (!AllOff) Start = micros();
    AllOff = true;
    period = micros()-Start;
    if (period >16666 & Running){// been stopped for more than a cycle
      if(cntU > cntD) CurHeading +=  RunOn;
      if(cntU < cntD) CurHeading -=  RunOn;
      ChkLimits();
      //Serial.println(cnt);
      //Serial.print("Heading: ");
      //Serial.println(CurHeading);
      cnt =0;
      cntU =0;
      cntD =0;
      Running = false;
    }
  }
  if (AllOff & (!Pin15Val || !Pin14Val)){
    if (!Pin14Val & Pin15Val) Dir ="^";
    if (!Pin15Val & Pin14Val) Dir ="_";
    if (!Pin15Val & !Pin14Val){
      //Serial.print(Dir);
      AllOff = false;
      cnt ++;
      if(Dir == "^") cntU ++;
      if(Dir == "_") cntD ++; 
      if (cntU > cntD) CurHeading +=  HeadingInc;
      if (cntU < cntD) CurHeading -=  HeadingInc;
      ChkLimits();
      Dir ="+";
      Running = true;
    }
  }
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

void ChkLimits(){
      if(CurHeading <0.0) CurHeading=0.0;
      if(CurHeading >=360.0) CurHeading=359.9;
}      
