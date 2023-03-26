
# GPS Autonomous Navigation System


## Description

That sounds like an exciting and challenging project! Building a functional waypoint base autonomous navigation car from scratch requires a diverse set of skills, including electrical engineering and coding. Before starting, develop a detailed plan that includes a list of necessary components, their specifications, and where to source them.


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


<img width="897" alt="Screenshot 2023-03-27 at 3 57 39 AM" src="https://user-images.githubusercontent.com/42220653/227807293-3fba0681-18e8-457e-9914-18acbf0a9c04.png">



## Coding
Get all real-time data from the module. Calculate the values and find target heading and target distance. We use some mathematical method to get target heading and target distance for calibrating our rover. 

If the rover's heading degree and calculated heading degree do not match, then the rover will turn left or turn right and calibrate heading. Then if the rover's heading degree and calculated heading degree are matched, then go forward until a distance less than 5 meters.
If the rover reaches a waypoint, then the rover will get ready to move to the next waypoint. 



Use your locations Magnetic Declination Angle for compass.

For Dhaka Magnetic Declination: -0° 29'. [World magnetic declination](https://www.magnetic-declination.com/)

```bash
  float declinationAngle = ((-0) + (29 / 60.0)) / (180 / M_PI);
```

This mathematical term provide distance between two cordinates.

```bash
  float getDistance( float lat1, float long1, float lat2, float long2 ){
  float latA = radians(lat1);
  float longA = radians(long1);
  float latB = radians(lat2);
  float longB = radians(long2);

  return ( 1000 * R * acos(  (sin(latA) * sin(latB)) + ( cos(latA) * cos(latB) * cos(longA - longB) ) ) );
  }
```


This mathematical term provide target heading degrees from current cordinate.
```bash
  float getAngle( float lat1, float long1, float lat2, float long2 )  {
  float dlon = radians(long2 - long1);
  float cLat = radians(lat1);
  float tLat = radians(lat2);
  float a1 = sin(dlon) * cos(tLat);
  float a2 = sin(cLat) * cos(tLat) * cos(dlon);
  a2 = cos(cLat) * sin(tLat) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  int targetHeading = degrees(a2);

  return targetHeading;
  }
```






## Conclusion
In conclusion, this project was a challenging but rewarding experience for the team. Our task was to build a functional GPS autonomous car from scratch, which required us to gather various components and assemble them together, and code the microcontroller.
