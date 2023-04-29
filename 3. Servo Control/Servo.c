/*
 * Servo.c
 *
 *
 *      Author: Matt O'Donnell
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */


#include <msp430.h>


void main(){

WDTCTL = WDTPW | WDTHOLD;                       //Disable watchdog

PM5CTL0 &= ~LOCKLPM5;


P6DIR |= BIT0;
P6OUT &= ~BIT0;

        P2OUT |= BIT3;                          //Set 2.3 as an input
        P2REN |= BIT3;                          //Enable pull-up resistor
        P2IES &= ~BIT3;                         // P2.3 Low --> High edge

        P2IFG &= ~BIT3;                         //Clear interrupt flag

        P4OUT |= BIT1;                          //Set 2.3 as an input
                P4REN |= BIT1;                          //Enable pull-up resistor
                P4IES &= ~BIT1;                         // P2.3 Low --> High edge

                P4IFG &= ~BIT1;



       TB1CCR1 = 1500;
        TB1CCR0 = 20000;
        TB1CTL = TBSSEL_1 | MC_1;


        while(1) {

            while(!P2IN) {
                TB1CCR1--;
                if (TB1CCR1 < 800) {
                    break;
                }
            }

            while(!P4IN) {
                TB1CCR1++;
                if (TB1CCR1 > 2100) {
                    break;
                }
            }
        }

}
