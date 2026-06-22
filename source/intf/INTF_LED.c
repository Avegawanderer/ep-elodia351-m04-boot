#include "app_config.h"
#include "INTF_LED.h"
#include "app_menu.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* LED2 Port/Pin definition */
//#define  LED2_PORT          (PortB)
//#define  LED2_PIN           (Pin05)

/* LED3 Port/Pin definition */
//#define  LED3_PORT          (PortB)
//#define  LED3_PIN           (Pin09)

/* LED0~3 toggle definition */
//#define  LED0_TOGGLE()      (PORT_Toggle(LED_GREEN_PORT, LED_GREEN_PIN))
//#define  LED1_TOGGLE()      (PORT_Toggle(LED1_PORT, LED1_PIN))
//#define  LED2_TOGGLE()      (PORT_Toggle(LED2_PORT, LED2_PIN))
//#define  LED3_TOGGLE()      (PORT_Toggle(LED3_PORT, LED3_PIN))

/* uncomment this line if wants to print information to Terminal I/O window */
//#define  __PRINT_TO_TERMINAL

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void LED_INTF_Init(void)
{
    stc_port_init_t stcPortInit;

    /*initiallize LED port*/
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enPinMode = Pin_Mode_Out;
    stcPortInit.enExInt = Disable;
    stcPortInit.enPullUp = Enable;

    /* LED0 Port/Pin initialization */
    PORT_Init(LED_GREEN_PORT, LED_GREEN_PIN, &stcPortInit);

    /* LED1 Port/Pin initialization */
    PORT_DebugPortSetting(TDO_SWO, Disable);
    //	PORT_SetFunc(LED_RED_PORT, LED_RED_PIN,Func_Gpio, Disable);
    PORT_Init(LED_RED_PORT, LED_RED_PIN, &stcPortInit);
    //	PB3
    //EIRQ3/WK
    //UP0_3
    //JTDO_TRACE
    //SWO
}
