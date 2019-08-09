/**
  ******************************************************************************
  * File Name          : USB_OTG.c
  * Description        : This file provides code for the configuration
  *                      of the USB_OTG instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_otg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

HCD_HandleTypeDef hhcd_USB_OTG_FS;

/* USB_OTG_FS init function */

void MX_USB_OTG_FS_HCD_Init(void)
{

  hhcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hhcd_USB_OTG_FS.Init.Host_channels = 12;
  hhcd_USB_OTG_FS.Init.speed = HCD_SPEED_FULL;
  hhcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hhcd_USB_OTG_FS.Init.phy_itface = HCD_PHY_EMBEDDED;
  hhcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  if (HAL_HCD_Init(&hhcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_HCD_MspInit(HCD_HandleTypeDef* hcdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcdHandle->Instance==USB_OTG_FS)
  {
  /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

  /* USER CODE END USB_OTG_FS_MspInit 0 */
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_FS GPIO Configuration    
    PA12     ------> USB_OTG_FS_DP
    PA11     ------> USB_OTG_FS_DM
    PA10     ------> USB_OTG_FS_ID
    PA9     ------> USB_OTG_FS_VBUS 
    */
    GPIO_InitStruct.Pin = USB_OTGFS_DP_Pin|USB_OTGFS_DM_Pin|USB_OTGFS_ID_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USB_OTGFS_VBUS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_OTGFS_VBUS_GPIO_Port, &GPIO_InitStruct);

    /* USB_OTG_FS clock enable */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    /* Enable VDDUSB */
    if(__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      HAL_PWREx_EnableVddUSB();
      __HAL_RCC_PWR_CLK_DISABLE();
    }
    else
    {
      HAL_PWREx_EnableVddUSB();
    }
  /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

  /* USER CODE END USB_OTG_FS_MspInit 1 */
  }
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef* hcdHandle)
{

  if(hcdHandle->Instance==USB_OTG_FS)
  {
  /* USER CODE BEGIN USB_OTG_FS_MspDeInit 0 */

  /* USER CODE END USB_OTG_FS_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
  
    /**USB_OTG_FS GPIO Configuration    
    PA12     ------> USB_OTG_FS_DP
    PA11     ------> USB_OTG_FS_DM
    PA10     ------> USB_OTG_FS_ID
    PA9     ------> USB_OTG_FS_VBUS 
    */
    HAL_GPIO_DeInit(GPIOA, USB_OTGFS_DP_Pin|USB_OTGFS_DM_Pin|USB_OTGFS_ID_Pin|USB_OTGFS_VBUS_Pin);

    /* Disable VDDUSB */
    if(__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      HAL_PWREx_DisableVddUSB();
      __HAL_RCC_PWR_CLK_DISABLE();
    }
    else
    {
      HAL_PWREx_DisableVddUSB();
    }
  /* USER CODE BEGIN USB_OTG_FS_MspDeInit 1 */

  /* USER CODE END USB_OTG_FS_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
