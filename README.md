# IntroductionToRobotics
This repository is a collection of assignments for the "Introduction to Robotics" course, taken at the Faculty of Mathematics and Computer Science of the University of Bucharest
## Table of Contents

- [Homework 1](#homework-1)
- [Homework 2](#homework-2)
- [Homework 3](#homework-3)

## Homework 1

The requirements of this homework where to use 3 potentiometers to control each pin of the RGB_LED. I connected the three potentiometers to the A0, A1, A2 analog pins on the arduino board and then assigned the read values to each pin of the RGB_LED.
<br />
Link the the Video: https://youtu.be/knyk0hIq7Lc
<details>
<summary>Photo</summary>

  ![Image](https://github.com/andreialexandru02/IntroductionToRobotics/assets/93448105/bcbff355-a2d6-4780-a124-39470bd5841e)

</details>

## HomeWork2

The requirements of this homework where to simulate an elevator. My elevator isnt working quite properly. I used classes to structure the code, and stored the 3 floors is an array of objects of type FloorLed and the three buttons in an array of objects type Button. When, for example, button 2 is pressed(with the initial floor being 0) a functin Elevator that takes as paramenters the starting and finishing floor is called. Inside this functions i iterate through the floors that should be passes and call another function on each iteration that waits for 2 seconds, lights a led for 2 seconds and waits another 2 seconds. A led is also set to flicker, indicating that the elevator is working. My problem comes from the fact that the Elevator function si called when pressing a button, which means that it doesnt loop. Therefore i cant use millis() to add a delay to the for iterations, which leads to the leds of the floors that should be passed all lighting at the same time. This also arises the problem of the flickering led not working, because the Elevator function takes very little time to run (because it has no delay). I tried placing the Elevator function inside the loop and "activating" it with a boolean value, but it takes certains parameters and can't just loop on random or past parameters. As of now, i can't figure out how i should solve this problem. 
<br />
Link the the Video: https://youtu.be/MzQaLC4wf3A

## HomeWork3
The requirments of this homework where to use the joystick switch to control the 7 segments display. It should be able to select certain segments and use the hold function of the joystick button to reset all the segments.
<br />

Link to the Video: "https://www.youtube.com/watch?v=gw2TKci28t8"

<details>
<summary>Photo</summary>

- ![HomeWork4Img1](https://github.com/andreialexandru02/IntroductionToRobotics/assets/93448105/33932749-a048-4faf-934e-96fac5d63557)

</details>
