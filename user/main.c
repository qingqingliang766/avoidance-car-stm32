#include "stm32f10x.h"
#include "app_button.h" // 只需要引入应用层头文件
#include "app_pwm.h"

int main(void) {
    
    // 1. 唤醒按键部门，完成所有初始化工作
    App_Button_Init(); 
	  App_PWM_Init();
	
    while (1) {
        // 2. 让按键部门自己去后台盯着，有单击情况它会自己开灯关灯
        App_Button_Proc(); 
        
        // 老板这里还可以安排其他部门的工作
        // App_Motor_Proc();
        // App_Screen_Proc();
    } 
}

