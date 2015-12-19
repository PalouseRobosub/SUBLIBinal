#include "Comparator.h"

void (*callback_comp1) (void);
void (*callback_comp2) (void);
void (*callback_comp3) (void);

Error initialize_Comparator(Comparator_Config config) {
	//This will initialize the comparator
	int stat = 0;
    
	//First, figure out the voltage reference
    if (updateVReference_Comparator(config.voltage_reference) == ERR_NO_ERR)
        stat = 1;

	//Now, configure the comparator itself
	switch (config.channel) {
		case COMP_CH_1:
			//Configure the edge polarity
			switch (config.edge) {
				case falling:
					CM1CONbits.EVPOL = 0b10;
					break;
				case rising:
					CM1CONbits.EVPOL = 0b01;
					break;
				case rising_or_falling:
					CM1CONbits.EVPOL = 0b11;
					break;
			}
			
			//Configure the negative reference selection
			CM1CONbits.CREF = stat;

			//Configure the Positive input selection
			switch (config.input) {
				case Pin_RPB2: //C1INB
					//Configure Analog select
					ANSELBbits.ANSB2 = 1;
					//Configure Tristate
                    TRISBbits.TRISB2 = 1; //input
                    CM1CONbits.CCH = 0;
					break;
				case Pin_RPB1: //C1INC
                    ANSELBbits.ANSB1 = 1;
                    TRISBbits.TRISB1 = 1; //input
                    CM1CONbits.CCH = 0b01;
					break;
				case Pin_RPB0: //C1IND
                    ANSELBbits.ANSB0 = 1;
                    TRISBbits.TRISB0 = 1;
                    CM1CONbits.CCH = 0b10;
					break;
				default:
                    return ERR_INVALID_PIN;
					break;
			}

			//Configure the interrupts
            callback_comp1 = config.callback;
            IFS1bits.CMP1IF = 0; //clear the flag
            IPC6bits.CMP1IP = 7; //Set priority
            IEC1bits.CMP1IE = config.enable; //enable interrupt
            
            //configure the negative input terminal
            if (stat == 0)
            {
                TRISBbits.TRISB3 = 1;
                ANSELBbits.ANSB3 = 1;
            }
            
            CM1CONbits.ON = config.enable; //Enable the module
			break;
		case COMP_CH_2:
            //Configure the edge polarity
			switch (config.edge) {
				case falling:
					CM2CONbits.EVPOL = 0b10;
					break;
				case rising:
					CM2CONbits.EVPOL = 0b01;
					break;
				case rising_or_falling:
					CM2CONbits.EVPOL = 0b11;
					break;
			}
			
			//Configure the negative reference selection
			CM2CONbits.CREF = stat;

			//Configure the Positive input selection
			switch (config.input) {
				case Pin_RPB2: //C2IND
					//Configure Analog select
					ANSELBbits.ANSB2 = 1;
					//Configure Tristate
                    TRISBbits.TRISB2 = 1; //input
                    CM2CONbits.CCH = 0b10;
					break;
				case Pin_RPB3: //C2INC
                    ANSELBbits.ANSB3 = 1;
                    TRISBbits.TRISB3 = 1; //input
                    CM2CONbits.CCH = 0b01;
					break;
				case Pin_RPB0: //C2INB
                    ANSELBbits.ANSB0 = 1;
                    TRISBbits.TRISB0 = 1;
                    CM2CONbits.CCH = 0;
					break;
				default:
                    return ERR_INVALID_PIN;
					break;
			}

			//Configure the interrupts
            callback_comp1 = config.callback;
            IFS1bits.CMP2IF = 0; //clear the flag
            IPC7bits.CMP2IP = 7; //Set priority
            IEC1bits.CMP2IE = config.enable; //enable interrupt
            
            //configure the negative input terminal
            if (stat == 0)
            {
                TRISBbits.TRISB1 = 1;
                ANSELBbits.ANSB1 = 1;
            }
            
            CM2CONbits.ON = config.enable; //Enable the module
			break;
		case COMP_CH_3:
            //Configure the edge polarity
			switch (config.edge) {
				case falling:
					CM3CONbits.EVPOL = 0b10;
					break;
				case rising:
					CM3CONbits.EVPOL = 0b01;
					break;
				case rising_or_falling:
					CM3CONbits.EVPOL = 0b11;
					break;
			}
			
			//Configure the negative reference selection
			CM3CONbits.CREF = stat;

			//Configure the Positive input selection
			switch (config.input) {
				case Pin_RPB14:
					//Configure Analog select
					ANSELBbits.ANSB14 = 1;
					//Configure Tristate
                    TRISBbits.TRISB14 = 1; //input
                    CM3CONbits.CCH = 0;
					break;
				case Pin_RPA0:
                    ANSELAbits.ANSA0 = 1;
                    TRISAbits.TRISA0 = 1; //input
                    CM3CONbits.CCH = 0b01;
					break;
				case Pin_RPB0:
                    ANSELBbits.ANSB0 = 1;
                    TRISBbits.TRISB0 = 1;
                    CM3CONbits.CCH = 0b10;
					break;
				default:
                    return ERR_INVALID_PIN;
					break;
			}

			//Configure the interrupts
            callback_comp1 = config.callback;
            IFS1bits.CMP3IF = 0; //clear the flag
            IPC7bits.CMP3IP = 7; //Set priority
            IEC1bits.CMP3IE = config.enable; //enable interrupt
            
            //configure the negative input terminal
            if (stat == 0)
            {
                TRISBbits.TRISB15 = 1;
                ANSELBbits.ANSB15 = 1;
            }
            
            CM3CONbits.ON = config.enable; //Enable the module
			break;
	}
    
    return ERR_NO_ERR;

    
}

