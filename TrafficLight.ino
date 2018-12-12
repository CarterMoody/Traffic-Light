#include <CPE123_Fall16.h>

// Define your pins

const int ledPin1 = 6;
const int ledPin2 = 7;
const int ledPin3 = 8;
const int ledPin4 = 9;
const int ledPin5 = 10;
const int ledPin6 = 11;
const int buttonPin1 = 2;
const int buttonPin2 = 3;

// Create your hardware

Led GreenPoly(ledPin1);
Led YellowPoly(ledPin2);
Led RedPoly(ledPin3);
Led GreenSmaller(ledPin4);
Led YellowSmaller(ledPin5);
Led RedSmaller(ledPin6);
Button CarPoly(buttonPin1);
Button CarSmaller(buttonPin2);

// This creates a software version of the LED on pin 13

void setup() {
  // put your setup code here, to run once:
  // Set up serial monitor and print out program info
  Serial.begin(9600);
  setupMessage(__FILE__);
  printLibVersion();
}

void loop() {
  // put your main code here, to run repeatedly:

   StateMachinePolyDrive();

}

void StateMachinePolyDrive()
{
  enum {PolyGreen, PolyYellow, PolyRed, SM2Question};
  static int state=PolyGreen;
  static MSTimer Thirty(3000);
  static MSTimer Ten;
  static MSTimer Five;

  switch(state)
  {
    case PolyGreen:
    GreenPoly.on();
    YellowPoly.off();
    RedPoly.off();
    GreenSmaller.off();
    YellowSmaller.off();
    RedSmaller.on();
    Ten.set(3000);
      if (CarSmaller.isPushed()==true && Thirty.done()==true)
      {
        Serial.println("Car at Smaller and Thirty Seconds Elapsed. Going to Yellow for Poly");
        Ten.set(10000); 
        GreenPoly.off();
        YellowPoly.on();
        RedPoly.off();
        GreenSmaller.off();
        YellowSmaller.off();
        RedSmaller.on();
        state=PolyYellow;
      }
    break;

    case PolyYellow:
      if (Ten.done()==true)
      {
        Serial.println("Ten Seconds Elapsed, Turning Red on Poly");
        Five.set(5000);
        GreenPoly.off();
        YellowPoly.off();
        RedPoly.on();
        GreenSmaller.off();
        YellowSmaller.off();
        RedSmaller.on();
        state=PolyRed;        
      }
    break;

    case PolyRed:
      if (Five.done()==true)
      {
        Serial.println("Five Seconds Elapsed, Moving to SM2");
        StateMachineSmallerRoad();
        state=SM2Question;         
      }
    break;

    case SM2Question:
    Serial.println("Waiting for True at SM2");
      if (StateMachineSmallerRoad()==true);
      {
        state=PolyGreen;
      }
    break;

    default:
        break;
  }
}

int StateMachineSmallerRoad()
{
  enum {BothRed, SmallerGreen, SmallerYellow, SM2InternalQuestion};
  static MSTimer TenLimit;
  static MSTimer FiveLimit;
  static int state=BothRed;
  int returnValue=false;
  
  switch(state)
  {
    case BothRed:

    GreenPoly.off();
    YellowPoly.off();
    RedPoly.on();
    GreenSmaller.off();
    YellowSmaller.off();
    RedSmaller.on();
      if (CarSmaller.isPushed()==true)
      {
        Serial.println("Car still at Smaller, Proceeding to Green");
        TenLimit.set(10000);
        state=SmallerGreen;
      }
      else if (CarSmaller.isPushed()==false)
      {
        Serial.println("Car no longer detected, aborting");
        StateMachinePolyDrive();
      }
    break;
        
    case SmallerGreen:
      if (TenLimit.done()==true)
      {
        Serial.println("Ten Seconds Elapsed, Turning Yellow on Smaller");
        FiveLimit.set(5000);
        GreenPoly.off();
        YellowPoly.off();
        RedPoly.on();
        GreenSmaller.off();
        YellowSmaller.on();
        RedSmaller.off();
        state=SmallerYellow;        
      }
    break;

    case SmallerYellow:
      if (FiveLimit.done()==true)
      {
        Serial.println("Five Seconds Elapsed, Turning Red on Smaller");
        GreenPoly.on();
        YellowPoly.off();
        RedPoly.off();
        GreenSmaller.off();
        YellowSmaller.off();
        RedSmaller.on();
        FiveLimit.set(5000);
        state=SM2InternalQuestion;                
      }
    break;

    case SM2InternalQuestion:
      if (FiveLimit.done()==true);
      {
        Serial.println("Five Seconds of Red Finished, Moving to SM1");
        returnValue=true;
        state=BothRed;
      }
    break;

     default:
         break;
  }

  return returnValue;
}
















 


