// ----------------------------------------------------------------------------
// Copyright (C)  qbrobotics. All rights reserved.
// www.qbrobotics.com
// ----------------------------------------------------------------------------

/**
* \file         handparam.c
*
* \brief        Command line tools to change qbHand  parameters
*
* \details      With this file is possible to change or read qbHand parameters
*               through command line tools. It is also possible to reset all the 
*               parameters to the factory defaults
*
* \copyright    (C)  qbrobotics. All rights reserved.
*/


//=================================================================     includes

#include "../../qbAPI/src/qbmove_communications.h"
#include "definitions.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

//=============================================================     declarations
int port_selection();
int open_port();
int initMemory();
void printMainMenu();
void printParamMenu();
void printVersion();

//==================================================================     globals

char get_or_set;                ///< Get parameter or set parameter option
comm_settings comm_settings_t;  ///< comm_settings structure containing the serial port handle


//=====================================================================     main
/** main loop
*/

int main() {
    int i;
    char c_choice;
    int  i_choice;

    int aux_int;
    short int aux_sint;
    unsigned char aux_uchar;
    uint8_t aux_ssuint;
    float aux_float;

    uint8_t device_id;
    float pid_control[3];
    short int offsets[NUM_OF_SENSORS];
    short int emg_threshold[NUM_OF_EMGS];
    int emg_max_values[NUM_OF_EMGS];
    uint8_t emg_speed;
    uint8_t input_mode;
    uint8_t control_mode;
    uint8_t double_encoder_on_off;
    int8_t motor_handle_ratio;
    int aux_max_step_pos, aux_max_step_neg;
    unsigned char resolution[NUM_OF_SENSORS];
    float multipliers[NUM_OF_SENSORS];
    int pos_limits[NUM_OF_MOTORS*2];
    short int current_limit;
    unsigned char motor_supply;
    float lookup_table[LOOKUP_DIM];


    printVersion();

    if (!open_port()) {
        assert(port_selection());
        assert(open_port());
    }


//================================================================     MAIN MENU
    printMainMenu();
    scanf("%c", &c_choice);

    switch(c_choice) {
        case 'g':
            get_or_set = c_choice;
            break;
        case 's':
            get_or_set = c_choice;
            break;
        case 'm':
            initMemory();
            break;

        default:
            break;
    }

    if (get_or_set == 'g' || get_or_set == 's') {
        printParamMenu();

        scanf("%d", &i_choice);
        getchar();

        //==============================================================     SET
        if (get_or_set == 's') {
            switch(i_choice) {

                //-----------------------------------------------     Set new ID
                case 1:
                    printf("Choose a new ID[0 <= ID <= 128]: ");
                    scanf("%hhu", &device_id);
                    fflush(stdin);

                    if ((device_id <= 0) ||(device_id > 255)) {
                        printf("ID out of range\n");
                        return 0;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_ID, &device_id, 1))
                        printf("Cannot set new ID\n");
                    break;

                //-----------------------------------     Set new PID parameters
                case 2:
                    printf("Set the new values for the PID controller:\nK_p: ");
                    scanf("%f", pid_control);

                    printf("K_i: ");
                    scanf("%f", pid_control + 1);

                    printf("K_d: ");
                    scanf("%f", pid_control + 2);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_PID_CONTROL, pid_control, 3))
                        printf("Cannot set new PID values\n");
                    break;

                //-------------------------     Set new PID parameters (current)
                case 3:
                    printf("Set the new values for the PID controller (current):\nK_p: ");
                    scanf("%f", pid_control);

                    printf("K_i: ");
                    scanf("%f", pid_control + 1);

                    printf("K_d: ");
                    scanf("%f", pid_control + 2);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_PID_CURR_CONTROL, pid_control, 3))
                        printf("Cannot set new Current PID values\n");
                    break;

                //------------------------------     Set startup activation flag
                case 4:
                    printf("Do you want the motor to be ON [1] or OFF [0] at startup?: ");
                    scanf("%d", &aux_int);

                    if (aux_int == 0) {
                        aux_uchar = 0;
                    } else {
                        aux_uchar = 3;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_STARTUP_ACTIVATION, &aux_uchar, 1))
                        printf("Cannot set new Startup activation flag\n");
                    break;

                //-------------------------------------------     Set input mode
                case 5:
                    printf("\nChoose Input Mode:\n");
                    printf("[0] - Default: Motors are commanded using the communication interface.\n");
                    printf("[1] - Handle.\n");
                    printf("\n--- EMG PART ---\n");
                    printf("[2] - EMG signal is used to proportionally control hand closure.\n");
                    printf("[3] - EMG signals are used to control opening and closing of the hand.\n");
                    printf("[4] - First EMG signal passing the threshold is used to control the hand.\n");
                    printf("[5] - First EMG signal passing the threshold is used to control the hand.\n");
                    printf("      Wait for both to go under threshold before going on.\n\n");
                    scanf("%hhu",&input_mode);

                    if ((input_mode < 0) || (input_mode > 5)) {
                        printf("Choice not available.\n");
                        return 0;
                    }

                    switch(input_mode) {
                        case 0:
                            input_mode = INPUT_MODE_EXTERNAL;
                            break;

                        case 1:
                            input_mode = INPUT_MODE_ENCODER3;
                            break;

                        case 2:
                            input_mode = INPUT_MODE_EMG_PROPORTIONAL;
                            break;

                        case 3:
                            input_mode = INPUT_MODE_EMG_INTEGRAL;
                            break;

                        case 4:
                            input_mode = INPUT_MODE_EMG_FCFS;
                            break;

                        case 5:
                            input_mode = INPUT_MODE_EMG_FCFS_ADV;
                            break;

                        default:
                            break;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_INPUT_MODE, &input_mode, 1))
                        printf("Cannot set new input mode\n");
                    break;

                //-----------------------------------------     Set control mode
                case 6:
                    printf("\nChoose Control Mode:\n");
                    printf("[0] - Default: Position control.\n");
                    printf("[1] - Direct PWM input (0 - 100).\n");
                    printf("[2] - Current control (set current in mA).\n");
                    printf("[3] - Current and Position control.\n");
                    scanf("%hhu",&control_mode);

                    if ((control_mode < 0) || (control_mode > 3)) {
                        printf("Choice not available.\n");
                        return 0;
                    }

                    switch(control_mode) {
                        case 0:
                            control_mode = CONTROL_ANGLE;
                            break;
                        case 1:
                            control_mode = CONTROL_PWM;
                            break;
                        case 2:
                            control_mode = CONTROL_CURRENT;
                            break;
                        case 3:
                            control_mode = CURR_AND_POS_CONTROL;
                            break;
                        default:
                            return -1;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_CONTROL_MODE, &control_mode, 1))
                        printf("Cannot set new Control mode\n");
                    break;

                //-------------------------------------------     Set resolution
                case 7:
                    printf("[0] - 360   degrees (1   turn).\n");
                    printf("[1] - 720   degrees (2   turn).\n");
                    printf("[2] - 1440  degrees (4   turn).\n");
                    printf("[3] - 2880  degrees (8   turn).\n");
                    printf("[4] - 5760  degrees (16  turn).\n");
                    printf("[5] - 11520 degrees (32  turn).\n");
                    printf("[6] - 23040 degrees (64  turn).\n");
                    printf("[7] - 46080 degrees (128 turn).\n");
                    printf("[8] - 92160 degrees (256 turn).\n");

                    for (i = 0; i < NUM_OF_SENSORS; i++) {
                        printf("\n\nChoose angle resolution for sensor #%d: ", (i + 1));
                        scanf("%d", &aux_int);
                        resolution[i] = aux_int;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_RESOLUTION, resolution, NUM_OF_SENSORS))
                        printf("Cannot set new resolution values\n");
                    break;

                //-----------------------------------------------     Set offset
                case 8:
                    printf("Set the new offsets:\n");

                    printf("Offset 1: ");
                    scanf("%hd", &aux_sint);
                    offsets[0] = aux_sint;

                    printf("Offset 2: ");
                    scanf("%hd", &aux_sint);
                    offsets[1] = aux_sint;

                    printf("Offset 3: ");
                    scanf("%hd", &aux_sint);
                    offsets[2] = aux_sint;

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_OFFSET, offsets, NUM_OF_SENSORS))
                        printf("Cannot set new offsets\n");
                    break;

                //------------------------------------------     Set multipliers
                case 9:
                    printf("Set new multipliers:\n");
                    for (i = 0; i < NUM_OF_SENSORS; i++) {
                        printf("\nMultiplier for sensor %d: ", i+1);
                        scanf("%f", &aux_float);
                        multipliers[i] = aux_float;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_MULTIPLIER, multipliers, NUM_OF_SENSORS))
                        printf("Cannot set new multipliers\n");
                    break;

                //----------------------------------     Set position limit flag
                case 10:
                    printf("Set the new values for max step:\nmax_step_pos: ");
                    scanf("%d", &aux_max_step_pos);
                    if (aux_max_step_pos < 0){
                        printf("max_step_pos must be a positive value\n");
                        return 0;
                    }
                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_POS, &aux_max_step_pos, 1))
                        printf("Cannot set new max positive step\n");

                    printf("max_step_neg: ");
                    scanf("%d", &aux_max_step_neg);
                    if(aux_max_step_neg > 0) {
                        printf("max_step_neg must be a negative value\n");
                        return 0;
                    }
                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_NEG, &aux_max_step_neg, 1))
                        printf("Cannot set new max negative step\n");
                    break;

                //----------------------------------     Set position limit flag
                case 11:
                    printf("Activate position limit? [1 YES, 0 NO]: ");
                    scanf("%d", &aux_int);
                    if ((aux_int != 0) && (aux_int != 1)) {
                        printf("Choice must be 1 or 0\n");
                        return 0;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT_FLAG, &aux_int, 1))
                        printf("Cannot set new position limit flag\n");
                    break;

                //---------------------------------------     Set position limit
                case 12:
                    printf("Set position limit for motors\n");
                    printf("\nInf limit 1: ");
                    scanf("%d", pos_limits);
                    printf("\nSup limit 1: ");
                    scanf("%d", pos_limits + 1);

                    pos_limits[2] = 0;
                    pos_limits[3] = 0;

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT, pos_limits, 4))
                        printf("Cannot set new position limits\n");
                    break;

                //----------------------------------------     Set current limit
                case 13:
                    printf("Set new current limit: ");
                    scanf("%hd", &current_limit);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_CURRENT_LIMIT, &current_limit, 1))
                        printf("Cannot set new current limit\n");
                    break;

                //---------------------------------     Set emg calibration flag
                case 14:
                    printf("Do you want calibration on startup? [y]n:");
                    scanf("%c", &c_choice);
                    fflush(stdin);

                    printf("%c\n", c_choice);
                    if (c_choice == 'n' || c_choice == 'N') {
                        aux_uchar = 0;
                    } else {
                        aux_uchar = 1;
                    }
                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_CALIB_FLAG, &aux_uchar, 1))
                        printf("Cannot set new Emg calibration flag\n");
                    break;

                //----------------------------------------     Set emg threshold
                case 15:
                    printf("Set new EMG threshold:\nEMG 1: ");
                    scanf("%hd", &emg_threshold[0]);
                    printf("EMG 2: ");
                    scanf("%hd", &emg_threshold[1]);
                    // Check numbers
                    if (emg_threshold[0] < 0 || emg_threshold[0] > 1024) {
                        printf("Threshold EMG 1 out of range [0 <= EMG threshold < 1024]\n");
                        break;
                    }
                    if (emg_threshold[1] < 0 || emg_threshold[1] > 1024) {
                        printf("Threshold EMG 2 out of range [0 <= EMG threshold < 1024]\n");
                        break;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_THRESHOLD, emg_threshold, 2))
                        printf("Cannot set new emg thresholds\n");
                    break;

                //----------------------------------------     Set emg max value
                case 16:
                    printf("Set EMG max values:\nEMG 1: ");
                    scanf("%d", &emg_max_values[0]);
                    printf("EMG 2: ");
                    scanf("%d", &emg_max_values[1]);

                    // check numbers
                    if (emg_max_values[0] < 0 || emg_max_values[0] > 4096) {
                        printf("EMG 1 max_value out of range [0 <= EMG max_value <= 4096]\n");
                        break;
                    }
                    if (emg_max_values[1] < 0 || emg_max_values[1] > 4096) {
                        printf("EMG 2 max_value out of range [0 <= EMG max_value <= 4096]\n");
                        break;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_MAX_VALUE, emg_max_values, 2))
                        printf("Cannot set new Emgs max values\n");
                    break;

                //--------------------------------------------     Set emg speed
                case 17:
                    printf("Set max speed when usgin EMG control [0 < speed <= 100]:\n");
                    scanf("%hhu", &emg_speed);
                    if ((emg_speed < 1) || (emg_speed > 100)) {
                        printf("Speed is out of range.\n");
                        break;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_SPEED, &emg_speed, 1))
                        printf("Cannot set new Emg speed\n");
                    break;

                //--------------------------------     Set double encoder on off
                case 18:
                    printf("Do you want to have memory of encoder position? YES 1, NO 0\n");
                    scanf("%hhu", &double_encoder_on_off);
                    if (double_encoder_on_off == 0 || double_encoder_on_off == 1) {
                        if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_DOUBLE_ENC_ON_OFF, &double_encoder_on_off, 1))
                            printf("Cannot set new Encoder flag\n");
                    } else {
                        printf("Not a valid option\n");
                    }
                    break;

                //-----------------------------------     Set motor handle ratio
                case 19:
                    printf("Set a new motor-handle ratio [-128  127]\n");
                    scanf("%hhd", &motor_handle_ratio);
                    if(motor_handle_ratio >= -128 && motor_handle_ratio <= 127) {
                        if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MOT_HANDLE_RATIO, &motor_handle_ratio, 1))
                            printf("Cannot set new handle ratio\n");
                    }
                    else
                        printf("Not a valid ratio value\n");
                    break;

                //-----------------------------------     Set motor supply type
                case 20:
                    printf("Do you want to activate PWM rescaling? \n");
                    printf("[0] No rescaling (12V on 12V hand or 24V on 24V hand)\n");
                    printf("[1] Activate rescaling (24V on 12V hand)\n");
                    scanf("%hhu", &motor_supply);
                    if(motor_supply == 0 || motor_supply == 1) {
                        if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MOTOR_SUPPLY, &motor_supply, 1))
                            printf("Cannot set new Motor supply type\n");
                    }
                    else
                        printf("Not a valid option\n");
                    break;

                //-----------------------------------     Set current lookup values
                case 21:
                    printf("Insert lookup table values:\n");
                    for(i = 0; i < LOOKUP_DIM; i++) { 
                        printf("p(%d): ", i);
                        scanf("%f", lookup_table + i);
                    }
                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_CURRENT_LOOKUP, lookup_table, 6))
                        puts("Cannot set new lookup table");
                    break;

                //--------------------------------------------------     DEFAULT
                default:
                    printf("Not a valid option\n");
                    return 0;
            }
            usleep(100000);
            commStoreParams(&comm_settings_t, BROADCAST_ID);
            usleep(100000);
        } else {
            //==========================================================     GET
            switch(i_choice) {

                //---------------------------------------------------     Get ID
                case 1:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_ID, &device_id, 1)) {
                        printf("Current ID is: %hhu\n", device_id);
                    } else {
                        printf("Unable to retrieve ID\n");
                    }
                    break;

                //---------------------------------------     Get PID parameters
                case 2:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_PID_CONTROL, &pid_control, 3)) {
                        printf("PID constants are:\n");
                        printf("P: %f\n", pid_control[0]);
                        printf("I: %f\n", pid_control[1]);
                        printf("D: %f\n", pid_control[2]);
                    } else {
                        printf("Unable to retrieve PID constants\n");
                    }
                    break;

                //-------------------------------     Get PID parameters current
                case 3:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_PID_CURR_CONTROL, &pid_control, 3)) {
                        printf("PID constants are:\n");
                        printf("P: %f\n", pid_control[0]);
                        printf("I: %f\n", pid_control[1]);
                        printf("D: %f\n", pid_control[2]);
                    } else {
                        printf("Unable to retrieve PID constants\n");
                    }
                    break;

                //------------------------------     Get startup activation flag
                case 4:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_STARTUP_ACTIVATION, &aux_uchar, 1)) {
                        if (aux_uchar & 0x02) {                     //answer is 1 if NO, 3 if YES
                            printf("Startup activation YES\n" );
                        } else {
                            printf("Startup activation NO\n");
                        }
                    } else {
                        printf("Unable to retrieve activation flag\n");
                    }
                    break;

                //-------------------------------------------     Get input mode
                case 5:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_INPUT_MODE, &input_mode, 1)) {
                        switch (input_mode) {
                            case INPUT_MODE_EXTERNAL:
                                printf("USB inputs\n");
                                break;
                            case INPUT_MODE_ENCODER3:
                                printf("Handle\n");
                                break;
                            case INPUT_MODE_EMG_PROPORTIONAL:
                                printf("EMG 1 proportional\n");
                                break;
                            case INPUT_MODE_EMG_INTEGRAL:
                                printf("EMG differential\n");
                                break;
                            case INPUT_MODE_EMG_FCFS:
                                printf("EMG FCFS\n");
                                break;
                            case INPUT_MODE_EMG_FCFS_ADV:
                                printf("EMG FCFS Advanced\n");
                                break;
                            default:
                                break;
                        }
                    } else {
                        printf("Unable to retrive input mode\n");
                    }
                    break;

                //-----------------------------------------     Get control mode
                case 6:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_CONTROL_MODE, &control_mode, 1)) {
                        switch (control_mode) {
                            case CONTROL_ANGLE:
                                printf("Position control\n");
                                break;
                            case CONTROL_PWM:
                                printf("PWM control\n");
                                break;
                            case CONTROL_CURRENT:
                                printf("Current control\n");
                                break;
                            case CURR_AND_POS_CONTROL:
                                printf("Current and position control\n");
                                break;
                            default:
                                break;
                        }
                    } else {
                        printf("Unable to retrive input mode\n");
                    }
                    break;

                //-------------------------------------------     Get resolution
                case 7:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_RESOLUTION, resolution, 3)) {
                        printf("Sensor 1: %d\n", resolution[0]);
                        printf("Sensor 2: %d\n", resolution[1]);
                        printf("Sensor 3: %d\n", resolution[2]);
                    } else {
                        printf("Unable to retrieve resolution\n");
                    }
                    break;

                //----------------------------------------------     Get  offset
                case 8:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_OFFSET, offsets, 3)) {
                        printf("Offset sensor 1: %d\n", offsets[0]);
                        printf("Offset sensor 2: %d\n", offsets[1]);
                        printf("Offset sensor 3: %d\n", offsets[2]);
                    } else {
                        printf("Unable to retrieve offsets\n");
                    }
                    break;

                //------------------------------------------     Get multipliers
                case 9:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_MULTIPLIER, multipliers, 3)) {
                        printf("Multiplier sensor 1: %f\n", multipliers[0]);
                        printf("Multiplier sensor 2: %f\n", multipliers[1]);
                        printf("Multiplier sensor 3: %f\n", multipliers[2]);
                    } else {
                        printf("Unable to retrieve multipliers\n");
                    }
                    break;

                //------------------------------------------     Get max steps
                case 10:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_POS, &aux_max_step_pos, 1) &&
                       !commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_NEG, &aux_max_step_neg, 1)) {
                        printf("max_step_pos: %d\n", aux_max_step_pos);
                        printf("max_step_neg: %d\n", aux_max_step_neg);
                    } else {
                        printf("Unable to retrieve position limit flag\n");
                    }
                    break;

                //----------------------------------     Get position limit flag
                case 11:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT_FLAG, &aux_ssuint, 1)) {
                        if (aux_ssuint) {
                            printf("Limits enabled: YES\n");
                        } else {
                            printf("Limits enabled: NO\n");
                        }
                    } else {
                        printf("Unable to retrieve position limit flag\n");
                    }
                    break;

                //---------------------------------------     Get position limit
                case 12:
                    if(!(commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT, pos_limits, 2))){
                        commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_RESOLUTION, resolution, 3);
                        printf("Position limit [inf , sup]: [%d , %d]\n", pos_limits[0] >> resolution[0], pos_limits[1] >> resolution[1]);
                    } else {
                        printf("Unable to retrieve position limits\n");
                    }
                    break;

                //----------------------------------------     Get current limit
                case 13:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_CURRENT_LIMIT, &current_limit, 1)) {
                        printf("Current limit: %d\n", current_limit);
                    } else {
                        printf("Unable to retrieve current limit\n");
                    }
                    break;

                //---------------------------------     Get emg calibration flag
                case 14:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_CALIB_FLAG, &aux_ssuint, 1)) {
                        if (aux_ssuint) {
                            printf("EMG calibration on startup: YES\n");
                        } else {
                            printf("EMG calibration on startup: NO\n");
                        }
                    } else {
                        printf("Unable to retrieve EMG calibration flag\n");
                    }
                    break;

                //----------------------------------------     Get emg threshold
                case 15:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_THRESHOLD, emg_threshold, 2)) {
                        printf("EMG threshold 1: %d\n", emg_threshold[0]);
                        printf("EMG threshold 2: %d\n", emg_threshold[1]);
                    } else {
                        printf("Unable to retrieve EMG thresholds\n");
                    }
                    break;

                //----------------------------------------     Get emg max value
                case 16:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_MAX_VALUE, emg_max_values, 2)) {
                        printf("EMG max_value 1: %d\n", emg_max_values[0]);
                        printf("EMG max_value 2: %d\n", emg_max_values[1]);
                    } else {
                        printf("Unable to retrieve EMG max_values\n");
                    }
                    break;

                //----------------------------------------     Get emg max speed
                case 17:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_EMG_SPEED, &emg_speed, 1)) {
                        printf("EMG speed: %d\n", emg_speed);
                    } else {
                        printf("Unable to retrieve EMG max_values\n");
                    }
                    break;

                //--------------------------------     Get double encoder on off
                case 18:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_DOUBLE_ENC_ON_OFF, &double_encoder_on_off, 1)) {
                        if (double_encoder_on_off == 1) {
                            printf("Absolute encoder position active: YES\n");
                        } else {
                            printf("Absolute encoder position active: NO\n");
                        }
                    } else {
                        printf("Unable to retrieve absolute encoder configuration\n");
                    }
                    break;

                //-----------------------------------     Get motor handle ratio
                case 19:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MOT_HANDLE_RATIO, &motor_handle_ratio, 1)) {
                        printf("Motor-handle ratio: %d\n", motor_handle_ratio);
                    } else {
                        printf("Unable to retrieve motor handle ratio\n");
                    }
                    break;

                //-----------------------------------    Get motor type
                case 20:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MOTOR_SUPPLY, &motor_supply, 1)) {
                        printf("Motor type: %hhu\n", motor_supply);
                        if(motor_supply) {
                            printf("PWM rescaling Active\n");
                        }
                        else {
                            printf("PWM rescaling Not active\n");
                        }
                    }
                    else
                        printf("Unable to retrieve motor supply type\n");
                break;

                //-----------------------------------    Get current lookup table
                case 21:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_CURRENT_LOOKUP, lookup_table, 6)) {
                        printf("Current lookup values: \n");
                        for(i = 0; i < LOOKUP_DIM; i++)
                            printf("p[%d]: %f\n", i, lookup_table[i]);
                    }
                    else
                        printf("Unable to retrieve current lookup table\n");
                break;

                //--------------------------------------------------     DEFAULT
                default:
                    break;
            }

        }

    }

    return 1;
}

