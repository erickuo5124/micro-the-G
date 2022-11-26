<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h2 align="center">Victoria</h2>
  <a href="https://github.com/kuouu/victoria">
    <img src="https://user-images.githubusercontent.com/20458463/204020064-fc8663e5-58d3-4700-8a0e-fb742d2321d0.gif" height="500px">
  </a>
  <p align="center">
    Super cool PIC18F based LED lightsaber
    <br />
    <a href="https://github.com/kuouu/victoria/issues">Report Bug</a>
    ·
    <a href="https://github.com/kuouu/victoria/issues">Request Feature</a>
  </p>
</div>

## Table of Contents

- [Usage](#usage)
- [Devices&Environment](#Devices&Environment)
- [Circuit Diagram](#Circuit-Diagram)
- [System Execution Process](#System-Execution-Process)
- [Interface&Library](#Interface&Library)
- [Operation Principle](#Operation-Principle)

## Usage

Wave Victoria, and when it exceeds a certain speed, the LED bulb will emit a smiley face (The Smile of the Goddess of Victory).

## Devices&Environment

- used hardware: PIC18F4520
- compiler: XC 8

## Circuit Diagram

![image4](https://user-images.githubusercontent.com/20458463/204086291-77ff5bc4-fa42-4380-bd5e-5550e18966a5.png)

## System Execution Process

![image2](https://user-images.githubusercontent.com/20458463/204086392-7a187b9d-3168-490a-b2d0-cb601715bff5.png)

## Interface&Library

- GY-61 (ADXL-335)
- LED lamp

## Operation Principle

GY-61 has three analog pins: the accelerations in the directions of X-axis, Y-axis and Z-axis. The values are read into PIC18F4520 and then processed.

### version 1

The first idea is to convert the vector to the current Victoria's tilt angle. The function library math.h is used here to convert the angle and side length, and atan2() is used to get the angle, and the range will be between -𝝅 and 𝝅.
Images are stored in an array. Consider the distance between the light bulb and the center point as the longest side of the triangle, multiply sin() and cos() respectively to get the (x, y) coordinates of the array, and use this to draw the graph.

#### version 1 difficulty

When converting to (x, y) coordinates, you need to use the round() function to obtain integers, which will sacrifice the detection accuracy. In addition, because GY-61 itself has some errors in detection, it will flicker continuously at certain "border" angles, causing difficulties in image recognition.

### version 2

Convert the current angle change to the corresponding element in the array. Assuming the current rotation degree is 0 degrees, the corresponding elements will be all the elements in the 0th column. If the current angle is 12 degrees, the corresponding elements in column 2 will be displayed.
Therefore, as long as we input the angle of the lightsaber into the conversion formula and convert it into the number of columns corresponding to the array, we can know which LED should be turned on.

#### version 2 difficulty

Although the picture will be slightly distorted into a fan shape, you can roughly see the image's profile. But the sampling rate of  GY-61 is too slow, it is difficult to get continuous angles, which results in broken pictures.

### final version

Get the acceleration of the Z axis directly, and display the picture if it exceeds the threshold we set.

The image storage method is still using version 2, but the display method is to display one by one through __delay_ms() to achieve the effect of visual persistence.
