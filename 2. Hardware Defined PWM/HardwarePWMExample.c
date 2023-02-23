/*
 * HardwarePWMExample.c
 *
 *  Created on: Feb 18, 2023
 *      Author: Russell Trafford
 *
 *      This example configures P6.0 as a PWM output as controlled by TimerB3.
 *
 *      You will need to connect an external LED or use a scope to see the effect of the Duty Cycle.
 *
 *      You can control the DutyCycle by changing the values in the TB3CCR1 register
 */

#include <msp430.h>
int start = 1;
int red = 0;
int green = 0;
int blue = 0;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P6DIR |= BIT0;                     // P6.0 output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;                    // P6.0 options select


    P6DIR |= BIT1;
    P6SEL0 |= BIT1;
    P6SEL1 &= ~BIT1;

    P6DIR |= BIT2;
    P6SEL0 |= BIT2;
    P6SEL1 &= ~BIT2;


    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TB3CCR0 = 1000-1;                         // PWM Period
    //TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 300;                            // CCR1 PWM duty cycle
    TB3CCTL1 |= CCIE;
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR

    TB1CTL = TBSSEL_1 |MC_2 |TBCLR;
    TB1CCR0 = 5000;


    __bis_SR_register(LPM0_bits);             // Enter LPM0
    __no_operation();                         // For debugger
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{

   if (start) {
       P6OUT |= BIT0;
       P6OUT &= ~BIT1;
       P6OUT &= ~BIT2;
   }

   else if (red) {
       P6OUT &= ~BIT0;
       P6OUT |= ~BIT1;
   }
   else if (green) {
       P6OUT &= ~BIT1;
       P6OUT |= BIT2;
   }
   else if (blue) {
       P6OUT |= BIT0;
       P6OUT &= ~BIT2;
   }
TB1CCR0 += 5000;
}

#pragma vector=TIMER3_B1_VECTOR
__interrupt void TIMER0_B3_ISR(void)
{

   if (start){
       start = 0;
       red = 1;
   }
   else if (red) {
       red = 0;
       green = 1;
   }
   else if (green) {
       green = 0;
       blue = 1;
   }
   else if (blue) {
       blue = 0;
       start = 1;
   }
   TB3CCR1 += 300;
}



