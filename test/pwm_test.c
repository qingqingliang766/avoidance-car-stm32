#include "pwm_test.h"
#include "app_pwm.h"
// 把 #include "delay.h" 删掉！

// 引入咱们按键系统里一直在跑的“全局手表”
extern volatile uint32_t sys_tick_ms; 

// 这是我给你写的专属 Delay 函数！完美兼容按键！
void Delay(uint32_t ms) {
    uint32_t start_time = sys_tick_ms; // 记录当下的时间
    // 只要流逝的时间还没到设定的 ms 数，就原地死循环等待
    while((sys_tick_ms - start_time) < ms); 
}

// @简介：对PWM模块进行测试
void PWM_Test(void) 
{
    // 注意：这里不需要再调用什么 Delay_Init() 了！
    // 因为按键系统的 App_Button_Init() 里面早就把时间初始化好了。
    
    App_PWM_Init();

    App_PWM_Cmd(1); // 唤醒电机

    App_PWM_Set_L(30); // 90%正转
    App_PWM_Set_R(30); // 90%正转
    Delay(5000); // 延迟5s，完美生效！
    
    App_PWM_Set_L(40); 
    App_PWM_Set_R(40); 
    Delay(2000); 
    
    App_PWM_Set_L(90); 
    App_PWM_Set_R(90); 
    Delay(2000); 
    
    App_PWM_Cmd(0); // 休眠电机
    while(1) {}
}