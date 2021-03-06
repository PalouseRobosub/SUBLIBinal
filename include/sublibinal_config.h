/* 
 * File:   sublibinal_config.h
 * Author: ryan
 *
 * Created on October 6, 2015, 10:24 AM
 */

#ifndef SUBLIBINAL_CONFIG_H
#define	SUBLIBINAL_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    //select programming pins
    #pragma config ICESEL = ICS_PGx1// ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)

    // DEVCFG3
    // USERID = No Setting
    #pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
    #pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
    #pragma config FUSBIDIO = OFF            // USB USID Selection (Controlled by the USB Module)
    #pragma config FVBUSONIO = OFF           // USB VBUS ON Selection (Controlled by USB Module)

    // DEVCFG2
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
    #pragma config FPLLMUL = MUL_15         // PLL Multiplier (15x Multiplier)
    #pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
    #pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
    #pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 1)

    // DEVCFG1
    #pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
    #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
    #pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
    #pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
    #pragma config OSCIOFNC = OFF            // CLKO Output Signal Active on the OSCO Pin (Enabled)
    #pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
    #pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
    #pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
    #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
    #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
    #pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

    // DEVCFG0
    #pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
    //programming pins are selected above
    #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
    #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
    #pragma config CP = OFF                 // Code Protect (Protection Disabled)

    #ifndef PB_CLK
        #define PB_CLK 15000000
    #endif
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* SUBLIBINAL_CONFIG_H */

