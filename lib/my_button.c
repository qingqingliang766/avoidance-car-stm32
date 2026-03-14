#include "my_button.h"
#include <stddef.h>

void My_Button_Init(Button_TypeDef *Button, Button_InitTypeDef *Button_InitStruct) {
    if (Button == NULL || Button_InitStruct == NULL) return;

    Button->LastState = 0;
    Button->ChangePending = 0;
    Button->PendingTime = 0;
    Button->LastPressedTime = 0;
    Button->LastReleasedTime = 0;
    Button->LongPressTicks = 0;
    Button->LastLongPressTickTime = 0;
    Button->ClickCnt = 0;

    Button->ClickInterval = 250;         // 默认连击间隔 250ms
    Button->LongPressThreshold = 1000;   // 默认长按阈值 1000ms
    Button->LongPressTickInterval = 100; // 默认长按触发间隔 100ms
    Button->DebounceTime = Button_InitStruct->DebounceTime;
    
    Button->Read_Pin_Level = Button_InitStruct->Read_Pin_Level;

    Button->PressCb = NULL;
    Button->ReleaseCb = NULL;
    Button->ClickCb = NULL;
    Button->LongPressCb = NULL;
}

void My_Button_Proc(Button_TypeDef *Button, uint32_t CurrentTimeMs) {
    if (Button == NULL || Button->Read_Pin_Level == NULL) return;

    uint8_t current_level = Button->Read_Pin_Level();

    // 1. 消抖逻辑
    if (current_level != Button->LastState && !Button->ChangePending) {
        Button->ChangePending = 1;
        Button->PendingTime = CurrentTimeMs;
    }

    if (Button->ChangePending && (CurrentTimeMs - Button->PendingTime >= Button->DebounceTime)) {
        Button->ChangePending = 0;
        if (current_level != Button->LastState) {
            Button->LastState = current_level;
            
            if (current_level == 1) { 
                Button->LastPressedTime = CurrentTimeMs;
                if (Button->PressCb) Button->PressCb();
            } else { 
                Button->LastReleasedTime = CurrentTimeMs;
                if (Button->ReleaseCb) Button->ReleaseCb();

                if ((CurrentTimeMs - Button->LastPressedTime) < Button->LongPressThreshold) {
                    Button->ClickCnt++;
                }
                Button->LongPressTicks = 0; 
            }
        }
    }

    // 2. 长按逻辑处理
    if (Button->LastState == 1) { 
        if ((CurrentTimeMs - Button->LastPressedTime) >= Button->LongPressThreshold) {
            if ((CurrentTimeMs - Button->LastLongPressTickTime) >= Button->LongPressTickInterval) {
                Button->LongPressTicks++;
                Button->LastLongPressTickTime = CurrentTimeMs;
                if (Button->LongPressCb) {
                    Button->LongPressCb(Button->LongPressTicks);
                }
            }
        }
    }

    // 3. 连击逻辑处理
    if (Button->LastState == 0 && Button->ClickCnt > 0) { 
        if ((CurrentTimeMs - Button->LastReleasedTime) >= Button->ClickInterval) {
            if (Button->ClickCb) {
                Button->ClickCb(Button->ClickCnt);
            }
            Button->ClickCnt = 0; 
        }
    }
}

uint8_t MyButton_GetState(Button_TypeDef *Button) {
    if (Button == NULL) return 0;
    return Button->LastState;
}

void My_Button_SetLongPressCb(Button_TypeDef *Button, void (*LongPressCb)(uint8_t ticks)) {
    if (Button) Button->LongPressCb = LongPressCb;
}
void My_Button_SetPressCb(Button_TypeDef *Button, void (*button_pressed_cb)(void)) {
    if (Button) Button->PressCb = button_pressed_cb;
}
void My_Button_SetReleaseCb(Button_TypeDef *Button, void (*button_released_cb)(void)) {
    if (Button) Button->ReleaseCb = button_released_cb;
}
void My_Button_SetClickCb(Button_TypeDef *Button, void (*button_clicked_cb)(uint8_t clicks)) {
    if (Button) Button->ClickCb = button_clicked_cb;
}
void My_Button_ClickIntervalConfig(Button_TypeDef *Button, uint32_t Interval) {
    if (Button) Button->ClickInterval = Interval;
}
void My_Button_LongPressConfig(Button_TypeDef *Button, uint32_t Threshold, uint32_t TickInterval) {
    if (Button) {
        Button->LongPressThreshold = Threshold;
        Button->LongPressTickInterval = TickInterval;
    }
}




