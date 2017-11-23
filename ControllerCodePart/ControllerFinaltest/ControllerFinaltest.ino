/* This code is a heavily modified version of leonardojoy's code and works for a arduino leonardo or  
pro micro which is based upon leonardo
and works with K-shootmania but also Sound voltex III pc port version.*/


#include <Mouse.h>
#include <Keyboard.h>
#define DELAY            5  // Delay per loop in ms
enum PinAssignments {
  encoderPinA = 0,
  encoderPinB = 1,
  encoderPinC = 2,
  encoderPinD = 3,

};
//This is up to your pin wiring

int encoderPos[] = {0, 0};
static boolean rotating[] = {false, false};

boolean A_set = false;
boolean B_set = false;
boolean C_set = false;
boolean D_set = false;
boolean SdvxComp = false;
boolean LightsReverseOn = false;
boolean On = false;
//for lightning animation
boolean offL = false;
boolean offR = false;
  // this is where you change the different timer values for the blinking, left and right is the same. in ms.
int speedI[] = {122, 102, 82, 62,42, 22};

int pinOut[] = {4,5, A0,A1};
int ledPinLeft = -1;
int ledPinRight = 5;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;  
//initial speed value, if you disable the encoderSpeed() funtion chagne these values to the animation speed you want. in ms.
 long interval1 = 75;   
 long interval2 = 75;

