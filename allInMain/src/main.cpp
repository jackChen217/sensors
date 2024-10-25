/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kuanglixian                                               */
/*    Created:      9/17/2024, 4:30:07 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*
{ Red sig
  "brightness": 50,
  "signatures": [
    {
      "name": "SIG_1",
      "parameters": {
        "uMin": 9045,
        "uMax": 10287,
        "uMean": 9666,
        "vMin": -1115,
        "vMax": -637,
        "vMean": -876,
        "rgb": 11029829,
        "type": 0,
        "name": "SIG_1"
      },
      "range": 2.5
    },
  ],
  "codes": []
}


blue sig
"parameters": {
        "uMin": -3083,
        "uMax": -2597,
        "uMean": -2840,
        "vMin": 1689,
        "vMax": 2335,
        "vMean": 2012,
        "rgb": 7444661,
        "type": 0,
        "name": "SIG_1"
      },
      "range": 2.5


*/

//correct parameter input
// vex::vision::signature::signature	(	int32_t 	id,
// int32_t 	uMin,
// int32_t 	uMax,
// int32_t 	uMean,
// int32_t 	vMin,
// int32_t 	vMax,
// int32_t 	vMean,
// float 	range,
// int32_t 	type 
// )	

#include <array>
#include "vex.h"

#include <utility> // for std::pair xy



using namespace vex;

vex::brain       Brain;

vex::motor LeftMotorA(PORT11, gearSetting::ratio18_1, true);
vex::motor RightMotorA(PORT20, gearSetting::ratio18_1, true);
vex::motor Arm(PORT19, gearSetting::ratio18_1, false);
vex::motor FrontArm(PORT15, gearSetting::ratio18_1, true);


vision::signature RedSig = vision::signature(1, 7099, 10703, 8901, -1405, -869, -1137, 2.5, 0);
vision::signature BlueSig = vision::signature(2, -3083, -2597, -2840, 1689, 2335, 2012, 2.5, 0);

// Create a new Color Signature "Red" with the Colordesc class.
// the way i gets these numbers, go to  https://codev5.vex.com/ and set color, and use a online color picker to get the code
//  id, red, green, blue, hue , Saturation 
aivision::colordesc redRing (0,     228,      37,     98,   11.00,    0.31);
aivision::colordesc blueRing = aivision::colordesc(1, 50, 165, 210, 10.00, 0.20);

class colordesc {
public:
    colordesc() {} // Default constructor
    colordesc(int id, int r, int g, int b, double size, double tolerance) {
        // Initialize the object with parameters
    }
};
std::array<aivision::colordesc, 2> colors = { redRing, blueRing };



aivision aiVision1 = aivision(PORT3, redRing, blueRing);


vex::controller Controller1 = controller(primary);
vex::distance Distance1(PORT10);
vex::optical ColorSensor(PORT16);
vex::vision VisionSensor(PORT2, 81, RedSig, BlueSig);// port, brightness, color signature1, sig2


// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

//all the functions
void leftDrive(double speed) {
  LeftMotorA.spin(directionType::fwd, speed, velocityUnits::pct);
//  LMotorB.spin(directionType::fwd, speed, velocityUnits::pct);
}

void rightDrive(double speed) {
  RightMotorA.spin(directionType::fwd, speed, velocityUnits::pct);
//  RMotorB.spin(directionType::fwd, speed, velocityUnits::pct);
}

void arcadeDrive(double forwardValue, double turningValue) {
  if (abs(forwardValue) < 10) {
    forwardValue = 0;
  }
  if (abs(turningValue) < 10) {
    turningValue = 0;
  }
  leftDrive(forwardValue + turningValue);
  rightDrive(forwardValue - turningValue);
}

void tankDrive(double leftForward, double rightForward){
  if (abs(leftForward) < 10) {
    leftForward = 0;
  }
  if (abs(rightForward) < 10) {
    rightForward = 0;
  }
  leftDrive(leftForward);
  rightDrive(rightForward);
}

