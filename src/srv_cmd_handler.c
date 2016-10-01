/*!\file srv_cmd_handler.c
   \brief \e File:  srv_cmd_handler.c \n
   \brief \e Purpose: \n    
*/

#include "../includes/includes.h"


/*!---------------------------------------------------------------------------------
 \defgroup SERVICE
		
-----------------------------------------------------------------------------------*/
///@{

/*!

    \page  file4 SERVICE  
	\brief This is service module containing recommended measurement reading \n
	       procedures used when fetching measured data form ADE7880.And also, \n
		   additional service functions used throught the program are included here
	       
	     
*/

/*!---------------------------------------------------------------------------------
 @fn float measure(uint8_t cmd,uint8_t channel,float samples)
 
 @param [in] cmd         Indicates the target register value to read          
 @param [in] channel     Indicates the target channel 					
 @param [in] samples	 Indicates the number of samples read by the function 
 					 
 \brief  The Function runs recommended reading procedures designed, 
	     to enhance the accuracy of measurements by taking the average of readings 
		 taken at different instance.   
		 
 \return on success  returns the avarge of the measured value, on failure return -1 .
 
-----------------------------------------------------------------------------------*/
///@{
float measure(uint8_t cmd,uint8_t channel,float samples){


    float measured_val=0;


         switch(cmd)
        {
           
				case  PHASE_VRMS:
                {
								
					uint16_t ii;
					uint16_t target_reg =  (channel==PHASE_A )?AVRMS
										  :(channel==PHASE_B )?BVRMS
										  :(channel==PHASE_C )?CVRMS
										  :0;								
					
					if(target_reg==0)return -1;							
					for(ii=0;ii<samples;ii++){
					if(wait_new_conversion()==-1)return -1;
					measured_val += (uint32_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,target_reg,sizeof(uint32_t))&0xFFFFFF)*TRANSFORMER_RATIO; 
					if(ii%50 == 0)printf("#\n");else printf("#");
					}
					measured_val /=samples;
					
			
					return measured_val/VLSB_CONST;
				
                }break;

				case  PHASE_IRMS:
                {
					uint16_t ii;
					uint16_t target_reg =  (channel==PHASE_A )?AIRMS
										  :(channel==PHASE_B )?BIRMS
										  :(channel==PHASE_C )?CIRMS
										  :0;
					if(target_reg==0)return -1;		
					for(ii=0;ii<samples;ii++){
					if(wait_new_conversion()==-1)return -1;
					measured_val += spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,target_reg,sizeof(uint32_t)); 
					if(ii%50 == 0)printf("#\n");else printf("#");
					
					}
					measured_val /=samples;
					
					return measured_val/ILSB_CONST;
				
                }break;
				
				
			    case  PHASE_ACTIVE_WH:/*considers also harmoics , trouble? then we change it to fundamental*/
                {
			
					uint16_t target_reg =  (channel==PHASE_A )?AWATTHR
										  :(channel==PHASE_B )?BWATTHR
										  :(channel==PHASE_C )?CWATTHR
										  :0;			
										  
				    measured_val = spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,target_reg,sizeof(uint32_t));
					printf("#");
					return measured_val/WHLSB_CONST;
				
                }break;
				
				case  TOTAL_ACTIVE_WH:/*considers also harmoics */
                {
					
					uint16_t target_reg =  (channel==PHASE_A )?AWATTHR
										  :(channel==PHASE_B )?BWATTHR
										  :(channel==PHASE_C )?CWATTHR
										  :0;			
					
					 return  measure(PHASE_ACTIVE_WH,PHASE_A,1)
					        +measure(PHASE_ACTIVE_WH,PHASE_B,1)
							+measure(PHASE_ACTIVE_WH,PHASE_C,1);
					 
				
                }break;
				
			    case  PHASE_ACTIVE_POWER:
                {
					uint16_t target_reg =    (channel==PHASE_A )?AWATT
											:(channel==PHASE_B )?BWATT
									        :(channel==PHASE_C )?CWATT
									        :0;			
										  
				    measured_val = spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,target_reg,sizeof(uint32_t));
					printf("#");
					return measured_val/WATTLSB_CONST;
					
                }break;
				
				case  TOTAL_ACTIVE_POWER:
                {
				
							
					 return measure(PHASE_ACTIVE_POWER,PHASE_A,1)
					        +measure(PHASE_ACTIVE_POWER,PHASE_B,1);
							+measure(PHASE_ACTIVE_POWER,PHASE_C,1);
					
                }break;
				
                default:
	            {    
	              // return -1;
                }break;
	 
        }
}
///@}
 
 
/*!---------------------------------------------------------------------------------
 @fn uint16_t  hex2val(uint8_t cc)
 
 @param [in] cc        Hexadecimal character value to be converted to integer       
	 
 \brief  Function converts hex character to the corresponding integer value.  
 
 \return on success returns the resulting 16 bit unsigned integer value.
 
-----------------------------------------------------------------------------------*/
///@{
uint16_t  hex2val(uint8_t cc)
{
	return  (uint16_t)(((cc >= '0' ) && (cc <= '9' )) ? (cc - '0') : (cc - 'A' + 10) );
     
}
///@}

/*!---------------------------------------------------------------------------------
 @fn uint16_t  make16(uint8_t* buf,uint16_t idx)
 @param [in] buf  pointer to Hexadecimal character buffer        
 @param [in] idx  index to first character of 16 bit hex number string \n 
                  (four charaters used).
				  
 \brief  Function converts string of hex character to the corresponding 16 bit 
		 unsigned integer value.
		 
 \return on success returns the resulting 16 bit unsigned integer value.
 
-----------------------------------------------------------------------------------*/
///@{
uint16_t  make16(uint8_t* buf,uint16_t idx)
{
    return ((hex2val(buf[idx+0])<<12)|(hex2val(buf[idx+1])<<8)|(hex2val(buf[idx+2])<<4)|(hex2val(buf[idx+3])<<0));    

}
///@}

/*!---------------------------------------------------------------------------------
 @fn uint8_t  make8(uint8_t* buf,uint16_t idx)
 
 @param [in] buf  pointer to Hexadecimal character buffer        
 @param [in] idx  index to first character of 16 bit hex number string \n 
                  (two charaters used)	 
				  
 \brief  Function converts string of hex character to the corresponding 8 bit 
		 unsigned integer value.
		 
 \return on success returns the resulting 8 bit unsigned integer value.
 
-----------------------------------------------------------------------------------*/
///@{

uint8_t  make8(uint8_t* buf,uint16_t idx)
{
    return (uint8_t)((hex2val(buf[idx+0])<<4)|(hex2val(buf[idx+1]))<<0);    
}

///@}

///@}
