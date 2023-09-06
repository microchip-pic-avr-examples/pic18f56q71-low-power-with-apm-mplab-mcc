<!-- Please do not change this html logo with link -->

<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# Building a Low-Power Sensor Application Using APM — Use Case for PIC18F56Q71 Microcontroller with MCC Melody

This project demonstrates the effectiveness of the Analog Peripheral Manager (APM), an integrated peripheral of the PIC18-Q71 device family, in reducing power consumption by enabling and disabling the controlled analog peripherals.

Three similar applications are benchmarked, each of which utilize different low-power techniques as described below:
<br>
 [1. Ambient Temperature Basic Application](#1-ambient-temperature-basic-application) - Shows the highest power consumption case by not using any power-saving methods.<br>
 [2. Ambient Temperature Application with APM](#2-ambient-temperature-application-with-apm) - Shows improvements in power savings by using the APM in conjunction with the previous example.<br>
[3. Ambient Temperature Application with APM and Sleep mode](#3-ambient-temperature-application-with-apm-and-sleep-mode) - Shows the optimal low-power application
through the use of the APM and Sleep mode technique.<br>

## Related Documentation

More details and code examples on the PIC18F56Q71 can be found at the following links:

- [PIC18F56Q71 Product Page](https://www.microchip.com/en-us/product/PIC18F56Q71)
- [PIC18F56Q71 Code Examples on GitHub](https://github.com/microchip-pic-avr-examples/?q=pic18f56q71)

More details about Low Power tips and tricks can be found at the following links:
- [AN1416 - Low-Power Design Guide](https://ww1.microchip.com/downloads/en/Appnotes/90001416a.pdf)
- [TB3144 - Doze, Idle and PMD Features of 8-Bit PIC® Microcontrollers](https://ww1.microchip.com/downloads/en/Appnotes/90003144B.pdf)

## Software Used

- [MPLAB® X IDE v6.15 or newer](http://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC8 v2.41 or newer](http://www.microchip.com/mplab/compilers)
- [PIC18F-Q Series Device Pack v1.20.405 or newer](https://packs.download.microchip.com/)

## Hardware Used

- The [PIC18F56Q71 Curiosity Nano Development board](https://www.microchip.com/en-us/development-tool/ev01g21a) is used as a test platform:
    <br><img src="images/pic18f56q71_cnano_board.png" width="800">

- [Power Debugger](https://www.microchip.com/en-us/development-tool/atpowerdebugger):
    <br><img src="images/power-debugger.png" width="800">

- [MCP9700 - Linear Active Thermistor IC](https://www.microchip.com/en-us/product/mcp9700):
    <br><img src="images/mcp9700.png" width="400">


## Operation

To program the Curiosity Nano board with this MPLAB X project, follow the steps provided in the [How to Program the Curiosity Nano Board](#how-to-program-the-curiosity-nano-board) chapter.<br><br>

## Concept 

Low-power applications are designed to operate with minimal power consumption while still providing the required functionality. This is achieved using advanced power management techniques, such as power gating, voltage scaling and clock gating. These techniques allow the device to operate in low-power modes when not in use, reducing power consumption and extending battery life. <br>

The three instances of the application were benchmarked using Microchip's Power Debugger and a PIC18F56Q71 Curiosity Nano board. The channel A ammeter of the Power Debugger was connected to the Curiosity Nano board to measure current. The output pin of the MCP9700 was connected to the RA4 pin of the Curiosity Nano board. The next figure depicts the setup for the three application variants.
<br><img src="images/low-power-connections.png" width="800">

**Important:** Cut the Target Power strap (J101) for Current measurement, and attach the Power Debugger's ammeter probes to the two terminals of J101. Refer to the [PIC18F56Q71 Curiosity Nano User’s Guide](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/UserGuides/PIC18F56Q71-CuriosityNano-HW-UG-DS50003481.pdf) for more information about Low-Power measurement.<br> 

This project presents one of the most significant characteristics of the APM peripheral - the capability to allow analog modules to be switched on and off without core interaction using the APM internal timer. The below pictures offer an overview of the peripherals utilized to drive the MCP9700
sensor and acquire data in all three application variants.
<br><img src="images/low-power-peripherals.png" width="800">

This example is designed to periodically read the ambient temperature using the MCP9700 Linear Active Thermistor ICs sensor, whose output voltage is directly proportional to the measured temperature values ranging from -40°C to +150°C. The obtained data is then transmitted via the Universal Asynchronous Receiver Transmitter (UART) module to MPLAB Data Visualizer. This sensor is driven using the integrated 10-bit DAC module with the sensor’s lowest accepted operating voltage. According to the sensor's data sheet, the sensor operating current is between 5 and 12 μA, well below the maximum current of 50 mA, which a pin can supply. The output voltage is connected to the positive input of the OPA module used in a noninverting configuration with a 2.67 gain. The output of the OPA is connected internally to Context 1 of the ADC with Computation and Context Switching module, which is configured in Average mode as the Operating Mode. This mode allows the MCU to accumulate eight values in eight seconds, then return the averaged value that will be converted to a representation of the ambient temperature in degrees Celsius readable via the UART module.

All the above functions are called in a logical, continuous cycle that is repeated once every one second and represented in the diagram below. 
<br><img src="images/low-power-flow-diagram.png" width="500">

The software project is built using two types of functions, those generated with MPLAB Code Configurator (MCC) and custom application functions, such as:
 - `ADC_UserContext1Callback`- sets a flag when the ADC interrupt is generated
 - `ClearADCFlag` - clears the flag set by the previous function
 - `TemperatureProcessing` - processes the temperature. It retrieves the averaged value from eight repeated ADC conversions every one second. The value is represented by the ADC-specific interrupt and processed using the ADC formula to determine the input voltage applied on the pin and stored in the `inputADCVoltage` variable. Next, it is computed using the specific formula for the MCP9700 sensor, described in its respective data sheet. In the final stage of the cycle, the value stored in the `calculatedTemperature` variable represents the argument of the `PrintTemperature` function
 - `PrintTemperature` - enables the UART communication, sends the specific text and value to the terminal, waits until the transmission is done and disables UART to reduce the necessary power 
 
 The infinite `while()` loop checks if the ADC conversions are finalized and then computes and prints the temperature.<br>

```
    while(1)
    {
        if(adcContext1Flag)
        {
            TemperatureProcessing();
            ClearADCFlag();
        }
    }
```
<br>

All three application examples use small, fixed periods for a simple and comprehensive presentation. The benchmarking period is eight seconds long. During this period, eight ADC conversions are accumulated, followed by printing the average ADC value using UART. The APM period can be set up to a maximum of 38 hours, meaning the application can be designed to use less power.<br>

## Application Examples Benchmarking

All the application examples are based on the same concept with power-reducing solution increments towards the lowest power consumption scenario, using an integrated low-power module, APM. The first application example presented is not optimized for power-consumption and is used to set the base for current consumption. The second one shows how to improve the previous consumption values by using the APM to switch the analog peripherals independently on and off. The last one implements the most efficient power-saving case using Sleep mode.<br>

The current consumption is measured with Microchip Power Debugger, and the measurements are displayed using MPLAB Data Visualizer.

All three scenarios are designed on the same structure, as described below.<br>

The following MCC Melody configuration is required to be set and it is common for all three scenarios (different from default configuration):<br>

1.	Clock Control:
    - Clock Source: HFINTOSC
    - System Clock (Hz): 1000000
    - Secondary Oscillator: Disabled
    - PLL Enable: Disabled

2.	Configuration Bits:
    - CONFIG1
        - External Oscillator Selection: Oscillator not enabled
        - Reset Oscillator Selection: LFINTOSC
    - CONFIG2:
        - Clock out Enable bit: CLKOUT function is disabled
        - Clock Switch Enable bit: Writing to NOSC and NDIV is allowed
        - Fail-Safe Clock Monitor Enable bit: Fail-Safe Clock Monitor Disabled
        - Fail-Safe Clock Monitor Primary XTAL Enable bit: Fail-Safe Clock Monitor Disabled 
        - Fail-Safe Clock Monitor Secondary XTAL Enable bit: Fail-Safe Clock Monitor Disabled
    - CONFIG3:
        - Low Power BOR Enable bit: Low-Power BOR disabled
        - Brown-out Reset Enable bits: Brown-out Reset disabled 
    - CONFIG5:
        - WDT operating mode: WDT disabled

3.	Pins:
    - RB5 – UART2 TX – input
    - RB4 – UART2 RX – output
    - RA2 – DAC1 OUTx – output
    - RA4 – OPA1 INx+ – input
    - RA1 – OPA1 OUT – output

4.	UART2:
    - UART PLIB Selector: UART2
    - Requested Baudrate: 9600
    - Data Size: 8
    - Parity: None
    - Stop Bits: 1
    - Redirect Printf to UART: Enabled
    - Enable Receive: Disabled
    - Enable Transmit: Enabled
    - Enable UART: Disabled

5.	DAC1:
    - VDD: 3.3 V
    - Required ref: 2.31 V
    - DAC Enable: Enabled
    - DAC Positive reference selection: VDD
    - DAC Negative reference selection: VSS
    - DAC Output Enable Selection: DACOUT1 Enabled and DACOUT2 Disabled

6.	OPA1:
    - Enable OpAmp: Enabled
    - Enable Internal Output: Enabled
    - Op Amp Configuration: Non-Inverting Programmable Gain Amplifier
    - Positive Channel: OPA1IN+
    - Positive Source Selection: OPA1IN1+
    - Negative Channel: GSEL
    - Negative Source Selection: Vss
    - Internal Resistor Ladder Selection: R1 = 6R and R2 = 10R, R2/R1=1.67
    - Gain (1+R2/R1): 2.67
    - Feedback Mode Selection: OPA1OUT

7.	ADC:
    - Input Configuration: single ended mode
    - Clock Selection: ADCRC
    - ADI Interrupt Enable: Enabled
    - Context1:
        - Positive Channel Selection: OPA1IN1+
        - Positive Voltage Reference: VDD
        - Operating Mode Selection: Average_mode
        - Threshold Interrupt Mode: Enabled
        - Upper Threshold: 0
        - Lower Threshold: 0
        - Threshold Setpoint: 0
        - Repeat Threshold: 8
        - Accumulator Right Shift: 3
        - ADCH1 Interrupt Enable: Enabled

The UART module sends the processed ambient temperature in degrees Celsius to the terminal every eight seconds.<br>

The DAC module powers the MCP9700 external sensor with the lowest acceptable input (2.30V). The DAC is used instead of I/O pins because the voltage level can be precisely set, and the DAC can be switched on/off by the APM independently, without any other software necessity.<br>

The OPA module amplifies the output voltage from the sensor to enhance the precision of the ambient temperature readings. The OPA is configured as a noninverting programmable gain amplifier with x2.67 gain. The maximum value of the input voltage for the ADC is set to 3.3V, so the maximum voltage after the OPA will also be 3.3V. This gain is chosen to amplify a specific range of ambient temperature, and the maximum output voltage from the sensor is about 1.24V, which implies a temperature range between -50 and 70 degrees Celsius. Refer to <i> Figure 2-17: Output Voltage vs. Ambient </i> from the MCP9700 sensor data sheet for more information. The OPA output is internally connected to the positive input of the ADC.<br>

The ADC is configured in Single-Ended mode, with ADCRC as the necessary clock unaffected by the Sleep power-saving mode. It only needs one context, so Context 1 is configured to read the input voltage from a positive channel selection. The operating mode is Average mode, which allows the user to use the Threshold Interrupt mode. It is configured to accumulate up to eight samples then to average the value by right-shifting it with three positions without any voltage levels. A context interrupt is triggered when this value is processed in hardware and stored in the ADFLTR register. Other configurations will be described for each scenario, because every single one presents minor differences as described above.<br>

Other configurations will be described for each example, because every single one presents small differences as described above.

### 1. Ambient Temperature Basic Application 

This application will make use of the UTMR for the scheduling of the sensor data readings and processing.<br>

The UTMR module is used as an auto-trigger source for the ADC, and is configured to give a pulse every one second then reset at PR Match. The Low-Frequency Internal Oscillator (LFINTOSC) is selected as clock source, providing the flexibility to set the period up to nine minutes, depending on user preference.<br>

In addition of the common part for the MCC Melody configuration, the following settings are required and example-specific:

1.	UTMR:
    - Timer Dependency Selector: TU16A
    - Enable Timer: Enabled
    - Clock Source Selection: LFINTOSC
    - Timer Period (s): 1
    - Reset Condition: At PR Match

This measurement presents one period described by an average current of 1.25 mA.

<br><img src="images/current-application-1.PNG" width="1000">

### 2. Ambient Temperature Application with APM

Unlike the previous application example, this application will use the APM for the sensor data
readings and processing schedules.<br>

The APM module is configured with LFINTOSC as clock source with a one second period. The Start 1 event comes up in one ms and during this event the ADCA, OPA1 and DAC1 modules are enabled. The Start 2 event will be delayed with one ms after the previous event. The sensor needs up to 800 μs to turn on and enables the ADCD part of the ADC. The End 1 event is set to 1.2 ms after the Start 1 event and disables the ADCA part and DAC1, while the End 2 event is set to 1.3 ms after the same Start 1 event and will disable the ADCD part and OPA1 module. The figure below shows the APM timing.<br>

<br><img src="images/low-power-apm-timing.png" width="800">

In addition of the common part for the MCC Melody configuration, the following settings are required and scenario-specific:

1.	APM:
    - Enable APM: Enabled
    - Clock Source: LFINTOSC
    - Time unit: seconds
    - Requested Period (s): 1
    - Requested Start 1 (s): 0.001 (1 ms)
    - Requested End 1 (s): 0.0012 (1.2 ms)
    - Requested Start 2 (s): 0.001 (1 ms)
    - Requested End 2 (s): 0.0013 (1.3 ms)
    - APM Events:
        - Start 1: ADCA, OPA1, DAC1
        - End 1: ADCA, DAC1
        - Start 2: ADCD
        - End 2: ADCD, OPA1


**Important:** Enable the analog part of the ADC peripheral before a conversion can occur, so generally, the ADCA Start Event must occur before the ADCD Start Event to ensure that the controller has been enabled and initialized before a conversion is triggered. Refer to the APM section of the Q71 data sheet for more information about the APM operation with ADC.<br>

**Note:** It is recommended to disable all the analog peripherals controlled by the APM when
initializing.

2.	DAC1:
    - Enable DAC: Disabled

3.	ADC:
    - Enable ADC: Disabled

4.	OPA1:
    - Enable OpAmp: Disabled

In this scenario, the first current usage reduction is shown. The average is around 406 μA. Considering the APM feature to periodically switch ON and OFF the used analog peripherals, observe a standby current decrease of up to three times. Eight ADC readings describes the shown signal. The image below presents the first one, where the UART module is used to print data on the PC terminal.

<br><img src="images/current-application-2.PNG" width="1000">

### 3. Ambient Temperature Application with APM and Sleep mode

This application will use the APM for the sensor data readings and processing scheduling and Sleep
mode for power optimization.<br>

In addition of the common part for the MCC Melody configuration, the following settings are required and scenario-specific:

1.	APM:
    - Enable APM: Enabled
    - Clock Source: LFINTOSC
    - Time unit: seconds
    - Requested Period (s): 1
    - Requested Start 1 (s): 0.001 (1 ms)
    - Requested End 1 (s): 0.0012 (1.2 ms)
    - Requested Start 2 (s): 0.001 (1 ms)
    - Requested End 2 (s): 0.0013 (1.3 ms)
    - APM Events:
        - Start 1: ADCA, OPA1, DAC1
        - End 1: ADCA, DAC1
        - Start 2: ADCD
        - End 2: ADCD, OPA1

2.	DAC1:
    - Enable DAC: Disabled

3.	ADC:
    - Enable ADC: Disabled

4.	OPA1:
    - Enable OpAmp: Disabled

5.	Main.c:
    - Add the following code paragraph at the end of the while(1) function:
        
        ```
            SLEEP();
            NOP();
        ```

**Important:** It is recommended to add a NOP as the immediate instruction after the SLEEP instruction.

This scenario presents the lowest current consumption with an average of around 13.8 μA, which signifies a decrease of up to 90 times compared to the first scenario and up to 30 times compared to the second scenario due to the usage of the SLEEP instruction that puts the CPU in the lowest power consumption mode. The current in Sleep mode is around 1 μA, and the ADC readings are approximately 30 μA. The UART usage for printing data on the PC terminal includes a significant current input, around 400 μA.

<br><img src="images/current-application-3.PNG" width="1000">

<br><img src="images/low-power-apm-data-terminal.png" width="1000">

Expanding the formula for the average needed current, based on the third application example, the conclusion is that the current consumption will decrease if increasing the APM period. For example, the desired period can be up to one hour. Without considering the short pulses that represent only the ADC readings, every eighth period of the signal will stay ON approximately 0.24 seconds (240 ms) and in Sleep mode for the rest of the time, around 3600 seconds - so the average current will be the same as in Sleep mode, around 1.18 μA. The APM offers a flexible scheduling system for the analog peripherals, which various sensor applications can take full advantage of.

## Conclusion

Low-power applications are becoming increasingly important due to the growing demand for portable and battery-powered devices. These applications require less power consumption, which leads to longer battery life and reduced energy costs. It is essential to consider low-power design techniques and technologies when developing electronic devices to meet the market and environment needs.<br>

These examples demonstrate the usage of various power-saving configurations available for the PIC18-Q71 MCUs. As shown in the application examples, some operations utilize more current in Active mode compared to when in Sleep mode. In PIC18-Q71 MCUs, Sleep mode provides the lowest MCU power consumption, as clock is disabled to CPU and all the other peripherals, except of the APM module, which keeps running.<br>

In the third application example, the conclusion is that the current consumption will decrease by increasing the period of the APM.

##  How to Program the Curiosity Nano Board

This chapter demonstrates how to use the MPLAB X IDE to program a PIC® device with an Example_Project.X. This is applicable to other projects.

1.  Connect the board to the PC.

2.  Open the Example_Project.X project in MPLAB X IDE.

3.  Set the Example_Project.X project as main project.
    <br>Right click the project in the **Projects** tab and click **Set as Main Project**.
    <br><img src="images/Program_Set_as_Main_Project.PNG" width="600">

4.  Clean and build the Example_Project.X project.
    <br>Right click the **Example_Project.X** project and select **Clean and Build**.
    <br><img src="images/Program_Clean_and_Build.PNG" width="600">

5.  Select **PICxxxxx Curiosity Nano** in the Connected Hardware Tool section of the project settings:
    <br>Right click the project and click **Properties**.
    <br>Click the arrow under the Connected Hardware Tool.
    <br>Select **PICxxxxx Curiosity Nano** (click the **SN**), click **Apply** and then click **OK**:
    <br><img src="images/Program_Tool_Selection.jpg" width="600">

6.  Program the project to the board.
    <br>Right click the project and click **Make and Program Device**.
    <br><img src="images/Program_Make_and_Program_Device.PNG" width="600">

<br>

- - - 
## Menu
- [Back to Top](#building-a-low-power-sensor-application-using-apm--use-case-for-pic18f56q71-microcontroller-with-mcc-melody)
- [Back to Related Documentation](#related-documentation)
- [Back to Software Used](#software-used)
- [Back to Hardware Used](#hardware-used)
- [Back to Operation](#operation)
- [Back to Concept](#concept)
- [Back to Application Examples Benchmarking](#application-examples-benchmarking)
    - [Back to 1. Ambient Temperature Basic Application](#1-ambient-temperature-basic-application)
    - [Back to 2. Ambient Temperature Application with APM](#2-ambient-temperature-application-with-apm)
    - [Back to 3. Ambient Temperature Application with APM and Sleep mode](#3-ambient-temperature-application-with-apm-and-sleep-mode)
- [Back to Conclusion](#conclusion)
- [Back to How to Program the Curiosity Nano Board](#how-to-program-the-curiosity-nano-board)