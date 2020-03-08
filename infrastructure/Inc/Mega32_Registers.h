/****************************************************************************
 * @file  mega32_regsiters.h
 * @brief this file includes some macros to dereference hardware addresses
 *
 * @author Abdelmonem Mostafa
 * @date  OCT 2019
 *
 ****************************************************************************/


#ifndef MEGA32_REGISTER_H_
#define MEGA32_REGISTER_H_
#include "gpio.h"


/************************************************************************/
/* IO registers definitions                                                                     */
/************************************************************************/

#define OUT_A												(*(reg8_type) 0x3B)
#define IN_A												(*(reg8_type) 0x39)
#define DIR_A												(*(reg8_type) 0x3A)

#define OUT_B												(*(reg8_type) 0x38)
#define IN_B												(*(reg8_type) 0x36)
#define DIR_B												(*(reg8_type) 0x37)

#define OUT_C												(*(reg8_type) 0x35)
#define IN_C												(*(reg8_type) 0x33)
#define DIR_C												(*(reg8_type) 0x34)

#define OUT_D												(*(reg8_type) 0x32)
#define IN_D												(*(reg8_type) 0x30)
#define DIR_D												(*(reg8_type) 0x31)


/************************************************************************/
/* Timer0 registers                                                                     */
/************************************************************************/
#define TIMER0_CTL_REG										(*(reg8_type) 0x53)
#define TIMER0_COUNTER_REG									(*(reg8_type) 0x52)
#define TIMER0_COMPARE_REG									(*(reg8_type) 0x5C)



#define TIMER0_OVF_FLAG										BIT0
#define TIMER0_OC_FLAG										BIT1

#define TIMER0_OVF_INT_EN									BIT0
#define TIMER0_OC_INT_EN									BIT1

#define TIMER0_OC_PIN										BIT3
#define TIMER0_OC_PIN_PORT									PORT_B


#define TIMER0_EXT_CLK_PIN  								BIT0
#define TIMER0_EXT_CLK_PIN_PORT								PORT_B



/************************************************************************/
/* Timer1 registers                                                                     */
/************************************************************************/
#define TIMER1A_CTL_REG										(*(reg8_type) 0x4F)
#define TIMER1B_CTL_REG										(*(reg8_type) 0x4E)
#define TIMER1_COUNTER_REG									(*(reg16_type) 0x4C)
#define TIMER1_COMPARE_A_REG								(*(reg16_type) 0x4A)
#define TIMER1_COMPARE_B_REG								(*(reg16_type) 0x48)
#define TIMER1_INPUT_CAPTURE_REG							(*(reg16_type) 0x46)



#define TIMER1_OVF_FLAG										BIT2
#define TIMER1_OCB_FLAG										BIT3
#define TIMER1_OCA_FLAG										BIT4
#define TIMER1_IC_FLAG										BIT5

#define TIMER1_OVF_INT_EN									BIT2
#define TIMER1_OCB_INT_EN									BIT3
#define TIMER1_OCA_INT_EN									BIT4
#define TIMER1_IC_INT_EN									BIT5

#define TIMER1_OCA_PIN										BIT5
#define TIMER1_OCA_PIN_PORT									PORT_D


#define TIMER1_OCB_PIN										BIT4
#define TIMER1_OCB_PIN_PORT									PORT_D

#define TIMER1_EXT_CLK_PIN							      	BIT1
#define TIMER1_EXT_CLK_PIN_PORT	     						PORT_B

/************************************************************************/
/*					TIMER2                                              */
/************************************************************************/


#define TIMER2_CTL_REG										(*(reg8_type) 0x45)
#define TIMER2_COUNTER_REG									(*(reg8_type) 0x44)
#define TIMER2_COMPARE_REG									(*(reg8_type) 0x43)



#define TIMER2_OVF_FLAG										BIT6
#define TIMER2_OC_FLAG										BIT7

#define TIMER2_OVF_INT_EN									BIT6
#define TIMER2_OC_INT_EN									BIT7

#define TIMER2_OC_PIN										BIT7
#define TIMER2_OC_PIN_PORT									PORT_D






/************************************************************************/
/*				interrupt registers                                                                     */
/************************************************************************/

#define STATUS_REG											(*(reg8_type) 0x5F)
#define TIMER_MSK_REG										(*(reg8_type) 0x59)
#define TIMER_INTERRUPT_FLAGS_REG							(*(reg8_type) 0x58)
#define MCU_CTL												(*(reg8_type) 0x55)

#define INT_GCTL											(*(reg8_type) 0x5B)



#define INT_GLOBAL_FLAG										BIT7



/************************************************************************/
/*						UART Registers                                   */
/************************************************************************/

#define	USART_BAUDRATED_LOW_REG								(*(reg8_type) 0x29)
#define	USART_BAUDRATED_HIGH_REG							(*(reg8_type) 0x40)

#define	USART_CONTROL_STATUS_B_REG							(*(reg8_type) 0x2A)
#define	USART_CONTROL_STATUS_A_REG							(*(reg8_type) 0x2B)
#define	USART_CONTROL_STATUS_C_REG							(*(reg8_type) 0x40)

#define	USART_DATA_REG										(*(reg8_type) 0x2C)


/************************************************************************/
/*					SPI registers                                       */
/************************************************************************/

#define SPI_CTL_REG					(*(reg8_type) 0x2D)
#define SPI_STATUS_REG				(*(reg8_type) 0x2E)
#define SPI_DATA_REG				(*(reg8_type) 0x2F)



/************************************************************************/
/*            commons macros                                             */
/************************************************************************/
#define REG_HIGH_BYTE(data)		( (uint8)(data>>8) )
#define REG_LOW_BYTE(data)		( (uint8)data )

#endif /* MEGA32_REGISTER_H_ */