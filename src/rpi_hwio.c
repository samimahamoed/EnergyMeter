
   
/*!\file rpi_hwio.c
   \brief \e File:  rpi_hwio.c \n
   \brief \e Purpose: \n    
*/


#include "../includes/includes.h"

/*!---------------------------------------------------------------------------------
 \defgroup DEVICE_CONFIGERATION
		
-----------------------------------------------------------------------------------*/
///@{

/*!

    \page  file3 DEVICE CONFIGERATION 
	
	\brief The module contains functions that are prepared and used to configure the \n
	       Raspberry pi GPIO and spi interface, As well as ADE7880 Power mode, \n
		   Serial Port selection (that is SPI port activation) and Default configuration \n 
		   procedure as described in the device datasheet section QUICK SETUP AS ENERGY METER.\n
	     
*/

/*!---------------------------------------------------------------------------------
 @fn int8_t spi_init(uint8_t chip_select)
 @param [in] chip_select    Indicates the selected chip for communication         
 
 \brief  function initializ the Rpi SPI interfce.   
 
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{
int8_t spi_init(uint8_t chip_select){
    
	bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);    
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); 				  	// Data are propagated on a falling edge 
																
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);     // 65536 = 262.144us = 3.814697260kHz 
    bcm2835_spi_chipSelect(chip_select);   						                    
    bcm2835_spi_setChipSelectPolarity(chip_select, LOW);            // SS/HSA chip pin of ade7880 should be low for comminication      

return 0;	
}
///@}

/*!---------------------------------------------------------------------------------
 @fn int8_t rpi_gpio_init(void)      
	 
 \brief  configure used Rpi gpio pins. Pins used by the spi, will be setup up when \n
		 the module is initialized.
         
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{

int8_t rpi_gpio_init(void){
  	
		bcm2835_gpio_fsel(PIN_SS,  BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(PIN_PM0,  BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(PIN_PM1,  BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(PIN_IRQ0, BCM2835_GPIO_FSEL_INPT);
		bcm2835_gpio_fsel(PIN_IRQ1, BCM2835_GPIO_FSEL_INPT);
	    	
return 0;	
}

///@}



/*!---------------------------------------------------------------------------------
 @fn int8_t config_cmd(uint32_t cmd,uint8_t arg,...)
 
 @param [in] cmd         Indicates specific configuration procedure to be execute        
 @param [in] arg         Additional argument to control the execution 					
 @param [in,out] ...	 argument list attachment that is required by 
						 the procedure to be executed
 					 
 \brief  Function runs the selected configuration procedure on ADE7880.
 
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{
int8_t config_cmd(uint32_t cmd,uint8_t arg,...){
    
		   va_list va;
		   va_start(va,arg);
		   uint8_t cc = 0;
		   int8_t result = -1;
        switch(cmd)
        {
           
				case  ADE7880_SPI_PORT_ACTIVATE:
                {
				   /*!
					\page  file3 DEVICE CONFIGERATION 
				   	\brief \em ADE7880 SPI PORT ACTIVATION \n
					   \li TOGEL SS PIN 3 TIMES or execute three SPI write operations to a location in the address 
						   space that is not allocated to a specific register
					   \li Write to CONFIG2 register to lock the port.
				   */
                  	uint8_t ii;
                  	bool PIN_SET = 0;
                  	config2_reg_u config2_reg;
                  	
					
                	for(ii=0;ii<3/*TOGEL SS PIN 3 TIMES*/;ii++){
     
						if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,DUMMIY,0xFF,sizeof(uint8_t))!=0)
                	    	return -1; 
							
                	    usleep(1);
                	}
				      
					
					 uint8_t cc = 0;while(((result = config_cmd(SET_RAM_WR_PROTECTION,1,DISABLE))== -1)&& (cc++ < 3));if(result==-1)return -1; 
					 
					  config2_reg.reg_all=0;
	        
                	 config2_reg.reg_all = (uint8_t)spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,CONFIG2,sizeof(config2_reg.reg_all));
                	
             
                   //put it back, this will lock the spi port antill the next device reset  
                   	 if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,CONFIG2,config2_reg.reg_all,sizeof(config2_reg.reg_all))!=0)
                	    	return -1; 
							
                
                }break;
				
				
				  
				case DSP_RUN:
				{
				
					if(arg != 1)
					 return -1;
					                                	                	
				    uint8_t prt_arg = va_arg(va,int);
				    uint16_t ii;
				    for(ii=0;ii<3;ii++){
                    if(prt_arg == ENABLE){
                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,RUN,0x0001,sizeof(uint16_t))!=0)
                    return -1;
                    }else{
                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,RUN,0x0000,sizeof(uint16_t))!=0)
                    return -1;
                    }
                    }
                 
		
				}break;
				
				case  ADE7880_RESET:
                {
				
                	
                	
                }break;
                
                
				
				case  GAIN:
                {          
                	if(arg != 3)
                		return -1;
                	
                	gain_reg_u gain;
                	gain.reg_all = 0;
          
                	gain.bits.PGA1 = (pga_et)va_arg(va,int);
                	gain.bits.PGA2 = (pga_et)va_arg(va,int);
                	gain.bits.PGA3 = (pga_et)va_arg(va,int);
                       
                	if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,GAIN,gain.reg_all,sizeof(gain.reg_all))!=0)
                	return -1;
                	
                }break;
				
				
				case  POWER_MODE:
                { 
                	if(arg != 1)
                	  return -1;
                	
                    uint8_t mode = va_arg(va,int);
                	                    
                   if(ade7880_power_mode(mode)!=0)
                	    return -1;  
                	    
                }break;
				
				case VLEVEL:
                {
                	if(arg !=1)
                	  return -1;
                	                	
                	uint32_t vlevel_arg = va_arg(va,int);
                	                    
                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,VLEVEL,vlevel_arg,sizeof(uint32_t))!=0)
                        return -1; 
                }break;
                
                
				case COMPMODE:
	            {
	                	if(arg != 1)
	                	  return -1;
	                	                	
	                	uint16_t compmode_arg = va_arg(va,int);
	                	                    
	                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,COMPMODE,compmode_arg,sizeof(uint16_t))!=0)
	                        return -1; 
	            }break;
				
				
				case VNOM:
	            {
	                	if(arg != 1)
	                	  return -1;
	                	                	
	                	uint32_t vnom_arg = va_arg(va,int);
	                              
	                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,VNOM,vnom_arg,sizeof(uint32_t))!=0)
	                        return -1; 
	            }break;
	    	            
	            
				
				
				case  THRESHOLD:
                {
                	if(arg != 3)
                      return -1;
                                	                	
                     uint8_t wthr_arg = va_arg(va,int);
                     uint8_t varthr_arg = va_arg(va,int);
                     uint8_t vathr_arg = va_arg(va,int);
                     
                	 
                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,WTHR,WTHR_DEFULT,sizeof(uint8_t))!=0)
                	  return -1;
                	
					
                	
                    if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,VARTHR,VARTHR_DEFULT,sizeof(uint8_t))!=0)
                	  return -1;
                	
                	
                	if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,VATHR,VATHR_DEFULT,sizeof(uint8_t))!=0)
                	   return -1;
                
				
                }break;
                
                
                
               
                //Enables/Disable data memory protection
				case SET_RAM_WR_PROTECTION:
				{
				
					if(arg != 1)
					 return -1;
					                                	                	
				    uint8_t prt_arg = va_arg(va,int);
					 
             	       // spi_ram_protection(prt_arg,CHIP_ADDRESS1);  
			
				}break;
				
				
				
              

				
				
                default:
	            {    
	              // return -1;
                }break;
	 
        }
	 
	    va_end(va);
	    	