/** This function selects the serial port from a list of serial ports 
    connected to the computer
*/

int port_selection() {
    int i;
    int aux_int;
    int num_ports = 0;
    char my_port[255];
    char ports[10][255];
    FILE *file;

    while(1) {
        num_ports = RS485listPorts(ports);

        if(num_ports) {
            puts("\nChoose the serial port for your QB:\n");

            for(i = 0; i < num_ports; ++i) {
                printf("[%d] - %s\n\n", i+1, ports[i]);
            }
            printf("Serial port: ");
            scanf("%d", &aux_int);
            getchar();

            if( aux_int && (aux_int <= num_ports) ) {
                strcpy(my_port, ports[aux_int - 1]);
            } else {
                puts("Choice not available");
                continue;
            }

            file = fopen(HANDMOVE_FILE, "w+");
            if (file == NULL) {
                printf("Cannot open qbmove.conf\n");
            }
            fprintf(file,"serialport %s\n", my_port);
            fclose(file);
            return 1;

        } else {
            puts("No serial port available.");
            return 0;
        }
    }
}

/** This functions open the serial port connected to the computer, in order to
    communicate with the device chosen
*/

int open_port() {
    FILE *file;
    char port[255];

    file = fopen(HANDMOVE_FILE, "r");

    if (file == NULL) {
        printf("Error opening file %s\n", HANDMOVE_FILE);
        return 0;
    }

    fscanf(file, "serialport %s\n", port);

    fclose(file);


    openRS485(&comm_settings_t, port);

    if(comm_settings_t.file_handle == INVALID_HANDLE_VALUE)
    {
        puts("Couldn't connect to the serial port.");
        return 0;
    }
    usleep(100000);

    return 1;
}