//All Arms
void armUp(){
  Arm.spin(forward);
  //Arm.spinFor(forward,400, degrees, true);
}
void armDown(){
  //Arm.spinFor(reverse,400, degrees, true);
  Arm.spin(reverse);
}
void armControl(){
  Controller1.ButtonR1.pressed(armUp),Controller1.ButtonR2.pressed(armDown);
} 
void fArmUp(){
  //FrontArm.spinFor(forward,60, degrees, true);
  FrontArm.spin(forward);
}
void fArmDown(){
  //FrontArm.spinFor(reverse,60, degrees, true);
  FrontArm.spin(reverse);
}
void frontArmControl(){
  Controller1.ButtonL1.pressed(fArmUp),Controller1.ButtonL2.pressed(fArmDown);
}
 
// parameter takes a index from "colors" array
void  aiObjCount(int index){
  aiVision1.takeSnapshot(colors[index]);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Object Count: %d", aiVision1.objectCount);
}


void aiGetXPos(int index){
  aiVision1.takeSnapshot(colors[index]);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("x pos: %d", aiVision1.objects[0].centerX);

}





//distance 
void getDistance(){

    if(Distance1.isObjectDetected()){
      double disValue = Distance1.objectDistance(mm);
    // return disValue;
      Brain.Screen.clearLine(7);
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("obj detected");
      //Brain.Screen.Screen.print("%s")
      Brain.Screen.newLine();
      Brain.Screen.clearLine(8);
      //ithink 1000mm is the max it can detect, although vex says 2000mm
      Brain.Screen.print("distance : %d", (int)disValue);
      Brain.Screen.newLine();
      Brain.Screen.clearLine();
      if (Distance1.objectSize() == sizeType::large) {
        Brain.Screen.print("Object: Large");
        Brain.Screen.newLine();
      }
      if (Distance1.objectSize() == sizeType::medium) {
        Brain.Screen.print("Object: Medium");
        Brain.Screen.newLine();
      }
      if (Distance1.objectSize() == sizeType::small) {
        Brain.Screen.print("Object: Small");
        Brain.Screen.newLine();
        
      }
      if (Distance1.objectSize() == sizeType::none) {
        Brain.Screen.print("Object: none");
        Brain.Screen.newLine();
      }
      Brain.Screen.setCursor(10, 1);
      Brain.Screen.clearLine();
      Brain.Screen.print("obj m/s: %.3f",Distance1.objectVelocity());

      Brain.Screen.setCursor(11, 1);
      Brain.Screen.clearLine();
      double RawSize = Distance1.objectRawSize();
      Brain.Screen.print("RawSize: %.1f", RawSize);

    }
    else{
      Brain.Screen.clearLine(7);
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("no obj detected");
      Brain.Screen.newLine();
      Brain.Screen.clearLine();
     Brain.Screen.print("distance : 0");
    }
  
}



void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................



      // Declare a pair to hold X and Y values
  std::pair<double, double> XandY;

      // Call pairXY to get the x and y coordinates of object with id (which red is 1, blue is 2)
  // XandY = pairXY(2);

    // Access the x and y values
    double X = XandY.first;   // Access the x value
    double Y = XandY.second;  // Access the y value

// right0.0, left 315, center 157
  if (X>170){ // obj is on the left side
    //pct -> percentage
    // LeftMotorA.setVelocity(30, pct);
    // RightMotorA.setVelocity(30, pct);
    rightDrive(30);

  }
  else if(X<145){ //obj is on the right side
    leftDrive(30);
  }
  else {
    printf("good");
  }



}






void usercontrol(void) {

  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
// void distanceInfo(){
//   printf("a");
// }

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {

    //printf("%d\n",blueRing.id);// -> print 1

    RightMotorA.setStopping(brake);
    LeftMotorA.setStopping(brake);
    Arm.setStopping(hold); // hold, brake, coast
    //FrontArm.setStopping(brake);
    tankDrive(-Controller1.Axis3.value(), Controller1.Axis2.value());

    armControl();
    frontArmControl();
    // Brain.Screen.clearScreen();
    // Brain.Screen.setCursor(1, 1);
    // Brain.Screen.print(visionSensor.objectCount);
// (colorDesc1, colorDesc2)


//Ai Vision calls
    // 0->redRing, 1-> bluering for index
    aiObjCount(0); // ->red
    aiGetXPos(0);



    
    // checkRedBlue();
    // printCenterXY(0);
    getDistance();
    
    wait(100, msec);


  }
}






