return 0;	
}

///@}

/*!---------------------------------------------------------------------------------
 @fn int8_t ade7880_power_mode(uint8_t mode)
 
 @param [in] mode         indicates the selected power mode       
	 
 \brief  Function to set up power mode of ADE7880.  
 
 \return function return 0 or -1, on success or failure respectively.
-----------------------------------------------------------------------------------*/
///@{
int8_t ade7880_power_mode(uint8_t mode){
    /*!
    \page  file3 DEVICE CONFIGERATION 
    \brief \em POWER MODE SELECTION  \n
	       The ADE7880 has four modes of operation, determined by the state of the PM0 and PM1 pins
		   \image html  power_mode.PNG
	       \image latex power_mode.PNG
		
    */
	
		 if(!bcm2835_init())
	       return -1;
		   
        switch(mode)
        {
                        
                case  PSM0:
                {    
				     //! Normal Power Mode
	               	 bcm2835_gpio_write(PIN_PM0, HIGH);     
	                 bcm2835_gpio_write(PIN_PM1, LOW);
	                  
                }break;
				
				 case  PSM1:
                {
				    //! Reduced Power Mode
	               	 bcm2835_gpio_write(PIN_PM0, LOW);     
	                 bcm2835_gpio_write(PIN_PM1, LOW);
                }break;
				
				 case  PSM2:
                {
	               //! Low Power Mode
	               	 bcm2835_gpio_write(PIN_PM0, LOW);     
	                 bcm2835_gpio_write(PIN_PM1, HIGH);
	             
                }break;
				

				 case  PSM3:
                {
	               //! Sleep Mode 
	               	 bcm2835_gpio_write(PIN_PM0, HIGH);     
	                 bcm2835_gpio_write(PIN_PM1, HIGH);
	                  
                }break;
				
                default:
	            {    
	               return -1;
                }break;
	 
        }
	 
	 //we may need to check the state of the pin's here to verify 
	    	
return 0;	
}
///@}

