![Gen 2 Prototype](https://github.com/bluepylons/Open-Switch-Curve-Meter/blob/main/Pics/Gen-2-front.JPG?raw=true)

# About 
This is an open-source, low-cost measurement device for generating force curve measurements for MX-style switches. There are two versions - a much older Gen 1 and a newer Gen 2 design.  Force curves of various switches I've measured are in the Force curve measurements\ directory. Detailed documentation for Gen 2 is in progress and will be available [here](https://github.com/bluepylons/Open-Switch-Curve-Meter/blob/main/Gen%202%20Design/README.md).

This was designed specifically for MX switches to simplify the design and make everything cheaper and more compact, and since most of the switches I am curious about (as well as almost all new switches released today) use the MX form factor. With different switch holders it can hold other discrete switches - it was used to measure a Futaba switch, and could theoretically be used to measure Alps and other discrete swithces as well. This is not as versatile as force-curve testers built around a test stand (like [the one Haata built](https://deskthority.net/viewtopic.php?t=15133)) and can't test arbitrary keyboards like that one, but it's much cheaper to build (I spent about $200 building this, and in theory this is buildable for ~$100 if one foregoes the custom PCB and order the parts from China).

This project is licensed under the CERN OHL-S (strongly reciprocal) license for the hardware designs and the software. This is a share-alike license, and if you modify the project files you are required to also open-source your modifications. 

The force curve graphs themselves - the .ODS spreadsheets and the .PNG images of the graphs in the Force curve measurements\ folder - are licensed under [Creative Commons CC-BY 4.0](https://creativecommons.org/licenses/by/4.0/deed.en). 

# The key parts
* A linear stage. I designed around [this inexpensive $55 linear stage off Amazon](https://www.amazon.com/Sliding-Precision-Stepper-T-Shaped-Electric/dp/B07QBCG9YC/ref=sr_1_5?dchild=1&keywords=50mm%2Blinear%2Bstage&qid=1615263982&sr=8-5&th=1). You can find what appear to be identical units [on Aliexpress](https://www.aliexpress.com/item/4001272086575.html?spm=a2g0o.productlist.0.0.61906e9d3wWNyG&algo_pvid=04df7b52-bc18-4d1c-b806-9cd889c3acae&algo_expid=04df7b52-bc18-4d1c-b806-9cd889c3acae-12&btsid=0bb0623916170292735655874e869e&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_) and elsewhere for less (if you get them from Aliexpress, make sure you buy the model with 1mm pitch (lead) on the leadscrew). Gen 2 will use the same linear stage. 
* A stepper motor. The stepper motor serves two purposes - it moves the linear stage, and also keeps track of position to plot the X-axis of the force curve. Stepper motors, as long as they aren't stalled or skip steps, move a given amount based on the number of step commands sent, and lets me forego having to directly measure position. Instead, I can just count the number of steps sent to the stepper motor, since I know how much the stepper motor will move for a given number of steps. I used the included stepper motor in the linear stage above, which moves 1.8° per step (200 steps per full rotation). Combined with the 1mm lead leadscrew on the linear stage, a single step (without microstepping) moves the switch 0.005mm. 
* A stepper motor driver. I used a Trinamic TMC2209 on Gen 1, and a Trinamic TMC2208 module on Gen 2. Communication occurs using rudimentary STEP/DIR control.  
* A load cell, to measure the force on the switch. I used [this 500g load cell](https://www.sparkfun.com/products/14728) from Sparkfun. It can be found on Aliexpress for slightly less. 
* A load cell amplifier and  ADC for reading the load cell and converting the readinging into a digital signal that can be easily read by the microcontroller. I used Avia's HX711 on both Gen 1 and Gen 2 as [Sparkfun made a module for it](https://www.sparkfun.com/products/13879), and there is an Arduino library available for it. Gen 2 has provisions for SPI and I2C for potential future upgrades with faster and more expensive ADCs (e.g. Analog Devices AD7730).
* A microcontroller. I used an Arduino Nano clone on Gen 1 and Gen 2 as I had one lying around and it's straightforward to write code for. My initial plans for Gen 2 involved using [Adafruit's Feather RP2040](https://www.adafruit.com/product/4884), though I have not had time to learn its programming language yet so I decided to spin another PCB for an Arduino Nano. 
* Software to transfer the information over to a computer. Right now, I use an Arduino 1.8.19's serial monitor to transfer the information over from the Arduino Nano clone. It's copied into a .csv file using Notepad. Cleanup of the data can be done either manually in a spreadsheet program (I use LibreOffice Calc), or can be done using [a Python script I wrote up](https://github.com/bluepylons/Open-Switch-Curve-Meter/tree/main/Processing-Python-script). Plotting is currently done with Libreoffice Calc.

There is also an RGBLED for status indication, and MX switches (2 on Gen 1, 4 on Gen 2) for control. 

The CAD was done in Alibre Atom3D. STEP files are provided. 

Gen 1 and Gen 2 both have custom PCBs, designed in KiCAD and fabbed by JLCPCB. You will need KiCAD to open the files. In theory, a PCB is not necessary - you can build this on perfboard, as off-the-shelf HX711 modules are available from [Sparkfun](https://www.sparkfun.com/products/13879) and elsewhere, and you can steper motor drivers [on breadboard-friendly breakout board](https://www.digikey.com/en/products/detail/trinamic-motion-control-gmbh/TMC2209-SILENTSTEPSTICK/10232488). However, it makes things a lot neater. 

The HX711 is powered off the microcontroller's 5V supply. The stepper motor is powered off a separate external 12V power supply.

# To do list (Gen 2)

* Documentation
	* Just like, everything
	* I'll make a video of me operating the force curve meter at some pointed
	* Write a guide on how to build one of these using off-the-shelf HX711 load cell amplifier and stepper motor modules. The PCB was done mostly for my own convenience, and it's possible to do it using off-the-shelf modules and a breadboard, though it's somewhat messier wiring wise. 
	
* Hardware
	* (eventually) Design a 3D-printed case for the PCB so that it's not exposed

* Firmware
	* Gen 2's RGBLED does not changes states like I expect to at the end of the run, so I need to debug this
	* (eventually) Write a more user-friendly UI for processing the data (the current workflow, which involves a lot of spreadsheets and copy/pasting data from Arduino Serial Monitor for both calibration and measurement, is tedious)

* Possible long-term improvements:
	* Better hosting of graphs - possibly moving off Github
		* There's something slowly in the works with various other people that currently possess force curve meters, but it will be some time before it will be up
		* We'd want something more interactive such as Plotly (which no longer takes new uploads), and something that can allow for easy comparisons between switches (e.g. overlaying graphs)
	* Some way to correct for temperature-induced drift on the load cell or amp that isn't just calibrating frequently 
		* I'm fairly annoyed by how much the load cell readings drift with temperature (simply opening a window often forces me to recalibrate), and I am considering adding additional load cell modules solely to do temperature correction (you can cancel out the temperature-dependent terms in the load cell equation if you use multiple identical load cells)/strain gauges, or doing temperature measurements and using a look up table to apply corrections or something. 
	* Possibly move away from the HX711	
		* There's a lot of sample variation among HX711's I've encountered, with some having a lot of drift and others mostly working fine. 
	
# Gen 1 Errata
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
	* [Moyu.studio](https://moyustudio.cn/)
	* [Adobau Labs](https://adobaulabs.com/)
	* [Prevail Key Co.](https://prevailkeyco.com/)
	* Mechanicalbionicle
	* Nick7790
	* [JLabs](https://jlabs.co/)
	* Laika
	* notdoctorq
	* [Switch Oddities](https://switchoddities.com/)
	
# Links to other people with force curve meter designs and data:

* Ruddy, who's affiliated with [GeekBoards.ru](https://geekboards.ru/), independently developed a similar force curve meter and open-sourced his design on Github [here](https://github.com/geekboards/switch-force-tester). His measurements are [here](https://github.com/geekboards/switch-force-tester/tree/main/python-app/results)

* HaaTa, who founded [Input Club](https://input.club/), and well known for his force curve measurements. His design is on Github [here](https://github.com/kiibohd/force) and described on Deskthority [here](https://deskthority.net/viewtopic.php?f=62&start=&t=15133), and results are on Plotly [here](https://chart-studio.plotly.com/~haata#/)

* Nebulant and ManOfInterests also possess force curve meters, though I am not aware of any centralized location they publish their measurements. You can find their graphs on their Instagrams, across various Discord servers, Twitter, and on Keebtalk. 

* Romly built a force curve meter over in Japan. [Details here](https://romly.com/blog/c99_jikunohakari/).

* Theremingoat has a force curve meter and now has a [massive respository](https://github.com/ThereminGoat/force-curves).
