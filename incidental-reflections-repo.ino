#include <AccelStepper.h>

#define HALFSTEP 8
#define FULLSTEP 4
const bool DEBUG = false;

const int STEP = FULLSTEP;
const long ZERO_DISTANCE = 0.0;

//for half:
//const int STEPS_PER_REV = 2048; //estimate
//const int MAX_SPEED = 2000; //i believe

//for full:
const int STEPS_PER_REV = 1024; //estimate
const int MAX_SPEED = 1000; //i believe

const float REVS = 8;
long target = (long)( float(STEPS_PER_REV) * REVS );

const int NUM_STEPPERS = 11;
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper steppers[NUM_STEPPERS] = {
  AccelStepper(STEP, 47, 51, 49, 53), //1
  AccelStepper(STEP, 6, 8, 7, 9), //2
  AccelStepper(STEP, 31, 35, 33, 37), //3
  AccelStepper(STEP, 13, 11, 12, 10), //4
  AccelStepper(STEP, 1, 14, 0, 15), //5
  AccelStepper(STEP, 30, 34, 32, 36), //6
  AccelStepper(STEP, 5, 3, 4, 2), //7
  AccelStepper(STEP, 46, 50, 48, 52), //8
  AccelStepper(STEP, 23, 27, 25, 29), //9
  AccelStepper(STEP, 38, 42, 40, 44), //10
  AccelStepper(STEP, 39, 43, 41, 45) //11
};

long positions[NUM_STEPPERS];

const int stepperIndicesAtEnds[2] = {0, NUM_STEPPERS-1};
int stepperIndexAtEndRef = 1;
int lastStepperToFinishIndex = stepperIndicesAtEnds[stepperIndexAtEndRef]; //NUM_STEPPERS-1
AccelStepper* lastStepperToFinish = &steppers[lastStepperToFinishIndex];

//////////

void setup()
{
  Serial.begin(9600);
  Serial.println("----- BEGIN SETUP CODE");
  
  for(int i=0; i<NUM_STEPPERS; ++i) {
    steppers[i].setMaxSpeed(MAX_SPEED);
    steppers[i].setSpeed(MAX_SPEED);
    steppers[i].setAcceleration(MAX_SPEED*4);
    positions[i] = calculateIncrementalTargetPosition(target, i, NUM_STEPPERS, true);
    Serial.println(positions[i], DEC);
    steppers[i].moveTo(positions[i]);
  }

  Serial.println("----- END SETUP CODE");
}

void loop()
{
  long distance = lastStepperToFinish->distanceToGo();
//  if(DEBUG) {
//    Serial.print("last stepper (index ");
//    Serial.print(lastStepperToFinishIndex);
//    Serial.print("): ");
//    Serial.println((int)distance);
//  }
  if(distance == 0) {
    Serial.println("ALL STEPPERS HAVE REACHED TARGET. Reversing now...");
    
    target = -target;
    
    stepperIndexAtEndRef = (stepperIndexAtEndRef + 1) % 2;
    lastStepperToFinishIndex = stepperIndicesAtEnds[stepperIndexAtEndRef];
    lastStepperToFinish = &steppers[lastStepperToFinishIndex];
    
    bool isForwards = (lastStepperToFinishIndex != 0);
    if(!isForwards) {
      for(int i=NUM_STEPPERS; i>=0; --i) {
        positions[i] = calculateIncrementalTargetPosition(target, i, NUM_STEPPERS, isForwards);
        steppers[i].moveTo(positions[i]);
      }
    } else {
      for(int i=0; i<NUM_STEPPERS; ++i) {
        positions[i] = calculateIncrementalTargetPosition(target, i, NUM_STEPPERS, isForwards);
        steppers[i].moveTo(positions[i]);
      }
    }
    
  }

  for(int i=0; i<NUM_STEPPERS; ++i) {
    if(steppers[i].distanceToGo() != 0) {
      steppers[i].run();
    }
  }
   
}

/////////////////
//HELPER FUNCTIONS
/////////////////

long calculateIncrementalTargetPosition(long maxTarget, int i, int totalCount, bool isForwards)
{
  if(i == totalCount) return maxTarget;
   
  float fraction = (float)(i+1) / (float)totalCount;
  float targetPos = (isForwards) ?
    (float)( 0.3 + 0.7*fraction ) * (float)maxTarget :
    targetPos = (float)( 1.0 - 0.7*fraction ) * (float)maxTarget; //for backwards

  return (long)targetPos;
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
