
/*!\file rpi_hwio.h
   \brief \e File:  rpi_hwio.h \n
   \brief \e Purpose: \n 
*/

#ifndef __RPI_GPIO_H
#define __RPI_GPIO_H

  #define PIN_SS   RPI_GPIO_P1_24  		/*!<Rpi chip select pin */ 
  #define PIN_SS2  RPI_GPIO_P1_26       /*!<Rpi second chip select pin*/ 
  
  #define PIN_PM0  RPI_GPIO_P1_11       /*!<ADE7880 PM0 connection pin*/ 
  #define PIN_PM1  RPI_GPIO_P1_13       /*!<ADE7880 PM1 connection pin*/ 
  #define PIN_IRQ0 RPI_GPIO_P1_16       /*!<ADE7880 IRQ0 connection pin*/ 
  #define PIN_IRQ1 RPI_GPIO_P1_18       /*!<ADE7880 IRQ1 connection pin*/ 
  
  
  int8_t  rpi_gpio_init();
  int8_t  ade7880_power_mode(uint8_t mode);
  int8_t  ade7880_config_reg_default();
  int8_t  config_cmd(uint32_t cmd,uint8_t arg,...);

#endif

