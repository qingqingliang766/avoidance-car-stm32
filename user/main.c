#include "stm32f10x.h"
#include "delay.h"     
#include "app_button.h"  
#include "app_pwm.h"
#include "pwm_test.h"

int main(void) 
{
    // 系统上电
    Delay_Init();  
    
    // 2. 唤醒各个部门 
    App_Button_Init();  
    App_PWM_Init();
    
    // 3.开机先自动跑一段电机的调速测试 
    // 测试时电机自动转，测试完电机会停下
    PWM_Test();
    
    // 4. 正式进入后台巡逻：监听按键
    // 可以通过按 PA0 按键，自由控制电机的启停
    while (1) {
        App_Button_Proc(); 
    } 
}

