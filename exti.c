#include "exti.h"
#include "motor.h"

void config_EXTI(void) {
    RCC->APB2ENR |= (1 << 14); 	
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0; 
    EXTI->IMR |= EXTI_IMR_IM0;            
    EXTI->RTSR |= EXTI_RTSR_TR0;
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void) {   
    if (EXTI->PR & EXTI_PR_PR0) {       
        if (TIM3->CR1 & 1) {          // Moteurs tournent ? STOP
            TIM3->CR1 &= ~1;  
            TIM3->CCR1 = 0; TIM3->CCR2 = 0;
            TIM3->CCR3 = 0; TIM3->CCR4 = 0;
            speed = 500; 
        } else {                      // Moteurs arrêtés ? START
            TIM3->CR1 |= 1;   
            TIM3->CCR1 = speed; TIM3->CCR2 = 0;
            TIM3->CCR3 = 0;     TIM3->CCR4 = speed;
        }
        // FIX : on utilise = au lieu de |=
        EXTI->PR = EXTI_PR_PR0; 
    }
}