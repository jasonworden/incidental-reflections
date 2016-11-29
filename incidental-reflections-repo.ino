// Initialize Accelsteppers with pin sequence IN1-IN3-IN2-IN4 for 28BYJ-48 motors
#include <AccelStepper.h>

const int HALFSTEP = 8;
const int FULLSTEP = 4;
const bool DEBUG = false;

const int STEP = FULLSTEP;
const int STEPS_PER_REV__FULL = 1045;
const int STEPS_PER_REV = (STEP == HALFSTEP) ? STEPS_PER_REV__FULL*2 : STEPS_PER_REV__FULL;
const int MAX_SPEED = (STEP == HALFSTEP) ? 1500 : 750;

//How many revolutions do we want? This is arbitrary.
const float REVS = 8;
const long targetMagnitude = (long)( float(STEPS_PER_REV) * REVS );

const int NUM_STEPPERS_ARRAY = 13;
const int NUM_STEPPERS_BOARD = 16;
const float NUM_STEPPERS_ARRAY_FLOAT = (float)NUM_STEPPERS_ARRAY;
const float NUM_STEPPERS_BOARD_FLOAT = (float)NUM_STEPPERS_BOARD;
const int NUM_STEPPERS_DIFF = NUM_STEPPERS_BOARD - NUM_STEPPERS_ARRAY;

// Give each stepper a name/# which begins at 1 at one of the middle motors
// and spirals outward towards an outer edge corner of the 4x4 motor grid

//Steppers 1-4 are controlled as 1 unit as per stepper4's output:
AccelStepper stepper4(STEP, 13, 11, 12, 10);

//Steppers 5-16 are all individually controlled:
AccelStepper stepper5(STEP, 22, 26, 24, 28);
AccelStepper stepper6(STEP, 30, 34, 32, 36);
AccelStepper stepper7(STEP, 5, 3, 4, 2);
AccelStepper stepper8(STEP, 46, 50, 48, 52);
AccelStepper stepper9(STEP, 23, 27, 25, 29);
AccelStepper stepper10(STEP, 38, 42, 40, 44);
AccelStepper stepper11(STEP, 39, 43, 41, 45);
AccelStepper stepper12(STEP, 39, 43, 41, 45);
AccelStepper stepper13(STEP, 39, 43, 41, 45);
AccelStepper stepper14(STEP, 39, 43, 41, 45);
AccelStepper stepper15(STEP, 39, 43, 41, 45);
AccelStepper stepper16(STEP, 39, 43, 41, 45);

AccelStepper steppers[NUM_STEPPERS_ARRAY] = {
  stepper4,
  stepper5,
  stepper6,
  stepper7,
  stepper8,
  stepper9,
  stepper10,
  stepper11,
  stepper12,
  stepper13,
  stepper14,
  stepper15,
  stepper16
};

bool isForwards = true;
int lastStepperIndex = NUM_STEPPERS_ARRAY-1;
AccelStepper* lastStepper = &steppers[lastStepperIndex];
AccelStepper* stepper;  //for current stepper when iterating thru array
float progressToTarget;         //used in steppers array interation

//////////

void setup()
{
  Serial.begin(9600);
  setTargetForSteppers(targetMagnitude);
}

void loop()
{
  long distanceToGo = lastStepper->distanceToGo();
  if(distanceToGo == 0)
  {
    Serial.println("Reversing now...");
    isForwards = !isForwards;
    lastStepperIndex = isForwards ? NUM_STEPPERS_ARRAY - 1 : 0;
    lastStepper = &steppers[lastStepperIndex];

    long newTarget = isForwards ? targetMagnitude : -targetMagnitude;
    setTargetForSteppers(newTarget);
  }

  progressToTarget = (float)(targetMagnitude - abs(distanceToGo))/(float)targetMagnitude;
  
  if(isForwards)
  {
    for(int i=0; i<NUM_STEPPERS_ARRAY; ++i) {  stepper = &steppers[i];
      if(calculatePositionFraction(i) > progressToTarget)  stepper->run();
    }
  } else {
    for(int i=NUM_STEPPERS_ARRAY; i>=0; --i) {  stepper = &steppers[i];
      if(calculatePositionFraction(i) < progressToTarget)  stepper->run();
    }
  }
  
}

/////////////////
//HELPER FUNCTIONS
/////////////////

float calculatePositionFraction(int steppersArrayIndex) {
  return (float)(steppersArrayIndex+1+NUM_STEPPERS_DIFF)/NUM_STEPPERS_BOARD_FLOAT;
}

void setTargetForSteppers(long targetPosition) {
  for(int i=0; i<NUM_STEPPERS_ARRAY; ++i) {  stepper = &steppers[i];
    stepper->setCurrentPosition(0);
    stepper->setMaxSpeed(MAX_SPEED);
    stepper->setAcceleration(MAX_SPEED);
    stepper->moveTo(targetPosition);
  }
}


