# InvenTeX

## Inspiration
Have you ever finished up ECE lab or a small project like a PennApps hardware project and found that there were all of these screws, nuts, and resistors lying around everywhere? Or maybe you're just a hardware enthusiast but hate keeping track of all of your stuff (we like to do the old-fashioned way: the "hardware pile.")
With InvenTeX, there is finally an easy solution to inventory maintenance. Gone are the days of hoping you have quarter inch threads and settling for glue, or even having to remember the resistor color code. InvenTeX does all for your inventory, so you can focus your all on your hardware.

## What it does
InvenTeX identifies objects you want to keep in your inventory and helps you track them in groups so you don't have to.
The workflow of InvenTeX is simple.
When you first launch the app, you can choose to see what is in your inventory (at this point, nothing) and to insert something into the InvenTeX. 
####Taking a 100-ohm resistor as an example:
To insert, all you need to do is press the button on the app and take a picture of your resistor. It will then be identified and you can drop it and any other 100-ohm resistors into the loading bin. If you already have 100-ohm resistors stored in InvenTeX, your new resistor will be placed with the ones you already have in store.
To retrieve, you start from the drop down menu inside the app. There it will show everything that you have stored in InvenTeX. Pick the item you want to retrieve and InvenTeX will pop out the drawer with it. And once you're done grabbing what you need, the drawer will pull itself back in and you don't even need to remember where you got it from.

## How we built it
Initially, we wanted to use 3D printed parts for the hard to craft pieces of the enclosure and internal hardware. However, after finding out that we had access to laser cutters and an almost infinite supply of acrylic, we changed up our modeling plans and building timeline completely. We also initially intended on using a standard android app and using Bluetooth as the connection interface between it and the Raspberry Pi. And again, all plans changed with the introduction of Expo.io into the hack.
### Hardware
The entire enclosure and all of the internal structure is made with laser cut acrylic (1/8" and 1/4" black acrylic). We are also using two stepper motors to drive the belts and two servo motors to actuate the trap door (for part insertion) and magnetic arm (part retrieval).
### Controller
The Arduino Mega 2560 is the brain of the controls. The Arduino is only controlling movement and waiting for communication from the Raspberry Pi through I2C protocol. We are using an Adafruit Motor Shield (V2.3) for easy stepper and servo motor control with its built in stepper classes and PWM control for the servos. Everything draws power through a 5V external power source.
### Software
**Vision Processing**
Due to the Raspberry Pi camera not being clear enough to detect color bands of resistors, we switched over to phone cameras that had greater resolution (but also came with some other issues of connectivity, see below).
We started with simple color detection, differentiating between LEDs of different colors. Then moving on to shape and color detection, differentiating between different nuts and bolts. Finally moving on to pattern recognition and orientation detection, differentiating between resistors with different color bands.
#Communication#
Since we had replaced the Rapsberry Pi camera with the phone camera, we needed to transfer the image to the Raspberry Pi for vision processing. HTTP requests to send the image from the phone to the Raspberry Pi. HTTP requests to send retrieval signals to the Raspberry Pi.

## Challenges we ran into
###Software
**Vision processing:** Raspberry Pi camera is difficult to focus without a specialized tool. Resistor color codes had to be fine-tuned (HSV values). Resistor colors were also difficult to differentiate from each other and from the resistor itself, being heavily influenced by the color of the light source.

**Phone-to-Pi Communication:** We had to determine a system to send information to pi and get information back. We considered using bluetooth and an online database but ultimately decided to use HTTP protocols. Considerable thought went into organizing into which device ran what kind of processes and how to share information between the phone and pi. 

**Expo Development:** No experience with React-Native. Rendering code changes would sometimes hang, so app development was not as ideally productive. 
###Hardware
**Fitting Components:** Because all of the internal components of the enclosure must have slots and notches on the outer in order to fit properly, many of the internal structures that had to be swapped or structures that needed to be added in later for internal support had to be glued in making some areas less structurally stable than we would like.

**Stepper Motor Belt Tension:** The tension required for the stepper belts to drive properly was actually a bit too much for our structure to handle and some internal pieces broke off from the main frame due to tension forces. We fixed this by using zip ties instead of glue for a much stronger belt support structure.

## Accomplishments that we're proud of
**Integration:** We really had to utilize every possible piece of knowledge of every team member in order to fully integrate this project. There was (albeit simple) wiring to do be done with the microcontrollers, all the way up to implementing two different servers. In terms of code we wrote code in C, Python, and Javascript. We had to be able to pass information between all of our systems through wired and wireless connections. This extremely wide range of integration is not something we have ever done in so little time as a team and we're really excited that we pulled it off.

**Fully Assembled CAD Model:** We needed to know all of the tolerances in the system before printing or cutting any piece of it. In order to have everything fitted together as seamlessly as possible, we generated a fully assembled CAD model of every single component in the hack and cut almost every piece in one go. If we had completed the project by designing and cutting chunks at a time, without taking some time to look at the whole picture (literally) then there definitely would have been some more trips to the laser cutting room. We're glad that we took the time to fully CAD everything so that we could have all of the measurements and designs happening simultaneously between two CAD designers be integrated with each other.

## What we learned


## What's next for ToolHub
Bigger == Better. We want to try to use stronger materials with a stiffer frame that will not break down on us. We will also perform some stress analysis before redesigning some of the internal bracing in order for the structure to hold under the stepper torque.
The vision processing algorithms and implementation could also always be snappier and more robust.
