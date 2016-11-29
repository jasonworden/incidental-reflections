// Initialize Accelsteppers with pin sequence IN1-IN3-IN2-IN4 for 28BYJ-48 motors
#include <AccelStepper.h>

#define HALFSTEP 8
#define FULLSTEP 4
const bool DEBUG = false;

const int STEP = FULLSTEP; //or HALFSTEP
const int STEPS_PER_REV = (STEP == HALFSTEP) ? 2048 : 1024;
const int MAX_SPEED = (STEP == HALFSTEP) ? 2000 : 1000;

const float REVS = 8;
long target = (long)( float(STEPS_PER_REV) * REVS );

const int NUM_STEPPERS = 11;
const float NUM_STEPPERS__FLOAT = (float)NUM_STEPPERS;

// Give each stepper a name/# which begins at 1 at one of the middle motors
// and spirals outward towards an outer edge corner of the 4x4 motor grid:
AccelStepper stepper1(STEP, 47, 51, 49, 53);
AccelStepper stepper2(STEP, 6, 8, 7, 9);
AccelStepper stepper3(STEP, 31, 35, 33, 37);
AccelStepper stepper4(STEP, 13, 11, 12, 10);
AccelStepper stepper5(STEP, 22, 26, 24, 28);
AccelStepper stepper6(STEP, 30, 34, 32, 36);
AccelStepper stepper7(STEP, 5, 3, 4, 2);
AccelStepper stepper8(STEP, 46, 50, 48, 52);
AccelStepper stepper9(STEP, 23, 27, 25, 29);
AccelStepper stepper10(STEP, 38, 42, 40, 44);
AccelStepper stepper11(STEP, 39, 43, 41, 45);
  
AccelStepper steppers[NUM_STEPPERS] = {
  stepper1,
  stepper2,
  stepper3,
  stepper4,
  stepper5,
  stepper6,
  stepper7,
  stepper8,
  stepper9,
  stepper10,
  stepper11
};

bool isForwards = true;
int lastStepperIndex = NUM_STEPPERS-1;
AccelStepper* lastStepper = &steppers[lastStepperIndex];
AccelStepper* stepper;

float progress;
float positionFraction;

//////////

void setup()
{
  Serial.begin(9600);

  setTargetForSteppers(target);
}

void loop()
{
  long distanceToGo = lastStepper->distanceToGo();
  if(distanceToGo == 0) {
    Serial.println("Reversing now...");
    isForwards = !isForwards;
    lastStepperIndex = isForwards ? NUM_STEPPERS-1 : 0;
    lastStepper = & steppers[lastStepperIndex];

    long newTarget = isForwards ? target : -target;
    setTargetForSteppers(newTarget);
  }

  progress = (float)(target - abs(distanceToGo))/(float)target;
//  Serial.println(progress);
  
  if(isForwards) {
    for(int i=0; i<NUM_STEPPERS; ++i) {  stepper = &steppers[i];
      positionFraction = (float)(i+1)/NUM_STEPPERS__FLOAT;
      if(positionFraction > progress)  stepper->run();
    }
  } else {
    for(int i=NUM_STEPPERS; i>=0; --i) {  stepper = &steppers[i];
      positionFraction = (float)(i+1)/NUM_STEPPERS__FLOAT;
      if(positionFraction < progress)  stepper->run();
    }
  }
  
}

/////////////////
//HELPER FUNCTIONS
/////////////////

long calculateIncrementalTargetPosition(long maxTarget, int i, int totalCount, bool isForwards)
{
  if(i == totalCount) return maxTarget;
  if(!isForwards)     maxTarget = -maxTarget;
  float fraction = (float)(i+1) / (float)totalCount;
  float targetPos = (isForwards) ?
    (float)( 0.3 + 0.7*fraction ) * (float)maxTarget :
    targetPos = (float)( 1.0 - 0.7*fraction ) * (float)maxTarget; //for backwards

  return (long)targetPos;
}

void setTargetForSteppers(long targetPosition) {
  for(int i=0; i<NUM_STEPPERS; ++i) {  stepper = &steppers[i];
    stepper->setCurrentPosition(0);
    stepper->setMaxSpeed(MAX_SPEED);
    stepper->setAcceleration(MAX_SPEED*4);
    stepper->setSpeed(MAX_SPEED);
    stepper->moveTo(targetPosition);
  }
}

















//OLD CODE:

//  for(int i=0; i<NUM_STEPPERS; ++i) {
//    // If at the end of travel go to the other end
//    if (steppers[i].distanceToGo() == 0) {
//      steppers[i].moveTo(-steppers[i].currentPosition());
//    }
//    steppers[i].run();
//  }

//  if (steppers[0].distanceToGo() == 0)  {
//    for(int i=0; i<num_steppers; ++i) {
//      steppers[i].moveTo(-steppers[i].currentPosition());
//    }
//  }
//  for(int i=0; i<num_steppers; ++i) {
//    steppers[i].run();
//  }
