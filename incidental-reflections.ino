// Initialize Accelsteppers with pin sequence IN1-IN3-IN2-IN4 for 28BYJ-48 motors
#include <AccelStepper.h>

const int HALFSTEP = 8;
const int FULLSTEP = 4;
const bool DEBUG = false;
const int DELAY_WHEN_REVERSING__MIN = 1500;
const int DELAY_WHEN_REVERSING__MAX = 8500;

const int STEP = FULLSTEP;
const int STEPS_PER_REV__FULL = 1045;
const int STEPS_PER_REV = (STEP == HALFSTEP) ? STEPS_PER_REV__FULL*2 : STEPS_PER_REV__FULL;
const int MAX_SPEED = (STEP == HALFSTEP) ? 1500 : 750;
const int ACCELERATION = MAX_SPEED;

//How many revolutions do we want? This is arbitrary.
const float REVS = 4;
const long targetMagnitude = (long)( float(STEPS_PER_REV) * REVS );

const int NUM_STEPPERS_ARRAY = 13;
const int NUM_STEPPERS_BOARD = 16;
const float NUM_STEPPERS_ARRAY_FLOAT = (float)NUM_STEPPERS_ARRAY;
const float NUM_STEPPERS_BOARD_FLOAT = (float)NUM_STEPPERS_BOARD;
const int NUM_STEPPERS_DIFF = NUM_STEPPERS_BOARD - NUM_STEPPERS_ARRAY;

// Give each stepper a name/# which begins at 1 at one of the middle motors
// and spirals outward towards an outer edge corner of the 4x4 motor grid

// AccelSteppers constructor wants a non-linear order of the pins coming
// from the ULN2003 driver.
// Refer to the pins IN1 - IN4 on the driver board.
// Declare like so:
// AccelStepper stepper5(STEP, IN1, IN3, IN2, IN4);

//Steppers 1-4 are controlled as 1 unit as per stepper4's output:
AccelStepper stepper4(STEP, 23, 27, 25, 29);

//Steppers 5-16 are all individually controlled:
AccelStepper stepper5(STEP, 37, 33, 35, 31);
AccelStepper stepper6(STEP, 52, 48, 50, 46);
AccelStepper stepper7(STEP, 36, 32, 34, 30);
AccelStepper stepper8(STEP, 45, 41, 43, 39);
AccelStepper stepper9(STEP, 44, 40, 42, 38);
AccelStepper stepper10(STEP, 5, 3, 4, 2);
AccelStepper stepper11(STEP, 53, 49, 51, 47);
AccelStepper stepper12(STEP, 9, 7, 8, 6);
AccelStepper stepper13(STEP, 13, 11, 12, 10);
AccelStepper stepper14(STEP, 28, 24, 26, 22);
AccelStepper stepper15(STEP, 14, 16, 15, 17);
AccelStepper stepper16(STEP, 18, 20, 19, 21);

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

//The inside steppers 1-4 (controlled as unit as stepper 4)
//move against the grain by design.
// Also, 14 and 15 move reverse than desired... this fixes that.
//TODO: use stuct with an AccelStepper* and a bool instead of 2 arrays
const bool movesAgainstTheGrain[NUM_STEPPERS_ARRAY] = {
  true, //stepper4,
  false, //stepper5,
  false, //stepper6,
  false, //stepper7,
  false, //stepper8,
  false, //stepper9,
  false, //stepper10,
  false, //stepper11,
  false, //stepper12,
  false, //stepper13,
  true, //stepper14,
  true, //stepper15,
  false, //stepper16
};

bool isForwards = true;
int lastStepperIndex = NUM_STEPPERS_ARRAY-1;
AccelStepper* lastStepper = &steppers[lastStepperIndex];
AccelStepper* stepper;  //for current stepper when iterating thru array
//float progressToTarget;         //used in steppers array interation
int loopIndex;

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

    delay( random(DELAY_WHEN_REVERSING__MIN, DELAY_WHEN_REVERSING__MAX) );
//    delay(DELAY_WHEN_REVERSING);
  }
  
  runSteppers();
}

/////////////////
//HELPER FUNCTIONS
/////////////////

void setTargetForSteppers(long targetPosition) {
  long stepperTarget;
  const long negativeTargetPosition = -targetPosition;
  for(int i=0; i<NUM_STEPPERS_ARRAY; ++i) {  stepper = &steppers[i];
    stepperTarget = movesAgainstTheGrain[i] ? negativeTargetPosition : targetPosition;

    //TODO: test if works as expected
    stepper->setCurrentPosition(0);
    
    stepper->setMaxSpeed(MAX_SPEED);
    stepper->setAcceleration(ACCELERATION);
    stepper->moveTo(stepperTarget);
  }
}

void runSteppers() {
  for(loopIndex=0; loopIndex<NUM_STEPPERS_ARRAY; ++loopIndex) {  //stepper = &steppers[i];
    steppers[loopIndex].run();   //stepper->run();
  }
}