void setup() {
    // StartBTN
  pinMode(A2, INPUT_PULLUP); //Start BTN
    // FX buttons
  pinMode(9, INPUT_PULLUP); // FX-R
  pinMode(10, INPUT_PULLUP); // FX-L
    //BT buttons
  pinMode(7, INPUT_PULLUP); //BT-C
  pinMode(6, INPUT_PULLUP); // BT-D
  pinMode(14, INPUT_PULLUP); // BT-B
  pinMode(15, INPUT_PULLUP); // BT-A
    //Start BTN Light
  pinMode(A3, OUTPUT); //START BTN Light
    //FX LIGHTS
  pinMode(8, OUTPUT); // FX-L LIGHT
  pinMode(16, OUTPUT); // FX-R LIGHT
    //BT Lights
  pinMode(4, OUTPUT); //BT-D Light
  pinMode(5, OUTPUT); //BT-C Light
  pinMode(A0, OUTPUT); // BT-B LIGHT
  pinMode(A1, OUTPUT); // BT-A LIGHT

  Keyboard.begin();
    // VOL-L
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

    //----------------------------------

    //VOL-R
  pinMode(encoderPinC, INPUT_PULLUP);
  pinMode(encoderPinD, INPUT_PULLUP);
  //Not the same pin as encoderPinA etc!
  attachInterrupt(2, doEncoderA, CHANGE);
  attachInterrupt(3, doEncoderB, CHANGE);

  attachInterrupt(0, doEncoderC, CHANGE);
  attachInterrupt(1, doEncoderD, CHANGE);

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(7) == LOW) {
    Keyboard.press('i');
    if(LightsReverseOn == true) digitalWrite(5, LOW);
      else digitalWrite(5, HIGH);
  }else if (digitalRead(7) == HIGH) {
    Keyboard.release('i');
    if(LightsReverseOn == true) digitalWrite(5, HIGH);
      else if(offL == false && offR == false) digitalWrite(5, LOW);
  }
  if (digitalRead(6) == LOW) {
    Keyboard.press('o');
    if(LightsReverseOn == true) digitalWrite(4, LOW);
      else digitalWrite(4, HIGH);
  } else if (digitalRead(6) == HIGH) {
    Keyboard.release('o');
    if(LightsReverseOn == true) digitalWrite(4, HIGH);
      else if(offL == false && offR == false) digitalWrite(4, LOW);
  }
  if (digitalRead(14) == LOW) {
    Keyboard.press('e');
    if(LightsReverseOn == true) digitalWrite(A0, LOW);
      else digitalWrite(A0, HIGH);
  } else if (digitalRead(14) == HIGH) {
      Keyboard.release('e');
      if(LightsReverseOn == true) digitalWrite(A0, HIGH);
       else if(offL == false && offR == false) digitalWrite(A0, LOW);
  }
  if (digitalRead(15) == LOW) {
    Keyboard.press('w');
     if(LightsReverseOn == true) digitalWrite(A1, LOW);
     else digitalWrite(A1, HIGH);
  } else if (digitalRead(15) == HIGH) {
      Keyboard.release('w');
      if(LightsReverseOn == true) digitalWrite(A1, HIGH);
       else if(offL == false && offR == false) digitalWrite(A1, LOW);
  }
  if (digitalRead(9) == LOW) {
      Keyboard.press('m');
      if(LightsReverseOn == true) digitalWrite(8, LOW);
       else digitalWrite(8, HIGH);
  } else if (digitalRead(9) == HIGH) {
      Keyboard.release('m');
      if(LightsReverseOn == true) digitalWrite(8, HIGH);
        else digitalWrite(8, LOW);
  }
  if (digitalRead(10) == LOW) {
    Keyboard.press('c');
    if(LightsReverseOn == true) digitalWrite(16, LOW);
      else digitalWrite(16, HIGH);
  } else if (digitalRead(10) == HIGH) {
      Keyboard.release('c');
      if(LightsReverseOn == true) digitalWrite(16, HIGH);
        else digitalWrite(16, LOW);
  }
  //presses ENTER button
  if (digitalRead(A2) == LOW) {
    Keyboard.press(0xB0);
    if(SdvxComp == true || LightsReverseOn == true) digitalWrite(A3, LOW);
      else  digitalWrite(A3, HIGH);
  } else if (digitalRead(A2) == HIGH) {
      Keyboard.release(0xB0);
    if(SdvxComp == true || LightsReverseOn == true) digitalWrite(A3, HIGH);
      else   digitalWrite(A3, LOW);
  }
    //press both FX-L and FX-R and Start BTn to press ESC
  if(digitalRead(A2) == LOW && digitalRead(7) == LOW ||digitalRead(A2) == LOW && digitalRead(6) == LOW ||digitalRead(A2) == LOW && digitalRead(14) == LOW ||digitalRead(A2) == LOW && digitalRead(15) == LOW) {
    Keyboard.press(0xB1);
  } else if(digitalRead(A2) == HIGH && digitalRead(7) == HIGH || digitalRead(A2) == HIGH && digitalRead(6) == HIGH || digitalRead(A2) == HIGH && digitalRead(14) == HIGH || digitalRead(A2) == HIGH && digitalRead(15) == HIGH) {
      Keyboard.release(0xB1);
    }
    //Press FX-L and FX-R, BT-D and START BTN to toggle Sdvx compability
  if(digitalRead(A2) == LOW && digitalRead(7) == LOW && digitalRead(9) == LOW && digitalRead(10) == LOW && On == false){
    SdvxComp = !SdvxComp;
    On = true;
  }else if(digitalRead(A2) == HIGH && digitalRead(7) == HIGH && digitalRead(9) == HIGH && digitalRead(10) == HIGH && On == true) On = false;


  
   //Press FX-L and FX-R, BT-C And Start BTN to toggle light Reverse
  if(digitalRead(A2) == LOW && digitalRead(14) == LOW && digitalRead(9) == LOW && digitalRead(10) == LOW && On == false) {
     LightsReverseOn = !LightsReverseOn;
     On = true;
  }else if(digitalRead(A2) == HIGH && digitalRead(14) == HIGH && digitalRead(9) == HIGH && digitalRead(10) == HIGH && On == true) On = false;
   
   
   //Encoder Positioning
  for (int i = 0; i <= 1; i++) {
    rotating[i] = true;
    if (encoderPos[i] != 0) {
      //for ligtning animation
      if(encoderPos[1] > 5 && offL == false || encoderPos[0] < -5 && offL == false && LightsReverseOn == false){
       encoderSpeed();
       offL = true; 
      }
      if(encoderPos[1] < -2 && offR == false || encoderPos[0] > 5 && offR == false && LightsReverseOn == false){
        encoderSpeed(); 
        offR = true;
      }            
      //This is the SDVX Compability toggle for the encoders
      if(SdvxComp == true){
      //Rotation Left
        if(encoderPos[0] < 0){
          Keyboard.press('2');
          delay(10);
          encoderPos[i] = 0;
        }else if(encoderPos[0] > 0){
          Keyboard.press('3');
          delay(10);
          encoderPos[i] = 0;
        } 
          if(encoderPos[1] == 0) {
            Keyboard.release('2');
            Keyboard.release('3');
        }
         //Rotation Right
        if(encoderPos[1] < 0){
          Keyboard.press('9');
          delay(DELAY);
          encoderPos[i] = 0;
           //Rotation Left
        }else if(encoderPos[1] > 0){
          Keyboard.press('8');
          delay(DELAY);
          encoderPos[i] = 0;
        }
          if(encoderPos[1] == 0) {
            Keyboard.release('8');
            Keyboard.release('9');
        }
          //Normal Mouse movement mode
        }else{
          if(i == 0)  Mouse.move(encoderPos[i], 0, 0);
          if(i == 1)  Mouse.move(0, encoderPos[i], 0);

          encoderPos[i] = 0;
        }
    }
  }
    
    // this is the function that makes the lightning animation happen.
     unsigned long currentMillis1 = millis();
     unsigned long currentMillis2 = millis();
     delay(10);
  if (currentMillis1 - previousMillis1 >= interval1 && offL == true) {
    previousMillis1 = currentMillis1;
        ledPinLeft++;
        if(ledPinLeft <4 ) {
          
          digitalWrite(pinOut[ledPinLeft], HIGH);
         
        }
        
        if(ledPinLeft >= 2 ){
          digitalWrite(pinOut[ledPinLeft-2], LOW);
        }
          if(ledPinLeft >4){ 
           
            offL = false;
            ledPinLeft = -1;
          
    }
  }
  
    if (currentMillis2 - previousMillis2 >= interval2 && offR == true) {
      previousMillis2 = currentMillis2;
        ledPinRight--;
       if(ledPinRight > -1 ){
          digitalWrite(pinOut[ledPinRight], HIGH);
        }
       
        if(ledPinRight < 2 ){
          digitalWrite(pinOut[ledPinRight+2], LOW);
        }
          if(ledPinRight < -1){
            offR = false;
            ledPinRight = 5;
      }   
    }
  }
  