// /*
// std::map<std::string, double>  getCenterX(int id){
//   //map yue= dictionary, this create a map for x value;
//   std::map<std::string, double> XValue;
//   //get x from vision sensor
//   double centerX = VisionSensor.objects[id].centerX;
//   XValue["XVal"] = centerX;
//   return XValue;
// }

// std::map<std::string, double>  getCenterY(int id){
//   //map yue= dictionary, this create a map for y value;
//   std::map<std::string, double> YValue;
//   //get y from vision sensor
//   double centerY = VisionSensor.objects[id].height;
//   YValue["Yval"] = centerY;
//   return YValue;
// }
// // get result 
// // std::map<std::string, double> redXY = getCenterX(id);


// */



// // all the vision sensor

// // "when I receive checkRed" hat block
// void checkRedBlue(){
//   Brain.Screen.setFont(mono15);
//   //Controller1.Screen.clearLine(2);

//   //visionSensor.setBrightness(81);
//     //checkRed
//   // Get a snapshot of all RED Color objects.
//     VisionSensor.takeSnapshot(RedSig);
//     if (VisionSensor.objects[0].exists) {
//         Brain.Screen.clearLine(2);
//         Brain.Screen.setCursor(Brain.Screen.row(), 1);
//         Brain.Screen.setCursor(2, 1);
//         Brain.Screen.print("Red Object Found" );
//     }
//     else {
//       Brain.Screen.clearLine(2);
//       Brain.Screen.setCursor(Brain.Screen.row(), 1);
//       Brain.Screen.setCursor(2, 1);
//       Brain.Screen.print( "No Red Object found");
//     }
// //checkBlue
//     VisionSensor.takeSnapshot(BlueSig);
//     if (VisionSensor.objects[1].exists) {
//       Brain.Screen.clearLine(3);
//       Brain.Screen.setCursor(Brain.Screen.row(), 1);
//       Brain.Screen.setCursor(3, 1);
//       Brain.Screen.print("Blue Object Found");
//     }
//     else {
//       Brain.Screen.clearLine(3);
//       Brain.Screen.setCursor(Brain.Screen.row(), 1);
//       Brain.Screen.setCursor(3, 1);
//       Brain.Screen.print("No Blue Object found");
//     }
// }

//                 //0 is red, 1 is blue
// void printCenterXY(int id){
// // right0.0, left 315, center 157
//     VisionSensor.takeSnapshot(RedSig);
//     if( VisionSensor.objects[id].exists){
//         Brain.Screen.clearLine(5);
//         Brain.Screen.setCursor(5, 1);
//         Brain.Screen.print("id: %d,  Center X: %d", id, VisionSensor.objects[id].centerX);
//         Brain.Screen.print(". Center Y: %d", VisionSensor.objects[id].centerY);

//         Brain.Screen.clearLine(6);
//         Brain.Screen.setCursor(6, 1);
//         Brain.Screen.print("id: %d, width : %d", id,  VisionSensor.objects[id].width);
//         Brain.Screen.print("  height : %d", VisionSensor.objects[id].height);

//     }
// }



// double getCenterX(int id){
//   double centerX = VisionSensor.objects[id].centerX;
//   return centerX;
// }

// double getCenterY(int id){
//   double centerY = VisionSensor.objects[id].centerX;
//   return centerY;
// }


// std::pair<double, double> pairXY(int id) {
//     double centerX = VisionSensor.objects[id].centerX; // Get x from the sensor
//     double centerY = VisionSensor.objects[id].centerY; // Get y from the sensor

//     // Return both x and y as a pair
//     return std::make_pair(centerX, centerY);
// }

// /*
// access to this 
// std::pair<double, double> position = getCenterXY(id); // Get the x and y values

//     // Access the x and y values from the pair
//     double centerX = position.first;  // First element of the pair is x
//     double centerY = position.second;

// */

