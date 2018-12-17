#include <CPE123_Fall16.h>
// Provided Library CPE 123 Fall 2016

// Define your pins
const int ledPin1 = 6;    // LED GreenPoly, Green Traffic Light at Poly Drive Intersection
const int ledPin2 = 7;    // LED YellowPoly, Yellow Traffic Light at Poly Drive Intersection
const int ledPin3 = 8;    // LED RedPoly, Red Traffic Light at Poly Drive Intersection
const int ledPin4 = 9;    // LED GreenSmaller, Green Traffic Light at Smaller Road
const int ledPin5 = 10;   // LED YellowSmaller, Yellow Traffic Light at Smaller Road
const int ledPin6 = 11;   // LED RedSmaller, Red Traffic Light at Smaller Road
const int buttonPin1 = 2; // BUTTON CarPoly, Creates a Car at Poly Drive Intersection
const int buttonPin2 = 3; // BUTTON CarSmaller, Creates a Car at Smaller Road Intersection

// Hardware Creation
Led GreenPoly(ledPin1);
Led YellowPoly(ledPin2);
Led RedPoly(ledPin3);
Led GreenSmaller(ledPin4);
Led YellowSmaller(ledPin5);
Led RedSmaller(ledPin6);
Button CarPoly(buttonPin1);
Button CarSmaller(buttonPin2);

// Initial Setup Function
void setup() {
  // Set up serial monitor and print out program info
  Serial.begin(9600);
  setupMessage(__FILE__);
  printLibVersion();
}

// Main Process Loop
void loop() {
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
    case PolyGreen: // The Light is Green at Poly Drive
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
        // Adjust Lights at intersection
        GreenPoly.off();
        YellowPoly.on();
        RedPoly.off();
        GreenSmaller.off();
        YellowSmaller.off();
        RedSmaller.on();
        state=PolyYellow;
      }
    break;

    case PolyYellow: // The Light is Yellow at Poly Drive
      if (Ten.done()==true)
      {
        Serial.println("Ten Seconds Elapsed, Turning Red on Poly");
        Five.set(5000);
        // Adjust Lights at intersection
        GreenPoly.off();
        YellowPoly.off();
        RedPoly.on();
        GreenSmaller.off();
        YellowSmaller.off();
        RedSmaller.on();
        state=PolyRed;        
      }
    break;

    case PolyRed: // The Light is Red at Poly Drive
      if (Five.done()==true)
      {
        Serial.println("Five Seconds Elapsed, Moving to SM2");
        StateMachineSmallerRoad();
        state=SM2Question;         
      }
    break;

    case SM2Question:
    Serial.println("Waiting for True at SM2");
      if (StateMachineSmallerRoad()==true); // Light Cycle has Finished at Smaller Road
      {
        // Transition back to Green Light on Poly Drive
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
    case BothRed: // Both Lights are Red for each intersection
    // Adjust Lights at Intersection
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
        
    case SmallerGreen: // Light is Green at Smaller Intersection
      if (TenLimit.done()==true)
      {
        Serial.println("Ten Seconds Elapsed, Turning Yellow on Smaller");
        FiveLimit.set(5000);
        // Adjust Lights at Intersection
        GreenPoly.off();
        YellowPoly.off();
        RedPoly.on();
        GreenSmaller.off();
        YellowSmaller.on();
        RedSmaller.off();
        state=SmallerYellow;        
      }
    break;

    case SmallerYellow: // Light is Yellow at Smaller Intersection
      if (FiveLimit.done()==true)
      {
        Serial.println("Five Seconds Elapsed, Turning Red on Smaller");
        // Adjust Lights at Intersection
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
      if (FiveLimit.done()==true); // We have waited five seconds with Red Light on Smaller Intersection
      {
        Serial.println("Five Seconds of Red Finished, Moving to SM1");
        returnValue=true;
        state=BothRed;
      }
    break;

     default:
         break;
  }
  // Return Value set at SM2 Internal Question, has the light been red for five seconds?
  return returnValue;
}
















 


