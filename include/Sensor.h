/* 
 * File:   Sensor.h
 * Author: James Irwin
 *
 * Created on March 29, 2015, 6:04 PM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/*Include Statements*/
#include "System.h"
#include "I2C.h"


    typedef struct SENSOR_DATA{
        I2C_Channel channel;
        I2C_Node *config_nodes;
        uint8 config_nodes_size;
        I2C_Node *read_nodes;
        uint8 read_nodes_size;
        void (*callback) (struct SENSOR_DATA);
    } Sensor_Data;

    void initialize_sensor(Sensor_Data data);
    void read_sensor(Sensor_Data data);




#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */

