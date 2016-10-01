/*!\file ade7880_driver.c
   \brief \e File:  ade7880_driver.c \n
   \brief \e Purpose: \n    
*/

#include "../includes/includes.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>    


/*!\file ade7880_driver.c 
   \brief \e File:  ade7880_driver.c \n
   \brief \e Purpose: \n 
*/


/*!-------------------------------------------------------------------------------
  \mainpage										
  \brief \e PROGRAM NAME:\n
     	        Energy Meter Driver  
  
 \brief \e PURPOSE: \n
                The Program is made to configure and read Registers of an Energy \n
				Meter IC ADE7880, for the purpose of obtaining instantaneous values \n
				of Voltage, Current, Power and watt hour values read at specific \n
				measurement point.\n

 \par  \e VERSIONS: 					

 \par \a Version history: 
		    \brief \version Version Number 1
       		\date xx.xx.2014/ \author TEAM HW / Sami Mahamoed 

 \details \a Added fun :\n 
	      	\brief Additional functinality is add to the programm in order to read \n
			and write to or from a commen file used between the Python script and \n
			this module  \n
		


		
-----------------------------------------------------------------------------------*/




/*!---------------------------------------------------------------------------------
 
\defgroup USER_INTERFACE	
-----------------------------------------------------------------------------------*/

///@{
/*!

    \page  Module1 USER INTERFACE 
	
	\brief Top most module in the application, responsible for handling commands received \n
	       from the shell or via config.csv file. In order that, the program runs accordingly.:
	
	\verbatim [executable] + command \endverbatim \n
	\brief The followoing requests are supported: 
       
    \verbatim  driver_ade7880  config \endverbatim 

	\brief  			   -> Runs Default configuration procedure. As described in the device datasheet 
								  section QUICK SETUP AS ENERGY METER \n
								  (Outputs resulting message to consol on success or failure.)\n\n
							
    \verbatim  driver_ade7880  read  [register phisical address (HEX eg. 0xFFFF)] [register size in byte (decimal)]  \endverbatim
	\brief 				   -> Reads Specific register value \n
							      (Outputs resulting message to consol on success or failure)\n\n

    \verbatim  driver_ade7880  write [register phisical address (HEX eg. 0xFFFF)] [value to be stored (HEX eg. 0xFFFF)] [register size in byte (decimal)]  \endverbatim 
	\brief   			   -> Writes the given value to Specific register \n 
							      (Outputs resulting message to consol on success or failure)\n\n

    \verbatim  driver_ade7880  run \endverbatim
	\brief 				   -> Runs the normal program execution routine \n
								 (Outputs resulting message to consol on success or failure.)\n\n 								  
        
  
	
	There are also common commands between Python script and this module used to alter the normal \n
	execution routine via common configuration csv format interface file . i.e. the program continuously \n
    Monitor this file so as to act accordingly if there is any change.  	

    \brief The structure of the command used is simple \n
	
	\verbatim 
id(int);fileName;Sampling_Rate(uint32_t);loop_control(bool);pause(bool)
   |      |           |                       |                    |
   |      |           |                       |                    |___ Control flag: 
   |      |           |                       |						      if set to 1 pause reading
   |      |           |                       |						      and set to 0 resume reading
   |      |           |                       |________________________ Control flag: 
   |      |           |                                                   if set to 1, the program will
   |      |           |                                                   exit reading loop and end execution    
   |      |           |________________________________________________ Delay_control: 
   |      |                                                               Adds the specified number of
   |      |                                                               millsecond delay on the    
   |      |                                                               main reading loop
   |      |____________________________________________________________ Sets file name 
   | 																	where data to be saved   
   |___________________________________________________________________ Sets device id  
		 
	\endverbatim	 

*/

/*!---------------------------------------------------------------------------------
 @fn int8_t wait_new_conversion(void)
 
 \brief Function clears STATUS0 register Data-Ready bit and waits until the DSP set \n 
        it back again. this verifies new reading is loaded to value registers.
		
 \return function return 0 or -1 on success or failure respectively.  
		
-----------------------------------------------------------------------------------*/
///@{
int8_t wait_new_conversion(void){

		   int8_t cc =0;
		   int16_t result =-1;
	
           struct timeval tv;
           status0_reg_u status0;
		   status0.reg_all = spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,STATUS0,sizeof(uint32_t));
		  
		   
		
		   if(status0.bits.DREADY==1){
	
		   result=0;cc = 0;while(((result = config_cmd(SET_RAM_WR_PROTECTION,1,DISABLE))== -1)&& (cc++ < 3));	
           //put back the value to clears status flags		   
		   if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,STATUS0,status0.reg_all,sizeof(uint32_t))!=0)result =-1; 
		   status0.reg_all = spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,STATUS0,sizeof(uint32_t));
		   result=0;cc = 0;while(((result = config_cmd(SET_RAM_WR_PROTECTION,1,ENABLE))== -1)&& (cc++ < 3)); 
		 
		   
		   if(result == -1){
                	printf("\nERROR: Couldn't write\n");  
					return -1;
		   }
		   
		    
		   }
		   
		   
		   
		
		   gettimeofday(&tv,NULL);
		   uint32_t t1,t2;
		   t1=t2 = tv.tv_usec;	
		   #ifdef DEBUG 		   
		   printf("\nSTATUS REG VALUE %08X,-------------------------------------------- us time %lu\n",status0.reg_all,t1);
		   #endif
		   while(status0.bits.DREADY==0){ //wait till conversion is done
		 
		     status0.reg_all = spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,STATUS0,sizeof(uint32_t));
			 gettimeofday(&tv,NULL);
			 
			 t2 = tv.tv_usec;	
			 #ifdef DEBUG 
			 printf("\nSTATUS REG VALUE %08X,-------------------------------------------- us time %lu\n",status0.reg_all,t2);  
			 #endif
			 if((t2-t1)>20000)
			 return -1;
			 
		   }
		   
		   

  return 0;
}

///@}
 

/*!---------------------------------------------------------------------------------
 @fn int16_t main(int argc, const char **argv)
 
 \brief \e Subroutine: \brief main program subroutine 
 
 @param [in]  argc number of arguments sent from the shell  
 @param [out] argv pointer to the argument buffer
 
 \brief Function commonly perform library Initialization, Setup Rpi gpio and spi 
		interface. Then run the requested procedure based on the argument passed 
		from shall.  
		
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{

int16_t main(int argc, const char **argv){
	
	 
 	 
 	int8_t cc =0;
 	int16_t result =-1;
	
    struct timeval tv;

 	 if((argc < 1)|| !bcm2835_init()/*library has to be initialized*/) 	 
	       return -1;	 
	 
	rpi_gpio_init();
    ade7880_power_mode(PSM0);
	usleep(50);
	
	if(spi_init(BCM2835_SPI_CS0)!= 0)
	return -1;
        #ifdef DEBUG 
		uint16_t cp=0;
		printf("\n#######################################%d###################################################\n",cp++);
	    #endif
	 
	
	 if(strcmp (argv[1],"config")==0)
	 {
        if(ade7880_config_reg_default()!=-1)
		      printf("\nDevice Ready to use\n");
	 } 
	 else 
	 if(strcmp(argv[1],"read")==0)	 
	 {
	    if(argc<4){printf("\nCMD ERROR: RD\n");return-1;}
		  
		for(cc = 0;cc<((argc>=5)?atoi(argv[4]):1);cc++){
		 spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,make16((uint8_t *)argv[2],2),atoi(argv[3]));
		 gettimeofday(&tv,NULL);
		 printf("<----------------------------------------------------------- %d timesamp: %lf\n",
		 cc+1,(double)(tv.tv_sec + tv.tv_usec/(double)10E6));
		}
	 return 0;
	 }
	 else 
	 if(strcmp(argv[1],"write")==0 )
	 {
	    if(argc<4){printf("\nCMD ERROR: WR\n");return-1;}
	int ii;
	   for(ii=0;ii<argc;ii++)
	        printf(" argval %d, = %s",ii,argv[ii]);
       	
		result=0;cc = 0;while(((result = config_cmd(SET_RAM_WR_PROTECTION,1,DISABLE))== -1)&& (cc++ < 3)); 
		if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,make16((uint8_t *)argv[2],2)
		,make8((uint8_t *)argv[3],2),atoi(argv[4]))!=0)
		  result =-1;
		 result=0;cc = 0;while(((result = config_cmd(SET_RAM_WR_PROTECTION,1,ENABLE))== -1)&& (cc++ < 3)); 
		 if(result == -1){
                	printf("ERROR: Couldn't write");  
					  return -1;
		 }
		 
	 return 0;
	 }
	 else
	 if(strcmp(argv[1],"run")==0) 
	 {
	    #ifdef DEBUG
	    if(argc >=3){
		   if((strcmp(argv[2],"dprint=off")==0))
		       spi_enable_msg_debug_print(DISABLE);
		    else if((strcmp(argv[2],"dprint=on")==0))
		        spi_enable_msg_debug_print(ENABLE);
		}   
		printf("\n#######################################%d###################################################\n",cp++);
	    #endif
		
        if(argc<2){printf("\nCMD ERROR: measure\n");return-1;} 
		   
		   reading_loop();
	 }

	 
	 bcm2835_spi_end();
       
	 bcm2835_gpio_write(PIN_SS, HIGH);
	   
	 bcm2835_close();
	 
return 0;
}
///@}

/*!---------------------------------------------------------------------------------
 @fn void reading_loop(void)
 
 \brief Function reads measured values of main interest form ADE7880, checks the \n
		current configuration command form config.csv, and saves the data to the \n
		specified interface file.  The configuration command affect the behavior \n
		at which this subroutine execute, please use this document for more detail. 
		
 \return none.
 
-----------------------------------------------------------------------------------*/
///@{

void reading_loop(void){

  int16_t 	rpi_address; 
  char * 	filename;
  uint32_t 	cyc_time;
  uint8_t 	loop_ctrl;
  uint8_t   pause;
  int8_t 	result=0;
  int8_t startup = 1;
  
	measured_data_t data;
	
	    #ifdef DEBUG 
		printf("\n\n");
	    #endif	
		 printf("\nEntering Main loop ...\n");
         while(1){	
		 printf("\n");
		 
         printf("\nReading Phase A values ...\n");	 
		 if((data.phase_a.IRMS 	   	    = measure(PHASE_IRMS,PHASE_A,100))==-1)result = -1;
		 if((data.phase_a.VRMS 	   	    = measure(PHASE_VRMS,PHASE_A,100))==-1)result = -1;
		 if((data.phase_a.WH			= measure(PHASE_ACTIVE_WH,PHASE_A,1))==-1)result = -1;
		 if((data.phase_a.POWER   	    = measure(PHASE_ACTIVE_POWER,PHASE_A,1))==-1)result = -1;
		 
		 printf("\nReading Phase B values ...\n");	
		 if((data.phase_b.IRMS 	   	    = measure(PHASE_IRMS,PHASE_B,100))==-1)result = -1;
		 if((data.phase_b.VRMS 	   	    = measure(PHASE_VRMS,PHASE_B,100))==-1)result = -1;
		 if((data.phase_b.WH			= measure(PHASE_ACTIVE_WH,PHASE_B,1))==-1)result = -1;
		 if((data.phase_b.POWER   	    = measure(PHASE_ACTIVE_POWER,PHASE_B,1))==-1)result = -1;
		 
		 printf("\nReading Phase C values ...\n");	
		 if((data.phase_c.IRMS 	   	    = measure(PHASE_IRMS,PHASE_C,100))==-1)result = -1;
		 if((data.phase_c.VRMS 	   	    = measure(PHASE_VRMS,PHASE_C,100))==-1)result = -1;
		 if((data.phase_c.WH			= measure(PHASE_ACTIVE_WH,PHASE_C,1))==-1)result = -1;
		 if((data.phase_c.POWER   	    = measure(PHASE_ACTIVE_POWER,PHASE_C,1))==-1)result = -1;
		 
	    printf("\n\n\nREADINGS:\n");
        printf("\n");		
		printf("\n---------------------------------------------------------------------PHASE A KWH  : %f\n",data.phase_a.WH);
		printf("\n---------------------------------------------------------------------PHASE A POWER: %f\n",data.phase_a.POWER);
        printf("\n---------------------------------------------------------------------PHASE A VRMS : %f\n",data.phase_a.VRMS);
		printf("\n---------------------------------------------------------------------PHASE A IRMS : %f\n",data.phase_a.IRMS);
		printf("\n");
		printf("\n---------------------------------------------------------------------PHASE B KWH  : %f\n",data.phase_b.WH);
		printf("\n---------------------------------------------------------------------PHASE B POWER: %f\n",data.phase_b.POWER);
        printf("\n---------------------------------------------------------------------PHASE B VRMS : %f\n",data.phase_b.VRMS);
		printf("\n---------------------------------------------------------------------PHASE B IRMS : %f\n",data.phase_b.IRMS);
		printf("\n");
		printf("\n---------------------------------------------------------------------PHASE C KWH  : %f\n",data.phase_c.WH);
		printf("\n---------------------------------------------------------------------PHASE C POWER: %f\n",data.phase_c.POWER);
        printf("\n---------------------------------------------------------------------PHASE C VRMS : %f\n",data.phase_c.VRMS);
		printf("\n---------------------------------------------------------------------PHASE C IRMS : %f\n",data.phase_c.IRMS);
	    			 
        if(result == -1){			 
            ade7880_config_reg_default(); //this is the only thing we do for now
		//	continue;
		}
		
		do{
		if(config_recheck (CONFIG_FILE_NAME,CONFIG_CMD_FORMAT,&rpi_address ,filename, &cyc_time,&loop_ctrl,&pause)==0){
		
		if(pause==0){
		
		  while(save_to_file(rpi_address,data, filename,
		  startup 
		  /*!
		       \note on startup new file will be used to save data, \n 
			   otherwise data will be appended to the existing \n
			   interface file. \n
			   This is required due to the reason that, after power down \n 
			   on reset the file could be corrupted.
		  */
		  
		  )!=0)
		
		  startup = 0; 
		  
		 }
		 
		 }
		 
		 usleep(cyc_time);
		}while(pause==1);
		 
		if(loop_ctrl == 1)
		break;
		
        
		
           		
	 	#ifdef DEBUG
		if(result!=-1)
	    printf("\nREADING SUCESS\n");
		spi_enable_msg_debug_print(ENABLE);
		#endif
		
		
	 }

}
///@}

/*!---------------------------------------------------------------------------------
 @fn int8_t save_to_file(uint16_t rpi_address,measured_data_t data, const char *fileName,uint8_t clean)
 
 @param [in] rpi_address address of the Rpi 
 @param [in] data        data structure with the data to be saved to the file  
 @param [in] fileName     pointer to the fileName array
 @param [in] clean        boolean flag, if 0 data will be appended to the file,
		                  if 1 new file will be used 
 
 \brief  function saves data to the specified file in csv file format. 
 
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{

int8_t save_to_file(uint16_t rpi_address,measured_data_t data, const char *fileName,uint8_t clean) 
{ 
uint32_t dummy=0;
  FILE *f = fopen(fileName, (clean == 1)?"w":"a");  
  errno = 0;
  if (f == NULL){ 
   warn("%s: Couldn't open file %s; %s\n",fileName, strerror (errno));
  
  return -1;
  }
  
  
  fprintf(f, "%d;%lu;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d;%d;%d;%d;%d;%d\n", 
          rpi_address,
		  (unsigned)time(NULL),
	
			   data.phase_a.IRMS, 	
			   data.phase_a.VRMS, 	
			   data.phase_a.WH,			
			   data.phase_a.POWER,  	
				
			   data.phase_b.IRMS, 	
			   data.phase_b.VRMS, 	
			   data.phase_b.WH,			
			   data.phase_b.POWER,

			   data.phase_c.IRMS, 	
			   data.phase_c.VRMS, 	
			   data.phase_c.WH,			
			   data.phase_c.POWER,		   			
			   DUMMY_MSG,
			   DUMMY_MSG,  	 
			   DUMMY_MSG,  	
			   DUMMY_MSG,
               DUMMY_MSG,			   
			   DUMMY_MSG 			    
  ); 
 
  
  fclose(f); 
  return 0; 
} 
///@}

/*!---------------------------------------------------------------------------------
 @fn int16_t config_recheck(const char * config_file, const char * format, ...)
 
 @param [in] config_file pointer to the file Name array
 @param [in] format      String format specifying configuration command structure \n
						 anything used on printf function can also be used here 
 @param [in,out] ...	 the function expects at least as many additional arguments \n
						 as specified by format.					 
                            
 
 \brief Function reads the specified file and fills into passed arguments.  
 
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{

int16_t config_recheck(const char * config_file, const char * format, ...)
{
	
   int16_t result;
   int16_t ii,cc;
   FILE *f =fopen(config_file,"r");
   if (f == NULL) return -1; 
   
  char cmd_line[100];
  va_list args;
  va_start (args, format);
   
  if(fgets(cmd_line,100,f)!=NULL){
      for(ii=0,cc=0;*(cmd_line+ii)!='\0';ii++)
 	    if(*(cmd_line+ii)==';'){*(cmd_line+ii)= ' '; ++cc;}
    
    result=vsscanf (cmd_line, format, args);
    result = (result == cc)?0:-1;
  }
  
  va_end (args);
  fclose(f); 
  return result;
}
///@}
///@}







