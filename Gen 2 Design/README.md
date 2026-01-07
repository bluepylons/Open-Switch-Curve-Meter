# Work In Progress

**Order parts at your own risk. These files are provided as-is, and I am not responsible for any losses, harm, or damage incurred by following this guide, by ordering parts, by using the files provided, by using the resultant parts or assembled devices, or from any mistakes or errors in the files, ordering guide, or resultant parts.**

# Gen 2 

Due to the general jankiness of Gen 1, I decided to do a much neater redesign for Gen 2 back in 2021. I initially thought about [running a group buy](https://geekhack.org/index.php?topic=115453.0), though life has gotten in the way of that. I finally had some time to pick this project back up in late 2025, and am currently documenting the Gen 2 design to the point it can be easily replicated by other people. This document is very much a work in progress, though I have built and operated Gen 2. 

Mechanical CAD is done in [Alibre Atom3D](https://www.alibre.com/atom3d/), and the PCB files are done using KiCAD V9.

# ORDERING GUIDE

# Mechanicals

## Laser cut metal parts

The following parts are laser-cut out of metal. DXFs are provided [here](https://github.com/bluepylons/Open-Switch-Curve-Meter/tree/main/Gen%202%20Design/Mechanical%20files/Laser%20cut). STEP files are provided as well.

| Part name                | Material thickness         | Quantity |
| ------------------------ | -------------------------- | -------- |
| Baseplate-stand          | 3mm-3.2mm (0.118-125")     | 1        |
| Bumpon-holder            | 1.5mm-1.6mm (0.059-0.063") | 4        |
| Load-cell-mounting-plate | 3mm-3.2mm (0.118-125")     | 1        |
| Switch-plate             | 1.5mm-1.6mm (0.059-0.063") | 1        |
| Vertical plate           | 3mm-3.2mm (0.118-0.125")   | 1        |

Aluminum is generally recommended for most of the parts, though other metals (steel, etc.) can work. For the baseplate stand, steel, brass, or stainless steel may be useful as the additional weight can provide more stability. All the parts on the ones I have built are cut out of aluminum 6061-T6.

Aluminum or plain steel are usually the cheapest materials. Note that stay may corrode or rust without treatment (e.g. painting, powder coating). 

In the US, I recommend [Sendcutsend](https://sendcutsend.com/). In the EU, [Laserboost](https://www.laserboost.com/) is a good source. 

For Sendcutsend, deburring is recommended, though it still tends to leave a sharp edge and may require manual deburring afterwards (with a deburring tool such as [this](https://www.mcmaster.com/4289A71/)). [Tumbling](https://sendcutsend.com/services/tumbling/) is recommended for eligible parts as it softens the edges.

## T-slotted extrusions and hardware 

I use Misumi 80/20-style extrusions, though tther brands of 20mm 80/20-style T-slotted extrusions likely work. The extrusions are tapped M5x0.8 on each end (Misumi will do this for you; the part number specifies it). Note that nuts for other 2020 (such as McMaster or actual 80/20(R) hardware) are often incompatible as the slot opening on Misumi extrusions is a bit smaller. Be aware of this if ordering fasteners. 

| Misumi Part Number       | Description | Quantity |
| ------------------------ | ----------- | -------- |
| HFS5-2020-132-TPW        | Extrusion   | 1        |
| HNTAP5-5                 | M5 nuts     | 4        |

## 3D-printed pieces
The following parts are 3D-printed. STLs are available [here](https://github.com/bluepylons/Open-Switch-Curve-Meter/tree/main/Gen%202%20Design/Mechanical%20files/3DP).

| Part name                    | Quantity |
| ---------------------------  | -------- |
| Switch mounting bracket      | 1        |
| load-cell protection bracket | 1        |
| ADC-PCB-spacer               | 1        |
| Main-PCB-spacer              | 1        |

I have generally printed these out of PETG, though other materials such as ABS and PLA are likely fine. 

## Linear actuator 

The design uses a cheap Chinese 50mm linear actuator with a 1mm pitch leadscrew. There's no exact part number for this. I ordered [this one](https://www.amazon.com/Sliding-Precision-Stepper-T-Shaped-Electric/dp/B07QBCG9YC/ref=sr_1_5?th=1) off Amazon; it is no longer available. However, [this], [this], [this], and [this] are likely compatible. [This](https://www.amazon.com/Linear-Actuator-Stepper-Milling-Machine/dp/B07K7FWDZJ/ref=sr_1_8) is likely compatible.

## Hardware

The following fasteners are needed:

| Description                               | Quantity | McMaster-Carr part number | Notes                                                     | 
| ----------------------------------------- | -------- | ------------------------- | --------------------------------------------------------- |
| M3x5 ISO 14583 Torx !0 screw              | 16       | 90362A112                 | ISO 7380 button-head (McMaster 90991A111) would also work | 
| M3x8 screw (ISO 7380 Torx T10 used)       | 4        | 90991A111                 | Pretty much any head type works as a substitute           |
| M3x10 ISO 7380 Torx T10 screw             | 12       | 90991A111                 |                                                           |
| M3x10 ISO 4762 socket head cap screw      | 6        | 91292A113                 |                                                           | 
| M3 DIN 125 washer                         | 16       | 91292A113                 |                                                           |
| M3 DIN 934 zinc-plated hex nut            | 16       | 91292A113                 | Zinc-plated instead of stainless to prevent galling       |
| M3x6 4.5-4.8mm hex Female-Female standoff | 8        | 95947A002                 | Cheaper on Digi-key                                       |
| M5x8 ISO 7380 hex socket screw            | 4        | 94500A230                 | McMaster 92095A207	also works but larger package size     |  
| M5x12 ISO 7380 hex socket screw           | 4        | 92095A210                 | Longer lengths (up to 16mm) would also work               | 

The following other hardware are needed:

| Description                               | Quantity | Notes                                                     | 
| ----------------------------------------- | -------- | --------------------------------------------------------- |
| 3M SJ-5003 bumpons (or equivalent)        | 4        | Available in different colors                             |

# PCB 

There are two PCBs, the ADC board and the Main board. The ADC board has SMT components that JLCPCB can place, while the Main board is through hole. While the project was originally going to use an Adafruit Feather RP2040 for the controller, I later switched back to using an Arduino Nano clone so I could re-use the code from Gen 1. 

The PCB can be ordered off JLCPCB using the manufacturing files provided here.

## ADC board

HX711 pitfalls

The following additional through-hole parts are needed:

| Reference designators | Part description            | Manufacturer | Part number     | Quantity | Notes                                               | 
| --------------------  | --------------------------- | ------------ | --------------- | -------- | --------------------------------------------------- |
| J1                    | 10-pin shrouded connector   | CNC Tech     | 3020-10-0100-00 | 1        |                                                     | 
| J2                    | 4-pin terminal block        | Würth        | 691137710004    | 1        |                                                     |
| J3                    | 2-pin terminal block        | Würth        | 691137710002    | 1        |                                                     |

D2 can be left unpopulated if D1 is populated,  but Littelfuse P4KE6.8A can be used if not (if JLCPCB is out of stock of D1, for instance)


## Main board

The main board can be ordered as a bare PCB, which has additional through-hole parts that need to be soldered or installed on. These can be sourced from Digikey, Mouser, or your electronic parts distributor of choice.

| Reference designators | Part description            | Manufacturer | Part number     | Quantity | Notes                                               | 
| --------------------  | --------------------------- | ------------ | --------------- | -------- | --------------------------------------------------- | 
| A2                    | 2.54mm 15-pin female socket | Sullins      | PPPC151LFBN-RC  | 2        | You can also cut down a longer socket               |
| A2                    | Arduino Nano or clone       | Arduino      | Nano            | 1        | Do not use the Arduino Nano Every or ESP32 versions | 
| C1, C3                | 100uF 63V capacitor         | Nichicon     | UVY1J101MPD1TD2 | 2        | 63V rating is overkill, but I had some lying around |
| D1, D3-D5             | 5.8V TVS diode DO-204AL     | Littelfuse   | P4KE6.8A        | 4        |                                                     |
| F1                    | 16V 2A polyfuse             | Bel Fuse     | 0ZRN0200FF1E    | 1        |                                                     |
| J1                    | 5.5x2.5mm barrel jack       | Kycon        | KLDX-0202-B     | 1        |                                                     |
| J2                    | 10-pin shrouded connector   | CNC Tech     | 3020-10-0100-00 | 1        |                                                     |
| J8                    | 4-pin terminal block        | Würth        | 691137710004    | 1        |                                                     |
| LED1                  | 5mm through-hole RGBLED     | Adafruit     | 1938            | 1        |                                                     |
| U2                    | 2.54mm 8-pin female socket  | Würth        | 61300811821     | 2        | You can also cut down a longer socket               |
| U2                    | TMC2208 SilentStepStick     | Watterott    | 20170003-002    | 1        | TMC2209 also works                                  |
| SW1-SW4               | Cherry MX-compatible switch | Any          | Your favorite   | 4        |                                                     |

# Electrical parts

You will need the following electrical parts as well:

| Part description                                             | Manufacturer | Part number     | Quantity | Notes                                                                  | 
| ------------------------------------------------------------ | ------------ | --------------- | -------- | ---------------------------------------------------------------------- |
| 500gf TAL221 load cell                                       | Sparkfun     | 14728           | 1        | Can also be found on Aliexpress                                        | 
| 10-pin ribbon cable                                          | Assmann WSW  | H3CCH-1006G-ND  | 1        |                                                                        |
| 12V 2A power supply 5.5x2.5mm barrel jack, center positivee  | EDATEC       | ED-PSU1202-US   | 1        | Easily substitued; I use the power supply for a WD external hard drive |
| Alligator clip cable                                         | Adafruit     | 1008            | 1        | Easily substitued                                                      |

# Other parts

A 100g calibration weight is used to calibrate the load cell. These can easily be bought off Amazon for less than $1. 


# OPERATING PROCEDURE

## Software Needed
* [Arduino IDE](https://www.arduino.cc/en/software)
* Python 3 (Jupyter Notebook) - this can be installed via a package like the [Anaconda Distribution](https://www.anaconda.com/products/distribution) of Python 3
* Libreoffice Calc
* MS Paint

## Calibration

## Measurement

## Data processing
### Automated using Python
### Manual


