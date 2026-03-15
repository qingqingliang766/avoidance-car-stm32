#include "stm32f10x.h"
#include "app_button.h" 
#include "app_pwm.h"
#include "pwm_test.h"

int main(void) 
{
    // 1. 【必须先做】唤醒所有部门，尤其是把时间心跳（SysTick）跑起来！
    App_Button_Init();  
    App_PWM_Init();
    
    // 2. 【然后再做】现在手表在走了，Delay() 函数就不会卡死了
    PWM_Test();
    
    while (1) {
        App_Button_Proc(); 
    } 
}