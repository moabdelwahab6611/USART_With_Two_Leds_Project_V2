/* 
 * File     : application.c
 * Author   : Mohamed Ahmed Abdel Wahab
 * LinkedIn : https://www.linkedin.com/in/mohamed-abdel-wahab-162413253/
 * Github   : https://github.com/moabdelwahab6611
 * Created on May 21, 2023, 5:38 PM
 */

/**************************Includes-Section*****************************/
#include "application.h"
#include "MCAL_Layer/USART/hal_usart.h"
/***********************************************************************/

/***********************************************************************/
uint8 receive_usart_data;
led_t led1 = {.port_name = PORTD_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};
led_t led2 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN1, .led_status = GPIO_LOW};
volatile uint32 valid_usart_tx;
volatile uint32 valid_usart_rx;

void usart_module_init(void)
{
    Std_ReturnType ret = E_NOT_OK;
    usart_t usart_obj;
    
    usart_obj.USART_FramingErrorHandler = NULL;
    usart_obj.USART_OverrunErrorHandler = NULL;
    usart_obj.USART_RxDefaultInterruptHandler = NULL;
    usart_obj.USART_TxDefaultInterruptHandler = NULL;
    
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_config = BAUDRATE_ASYNCHRONOUS_MODE_8BIT_LOW_SPEED;
    
    usart_obj.usart_tx_cfg.usart_enable_tx = USART_ENABLE_ASYNCHRONOUS_MODE_TX;
    usart_obj.usart_tx_cfg.usart_enable_tx_interrupt = USART_DISABLE_ASYNCHRONOUS_MODE_TX_INTERRUPT;
    usart_obj.usart_tx_cfg.usart_enable_9bit_tx = USART_DISABLE_ASYNCHRONOUS_MODE_9BIT_TX;
   
    usart_obj.usart_rx_cfg.usart_enable_rx = USART_ENABLE_ASYNCHRONOUS_MODE_RX;
    usart_obj.usart_rx_cfg.usart_enable_rx_interrupt = USART_DISABLE_ASYNCHRONOUS_MODE_RX_INTERRUPT;
    usart_obj.usart_rx_cfg.usart_enable_9bit_rx = USART_DISABLE_ASYNCHRONOUS_MODE_9BIT_RX;
    
    ret = USART_Asynchronous_Initialize(&usart_obj);
}

void USART_FramingDefaultErrorHandler(void)
{
    uint8 _data_;
    USART_Asynchronous_ReadByte_NonBlocking(&_data_);
}

void USART_OverrunDefaultErrorHandler(void)
{
    USART_Asynchronous_Restart_RX();
}

void USART_TxDefaultInterruptHandler(void)
{
    valid_usart_tx++;
}

void USART_RxDefaultInterruptHandler(void)
{
    Std_ReturnType ret = E_NOT_OK;
    valid_usart_rx++;
    ret = USART_Asynchronous_ReadByte_NonBlocking(&receive_usart_data);
    switch(receive_usart_data)
    {
        case 'a' : led_turn_on(&led1); 
        ret = USART_Asynchronous_WriteString_Blocking("Led1 On\r", 8);
        break;
        case 'b' : led_turn_off(&led1); 
         ret = USART_Asynchronous_WriteString_Blocking("Led1 Off\r", 9);
        break;
        case 'c' : led_turn_on(&led2); 
        ret = USART_Asynchronous_WriteString_Blocking("Led2 On\r", 8);
        break;
        case 'd' : led_turn_off(&led2); 
        ret = USART_Asynchronous_WriteString_Blocking("Led2 Off\r", 9);
        break;
        default : led_turn_off(&led1); 
                  led_turn_off(&led2);
    }        
}

void usart_module_int_init(void)
{
    Std_ReturnType ret = E_NOT_OK;
    usart_t usart_obj;
    
    usart_obj.USART_FramingErrorHandler = USART_FramingDefaultErrorHandler;
    usart_obj.USART_OverrunErrorHandler = USART_OverrunDefaultErrorHandler;
    usart_obj.USART_RxDefaultInterruptHandler = USART_RxDefaultInterruptHandler;
    usart_obj.USART_TxDefaultInterruptHandler = USART_TxDefaultInterruptHandler;
    
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_config = BAUDRATE_ASYNCHRONOUS_MODE_8BIT_LOW_SPEED;
    
    usart_obj.usart_tx_cfg.usart_enable_tx = USART_ENABLE_ASYNCHRONOUS_MODE_TX;
    usart_obj.usart_tx_cfg.usart_enable_tx_interrupt = USART_ENABLE_ASYNCHRONOUS_MODE_TX_INTERRUPT;
    usart_obj.usart_tx_cfg.usart_enable_9bit_tx = USART_DISABLE_ASYNCHRONOUS_MODE_9BIT_TX;
   
    usart_obj.usart_rx_cfg.usart_enable_rx = USART_ENABLE_ASYNCHRONOUS_MODE_RX;
    usart_obj.usart_rx_cfg.usart_enable_rx_interrupt = USART_ENABLE_ASYNCHRONOUS_MODE_RX_INTERRUPT;
    usart_obj.usart_rx_cfg.usart_enable_9bit_rx = USART_DISABLE_ASYNCHRONOUS_MODE_9BIT_RX;
    
    ret = USART_Asynchronous_Initialize(&usart_obj);
}
/***********************************************************************/

/***********************Main Function-Section***************************/
int main() 
{ 
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();
    usart_module_int_init();     
    ret = led_initialize(&led1);
    ret = led_initialize(&led2);
    
    while(1)
    {   
        // ret = USART_Asynchronous_WriteString_Blocking("Mohamed", 7);
    }
    return (EXIT_SUCCESS);
}
/***********************************************************************/

/*************************Functions-Section*****************************/
void application_intialize(void)
{
    Std_ReturnType ret = E_NOT_OK;   
    ecu_layer_intialize();
}
/***********************************************************************/