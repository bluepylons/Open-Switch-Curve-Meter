# Gen 2 

These work in progress files for a potential GB of force curve meters ([Geekhack IC here](https://geekhack.org/index.php?topic=115453.0)). Mechanical CAD is done in [Alibre Atom3D](https://www.alibre.com/atom3d/), and the PCB files are done using KiCAD V6.

Due to restrictions on redistrbution of 3rd-party CAD files (e.g. for the 80/20-style extrusions from Misum), those files are excuded, but can be downloaded freely from their sites. More details in the [README for those files](https://github.com/bluepylons/Open-Switch-Curve-Meter/tree/main/Gen%202%20Design/CAD/Proprietary%20CAD%20files)

PCB prototypes have arrived and are currently being assembled.

# ORDERING GUIDE

# Mechanicals

## Laser cut metal parts

The following parts are laser-cut out of metal. DXFs are provided [here]()

| Part name                | Material thickness         | 
| ------------------------ | -------------------------- | 
| Baseplate-stand          | 3mm (0.125")               | 
| Bumpon-holder            | 1.5mm-1.6mm (0.059-0.063") | 
| Load-cell-mounting-plate | 3mm (0.125")               |
| Switch-plate             | 1.5mm-1.6mm (0.059-0.063") |
| Vertical plate           | 3mm (0.125")               |

Aluminum is generally recommended for most of the parts, though pretty much any metal works. For the baseplate stand, steel, brass, or stainless steel are recommended as the additional weight provides more stability. 

Aluminum or plain steel are usually the cheapest materials. Note that stay may corrode or rust without treatment (e.g. painting, powder coating). 

In the US, I recommend Sendcutsend. In the EU, Laserboost is a good source. 

For Sendcutsend, deburring is recommended, though it still tends to leave a sharp edge and may require manual deburring afterwards. For the smaller parts (Bumpon-holder, Switch-plate, and Load-cell-mounting plate), ask them to tumble deburr as it results in softer edges. 

## T-slotted extrusions and hardware 

Misumi HFS5 , tapped M5 at each end.
HNTAP5-5 

Other brands of 20mm 80/20-style T-slotted extrusions likely work, though you may need to tap the holes to M5 yourself. 

Note that Misumi and McMaster or actual 80/20(R) hardware are often incompatible as the slot opening on Misumi extrusions is a bit smaller. Be aware of this if ordering fasteners. 

## 3D-printed pieces
The following parts are 3D-printed:

## Linear actuator 

## Hardware
M3 standoffs, etc. 

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
* Python 3 (Jupyter Notebook) - this can be installed via a package like thte [Anaconda Distribution](https://www.anaconda.com/products/distribution) of Python 3
* Earl Philhower Core for the Arduino RP2040 
* Libreoffice Calc
* MS Paint

## Calibration

## Measurement

## Data processing
### Automated using Python
### Manual


