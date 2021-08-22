![Prototype](https://github.com/bluepylons/Open-Switch-Curve-Meter/blob/main/Pics/Prototype-1.jpg?raw=true)
![Prototype vertical](https://github.com/bluepylons/Open-Switch-Curve-Meter/blob/main/Pics/Prototype-1-vertical.jpg?raw=true)

# About 
This is an open-source keyboard switch force-curve tester for MX-style switches I'm working on. This is currently operational, and force curves of various switches I've measure are in the Force curve\ directory.

This was designed specifically for MX switches to simplify the design and make everything cheaper and more compact, and since most of the switches I am curious about (as well as almost all new switches released today) use the MX form factor. In theory it could be adapted to test say, Alps or other discrete switches (it would need a different plate and switch holder). This is not as versatile as force-curve testers built around a test stand (like [the one Haata built](https://deskthority.net/viewtopic.php?t=15133)) and can't test arbitrary keyboards like that one, but it's much cheaper to build (I spent about $200 building this, and in theory this is buildable for ~$100 if one foregoes the custom PCB and order the parts from China)

This project is licensed under the CERN OHL-S (strongly reciprocal) license for the hardware designs and the software. This is a share-alike license, and if you modify the project files you are required to also open-source your modifications. 

The force curves themselves - the .ODS spreadsheets and the .PNG images of the graphs in the Force Curves folder - are licensed under Creative Commons CC-BY 4.0. 

# The key parts:
* A linear stage. I designed around [this inexpensive $55 linear stage off Amazon](https://www.amazon.com/Sliding-Precision-Stepper-T-Shaped-Electric/dp/B07QBCG9YC/ref=sr_1_5?dchild=1&keywords=50mm%2Blinear%2Bstage&qid=1615263982&sr=8-5&th=1). You can find what appear to be identical units [on Aliexpress](https://www.aliexpress.com/item/4001272086575.html?spm=a2g0o.productlist.0.0.61906e9d3wWNyG&algo_pvid=04df7b52-bc18-4d1c-b806-9cd889c3acae&algo_expid=04df7b52-bc18-4d1c-b806-9cd889c3acae-12&btsid=0bb0623916170292735655874e869e&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_) and elsewhere for less (if you get them from Aliexpress, make sure you buy the model with 1mm pitch (lead) on the leadscrew). 
* A stepper motor. The stepper motor serves two purposes - it moves the linear stage, and also keeps track of position to plot the X-axis of the force curve. Stepper motors, as long as they aren't stalled or skip steps, move a given amount based on the number of step commands sent, and lets me forego having to directly measure position. Instead, I can just count the number of steps sent to the stepper motor, since I know how much the stepper motor will move for a given number of steps. I used the included stepper motor in the linear stage above, which moves 1.8° per step (200 steps per full rotation). Combined with the 1mm lead leadscrew on the linear stage, a single step (without microstepping) in theory moves 0.005mm. 
* A stepper motor driver. I used a Trinamic TMC2209, but it's overkill for this project. Since I'm using simple DIR/STEP control any inexpensive stepper motor controller or Arduino stepper motor shield should work. 
* A load cell, to measure the force on the switch. I used [this 500g unit](https://www.sparkfun.com/products/14728) from Sparkfun, but you can find these for much cheaper on Aliexpress and elsewhere.
* A load cell amplifier and  ADC. This is for reading the load cell. It is not strictly necessary if you have a nice microcontroller with a decent ADC (e.g. a Teensy 3.x), but it does make life easier with cheaper microcontrollers, and in any case gets you better measurements. I used Avia's HX711 as [Sparkfun made a module for it](https://www.sparkfun.com/products/13879), and there is an Arduino library available for it. 
* A microcontroller. I used an Arduino Nano clone as I had one lying around, and since they're inexpensive.

I also threw on an RGBLED and two MX switches for status indication and control. They ended up being extremely useful for debugging and feature testing while writing the firmware.

The CAD was done in Atom3D. STEP files are provided. All fasteners used are M3x0.5.  STLs are provided for the 3D printed parts. 

For convenience I ended up putting the TMC2209 stepper motor controller and HX711 load cell amplifier/ADC onto a custom PCB that I designed in KiCAD, for which the file are available. The PCBs were done in KiCAD's 5.99 nightly build. A PCB is not necessary though - you can build this on perfboard, as off-the-shelf HX711 modules are available from [Sparkfun](https://www.sparkfun.com/products/13879) and elsewhere, and you can buy the TMC2209 [on a breadboard-friendly breakout board](https://www.digikey.com/en/products/detail/trinamic-motion-control-gmbh/TMC2209-SILENTSTEPSTICK/10232488). 

The HX711 is powered off the Arduino's 5V. The stepper motor is powered off a separate external 12V power supply, though some steppers might work off 5V, and there is a solderable jumper if you want to try powering the stepper off the Arduino's 5V. Having the stepper powered separately is useful though, as I can do dry-run tests on the code with the power supply disconnected (preventing motor movement), and I can also disconnect the power supply if I need to stop the stepper for some reason.

The switch is mounted on a small switchplate. On this one I made it out of FR4 so I could get it made at the PCB fab, but I provided a DXF file if you want it laser-cut. There is a second PCB that connects the switch to a hotswap socket, for measuring the switch's actuation point.

The firmware initiates a test cycle when you press the left button (light goes from white to green). You can move the switch towards the load cell with the right button before the test cycle. Data is sent via Arduino Serial Monitor - you can then copy/paste the data into a spreadsheet, process the data, and graph it to generate a curve. The switch is moved towards the load cell as long as the force reading is <120gf (this limit is configurable), and automatically reverses when that value is exceeded. When the reversal happens, the LED goes from green to red. The stepper motor drives the stage back to start position, and is yellow when everything is done. At the end of the test cycle, you can press the right button to move the switch away from the load cell.

# How this Github repo is organized:

* CAD (Atom3D)/ is the mechanical CAD files. I use Atom3D (which is an inexpensive CAD program that no one really uses) and so the files are in that format, but I also include a STEP assembly file that should be openable in other CAD programs. There's subfolders for the PCB assembly mechanical CAD and the hotswap board mechanical CAD, a 3D model of an MX switch, and STL files for 3D printing. 
* Firmware/ is the firmware that runs on the Arduino on the PCB. There's one script (Switch-tester-firmware) that runs on the Arduino that runs the test cycle, and another script (basically the same script as in the HX711 library) for calibrating the load cell. 
* Force curves/ is where the end data is published. There's the big README file with PNG images and text, .ODS spreadsheet files (for Libreoffice) which contains the raw and processed data, and the actual PNG image files themselves. They're organized by calibration date, though that no longer makes sense as I've been calibrating more often now as I've noticed the load cell drifts a fair amount, and as more switches get added I may reorganize in terms of switch manufacturer or type. The earlier measurements also had cruder methodologies for processing the data (e.g. I didn't tare the load cell until mid-April and accepted up to ~1gf in zero offsets before recalibrating). I've also improved the PNG graphics over time. There's also a blank spreadsheet template I use for making the graphs that I copy-paste the data into.
* PCB files/ contain the KiCAD files for the main controller PCB (which holds the Arduino Nano and contains a stepper motor driver and a load cell amplifier), as well as the FR4 switchplate files and the PCB that holds the hotswap socket for the switch. There's also the KiCAD library files that those PCB files rely on, including the Acheron library. You will need to use the nightly version of KiCAD (the 5.99 versions) to open the KiCAD files. 
* Pics/ is just pictures of the force curve meter itself for the main page README

Calibration.xlsx (in the root directory) is an Excel spreadsheet I use to calibrate the load cell on a regular basis. I do a simple two-point linear calibration, by taking the raw ADC readings at 0gf (i.e. with nothing on the load cell), and at -100gf with a 100g calibration weight placed on the load cell, to determine the linear equation (y = mx + b) that converts the raw ADC reading into grams. While a 2-point calibration is a bit crude, the [published linearity specs](https://cdn.sparkfun.com/assets/9/9/a/f/3/TAL221.pdf) for the load cell is ±0.25gf (0.05% full-scale) over its entire ±500gf measurement range, and a 2-point calibration is much faster than a multipoint calibration.

# To do list

* Hardware
	* Design a 3D-printed case for the PCB (eventually....)
	* The HX711 load-cell amplifier occasionally drops out for a step or two, resulting in a -207gf reading, or "HX711 is not available" if I'm running the calibration script. This happens on occasion on mine (roughly once every 10 measurements), but I've talked to someone else using the HX711 module and they said it never happens to them. If the error occurs during a measurement I will generally re-do the measurement cycle, so it is a nuisance, but not a high priority one.

* Firmware 
	* Make a state diagram of the firmware code

* Various near-term content (planned) 
	* Make a video on the process as to how I'm measuring these - I have the video shot, but need to edit it. 
	* Write a guide on how to build one of these using off-the-shelf HX711 load cell amplifier and stepper motor modules. The PCB was done mostly for my own convenience, and it's possible to do it using off-the-shelf modules and a breadboard, though it's somewhat messier wiring wise. 
	* Write something up about sources of error in load cells (drift, temperature, etc.)

* Possible long-term improvements:
	* Automate the data processing and graph generation
		* The current data processing workflow involves copying the data from Arduino Serial Monitor over to Libreoffice Calc (a spreadsheet program), correcting any zero offsets, determining the start, bottom out, and end points, marking the actuation and release points, creating the plot, placing the actuation and release point markers in a spot where it doesn't interfere with the curve, exporting the plot, adding any text info to the plot (switch name, any notes about the switch e.g. if it's lubed, tolerances, credits if someone sent me the switch, etc.) in a suitable location, and adding the image to the Force curves/README doc. This takes 5-15 minutes per measurement. A lot of this can definitely be automated. However, my current Python and general scripting skills however are poor and this will take a lot of testing, so this is unlikely to be done in the near-term
	* Better hosting of graphs - possibly moving off Github
		* There's something slowly in the works with various other people that currently possess force curve meters, but it will be some time before it will be up
		* We'd want something more interactive such as Plotly (which no longer takes new uploads), and something that can allow for easy comparisons between switches (e.g. overlaying graphs)
	* Improve the analog stage for reading the load cell, which would enable much faster measurements 
		* The load-cell is currently read with an Avia HX711 chip, which incorporates an amplifier and a 24-bit ADC, and reads the sensitive low-level analog signal from the load cell and converts it into a digital reading for the Arduino (voltage changes on a load cell is on the order of single millivolts, and often fractions thereof). However, the HX711 is slow (~10Hz sampling rate), which leads to a fairly slow measurement cycle if you want good resolution - with 0.005mm steps as I measure right now, it takes about 3 minutes to measure the downstroke and upstroke of a 4mm travel switch, as the HX711 limits me to 10 measurements per second (and therefore 10 steps per second). The measurement can be done much faster if the analog stage was faster. I'm thinking of building a dedicated analog amplifier stage (using a nice op-amp circuit or other amplifier IC), and reading the analog signal using the ADC on a nice microcontroller such as a Teensy 3.2 (which has a 16-bit ADC that can sample above 100KHz). This would allow for measurement times to go down to a couple of seconds, and would save me a fair amount of time. 
		* Faster measurements could also impact the actual measurements - HaaTa pointed out that measurement speed likely results in slightly different curves due to differences between static and kinetic friction, and that slow or step-wise force-curve measurements (as how I am doing them with this current force curve meter) means that static friction plays more of a role. Considering that most people press switches fairly quickly when typing, and no one takes 90 seconds to press a switch when down they type, a fast measurement would produce a force curve that better reflects how a switch behaves in actual use. 
	* Some way to correct for temperature-induced drift on the load cell that isn't just calibrating frequently 
		* I'm fairly annoyed by how much the load cell readings drift with temperature (simply opening a window often forces me to recalibrate), and I am considering adding additional load cell modules solely to do temperature correction (you can cancel out the temperature-dependent terms in the load cell equation if you use multiple identical load cells)/strain gauges, or doing temperature measurements and using a look up table to apply corrections or something. 
	
# Errata
* I mistakenly connected Pin 1 and Pin 2 on the Arduino on the PCB - those are the TX/RX lines, and I learned that having those connected interferes with communications between the Arduino and your computer,  and may lead to failures when uploading sketches and trying to use Serial Monitor. These lines were originally intended for uART communication with the TMC2209, but I decided to just use DIR/STEP control as it's easier to write the code. Either cut the headers off the Arduino, or do not install those pins on the socket on the PCB. 


# Thanks yous: 
* The Switchmodder [subreddit](https://www.reddit.com/r/switchmodders/) and [Discord](https://discord.gg/xnSUeETRTf)
* Nebulant and xyz for advice
* Romly, for [posting photos of their tester](https://twitter.com/Romly/status/1358983963377688579)  
* Haata, for his [incredibly useful writeup](https://deskthority.net/viewtopic.php?t=15133) on building his switch tester. 
* Gondolindrim and the [Acheron Project](http://acheronproject.com/), as I use their hotswap PCB footprint and the 3D model of the MX switch. Their open-source projects have also been very helpful for me getting into keyboard design. 
* Thank you to the following people for sending me switches to make curves of:
	* Slowshi 
	* Window Dump
	* mfchill
	* Clark
	* Aiwanei 
	* ChromePcok
	* Deadeye 
	* [Rebult Keyboards](https://www.rebultkeyboards.com/)
	* [Dangkeebs](https://dangkeebs.com/)
	
# Links to other people with force curve meter designs and data:

* Ruddy, who's affiliated with [GeekBoards.ru](https://geekboards.ru/), independently developed a similar force curve meter and open-sourced his design on Github [here](https://github.com/geekboards/switch-force-tester). His measurements are [here](https://github.com/geekboards/switch-force-tester/tree/main/python-app/results)

* HaaTa, who founded [Input Club](https://input.club/), and well known for his force curve measurements. His design is on Github [here](https://github.com/kiibohd/force) and described on Deskthority [here](https://deskthority.net/viewtopic.php?f=62&start=&t=15133), and results are on Plotly [here](https://chart-studio.plotly.com/~haata#/)

* Nebulant and ManOfInterests also possess force curve meters, though I am not aware of any centralized location they publish their measurements. You can find their graphs on their Instagrams, across various Discord servers, Twitter, and on Keebtalk. 