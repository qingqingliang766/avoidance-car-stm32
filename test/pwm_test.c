#include "pwm_test.h"
#include "app_pwm.h"
#include "delay.h"

// @简介：对PWM模块进行测试
void PWM_Test(void) 
{
    // App_PWM_Init() 不要写在这里，我们统一放到 main 里面去初始化
    
    App_PWM_Cmd(1); // 唤醒电机

    App_PWM_Set_L(50); 
    App_PWM_Set_R(50); 
    Delay(5000);
    
    App_PWM_Set_L(50); 
    App_PWM_Set_R(-50); 
    Delay(2000); 
    
    App_PWM_Set_L(70); 
    App_PWM_Set_R(70); 
    Delay(2000);

		App_PWM_Set_L(50); 
    App_PWM_Set_R(-50); 
    Delay(2000); 
    
    App_PWM_Set_L(70); 
    App_PWM_Set_R(70); 
    Delay(2000); 
		
		App_PWM_Set_L(100); 
    App_PWM_Set_R(100); 
    Delay(2000); 
    
    App_PWM_Cmd(0); // 休眠电机
    // 注意：把 while(1){} 
}