/** This function is used to reset the parameters values to the factory ones.
    The factory values depend on the device connected
*/

int initMemory() {
    char choice;

    getchar();
    printf("WARNING: Restore initial memory settings? [y/N]\n");
    choice = getchar();

    if (choice != 'y' && choice != 'Y') {
        return 0;
    }

    printf("Initializing memory...");

    if (!commInitMem(&comm_settings_t, BROADCAST_ID)) {
        printf("DONE\n");
        return 1;
    }

    printf("Failed\n");
    return 0;
}

/** Prints on the terminal the version of the command line tools 
*/
void printVersion() {
    printf("========================================\n");
    printf("handmove version: %s\n", HANDADMIN_VERSION);
    printf("========================================\n");
}

/** Prints on the terminal the Main menu of the parameters. It is possible to 
    select if is needed to get or set a parameters. There is also an option to reset the
    parameters to the factory ones
*/

void printMainMenu() {
    printf("g: getParam\n");
    printf("s: setParam\n");
    printf("m: initMemory\n");
    printf("========================================\n");
}

/** Prints on the terminal the parameters list
*/

void printParamMenu() {
    printf("----------------------------------------\n");
    printf(" 1: ID\n");
    printf(" 2: PID params\n");
    printf(" 3: PID params current\n");
    printf(" 4: Startup activation flag\n");
    printf(" 5: Input mode\n");
    printf(" 6: Control mode\n");
    printf(" 7: Resolution\n");
    printf(" 8: Measurements offset\n");
    printf(" 9: Multipliers\n");
    printf("10: Max steps\n");
    printf("11: Position limit flag\n");
    printf("12: Position limits\n");
    printf("13: Current limit\n");
    printf("14: Emg calibration flag\n");
    printf("15: Emg threshold\n");
    printf("16: Emg max value\n");
    printf("17: Emg max speed\n");
    printf("18: Absolute encoder pos. activation YES/NO\n");
    printf("19: Motor handle ratio\n");
    printf("20: Motor supply voltage\n");
    printf("21: Current lookup table\n");
    printf("----------------------------------------\n");
}

/* END OF FILE */