void enable_Comparator(Comparator_Channel channel) {
    switch (channel) {
        case COMP_CH_1:
            IEC1bits.CMP1IE = 1;
            CM1CONbits.ON = 1;
            break;
        case COMP_CH_2:
            IEC1bits.CMP2IE = 1;
            CM2CONbits.ON = 1;
            break;
        case COMP_CH_3:
            IEC1bits.CMP3IE = 1;
            CM3CONbits.ON = 1;
            break;
    }
}

void disable_Comparator(Comparator_Channel channel) {
    switch (channel) {
        case COMP_CH_1:
            IEC1bits.CMP1IE = 0;
            CM1CONbits.ON = 0;
            break;
        case COMP_CH_2:
            IEC1bits.CMP2IE = 0;
            CM2CONbits.ON = 0;
            break;
        case COMP_CH_3:
            IEC1bits.CMP3IE = 0;
            CM3CONbits.ON = 0;
            break;
    }

}

Error updateVReference_Comparator(float voltage_reference) {
    float val_32step = .25, val_24step = 0;
	int config_32step = 0, config_24step = 0;
	float error_32, error_24;
    Error ret = ERR_INVALID_VREF;

	if ((voltage_reference > .75*3.3) || (voltage_reference < 0)) {
		//Calculate the values if using .25 - .75 reference
		while ((voltage_reference - val_32step) > (3.3/64)) { //While the difference is greater than half a step size
			//add a step to val
			val_32step += 3.3/32;
			config_32step++;
		}
	
		//Now, calculate the value if using 0 - .67 reference
		while ((voltage_reference - val_24step) > (3.3/48)) { //While the difference is greater than a half step
			val_24step += 3.3/24;
			config_24step++;
		}
	
		//Select the closest value
		error_32 = voltage_reference - val_32step;
		error_24 = voltage_reference - val_24step;
		if (error_32 < 0)
			error_32 *= -1;
		if (error_24 < 0)
			error_24 *= -1;
	
		CVRCON = 0; //Clear out CVref config
		if (error_24 < error_32)
		{
			//Use 24 step
			CVRCONbits.CVRR = 1; //Enable CVRR for 24 step
			CVRCONbits.CVR = config_24step;
		}
		else
		{
			//Use 32 step
			CVRCONbits.CVR = config_32step;
		}
		CVRCONbits.ON = 1; //Enable the comparator voltage reference
        ret = ERR_NO_ERR;
	}
    
    return ret;
}