void doEncoderA() {
  if ( digitalRead(encoderPinA) != A_set ) {
    A_set = !A_set;

    if ( A_set && !B_set )
      encoderPos[0] += 1;

    rotating[0] = false;
  }
}
void doEncoderB() {
   if ( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;

    if ( B_set && !A_set )
      encoderPos[0] -= 1;

    rotating[0] = false;
  }
}
void doEncoderC() {
  if ( digitalRead(encoderPinC) != C_set ) {
    C_set = !C_set;

    if ( C_set && !D_set )
      encoderPos[1] += 1;

    rotating[1] = false;
  }
}
void doEncoderD() {
  if ( digitalRead(encoderPinD) != D_set ) {
    D_set = !D_set;

    if ( D_set && !C_set )
      encoderPos[1] -= 1;

    rotating[1] = false;
  }
}
// this is the Function that changes int speed of the ligthning animation
void encoderSpeed() { 
  if(encoderPos[0] > -3  && encoderPos[0] < 0 || encoderPos[1] > 0 && encoderPos[1] < 3 ) {
    interval2 = speedI[0];
    
  }else if(encoderPos[0] > -5 && encoderPos[0] < -3 || encoderPos[1] > 3 && encoderPos[1] < 5 ) {
    interval2 = speedI[1];
    
  }else if(encoderPos[0] > -7 && encoderPos[0] < -5 || encoderPos[1] > 5 && encoderPos[1] < 7) {
    interval2 = speedI[2];
    
  }else if(encoderPos[0] > -10 && encoderPos[0] < -7 || encoderPos[1] > 7 && encoderPos[1] < 10) {
    interval2 = speedI[3];
    
  }else if(encoderPos[0] > -15 && encoderPos[0] < -10 || encoderPos[1] > 10 && encoderPos[1] < 15) {
    interval2 = speedI[4];
    
  }else if(encoderPos[0] < -15 ||  15 <encoderPos[1]) {
    interval2 = speedI[5];
  
  }
  if(encoderPos[1] > -3  && encoderPos[1] < 0 || encoderPos[0] > 0 && encoderPos[0] < 3 ) {
    interval1 = speedI[0];
    
  }else if(encoderPos[1] > -5 && encoderPos[1] < -3 || encoderPos[0] > 3 && encoderPos[0] < 5 ) {
    interval1 = speedI[1];
    
  }else if(encoderPos[1] > -7 && encoderPos[1] < -5 || encoderPos[0] > 5 && encoderPos[0] < 7) {
    interval1 = speedI[2];
    
  }else if(encoderPos[1] > -10 && encoderPos[1] < -7 || encoderPos[0] > 7 && encoderPos[0] < 10) {
    interval1 = speedI[3];
    
  }else if(encoderPos[1] > -15 && encoderPos[1] < -10 || encoderPos[0] > 10 && encoderPos[0] < 15) {
    interval1 = speedI[4];
    
  }else if(encoderPos[1] < -15 ||  15 <encoderPos[0]) {
    interval1 = speedI[5];
  
  }
}
