#include "app_button.h"
#include "my_button.h" // 引入底层的按键驱动大脑
#include "app_pwm.h"

// --- 内部全局变量（不对外暴露） ---
volatile uint32_t sys_tick_ms = 0; // 为按键提供时间基准
Button_TypeDef myButton;           // 实体化一个按键对象

// --- 1. 底层硬件读取函数 ---
uint8_t Read_Button_Pin(void) {
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET) {
        return 1; // 按下
    } else {
        return 0; // 松开
    }
}

// --- 2. 硬件引脚初始化 ---
void Hardware_Config(void) {
    SysTick_Config(SystemCoreClock / 1000); // 1ms 中断
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;

    // 按键引脚 PA0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 测试灯引脚 PC13
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOC, GPIO_Pin_13); // 默认关灯
}

static uint8_t pwm_on = 0; ///0 - 电机休眠，1 - 电机活动

// --- 3. 业务逻辑回调函数 ---
void On_Button_Clicked(uint8_t clicks) {
    if (clicks == 1) {
			//翻转电机状态
        if(pwm_on == 0)
				{
					pwm_on = 1;
				}
				else
				{
					pwm_on = 0;
				}
				App_PWM_Cmd(pwm_on);
    }
}

// ==========================================
// --- 以下是对外提供给 main.c 调用的接口 ---
// ==========================================

void App_Button_Init(void) {
    // 1. 先初始化引脚
    Hardware_Config();

    // 2. 配置按键驱动大脑
    Button_InitTypeDef init_struct;
    init_struct.Read_Pin_Level = Read_Button_Pin; 
    init_struct.DebounceTime = 20;               
    My_Button_Init(&myButton, &init_struct);

    // 3. 绑定单击动作
    My_Button_SetClickCb(&myButton, On_Button_Clicked);
}

void App_Button_Proc(void) {
    // 喂入当前时间戳，驱动状态机运转
    My_Button_Proc(&myButton, sys_tick_ms);
}
