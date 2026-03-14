#ifndef __APP_BUTTON_H__
#define __APP_BUTTON_H__

#include "stm32f10x.h"

// 对外提供的“极简”接口
void App_Button_Init(void); // 按键系统初始化
void App_Button_Proc(void); // 按键系统后台轮询

#endif /* __APP_BUTTON_H__ */

