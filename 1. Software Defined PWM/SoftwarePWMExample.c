/*
 * SoftwarePWMExample.c
 *
 *  Created on: Feb 18, 2023
 *      Author: Russell Trafford
 *
 *      This example controls the LED connected to Pin 1.0 by PWM. You can change the DutyCycle Global variable to change the brightness of the LED. You should vary this to see how the brightness can change.
 *      You can also change this in the Variables/Expressions tab in the debugger to experiment with it as well.
 */




#include <msp430.h>

unsigned int DutyCycleR = 1000;
unsigned int DutyCycleG = 1000;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT

    // Configure GPIO
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    P6DIR |= BIT6;
    P6OUT |= BIT6;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure Timer_A
    TB0CTL = TBSSEL_2 | MC_2 | TBCLR | TBIE;      // SMCLK, continuous mode, clear TBR, enable interrupt

    TB0CCTL1 |= CCIE;                             // Enable TB0 CCR1 Interrupt
    TB0CCTL2 |= CCIE;


    TB0CCR0 = 25000;
    TB0CCR1 = DutyCycleR;                          // Set CCR1 to the value to set the red duty cycle
    TB0CCR2 = DutyCycleG;                          //Set CCR2 for green duty cycle


            P2OUT |= BIT3;                          //Set 2.3 as an input
            P2REN |= BIT3;                          //Enable pull-up resistor
            P2IES &= ~BIT3;                         // P2.3 Low --> High edge
            P2IE |= BIT3;                           // Enable interrupts on 2.3
            P2IFG &= ~BIT3;                         //Clear interrupt flag

            P4OUT |= BIT1;                          //Set 4.1 as an input
            P4REN |= BIT1;                          //Enable pull-up resistor
            P4IES &= ~BIT1;                         // P4.1 Low --> High edge
            P4IE |= BIT1;                           // Enable interrupts on 4.1
            P4IFG &= ~BIT1;                         //Clear interrupt flag

    __bis_SR_register(LPM3_bits | GIE);           // Enter LPM3, enable interrupts
    __no_operation();                             // For debugger
}

// Timer0_B3 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt
        case TB0IV_TBCCR1:
            P1OUT &= ~BIT0;

            break;                               // CCR1 Set the pin to a 0
        case TB0IV_TBCCR2:
            P6OUT &= ~BIT6;
            break;                               // CCR2 used for green led
        case TB0IV_TBIFG:
            P1OUT |= BIT0;                       // overflow Set the pin to a 1
            P6OUT |= BIT6;
            break;
        default:
            break;
    }
}


#pragma vector=PORT2_VECTOR                 //Button 2.3 interrupt
__interrupt void Port_2(void)
{
    if (DutyCycleR < 25000)
        DutyCycleR += 5000;
    else  if (DutyCycleR >= 25000)
            DutyCycleR = 100;

P2IFG &= ~BIT3;
TB0CCR1 = DutyCycleR;
}

#pragma vector=PORT4_VECTOR                 //Button 2.3 interrupt
__interrupt void Port_4(void)
{
    if (DutyCycleG < 25000)
        DutyCycleG += 5000;
    else  if (DutyCycleG >= 25000)
            DutyCycleG = 100;

P4IFG &= ~BIT1;
TB0CCR2 = DutyCycleG;
}


