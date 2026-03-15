#include "app_button.h"
#include "my_button.h" // 引入底层的按键驱动大脑
#include "app_pwm.h"



// --- 内部全局变量（不对外暴露） ---
volatile uint32_t sys_tick_ms = 0; // 为按键提供时间基准
Button_TypeDef myButton1; // 【员工 1】按键1对象
Button_TypeDef myButton2; // 【员工 2】按键2对象


// --- 1. 底层硬件读取函数 ---
uint8_t Read_Button1_Pin(void) {
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) {
        return 1; // 按下
    } else {
        return 0; // 松开
    }
}
uint8_t Read_Button2_Pin(void) {
    // 按键 2 接在 PA2 (避开被电机占用的 PA1)
    return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == Bit_RESET) ? 1 : 0; 
}


// --- 2. 硬件引脚初始化 ---
void Hardware_Config(void) {
    SysTick_Config(SystemCoreClock / 1000); // 1ms 中断
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;

    // 按键引脚 PA0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_2;
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
void On_Button1_Clicked(uint8_t clicks) {
    if (clicks == 1) {
        if(pwm_on == 0) pwm_on = 1;
        else pwm_on = 0;
        
        App_PWM_Cmd(pwm_on);
        
        if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) {
            GPIO_ResetBits(GPIOC, GPIO_Pin_13); 
        } else {
            GPIO_SetBits(GPIOC, GPIO_Pin_13);   
        }
    }
}
// 按键 2 的工作：未来可以用来干别的（比如调速）
void On_Button2_Clicked(uint8_t clicks) {
    if (clicks == 1) {
        // 留空，或者你可以让它双击闪灯
    }
}

// ==========================================
// --- 以下是对外提供给 main.c 调用的接口 ---
// ==========================================

void App_Button_Init(void) {
    Hardware_Config(); // 1. 硬件引脚初始化

    Button_InitTypeDef init_struct;
    init_struct.DebounceTime = 20;               

    // 2. 录入按键 1 (签合同)
    init_struct.Read_Pin_Level = Read_Button1_Pin; 
    My_Button_Init(&myButton1, &init_struct);
    My_Button_SetClickCb(&myButton1, On_Button1_Clicked);

    // 3. 录入按键 2 (签合同)
    init_struct.Read_Pin_Level = Read_Button2_Pin; 
    My_Button_Init(&myButton2, &init_struct);
    My_Button_SetClickCb(&myButton2, On_Button2_Clicked);
}

void App_Button_Proc(void) {
    // 喂入当前时间戳，让两个按键各自运转！互不干扰！
    My_Button_Proc(&myButton1, sys_tick_ms);
    My_Button_Proc(&myButton2, sys_tick_ms);
}
