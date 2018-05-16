/* This code is a heavily modified version of leonardojoy's code and works for a arduino leonardo or
pro micro which is based upon leonardo
and works with K-shootmania but also Sound voltex III pc port version.
Made By Abborren, 2017-11-28
https://github.com/Abborren/SDVX-Controller-Project
the input pins are the same ones that LeovxHQ uses.*/


#define FxL A1 // FX-L
#define FxR A4 // FX-R
//BT buttons
#define BtA A0 //BT-A
#define BtB A2 //BT-B
#define BtC A3 //BT-C
#define BtD A5 //BT-D
const byte StartBtn = 13; // StartButton
const byte StartBtnL =  6; // Start button light
const byte FxLL = 8; // FX-L light
const byte FxRL = 11; // FX-R light

const byte BtAL = 7; // BT-A light
const byte BtBL = 9; // BT-B light
const byte BtCL = 10; //BT-C light
const byte BtDL = 12; //BT-D light

const byte EncoderLL = 0;
const byte EncoderLR = 1;
const byte EncoderRL = 2;
const byte EncoderRR = 3;


#include <Mouse.h>
#include <Keyboard.h>
#define DELAY            5  // Delay per loop in ms
enum PinAssignments {
  encoderPinA = EncoderLL, // encoder left plus movement
  encoderPinB = EncoderLR, // encoder left minus movement
  encoderPinC = EncoderRL,
  encoderPinD = EncoderRR,

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
//
boolean On = false;
//for lightning animation
boolean offL = false;
boolean offR = false;
boolean allowAnimation = false;
  // this is where you change the different timer values for the blinking, left and right is the same. in ms.
int speedI[] = {122, 102, 82, 62 ,42 , 22};

int pinOut[] = {BtDL,BtCL, BtBL, A1}; // this is the lightning pins for BT-A to BT-D used in the animation.
int ledPinLeft = -1;
int ledPinRight = 5;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
//initial speed value, these values to the animation speed you want. in ms. if you disable the encoderSpeed() this will remove the animation.
 long interval1 = 75;
 long interval2 = 75;

void setup() {
    // StartBTN
  pinMode(StartBtn, INPUT_PULLUP); //Start BTN
    // FX buttons
  pinMode(FxR, INPUT_PULLUP); // FX-R
  pinMode(FxL, INPUT_PULLUP); // FX-L
    //BT buttons
  pinMode(BtC, INPUT_PULLUP); //BT-C
  pinMode(BtD, INPUT_PULLUP); // BT-D
  pinMode(BtB, INPUT_PULLUP); // BT-B
  pinMode(BtA, INPUT_PULLUP); // BT-A
    //Start BTN Light
  pinMode(StartBtnL, OUTPUT); //START BTN Light
    //FX LIGHTS
  pinMode(FxLL, OUTPUT); // FX-L LIGHT
  pinMode(FxRL, OUTPUT); // FX-R RIGHT
    //BT Lights
  pinMode(BtAL, OUTPUT); // BT-A LIGHT
  pinMode(BtBL, OUTPUT); // BT-B LIGHT
  pinMode(BtCL, OUTPUT); //BT-C Light
  pinMode(BtDL, OUTPUT); //BT-D Light



  Keyboard.begin();
    // VOL-L
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

    //----------------------------------

    //VOL-R
  pinMode(encoderPinC, INPUT_PULLUP);
  pinMode(encoderPinD, INPUT_PULLUP);
  //Not the same pin as encoderPinA etc!
  attachInterrupt(EncoderRL, doEncoderA, CHANGE);
  attachInterrupt(EncoderRR, doEncoderB, CHANGE);

  attachInterrupt(EncoderLL, doEncoderC, CHANGE);
  attachInterrupt(EncoderLR, doEncoderD, CHANGE);

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(BtA) == LOW) {
   Keyboard.press('w');
     if(LightsReverseOn == true) digitalWrite(BtAL, LOW);
     else digitalWrite(BtAL, HIGH);
  } else if (digitalRead(BtA) == HIGH) {
      Keyboard.release('w');
      if(LightsReverseOn == true) digitalWrite(BtAL, HIGH);
       else if(offL == false && offR == false) digitalWrite(BtAL, LOW);
  }
  if (digitalRead(BtB) == LOW) {
    Keyboard.press('e');
    if(LightsReverseOn == true) digitalWrite(BtBL, LOW);
      else digitalWrite(BtBL, HIGH);
  } else if (digitalRead(BtB) == HIGH) {
      Keyboard.release('e');
      if(LightsReverseOn == true) digitalWrite(BtBL, HIGH);
       else if(offL == false && offR == false) digitalWrite(BtBL, LOW);
  }
  if (digitalRead(BtC) == LOW) {
    Keyboard.press('i');
    if(LightsReverseOn == true) digitalWrite(BtCL, LOW);
      else digitalWrite(BtCL, HIGH);
  }else if (digitalRead(BtC) == HIGH) {
    Keyboard.release('i');
    if(LightsReverseOn == true) digitalWrite(BtCL, HIGH);
      else if(offL == false && offR == false) digitalWrite(BtCL, LOW);
  }
  if (digitalRead(BtD) == LOW) {
    Keyboard.press('o');
    if(LightsReverseOn == true) digitalWrite(BtDL, LOW);
      else digitalWrite(BtDL, HIGH);
  } else if (digitalRead(BtD) == HIGH) {
    Keyboard.release('o');
    if(LightsReverseOn == true) digitalWrite(BtDL, HIGH);
      else if(offL == false && offR == false) digitalWrite(BtDL, LOW);
  }

