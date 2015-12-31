/* Comparator Header
 * Author: Ryan
 * This header provides definitions for the Comparator
 */

#ifndef COMPARATOR_H
#define COMPARATOR_H

#ifdef __cplusplus
extern "C" {
#endif

	/* Includes*/
#include "System.h"

	/* Enums */
	typedef enum {
		COMP_CH_1,
		COMP_CH_2,
		COMP_CH_3
	} Comparator_Channel;

	/* Object Defitions */
	typedef struct COMPARATOR_CONFIG {
		Pin input; ///< External Input to the comparator
		Polarity edge; ///< The type of edge that triggers an interrupt
		void *callback; ///< Callback function for interrupt
		boolean enable; ///< Enable the comparator module
		float voltage_reference; ///< The voltage reference to be used. If this is < 0 or > Vdd, will use external pin
        Comparator_Channel channel; ///< The channel of the comparator to initialize
	} Comparator_Config;

	/* Function Definitions */

	/**
	  * Initialize the Comparator with the filled out configuration structure
	  * \param config The structure containing initialization parameters for the comparator
	  * \see Error
	  * \return An error code status of the function success
	  */
	Error initialize_Comparator(Comparator_Config config);

	/**
	  * Enable Comparator
	  * \param channel Channel selection of the comparator to enable.
	  */
	void enable_Comparator(Comparator_Channel channel);

	/**
	  * Disable Comparator
	   \param channel Channel selection of the comparator to disable
	  */
	void disable_Comparator(Comparator_Channel channel);

	/**
	  * Update Voltage Reference for the Comparator
	  * \see Error
	  * \return An Error code on the functions success status
	  */
	Error updateVReference_Comparator(float voltage_reference);




#ifdef __cplusplus
}
#endif

#endif
