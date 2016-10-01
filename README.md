# EnergyMeter

ADE7880 Energy Meter Driver

Application made to configure and read Energy 
Meter IC ADE7880 registors via RPi, 

Depending on configuration the application write results to shell window or to a specified file 



the file config.csv is used to configure the program operation via external program. 

The structure of the command used is as follows  

divice_id(int);fileName;Sampling_Rate(uint32_t);loop_control_flag(bool);pause_flag(bool)
 
 
   
   
 Other supported commands for ADE7880 registors direct access  
 
 driver_ade7880  config 
-> Runs Default configuration procedure. As described in the device datasheet section QUICK SETUP AS ENERGY METER (Outputs resulting message to consol on success or failure.)

driver_ade7880  read  [register phisical address (HEX eg. 0xFFFF)] [register size in byte (decimal)]  
-> Reads Specific register value (Outputs resulting message to consol on success or failure)

driver_ade7880  write [register phisical address (HEX eg. 0xFFFF)] [value to be stored (HEX eg. 0xFFFF)] [register size in byte (decimal)]  
-> Writes the given value to Specific register (Outputs resulting message to consol on success or failure)

 
CALIBRATION CONSTANTS: This constants calculated so that readings fetched from target registers 
can be converted into meaningful values such as amps , volts, kwh, watt. 
The constants should be adjusted if there is any change to Hardware design 
The calibration steps can be found from this calibration document 
http://www.analog.com/static/imported-files/application_notes/AN-1171.pdf . 
Note
Since all phases on the meter are matched, the same constant can be used 
for all energy, power,current rms and voltage rms readings on any Rpi
VLSB_CONST              211184.71337579617834394904458599        LSB constant for voltage reading 24 volt max is assumed
ILSB_CONST              353022.88732394                          LSB constant for current reading 30 Amp max is assumed
WHLSB_CONST             (67.28)*2.275                            LSB constant for energy reading
WATTLSB_CONST           8505                                     LSB constant for power reading  


TRANSFORMER_RATIO         20.56621005
This value is the ratio of the voltage transformer 
used to step down the line voltage to lower level. the value should be adjusted if different transformer is used.


Moredetail is provided in the doxigen output 


 




