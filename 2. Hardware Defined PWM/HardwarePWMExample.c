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
int x = 0;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P6DIR |= BIT0;                     // Red
    P6OUT |= BIT0;
    P6SEL1 &= ~BIT0;

    P4DIR |= BIT6;                      //Green
    P4OUT |= BIT6;
    P4SEL1 &= ~BIT1;

    P3DIR |= BIT2;                  //Blue
    P3OUT |= BIT2;
    P3SEL1 &= ~BIT2;


    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TB3CCR0 = 1000-1;                         // PWM Period
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 300;                            // CCR1 PWM duty cycle
    TB3CCTL1 |= CCIE;
    TB3CTL = TBSSEL_1 | MC_1 | TBCLR;  // SMCLK, up mode, clear TBR


    TB1CCTL1 |= CCIE;
    TB1CTL = TBSSEL_1 |MC_2 |TBCLR;
    TB1CCR0 = 500;


    __bis_SR_register(LPM0_bits | GIE);             // Enter LPM0
    __no_operation();                         // For debugger
}



#pragma vector=TIMER3_B1_VECTOR
__interrupt void TIMER3_B1_ISR(void)
{


TB3CCR0 += 300;
}

#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void)
{



    switch (x) {
    case 0: {
        x++;
        P6OUT &= ~BIT0;
                  P4OUT |= BIT6;
                  P3OUT |= BIT2;
                  break;
    }
    case 1: {
        x++;
        P6OUT |= BIT0;
                  P4OUT &= ~BIT6;
                  break;
    }
    case 2: {
       x++;
        P4OUT |= BIT6;
                 P3OUT &= ~BIT2;
                 break;
    }
    case 3: {
        x=0;
        P6OUT &= ~BIT0;
                 P3OUT |= BIT2;
                 break;
    }



    }

    TB1CCR0 += 500;

}



