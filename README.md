
# GPS Autonomous Navigation System


## Description

That sounds like an exciting and challenging project! Building a functional autonomous car from scratch requires a diverse set of skills, including electrical engineering and coding. Before starting, develop a detailed plan that includes a list of necessary components, their specifications, and where to source them.


![23 799865,90 44337-2](https://user-images.githubusercontent.com/42220653/227798881-aca26ba6-6b61-4f08-8c00-ddadc5f520f1.png)


## Components

* NEO-M8 gps module
* HMC5883LCompass 
* Motor
* wheel
* Motor driver
* Battery
* Microcontroller.

## Assembly
Attach the motors to the chassis using screws or brackets. Make sure they are evenly spaced and aligned. Attach the wheels to the motors using shafts or couplings. Connect the motor driver board to the motors and battery. Then, connect GPS & compass to the microcontroller. Program the system using a microcontroller such as an Arduino or ESP32. 


## Coding
Get all real-time data from the module. Calculate the values and find target heading and target distance. We use some mathematical method to get target heading and target distance for calibrating our rover. 

If the rover's heading degree and calculated heading degree do not match, then the rover will turn left or turn right and calibrate heading. Then if the rover's heading degree and calculated heading degree are matched, then go forward until a distance less than 5 meters.
If the rover reaches a waypoint, then the rover will get ready to move to the next waypoint. 






## Conclusion
In conclusion, this project was a challenging but rewarding experience for the team. Our task was to build a functional GPS autonomous car from scratch, which required us to gather various components and assemble them together, and code the microcontroller.
