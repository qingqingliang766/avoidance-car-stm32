#include "pwm_test.h"
#include "app_pwm.h"
#include "delay.h" // 引入山羊大佬的库

// @简介：对PWM模块进行测试
void PWM_Test(void) 
{
    // App_PWM_Init() 不要写在这里，我们统一放到 main 里面去初始化
    
    App_PWM_Cmd(1); // 唤醒电机

    App_PWM_Set_L(30); 
    App_PWM_Set_R(30); 
    Delay(5000); // 完美调用大佬的延时！
    
    App_PWM_Set_L(40); 
    App_PWM_Set_R(40); 
    Delay(2000); 
    
    App_PWM_Set_L(90); 
    App_PWM_Set_R(90); 
    Delay(2000); 
    
    App_PWM_Cmd(0); // 休眠电机
    // 注意：把 while(1){} 删掉了！测完就溜，把控制权还给主函数！
}

