# Work In Progress

# Gen 2 

Due to the general jankiness of Gen 1, I decided to do a much neater redesign for Gen 2 back in 2021. I initially thought about [running a group buy](https://geekhack.org/index.php?topic=115453.0), though life has gotten in the way of that. I finally had some time to pick this project back up in late 2025, and am currently documenting the Gen 2 design to the point it can be easily replicated by other people. This document is very much a work in progress, though I have built and operated Gen 2. 

Mechanical CAD is done in [Alibre Atom3D](https://www.alibre.com/atom3d/), and the PCB files are done using KiCAD V9.

# ORDERING GUIDE

# Mechanicals

## Laser cut metal parts

The following parts are laser-cut out of metal. DXFs are provided [here](https://github.com/bluepylons/Open-Switch-Curve-Meter/tree/main/Gen%202%20Design/Mechanical%20files/Laser%20cut)

| Part name                | Material thickness         | Quantity |
| ------------------------ | -------------------------- | -------- |
| Baseplate-stand          | 3mm (0.125")               | 1        |
| Bumpon-holder            | 1.5mm-1.6mm (0.059-0.063") | 4        |
| Load-cell-mounting-plate | 3mm (0.125")               | 1        |
| Switch-plate             | 1.5mm-1.6mm (0.059-0.063") | 1        |
| Vertical plate           | 3mm (0.125")               | 1        |

Aluminum is generally recommended for most of the parts, though pretty much any metal works. For the baseplate stand, steel, brass, or stainless steel are recommended as the additional weight provides more stability. All the parts on the ones I have built are out of aluminum 6061-T6.

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
The following parts are 3D-printed:

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
M3 standoffs, bumpons, etc. 

For other hardware, 

# PCB 

There are two PCBs: 

The PCB can be ordered off JLCPCB with the attached files. 

# Electrical parts

* TAL221 load cell 
* Adafruit Feather RP2040
* Connectors

# Other parts

* 100g Calibration weight


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


