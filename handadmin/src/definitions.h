// ----------------------------------------------------------------------------
// Copyright (C)  qbrobotics. All rights reserved.
// www.qbrobotics.com
// ----------------------------------------------------------------------------

/**
* \file         definitions.h
*
* \brief        qbHand general definitions file
*
* \details      General definitions file included for all the executable files
*
* \copyright    (C)  qbrobotics. All rights reserved.
*/

#include <math.h>

#define HANDADMIN_VERSION "v5.3.0"

#define NUM_OF_SENSORS 3 						///< Default number of sensors connected
#define NUM_OF_MOTORS 2							///< Default number of motor used (2 for qbMove, 1 for Softhand)
#define NUM_OF_EMGS 2							///< Default number of emg sensors
#define PI 3.14159265359						///< PI

#define DEFAULT_RESOLUTION 3 					///< Default resolution
#define BROADCAST_ID 0							///< ID used for broadcast communication 					
#define DEFAULT_INCREMENT 100 					///< Default increment used for hand demonstration

#define DEG_TICK_MULTIPLIER (65536.0 / (360.0 * (pow(2, DEFAULT_RESOLUTION)))) ///< Conversion factor from degrees to encoder ticks
#define LOOKUP_DIM 6 							///< Lookup table dimension. The table is used for current estimation 

#define SIN_FILE "./../conf_files/sin.conf" 	///< Default location of a sinusoidal signal file
#define MOTOR_FILE "./../conf_files/motor.conf"	///< Default location of the file containing the values of motor gear ratios
#define HANDMOVE_FILE "./../conf_files/handmove.conf" 	///< Default location of where the device serial port is saved
#define EMG_SAVED_VALUES "./../emg_values.csv"	///< Default location where the emg sensors values are saved