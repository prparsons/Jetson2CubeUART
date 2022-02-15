#include <msp430.h>

/*
 *  Jetson          MSP430          Cube
 *  UART TX  -> P1.6|   |P1.2   ->  UART RX
 *  UART RX  <- P1.7|   |P1.1   <-  UART TX
 *
 * 56k Ohm resistors used for 1.6 & 1.7
 * Cube already has 40k Ohm resistors
 */

void initClockTo16MHz()
{
    // Don't load if no calibration
    if (CALBC1_16MHZ == 0xFF) {
        while(1);
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    initClockTo16MHz();

    // Could (should?) instead of outputting high, switch to input and let the pullup pull it high.
    P1DIR |= BIT7 + BIT2;   // Set the outputs
    P1OUT |= BIT7 + BIT2;   // Default high (repeating a UART, which holds signal high)
    P1IE |=  BIT1 + BIT6;   // P1 interrupt
    P1IES |= BIT1 + BIT6;   // Falling edge
    P1IFG &= ~BIT1;         // IFG cleared
    P1IFG &= ~BIT6;

__bis_SR_register(LPM4_bits + GIE); // LPM4 w/interrupt
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT1) { // If Cube TX has flipped
        if (P1IN & BIT1) {
            // Set Jetson RX high
            P1OUT |= BIT7;
            // Swap interrupt to falling edge
            P1IES |= BIT1;
        } else {
            // Set Jetson RX low
            P1OUT &= ~BIT7;
            // Interrupt back to rising edge
            P1IES &= ~BIT1;
        }
        P1IFG &= ~BIT1;
    }
    if (P1IFG & BIT6) { // If Jetson TX has flipped
        if (P1IN & BIT6) {
            // Set Cube RX high
            P1OUT |= BIT2;
            // Interrupt to falling edge
            P1IES |= BIT6;
        } else {
            // Set Cube RX low
            P1OUT &= ~BIT2;
            // Interrupt to rising edge
            P1IES &= ~BIT6;
        }
        P1IFG &= ~BIT6;
    }
}

