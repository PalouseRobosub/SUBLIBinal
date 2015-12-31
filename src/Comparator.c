#include "Comparator.h"

void (*callback_comp1) (void);
void (*callback_comp2) (void);
void (*callback_comp3) (void);

Error initialize_Comparator(Comparator_Config config) {
	//This will initialize the comparator
	Error stat = ERR_NO_ERR;
    
	//First, figure out the voltage reference
    stat = updateVReference_Comparator(config.voltage_reference);

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
            if (stat == 0)
                CM1CONbits.CREF = 1;
            else
                CM1CONbits.CREF = 0; //If 0, External reference is used

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
            if (stat != 0)
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
            if (stat == 0)
                CM2CONbits.CREF = 1;
            else
                CM2CONbits.CREF = 0; //If 0, External reference is used

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
            if (stat != 0)
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
            if (stat == 0)
                CM3CONbits.CREF = 1;
            else
                CM3CONbits.CREF = 0; //If 0, External reference is used

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
            if (stat != 0)
            {
                TRISBbits.TRISB15 = 1;
                ANSELBbits.ANSB15 = 1;
            }
            
            CM3CONbits.ON = config.enable; //Enable the module
			break;
	}
    
    return stat;

    
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
    float val_32step = .825, val_24step = 0; //.825 = 25% of 3.3
	int config_32step = 0, config_24step = 0;
	float error_32, error_24;
    Error ret = ERR_INVALID_VREF;
    
    //2.475 == 3.3 * 75%
	if (!(voltage_reference > 2.475) || (voltage_reference < 0)) {
		//Calculate the values if using .25 - .75 reference
        
        //We could do this with a look up table of some sort as well, to save on processing
		while ((voltage_reference - val_32step) > (0.0515625)) { //While the difference is greater than half a step size (3.3/64))
			//add a step to val
			val_32step += 0.103125; //(3.3/32), 1 step size
			config_32step++;
		}
	
		//Now, calculate the value if using 0 - .67 reference
		while ((voltage_reference - val_24step) > (0.06875)) { //While the difference is greater than a half step (3.3/48))
			val_24step += 0.1375; //3.3/24 = 1 step size
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

//ISRs
void __ISR(_COMPARATOR_1_VECTOR, IPL7AUTO) Comparator_Handler_1(void) {

    asm volatile ("di"); //Disable interrupts
    
    if (callback_comp1 != NULL)
    {
        callback_comp1();
    }
    
	IFS1bits.CMP1IF = 0;
    asm volatile ("ei");
}

void __ISR(_COMPARATOR_2_VECTOR, IPL7AUTO) Comparator_Handler_2(void) {

    asm volatile ("di"); //Disable interrupts
    
    if (callback_comp2 != NULL)
    {
        callback_comp2();
    }
    
	IFS1bits.CMP2IF = 0;
    asm volatile ("ei");
}

void __ISR(_COMPARATOR_3_VECTOR, IPL7AUTO) Comparator_Handler_3(void) {
	
    asm volatile ("di"); //Disable interrupts
    
    if (callback_comp3 != NULL)
    {
        callback_comp3();
    }
    
	IFS1bits.CMP3IF = 0;
    asm volatile ("ei");
}
