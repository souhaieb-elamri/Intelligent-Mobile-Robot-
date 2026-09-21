#include "motor.h"
#include <string.h>

int speed = 500;
const int max_speed = 999;
const int min_speed = 200;

void Config_TIM3(void) {
    RCC->APB1ENR |= (1 << 1); 
    TIM3->PSC = 159;
    TIM3->ARR = 999;
    TIM3->CCMR1 = 0x6868;
    TIM3->CCMR2 = 0x6868;
    TIM3->CCER |= 0b0001000100010001;
}

void Motor_Process_Command(char *cmd) {
    /* FIX : réactiver TIM3 s'il avait été arrêté */
    if (!(TIM3->CR1 & 1)) {
        TIM3->CR1 |= 1;
    }
    
    // GESTION AVANCE (Les deux moteurs avancent)
    if (strstr(cmd, "forward")) {
        TIM3->CCR1 = speed; TIM3->CCR2 = 0;
        TIM3->CCR3 = 0;     TIM3->CCR4 = speed;
    }
    // GESTION RECULE (Les deux moteurs reculent)
    else if (strstr(cmd, "backward")) {
        TIM3->CCR1 = 0;     TIM3->CCR2 = speed;
        TIM3->CCR3 = speed; TIM3->CCR4 = 0;
    }
    // GESTION TOURNER DROITE (Moteur Gauche avance, Moteur Droit STOP)
    else if (strstr(cmd, "right")) {
        TIM3->CCR1 = speed; TIM3->CCR2 = 0;
        TIM3->CCR3 = 0;     TIM3->CCR4 = 0;   // STOP
    }
    // GESTION TOURNER GAUCHE (Moteur Gauche STOP, Moteur Droit avance)
    else if (strstr(cmd, "left")) {
        TIM3->CCR1 = 0;     TIM3->CCR2 = 0;   // STOP
        TIM3->CCR3 = 0;     TIM3->CCR4 = speed;
    }
    // ARRET 
    else if (strstr(cmd, "stop")) {
        TIM3->CCR1 = 0; TIM3->CCR2 = 0;
        TIM3->CCR3 = 0; TIM3->CCR4 = 0;
        speed = 500;
    }
}