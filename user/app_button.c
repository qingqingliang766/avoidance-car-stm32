#include "app_button.h"
#include "button.h"
#include "delay.h"
#include "app_pwm.h"

Button_TypeDef myButton;
static uint8_t pwm_on = 0; 

// 业务逻辑：按钮被单击时的回调函数
void On_Button_Clicked(uint8_t clicks) {
    if (clicks == 1) {
        pwm_on = !pwm_on; // 状态翻转
        
        App_PWM_Cmd(pwm_on); // 唤醒/休眠电机驱动
        
        if (pwm_on) {
            App_PWM_Set_L(40); // 启动时默认给个中速
            App_PWM_Set_R(40);
        } else {
            App_PWM_Set_L(0);  // 停止
            App_PWM_Set_R(0);
        }
    }
}

// 给 main.c 调用的按键初始化接口
void App_Button_Init(void) {
    Button_InitTypeDef init_struct = {0}; // 清空配置表
    
    // 要求的配置表
    init_struct.GPIOx = GPIOA;          // 按键所在的端口
    init_struct.GPIO_Pin = GPIO_Pin_0;  // 按键的引脚 (PA0)
    init_struct.button_clicked_cb = On_Button_Clicked; // 绑定业务逻辑
    
    init_struct.LongPressTime = 1000;
    init_struct.ClickInterval = 200;
    init_struct.LongPressTickInterval = 100;
    
    // 把配置表交给初始化函数
    My_Button_Init(&myButton, &init_struct);
}

// 给 main.c 调用的轮询接口
void App_Button_Proc(void) {
    // 代码内部会自己调用 GetTick()，不需要传时间了
    My_Button_Proc(&myButton);
}