  if (digitalRead(FxR) == LOW) {
      Keyboard.press('m');
      if(LightsReverseOn == true) digitalWrite(FxRL, LOW);
       else digitalWrite(FxRL, HIGH);
  } else if (digitalRead(FxR) == HIGH) {
      Keyboard.release('m');
      if(LightsReverseOn == true) digitalWrite(FxRL, HIGH);
        else digitalWrite(FxRL, LOW);
  }
  if (digitalRead(FxL) == LOW) {
    Keyboard.press('c');
    if(LightsReverseOn == true) digitalWrite(FxLL, LOW);
      else digitalWrite(FxLL, HIGH);
  } else if (digitalRead(FxL) == HIGH) {
      Keyboard.release('c');
      if(LightsReverseOn == true) digitalWrite(FxLL, HIGH);
        else digitalWrite(FxLL, LOW);
  }
  //presses ENTER button
  if (digitalRead(StartBtn) == LOW) {
    Keyboard.press(0xB0);
    if(SdvxComp == true || LightsReverseOn == true) digitalWrite(StartBtnL, LOW);
      else  digitalWrite(StartBtnL, HIGH);
  } else if (digitalRead(StartBtn) == HIGH) {
      Keyboard.release(0xB0);
    if(SdvxComp == true || LightsReverseOn == true) digitalWrite(StartBtnL, HIGH);
      else   digitalWrite(StartBtnL, LOW);
  }
    //press both FX-L and FX-R and Start BTn to press ESC
  if(digitalRead(StartBtn) == LOW && digitalRead(BtC) == LOW ||digitalRead(StartBtn) == LOW && digitalRead(BtD) == LOW ||digitalRead(StartBtn) == LOW && digitalRead(BtB) == LOW ||digitalRead(StartBtn) == LOW && digitalRead(BtA) == LOW) {
    Keyboard.press(0xB1);
  } else if(digitalRead(StartBtn) == HIGH && digitalRead(BtC) == HIGH || digitalRead(StartBtn) == HIGH && digitalRead(BtD) == HIGH || digitalRead(StartBtn) == HIGH && digitalRead(BtB) == HIGH || digitalRead(StartBtn) == HIGH && digitalRead(BtA) == HIGH) {
      Keyboard.release(0xB1);
    }
    //Press FX-L and FX-R, BT-D and START BTN to toggle Sdvx compability
  if(digitalRead(StartBtn) == LOW && digitalRead(BtC) == LOW && digitalRead(FxR) == LOW && digitalRead(FxL) == LOW && On == false){
    SdvxComp = !SdvxComp;
    On = true;
  } else if(digitalRead(StartBtn) == HIGH && digitalRead(BtC) == HIGH && digitalRead(FxR) == HIGH && digitalRead(FxL) == HIGH && On == true) On = false;



   //Press FX-L and FX-R, BT-B And Start BTN to toggle light Reverse
  if(digitalRead(StartBtn) == LOW && digitalRead(BtB) == LOW && digitalRead(FxR) == LOW && digitalRead(FxL) == LOW && On == false) {
     LightsReverseOn = !LightsReverseOn;
     On = true;
  } else if(digitalRead(StartBtn) == HIGH && digitalRead(BtB) == HIGH && digitalRead(FxR) == HIGH && digitalRead(FxL) == HIGH && On == true) On = false;

//Press FX-L and FX-R, BT-A And Start BTN to toggle light animation of the encodres
 if(digitalRead(StartBtn) == LOW && digitalRead(BtA) == LOW && digitalRead(FxR) == LOW && digitalRead(FxL) == LOW) {

     allowAnimation =  !allowAnimation;
  }

   //Encoder Positioning
  for (int i = 0; i <= 1; i++) {
    rotating[i] = true;
    if (encoderPos[i] != 0) {
      if (allowAnimation == true) {
        //for ligtning animation
        if(encoderPos[1] > 5 && offL == false || encoderPos[0] < -5 && offL == false && LightsReverseOn == false){
         encoderSpeed();
         offL = true;
        }
        if(encoderPos[1] < -2 && offR == false || encoderPos[0] > 5 && offR == false && LightsReverseOn == false){
          encoderSpeed();
          offR = true;
        }
      }
      //This is the SDVX Compability toggle for the encoders
         if(SdvxComp == true){
      //Rotation Left
        if(encoderPos[0] < 0){
          Keyboard.press('2');
          delay(2);
          encoderPos[i] = 0;
        }else if(encoderPos[0] > 0){
          Keyboard.press('3');
          delay(2);
          encoderPos[i] = 0;
        }
          if(encoderPos[0] == 0) {
            delay(15);
            Keyboard.release('2');
            Keyboard.release('3');
        }
         //Rotation Right
        if(encoderPos[1] < 0){
          Keyboard.press('9');
          delay(2);
          encoderPos[i] = 0;
           //Rotation Left
        }else if(encoderPos[1] > 0){
          Keyboard.press('8');
          delay(2);
          encoderPos[i] = 0;
        }
          if(encoderPos[1] == 0) {
            delay(15);
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
     delay(FxL);
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
void encoderSpeed() {  // this is the Function that changes int speed of the ligthning animation
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
