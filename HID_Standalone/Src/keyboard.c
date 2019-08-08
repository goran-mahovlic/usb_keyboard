/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/keyboard.c 
  * @author  MCD Application Team
  * @brief   This file implements the HID keyboard functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KYBRD_FIRST_COLUMN               (uint16_t) 8
#define KYBRD_LAST_COLUMN                (uint16_t) BSP_LCD_GetXSize()
#define KYBRD_FIRST_LINE                 (uint8_t) 70
#define SMALL_FONT_COLUMN_WIDTH                    8
#define SMALL_FONT_LINE_WIDTH                      15
#define KYBRD_LAST_LINE                  (uint16_t)155
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern HID_DEMO_StateMachine hid_demo;
extern uint8_t *DEMO_KEYBOARD_menu[];
extern uint8_t prev_select;
extern uint32_t hid_demo_ready;
uint8_t KeybrdCharYpos = 0;
uint16_t KeybrdCharXpos = 0;
uint16_t CurrentLastXpos[KYBRD_LAST_LINE] = {0};
/* Private function prototypes -----------------------------------------------*/
static void USR_KEYBRD_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Manages Keyboard Menu Process.  
  * @param  None
  * @retval None
  */
void HID_KeyboardMenuProcess(void)
{
  switch(hid_demo.keyboard_state)
  {
  case HID_KEYBOARD_IDLE:
    hid_demo.keyboard_state = HID_KEYBOARD_START;
    HID_SelectItem(DEMO_KEYBOARD_menu, 0);   
    hid_demo.select = 0;
    prev_select = 0;       
    break;
    
  case HID_KEYBOARD_WAIT:
    if(hid_demo.select != prev_select)
    {
      prev_select = hid_demo.select;
      HID_SelectItem(DEMO_KEYBOARD_menu, hid_demo.select & 0x7F);
      /* Handle select item */
      if(hid_demo.select & 0x80)
      {
        hid_demo.select &= 0x7F;
        switch(hid_demo.select)
        {
        case 0: 
          hid_demo.keyboard_state = HID_KEYBOARD_START;
          break;
          
        case 1: /* Return */
          LCD_LOG_ClearTextZone();
          hid_demo.state = HID_DEMO_REENUMERATE;
          hid_demo.select = 0;
          break;
          
        default:
          break;
        }
      }
    }
    break; 
    
  case HID_KEYBOARD_START:
    USR_KEYBRD_Init();   
    hid_demo.keyboard_state = HID_KEYBOARD_WAIT;
    break;  
    
  default:
    break;
  }
}

/**
  * @brief  Init Keyboard window.     
  * @param  None
  * @retval None
  */
static void USR_KEYBRD_Init(void)
{
  LCD_LOG_ClearTextZone();
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Use Keyboard to type characters:                                                            "); 
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  KeybrdCharYpos = KYBRD_FIRST_LINE;
  KeybrdCharXpos = KYBRD_FIRST_COLUMN;
}

/**
  * @brief  Processes Keyboard data.
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
void USR_KEYBRD_ProcessData(uint8_t data)
{
  if(data == '\n')
  {
    KeybrdCharXpos = KYBRD_FIRST_COLUMN;
    
    /* Increment char Y position */
    KeybrdCharYpos += SMALL_FONT_LINE_WIDTH;
    
    if(KeybrdCharYpos > KYBRD_LAST_LINE)
    {
      LCD_LOG_ClearTextZone();
      KeybrdCharYpos = KYBRD_FIRST_LINE;
      KeybrdCharXpos = KYBRD_FIRST_COLUMN;
    }
  }
  else if(data == '\r')
  {
    /* Manage deletion of character and update cursor location */
    if(KeybrdCharXpos == KYBRD_FIRST_COLUMN) 
    {
      /* First character of first line to be deleted */
      if(KeybrdCharYpos == KYBRD_FIRST_LINE)
      {  
        KeybrdCharXpos = KYBRD_FIRST_COLUMN; 
      }
      else
      {
        KeybrdCharYpos -= SMALL_FONT_LINE_WIDTH;
        KeybrdCharXpos = (KYBRD_LAST_COLUMN - SMALL_FONT_COLUMN_WIDTH); 
      }
    }
    else
    {
      if(CurrentLastXpos[KeybrdCharYpos] > KYBRD_FIRST_COLUMN)
      {
        CurrentLastXpos[KeybrdCharYpos] -= SMALL_FONT_COLUMN_WIDTH;
        KeybrdCharXpos = CurrentLastXpos[KeybrdCharYpos];
      }
      else if(KeybrdCharYpos > KYBRD_FIRST_LINE)
      {
        KeybrdCharYpos -= SMALL_FONT_LINE_WIDTH;
        CurrentLastXpos[KeybrdCharYpos] -= SMALL_FONT_COLUMN_WIDTH;
        KeybrdCharXpos = CurrentLastXpos[KeybrdCharYpos];
      }
      else
      {
      }
    }
    BSP_LCD_DisplayChar(CurrentLastXpos[KeybrdCharYpos], KeybrdCharYpos, ' ');
  }
  else
  {
    /* Update the cursor position on LCD */
    BSP_LCD_DisplayChar(KeybrdCharXpos, KeybrdCharYpos, data);  
    
    /* Increment char X position */
    KeybrdCharXpos += SMALL_FONT_COLUMN_WIDTH;
    
    CurrentLastXpos[KeybrdCharYpos] = KeybrdCharXpos;
    /* Check if the X position has reached the last column */
    if(KeybrdCharXpos == KYBRD_LAST_COLUMN)
    {
      KeybrdCharXpos = KYBRD_FIRST_COLUMN;
      
      /* Increment char Y position */
      KeybrdCharYpos += SMALL_FONT_LINE_WIDTH;
    }
    
    if(KeybrdCharYpos > KYBRD_LAST_LINE)
    {
      LCD_LOG_ClearTextZone();
      KeybrdCharYpos = KYBRD_FIRST_LINE;
      /* Start New Display of the cursor position on LCD */
      BSP_LCD_DisplayChar(KeybrdCharXpos,KeybrdCharYpos, data);  
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
