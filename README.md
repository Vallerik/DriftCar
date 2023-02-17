DriftCar
====
![DriftCar](https://github.com/Vallerik/DriftCar/blob/main/image/1.jpg)
In this project, I showed the completion of a drift car with a complete alteration of electronics and the addition of lighting and music.
What a drift machine is initially capable of:
- The charge of the native battery is enough for 10 minutes, after 7 minutes the car just drives without drifting;
- Discrete control;
- Police lighting;
- Drift around;
- Drift eight.

[Video review of drift cars without modifications.](https://youtu.be/6MPHbPOtivM)

What do I want to do?
- Make front and rear lights;
- Introduce completely your own homemade electronics with proportional steering and gas control;
- Write a program for the typewriter and remote control;
- Put 3S battery;
- Add music to the machine with the ability to switch it from the remote control;
- Make the backlight on the addressable LEDs with the ability to switch it from the remote control;
- Servo steering wheel control.

[Video about drift cars with all the modifications.](https://youtu.be/wygo5vjzIAw)

Required components
====
For electronics, you will need the following components:

For the machine:
- Arduino Nano - 1 piece;
- Servo MG90D - 1 piece;
- HC-12 - 1 piece;
- Board with addressable LEDs - 2 pieces;
- Wires;
- Player DY-SV5W - 1 piece;
- microSD memory card - 1 piece;
- Speaker AIYIMA 5W 4 om - 1 piece;
- Racks for a column - 2 pieces;
- Hardware for the column and brass racks;
- LEDs white 5 mm - 2 pieces;
- LEDs white 3 mm - 4 pieces;
- LEDs red 5mm - 2 pieces;
- Resistors 100 ohm - 6 pcs, 150 ohm - 2 pcs, 200 ohm - 1 pc, 10 kOhm - 1 pc;
- Connectors XH 2.54 3P female - 4 pcs;
- Connectors XH 2.54 3P male - 4 pcs;
- Connector JST SM 2.54 2P female - 1 piece;
- Printed circuit boards.

For remote control:
- Arduino Nano - 1 piece;
- HC-12 - 1 piece;
- Charging board TP4056 with step-up DC-DC converter - 1 piece;
- CJMCU-103 - 2 pieces;
- Two-color LED with a common cathode - 1 pc;
- Resistors 150 ohm - 1 piece, 160 ohm - 1 piece;
- DIP Buttons - 2 pieces;
- Wires;
- Connector JST SM 2.54 2P female - 1 piece;
- Breadboard - 1 piece;
- LiPo 400mA - 1 piece.

For battery:
- Li-ion NCR 18650 with wires - 3 pieces;
- BMS 3S with balancing - 1 piece;
- Connector JST SM 2.54 2P male - 1 piece;
- Wires.

Revision history
====
I started reworking the machine by disassembling and studying its device. I took off all the native electronics with the steering mechanism and began to think about how and where to place all the components.
In this project, I have identified two difficult places for rework:
1. Steering.
I have a rough idea of how the steering should work on a servo, but how to place it and will the small servo have enough power? For me, these were the main questions and fears.
2. Alteration of the remote control.
Since the native equipment of the drift car has discrete control, it will be difficult to convert it to proportional control, according to my feelings, because you need to somehow build variable resistors into the remote control,
and what if it doesn't work? Well, as they say, the eyes are afraid, but the hands do!

It is very important in projects to find difficult places or things that cause the most problems, these things need to be done first and after solving all the difficult tasks, do everything else.
It would be very stupid to abandon the project at an almost finished stage, when all the simplest things have already been done, but you cannot complete the project because of 2 difficult tasks that you decided to postpone.
As a result, you get an abandoned project that needs to be redone or finalized.

After studying the device, I had several options for the location of the components, first of all, I tested the servo and decided on its power.
I thought over the steering mechanism, the location of the servo drive and the device of the control panel. In order to finally decide on the device of the machine and the remote control, then it is necessary to prepare the electronics.
I wrote a program in the Arduino IDE, assembled and debugged the prototype, after that I made a circuit diagram of the electronics of the machine and the remote control.


The device of the new steering mechanism
====
![The device of the new steering mechanism](https://github.com/Vallerik/DriftCar/blob/main/image/2.jpg)
The ideal place to fix the servo coincides with the plastic stand in the case. The second ear of the servo is already screwed to the brass stand in place. 
In the central position of the servo, a rocker with a drilled groove is screwed. A curved pin glued into the steering rack is inserted into this groove. 
The pin is made from the original spring of the steering gear, teeth for the gear were bitten off from the rack itself.
And the only important point, you need to install the rocker in the central position of the servo (90°) and the servo itself is screwed through a spacer printed on a 3D printer. 
The servo drive, turning the rocker, moves the pin along the groove, thereby setting the steering rack in motion. The mechanism works perfectly, 
there is enough servo power, there is no jamming, and I was even surprised how well the arrangement of the components coincided.


Location of electronics
====
In the compartment for the old battery, I screwed the board from the player onto the plastic racks, and fastened the speed controller to the plastic ties.

![Location of electronics](https://github.com/Vallerik/DriftCar/blob/main/image/3.jpg)

By the way, this regulator needs to be improved to prevent its heating when powered by a 3S battery. The fact is that this speed controller, according to the seller, only supports a 2S battery.
When a 3S battery is connected, the regulator also works, but the diode and transistor start to heat up, I dropped these components, the printed circuit board stopped heating and the regulator works properly.
All components on the PCB support voltages above 12 volts, so everything is fine.

![Location of electronics](https://github.com/Vallerik/DriftCar/blob/main/image/4.jpg)

Under the central cover in the middle, there is a board with an Arduino Nano, after which a power board is installed and panels with addressable LEDs are glued on the sides.
The native switch in the front is used to turn off the power, some holes were also made in the case for wires, well, the whole bunch of wires, if possible, was fastened with plastic ties.

![Location of electronics](https://github.com/Vallerik/DriftCar/blob/main/image/5.jpg)

All electronics are connected according to this scheme:
![Location of electronics](https://github.com/Vallerik/DriftCar/blob/main/principal%20connection%20diagrams/arduino.jpg)

Printed circuit boards
====

![Printed circuit boards](https://github.com/Vallerik/DriftCar/blob/main/image/6.jpg)
In order not to suffer like me with the manufacture of boards on a breadboard, I recommend ordering the manufacture of printed circuit boards from [JLC PCB](https://jlcpcb.com/RHS). 
This is the largest enterprise for the production of printed circuit boards of industrial quality, of any complexity, of any size.
For just $2 plus shipping, [JLC PCB](https://jlcpcb.com/RHS) will make you five 2-layer 100x100mm PCBs, or five 4-layer 50x50mm PCBs! [JLC PCB](https://jlcpcb.com/RHS) can not only manufacture boards, 
but also assemble them on their surface mount equipment. To place an order, you need to upload an archive with gerber files to the site, in the order itself you can configure the necessary parameters, 
such as the color of the mask, the thickness of the textolite, copper and order the assembly of boards, also [JLC PCB](https://jlcpcb.com/RHS) is engaged in 3D printing, 
making stencils for SMT mounting and all this makes in the shortest possible time and with the best quality, so I recommend them.
I made all the listed printed circuit boards from breadboards, soldering the tracks with solder. 
This method of manufacturing printed circuit boards is not always suitable and it is quite difficult to think through the passage of all the tracks, 
and the soldering process itself is complicated if you want to do it neatly and beautifully. Therefore, I recommend ordering printed circuit boards in production, do not suffer, 
do not repeat my mistakes :)

[https://jlcpcb.com/](https://jlcpcb.com/RHS)

GitHub link to download PCB gerber file: 
https://github.com/Vallerik/DriftCar/tree/main/gerber

Battery assembly
====
![Printed circuit boards](https://github.com/Vallerik/DriftCar/blob/main/image/7.jpg)
There is nothing complicated in assembling the battery, I think everything is clear just from the photo.
The wires of each can of 18650 are soldered to the corresponding BMS pin of the board, as in this photo.
To connect the battery, a wire with a JST SM 2.54 2P male connector is soldered to the BMS.


Headlights
====
Holes are drilled on the body in the center of all the painted headlights and LEDs are glued with hot glue. 
### Beforehand, it is necessary to solder the wires to the LEDs according to the scheme:
![Headlights](https://github.com/Vallerik/DriftCar/blob/main/principal%20connection%20diagrams/headlight%20connection.jpg)

The headlights are made from 3 and 5mm white LEDs. Reversing lights made of white 3mm LEDs. The oversized red headlights are made from matte red 5mm LEDs.
The wires from the headlights will be connected to the XH 2.54 3P connectors (Each wire is connected according to its color mark), 
printed circuit boards with these connectors are screwed to the side of the central cover of the machine.

![Headlights](https://github.com/Vallerik/DriftCar/blob/main/image/8.jpg)
Racks with a speaker are screwed to the same cover and the battery is attached to plastic ties.


Remote control modification
====
![Project results](https://github.com/Vallerik/DriftCar/blob/main/image/9.jpg)
To remake the control panel, the first thing I thought about was how to install the CJMCU-103 modules. To do this, I printed on a 3D printer a special mount and pins for turning the CJMCU-103, 
these pins are glued into the trigger and steering shaft. The first CJMCU-103 steering wheel module is screwed to the mount, and the second CJMCU-103 module is screwed to the control box cover.
Also, for the remote control, two printed circuit boards were cut out for the buttons, a two-color LED was glued in, a switch and a resistor for setting the charging current was changed on the green charging module, 
for a li-po battery with a capacity of 400 mA, I set the charging current to 300 mA. Charging takes place with a USB cable that plugs into a 5.5x2.1 mm jack.
There is nothing else special in the remote control, and well, I also changed my native antenna to a small one printed on a 3D printer. In general, the remote turned out great!
![Project results](https://github.com/Vallerik/DriftCar/blob/main/principal%20connection%20diagrams/remote%20controller.jpg)

Project results
====
Now let me summarize, has the car started to drift better? The result, of course, was not the same as I expected, but the car became more cheerful to drive from a 3S battery, due to the proportional control, 
the car became better controlled and, of course, it is better to look with headlights and RGB backlight. A loud speaker makes the bass vibrate and play the whole body, 
these are indescribable sensations of the joy of a working project. My main goal is accomplished, to make a musical drift car with my homemade electronics. From the remote control, 
I can turn on, switch music and backlight, the backlight still has 7 modes of operation, well, in general, I'm exhausted, but satisfied. I will not say that I am an expert in this matter, 
I just set a goal for myself and achieved it. And in this project, I shared my experience and, as a result, I want to warn you that developing your own electronics and modifying such toys is not as easy as it seems. 
If you want to repeat the project after me or do something similar, I would advise you to install ready-made electronics, a receiver, a speed controller, a purchased battery with equipment, 
it will come out easier, cheaper and faster.


Links to other important sources
------------
* [Thingiverse](https://www.thingiverse.com/yourobotics/designs) 3D Model for time-lapse photography.
* [VK group](https://vk.com/yourobotics) My group on the social network "Vkontakte".
* [YouRobotics](https://www.youtube.com/channel/UCt6Rz9BVB6opYcUrAq6mwWA) My YouTube channel.
------------
Made by: Valery Zalit