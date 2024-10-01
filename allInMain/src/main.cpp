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


#include "vex.h"

using namespace vex;

vex::brain       Brain;

vex::motor LeftMotorA(PORT1, gearSetting::ratio18_1, true);
vex::motor RightMotorA(PORT2, gearSetting::ratio18_1, true);
vex::motor Arm(PORT3, gearSetting::ratio18_1, false);
vex::motor FrontArm(PORT8, gearSetting::ratio18_1, true);


vision::signature RedSig = vision::signature(1, 7099, 10703, 8901, -1405, -869, -1137, 2.5, 0);
vision::signature BlueSig = vision::signature(2, -3083, -2597, -2840, 1689, 2335, 2012, 2.5, 0);



vex::controller Controller1 = controller(primary);
vex::distance Distance1(PORT9);
vex::optical ColorSensor(PORT16);
vex::vision VisionSensor(PORT10, 81, RedSig, BlueSig);// port, brightness, color signature1, sig2


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

//

// "when I receive checkRed" hat block
void checkRedBlue(){
  Brain.Screen.setFont(mono15);
  //Controller1.Screen.clearLine(2);

  //visionSensor.setBrightness(81);
    //checkRed
  // Get a snapshot of all RED Color objects.
    VisionSensor.takeSnapshot(RedSig);
    if (VisionSensor.objects[0].exists) {
        Brain.Screen.clearLine(2);
        Brain.Screen.setCursor(Brain.Screen.row(), 1);
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print("Red Object Found" );
    }
    else {
      Brain.Screen.clearLine(2);
      Brain.Screen.setCursor(Brain.Screen.row(), 1);
      Brain.Screen.setCursor(2, 1);
      Brain.Screen.print( "No Red Object found");
    }
//checkBlue
    VisionSensor.takeSnapshot(BlueSig);
    if (VisionSensor.objects[1].exists) {
      Brain.Screen.clearLine(3);
      Brain.Screen.setCursor(Brain.Screen.row(), 1);
      Brain.Screen.setCursor(3, 1);
      Brain.Screen.print("Blue Object Found");
    }
    else {
      Brain.Screen.clearLine(3);
      Brain.Screen.setCursor(Brain.Screen.row(), 1);
      Brain.Screen.setCursor(3, 1);
      Brain.Screen.print("No Blue Object found");
    }
}

                //0 is red, 1 is blue
void getCenterXY(int id){
// right0.0, left 315, center 157
    VisionSensor.takeSnapshot(RedSig);
    if( VisionSensor.objects[id].exists){
        Brain.Screen.clearLine(5);
        Brain.Screen.setCursor(5, 1);
        Brain.Screen.print("id: %d,  Center X: %d", id, VisionSensor.objects[id].centerX);
        Brain.Screen.print(". Center Y: %d", VisionSensor.objects[id].centerY);

        Brain.Screen.clearLine(6);
        Brain.Screen.setCursor(6, 1);
        Brain.Screen.print("id: %d, width : %d", id,  VisionSensor.objects[id].width);
        Brain.Screen.print("  height : %d", VisionSensor.objects[id].height);

    }
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
    
    checkRedBlue();
    getCenterXY(0);
    getDistance();
    
    wait(100, msec);


  }
}