/*!---------------------------------------------------------------------------------
 @fn int8_t ade7880_config_reg_default(void)      
	 
 \brief  configure ADE7880 as 3-Phase, 4-Wire, Wye Distribution Systems energy meter \n
         with the default values .    
		 
 \return function return 0 or -1, on success or failure respectively .
-----------------------------------------------------------------------------------*/
///@{

int8_t ade7880_config_reg_default(void){ 
 
   /*!
    \page  file3 DEVICE CONFIGERATION 
           \brief \em ADE7880 DEFAULT CONFIGERATION \n
	\brief  This is a procedure adapted from the device datasheet section QUICK SETUP AS ENERGY METER 
	        based on the application.
		   \li Configer gain registers 
		   \li Initialize WTHR, VARTHR, VATHR, VLEVEL and VNOM registers based Equation 26, Equation 37, 
		       Equation 44, Equation 22, and Equation 42, respectively, Please see the datasheet \n
			   \ref ADE7880 http://www.analog.com/static/imported-files/data_sheets/ADE7880.pdf.
		   \li Enable the data memory RAM protection, by writing 0xAD to an internal 8-bit register located \n
		       at Address 0xE7FE, followed by a write of 0x80 to an internal 8-bit register located at Address 0xE7E3.
			   
	       \li Start the DSP by setting Run = 1.

	*/
	
	uint16_t cc=0;
	int8_t result=0;
	compmode_reg_u compmode_reg;

	
        	//SET gain registers 
        	if(config_cmd(GAIN,3,GAIN_1,GAIN_1,GAIN_1) != 0)return -1;
        	
        	if(config_cmd(THRESHOLD,3,WTHR_DEFULT,VARTHR_DEFULT,VATHR_DEFULT) != 0)return -1;
            
            if(spi_write(BCM2835_SPI_CS0,CHIP_ADDRESS1,CONFIG3,CONFIG3_DEFULT,sizeof(uint8_t))!=0)return -1; 
				
			#if _VNOMAEN
            compmode_reg.reg_all = 0x0001;	
            compmode_reg.bits.VNOMAEN    = 0;
            compmode_reg.bits.VNOMBEN    = 0;
            compmode_reg.bits.VNOMCEN    = 0;
         
		   
            if(config_cmd(COMPMODE,1,compmode_reg.reg_all) != 0)
            return -1;
			#endif
        
        	
            while(++cc<3)
            if(config_cmd(VLEVEL,1,VLEVEL_DEFULT) != 0)
            return -1;
      
        	
			 result=0;cc = 0;while(((result = config_cmd(SET_RAM_WR_PROTECTION,1,ENABLE))== -1)&& (cc++ < 3));if(result==-1)return -1;
	         result=0;cc = 0;while(((result = config_cmd(DSP_RUN,1,ENABLE))== -1)&& (cc++ < 3));if(result==-1)return -1;
        	
        	
                   
                
        return 0;	
 }
///@}
///@}



