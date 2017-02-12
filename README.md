# Incidental Reflections

This code is for Jason Worden's project "Incidental Reflections" for the
Gray Area foundation of the Arts' Creative Coding Immersive Exhibition for the
Fall 2016 Cohort.

The code is publicly available for use and examination on [Github](https://github.com/jasonworden/incidental-reflections/).

Footage of the finished installation can be found [on Vimeo](https://vimeo.com/203724115):

[![Click to see footage on Vimeo](video-still.png)](https://vimeo.com/203724115)

## Arduino Mega

16 `28byj-48` stepper motors are controlled by a single Arduino Mega,
which gives us a whopping grand total of 54 digital I/O pins!

![Arduino Mega](https://i.stack.imgur.com/HZUsw.png)

## Arduino IDE

Use the Arduino IDE to upload this code to your Arduino mega.

## 28byj-48 stepper motors

###### IMPORTANT:

As detailed in this [awesome wiki resource](https://grahamwideman.wikispaces.com/Motors-+28BYJ-48+Stepper+motor+notes), different types of these exist with different gearings and voltages.

While the 5v version and the 12 version look the same, they desire differing voltages. Using the same code and board but with the 12v version (and supplying 5v), the motor spins very slowly. Haven't been able to test yet with a 12v power supply.

### Full step

```
#define FULLSTEP 4

int STEPS_PER_REV = 1024;
int MAX_SPEED = 1000;
```

### Half-step

```
#define HALFSTEP 8

int STEPS_PER_REV = 2048;
int MAX_SPEED = 2000;
```

### Helpful Resources

* [This article](http://www.jangeox.be/2013/10/stepper-motor-28byj-48_25.html)
is a great resource on the different types of motor stepping as pertinent to
the `28byj-48` stepper motors.
    * Has low-level example code at half-step and full-step without using external library such as AccelStepper
    * Detailed experiments for both half-stepping and full-stepping
