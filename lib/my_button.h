#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__

#include <stdint.h>

// 按钮初始化配置结构体
typedef struct {
    uint8_t (*Read_Pin_Level)(void); // 读取底层硬件引脚电平的函数指针 (返回 0 或 1)
    uint32_t DebounceTime;           // 消抖时间 (毫秒)
} Button_InitTypeDef;

// 按钮核心结构体
typedef struct {
    uint8_t  LastState;             // 按钮上次的状态，0 - 松开，1 - 按下
    uint8_t  ChangePending;         // 按钮的状态是否正在发生改变
    uint32_t PendingTime;           // 按钮状态开始变化的时间
    uint32_t LastPressedTime;       // 按钮上次按下的时间
    uint32_t LastReleasedTime;      // 按钮上次松开的时间
    uint8_t  LongPressTicks;        // 长按触发的次数
    uint32_t LastLongPressTickTime; // 上次长按触发的时间
    uint8_t  ClickCnt;              // 连击次数

    uint32_t ClickInterval;         // 连击判定的最大间隔时间
    uint32_t LongPressThreshold;    // 触发长按所需的时间阈值
    uint32_t LongPressTickInterval; // 长按期间连续触发的间隔时间
    uint32_t DebounceTime;          // 消抖时间

    uint8_t (*Read_Pin_Level)(void); 
    void (*PressCb)(void);
    void (*ReleaseCb)(void);
    void (*ClickCb)(uint8_t clicks);
    void (*LongPressCb)(uint8_t ticks);
} Button_TypeDef;

// 函数声明
void My_Button_Init(Button_TypeDef *Button, Button_InitTypeDef *Button_InitStruct);
void My_Button_Proc(Button_TypeDef *Button, uint32_t CurrentTimeMs);
uint8_t MyButton_GetState(Button_TypeDef *Button);

void My_Button_SetLongPressCb(Button_TypeDef *Button, void (*LongPressCb)(uint8_t ticks));
void My_Button_SetPressCb(Button_TypeDef *Button, void (*button_pressed_cb)(void));
void My_Button_SetReleaseCb(Button_TypeDef *Button, void (*button_released_cb)(void));
void My_Button_SetClickCb(Button_TypeDef *Button, void (*button_clicked_cb)(uint8_t clicks));

void My_Button_ClickIntervalConfig(Button_TypeDef *Button, uint32_t Interval);
void My_Button_LongPressConfig(Button_TypeDef *Button, uint32_t Threshold, uint32_t TickInterval);

#endif /* __MYBUTTON_H__ */

