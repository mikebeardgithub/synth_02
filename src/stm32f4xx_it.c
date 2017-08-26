/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <init_io.h>
#include "stm32f4xx_it.h"
#include "stm32f4xx_gpio.h"
#include "user_interface.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
/*  TimingDelay_Decrement(); */
}


/*
 * interrupt handler for vco waveform button
 */
void EXTI0_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line0);

	if(the_button.button_state == NOT_ACTIVE){
		the_button.button = vco_wave_button;
		the_button.button_state = ACTIVE;
		TIM_Cmd(TIM3, ENABLE);
	}
}



/*
 * interrupt handler for lfo waveform button
 */
void EXTI1_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line1);

	if(the_button.button_state == NOT_ACTIVE){
		the_button.button = lfo_wave_button;
		the_button.button_state = ACTIVE;
		TIM_Cmd(TIM3, ENABLE);
	}
}


/*
 * interrupt handler for lfo modulation type button
 */
void EXTI2_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line2);

	if(the_button.button_state == NOT_ACTIVE){
		the_button.button = lfo_mod_button;
		the_button.button_state = ACTIVE;
		TIM_Cmd(TIM3, ENABLE);
	}

}



/*
 * interrupt handler for adsr modulation type button
 */
void EXTI3_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line3);

	if(the_button.button_state == NOT_ACTIVE){
		the_button.button = adsr_mod_button;
		the_button.button_state = ACTIVE;
		TIM_Cmd(TIM3, ENABLE);
	}
}

//EXTI9_5_IRQn used for selector
void EXTI9_5_IRQHandler(void){
	// TODO: Is this a mistake?  Looks like EXTI_Line6 is twice.  Maybe should be EXTI_Line5.
	// EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);
	EXTI_ClearITPendingBit(EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);
	if(!((TIM4->CR1 & TIM_CR1_CEN) == TIM_CR1_CEN)){			//is debounce timer off
			TIM_Cmd(TIM4, ENABLE);								//turn on debounce timer
	}
	// update_state();
}

//EXTI15_10_IRQn used for selector
void EXTI15_10_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line10 | EXTI_Line11 | EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15);

	if(!((TIM4->CR1 & TIM_CR1_CEN) == TIM_CR1_CEN)){		//is debounce timer off
			TIM_Cmd(TIM4, ENABLE);							//turn on debounce timer
	}
}

// Timer for limiting button pushes 100ms
void TIM3_IRQHandler()
{
	// Note: the_button.button is set to vco_wave or whatever by
	// EXTI0_IRQHandler() or whatever.
	// That routine is called first and then this one.
	// It enables the TIM3 timer.  Then this function disables it.

	TIM_Cmd(TIM3, DISABLE);				//stop timer
	int temp = the_button.button;

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			//clear interrupt
	the_button.button_state = NOT_ACTIVE;
	if(temp == vco_wave_button)
	{
		// handle_enter();			// TODO
		update_vco_wave();

	}
	else if(temp == lfo_wave_button)
	{
		// handle_back();			// TODO
		update_lfo_wave();
	}
	else if(temp == lfo_mod_button)
	{
		// handle_up();			// TODO
		update_lfo_mod();
	}
	else if(temp == adsr_mod_button)
	{
		// handle_down();			// TODO
		update_adsr_mod();
	}
}

// Timer for debouncing selectors 50ms
void TIM4_IRQHandler()
{
	TIM_Cmd(TIM4, DISABLE);								//stop timer
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);			//clear interrupt

	// update_selector_state();							//update state
	// update_state();
}

// not set up yet but might to see if the adc is working
void ADC_IRQHandler() {
	/* acknowledge interrupt */
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
