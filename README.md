[![MCHP](images/microchip.png)](https://www.microchip.com)

# Noise Countermeasures with the AVR64DD32
This demonstration show the powerful noise suppression features available for ADC input on the AVR64DD32.  The input signal is fed through a sample-and-hold circuit ensuring that the voltage to the ADC is held at a constant level.  By sampling in bursts, a configurable number of conversions are accumulated and dramatically increase the signal to noise ratio.  A sample delay can be used to tune the burst sampling frequency away from any harmonic noise aliased from the sampled signal.    
## Related Documentation
- [AVR64DD32 Datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/AVR64DD32-28-Prelim-DataSheet-DS40002315B.pdf)
- [AVD64DD32 Curiosity Nano Hardware User Guide](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/UserGuides/AVR64DD32CNANO-Prel-HW-UserGuide-DS50003323.pdf)
- [Curiosity Nano Base Board Hardware User Guide](https://ww1.microchip.com/downloads/en/DeviceDoc/Curiosity-Nano-Base-for-Click-boards-User-Guide-50002839B.pdf)
- [Signal and Noise Generator](http://ww1.microchip.com/downloads/en/DeviceDoc/Signal-Noise-Generator-40002040A.pdf)
- [AN2551 - Noise Countermeasures for ADC Applications ](https://www.microchip.com/content/dam/mchp/documents/MCU08/ApplicationNotes/ApplicationNotes/AN2551-Noise-Countermeasures-for-ADC-Applications-00002551C.pdf)
  
## Software Used
- MPLAB® X IDE 6.0.0 or newer [(MPLAB X IDE 6.0)]()
- MPLAB XC8 2.40.0 or newer compiler [(MPLAB XC8 2.40)]()
- MPLAB Code Configurator (MCC) 5.1.9 or newer [(MPLAB Code Configurator)]()
- MPLAB® Data Visualizer [(MPLAB Data Visualizer)](https://www.microchip.com/en-us/tools-resources/debug/mplab-data-visualizer)

## Hardware Used

## Setup

#### Configuration

#### Option #1:
Easy setup - upload the pre-compiled binary:

* Download the repository
* Drag-and-Drop Program the AVR64DD32
  * Plug Curiosity Nano into windows and it will show up under File Explorer
  * Put .hex file (in dist folder) into Curiosity Nano folder in File Explorer and it will automatically program itself

Once the steps above are completed, continue to the *Operation* Section to see how to operate the demo

#### Option #2:

## Operation


## Summary
