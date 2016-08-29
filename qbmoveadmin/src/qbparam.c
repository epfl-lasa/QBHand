// --- INCLUDE ---
#include "../../qbAPI/src/qbmove_communications.h"
#include "definitions.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// function declaration
int port_selection();
int open_port();
int initMemory();
void printMainMenu();
void printParamMenu();
void printVersion();
int calibrate();

// global variables
char get_or_set;
comm_settings comm_settings_t;


// --- MAIN ---
int main() {
    int i;
    char c_choice;
    int  i_choice;

    int aux_int;
    short int aux_sint;
    uint8_t aux_ssuint;
    unsigned char aux_uchar;
    float aux_float;

    uint8_t device_id;
    float pid_control[3];
    short int offsets[NUM_OF_SENSORS];
    unsigned char resolution[NUM_OF_SENSORS];
    float multipliers[NUM_OF_SENSORS];
    short int pos_limits[NUM_OF_MOTORS*2];
    uint8_t control_mode;
    short int current_limit;
    uint8_t deflection_control;
    int aux_max_step_pos;
    int aux_max_step_neg;

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
        case 'c':
            calibrate();
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

                    if ((device_id < 0) ||(device_id > 128)) {
                        printf("ID out of range\n");
                        return 0;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_ID, &device_id, 1))
                        printf("Cannot set ID\n");
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
                        printf("Cannot set PID controller values\n");
                    break;

                //---------------------------     Set new PID parameters current
                case 3:
                    printf("Set the new values for the PID controller (current):\nK_p: ");
                    scanf("%f", pid_control);

                    printf("K_i: ");
                    scanf("%f", pid_control + 1);

                    printf("K_d: ");
                    scanf("%f", pid_control + 2);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_PID_CURR_CONTROL, pid_control, 3))
                        printf("Cannot set Current PID controller values\n");
                    break;

                //-------------------------------------------     Set input mode
                case 4:
                    printf("\nChoose Input Mode:\n");
                    printf("[0] - Default: Motors are commanded using the communication interface.\n");
                    printf("[1] - Encoder 3 works as a 'joystick' for the motors.\n");
                    scanf("%hhu",&aux_ssuint);

                    if ((aux_ssuint < 0) || (aux_ssuint > 5)) {
                        printf("Choice not available.\n");
                        return 0;
                    }

                    switch(aux_ssuint) {
                        case 0:
                            aux_ssuint = INPUT_MODE_EXTERNAL;
                            break;

                        case 1:
                            aux_ssuint = INPUT_MODE_ENCODER3;
                            break;

                        default:
                            break;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_INPUT_MODE, &aux_ssuint, 1))
                        printf("Cannot set input mode\n");

                    break;

                //-----------------------------------------     set control mode
                case 5:
                    printf("Choose control mode:\n\n");
                    printf("[0] - Position\n");
                    printf("[1] - PWM\n");
                    printf("[2] - Current\n");
                    printf("[3] - Current And Position\n");
                    scanf("%hhd", &control_mode);
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
                        printf("Cannot set control mode\n");

                    break;

                //-------------------------------------------     Set resolution
                case 6:
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
                        printf("Cannot set resolution\n");
                    break;

                //-----------------------------------------------     Set offset
                case 7:
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
                        printf("Cannot set offsets\n");
                    break;

                //------------------------------------------     Set multipliers
                case 8:
                    printf("Set new multipliers:\n");
                    for (i = 0; i < NUM_OF_SENSORS; i++) {
                        printf("\nMultiplier for sensor %d: ", i+1);
                        scanf("%f", &aux_float);
                        multipliers[i] = aux_float;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_MULTIPLIER, multipliers, NUM_OF_SENSORS))
                        printf("Cannot set multipliers\n");
                    break;

                //----------------------------------     Set position limit flag
                case 9:
                    printf("Activate position limit? [1 YES, 0 NO]: ");
                    scanf("%d", &aux_int);
                    if ((aux_int != 0) && (aux_int != 1)) {
                        printf("Choice must be 1 or 0\n");
                        return 0;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT_FLAG, &aux_int, 1))
                        printf("Cannot set position limit flag\n");
                    break;

                //---------------------------------------     Set position limit
                case 10:
                    printf("Set position limit for motors\n");
                    printf("\nInf limit 1: ");
                    scanf("%d", pos_limits);
                    printf("\nSup limit 1: ");
                    scanf("%d", pos_limits + 1);
                    printf("\nInf limit 2: ");
                    scanf("%d", pos_limits + 2);
                    printf("\nSup limit 2: ");
                    scanf("%d", pos_limits + 3);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT, pos_limits, 4))
                        printf("Cannot set position limits\n");
                    break;

                //------------------------------     Set startup activation flag
                case 11:
                    printf("Do you want the motors to be ON [1] or OFF [0] at startup?: ");
                    scanf("%d", &aux_int);

                    if (aux_int == 0) {
                        aux_uchar = 0;
                    } else {
                        aux_uchar = 3;
                    }

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_STARTUP_ACTIVATION, &aux_uchar, 1))
                        printf("Cannot set startup activation flag\n");
                    break;

                //----------------------------------------     Set current limit
                case 12:
                    printf("Set new current limit: ");
                    scanf("%hd", &current_limit);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_CURRENT_LIMIT, &current_limit, 1))
                        printf("Cannot set current limit\n");
                    break;

                //----------------------------------------     Set deflection control flag
                case 13:
                    printf("Do you want to activate deflection control? [1] YES / [0] NO\n");
                    scanf("%d", &deflection_control);

                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_DEFLECTION_CONTROL, &deflection_control, 1))
                        printf("Cannot set deflection control flag\n");
                    break;

                //----------------------------------------      Set maximum steps
                case 14:
                    printf("Set the new values for max step:\nmax_step_pos: ");
                    scanf("%d", &aux_max_step_pos);
                    if (aux_max_step_pos < 0){
                        printf("max_step_pos must be a positive value\n");
                        return 0;
                    }
                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_POS, &aux_max_step_pos, 1))
                        printf("Cannot set max positive step\n");
                    
                    printf("max_step_neg: ");
                    scanf("%d", &aux_max_step_neg);
                    if(aux_max_step_neg > 0) {
                        printf("max_step_neg must be a negative value\n");
                        return 0;
                    }
                    if(commSetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_NEG, &aux_max_step_neg, 1))
                        printf("Cannot set max negative step\n");
                    break;

                default:
                    break;
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

                //-------------------------------------------     Get input mode
                case 4:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_INPUT_MODE, &aux_ssuint, 1)) {
                        switch (aux_ssuint) {
                            case INPUT_MODE_EXTERNAL:
                                printf("USB inputs\n");
                                break;
                            case INPUT_MODE_ENCODER3:
                                printf("Encoder 3\n");
                                break;
                            default:
                                break;
                        }
                    } else {
                        printf("Unable to retrive input mode\n");
                    }
                    break;

                //-----------------------------------------     Get control mode
                case 5:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_CONTROL_MODE, &control_mode, 1)) {
                        switch(control_mode) {
                            case 0:
                                printf("Current control mode: Position\n");
                                break;

                            case 1:
                                printf("Current control mode: PWM\n");
                                break;

                            case 2:
                                printf("Current control mode: Current\n");
                                break;

                            default:
                                break;
                        }
                    } else {
                        printf("Unable to retrieve control mode\n");
                    }
                    break;

                //-------------------------------------------     Get resolution
                case 6:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_RESOLUTION, resolution, 3)) {
                        printf("Sensor 1: %d\n", resolution[0]);
                        printf("Sensor 2: %d\n", resolution[1]);
                        printf("Sensor 3: %d\n", resolution[2]);
                    } else {
                        printf("Unable to retrieve resolution\n");
                    }
                    break;

                //----------------------------------------------     Get  offset
                case 7:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_OFFSET, offsets, 3)) {
                        printf("Offset sensor 1: %d\n", offsets[0]);
                        printf("Offset sensor 2: %d\n", offsets[1]);
                        printf("Offset sensor 3: %d\n", offsets[2]);
                    } else {
                        printf("Unable to retrieve offsets\n");
                    }
                    break;

                //------------------------------------------     Get multipliers
                case 8:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MEASUREMENT_MULTIPLIER, multipliers, 3)) {
                        printf("Multiplier sensor 1: %f\n", multipliers[0]);
                        printf("Multiplier sensor 2: %f\n", multipliers[1]);
                        printf("Multiplier sensor 3: %f\n", multipliers[2]);
                    } else {
                        printf("Unable to retrieve multipliers\n");
                    }
                    break;

                //----------------------------------     Get position limit flag
                case 9:
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
                case 10:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_LIMIT, pos_limits, 4) &&
                       !commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_POS_RESOLUTION, resolution, 3)) {
                        printf("Position limit 1 [inf - sup]: [%d - %d]\n", (pos_limits[0] >> resolution[0]), (pos_limits[1] >> resolution[0]));
                        printf("Position limit 2 [inf - sup]: [%d - %d]\n", (pos_limits[2] >> resolution[1]), (pos_limits[3] >> resolution[1]));
                    } else {
                        printf("Unable to retrieve position limits\n");
                    }
                    break;

                //------------------------------     Get startup activation flag
                case 11:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_STARTUP_ACTIVATION, &aux_uchar, 1)) {
                        if (aux_uchar) {
                            printf("Startup activation YES\n");
                        } else {
                            printf("Startup activation NO\n");
                        }
                    } else {
                        printf("Unable to retrieve activation flag\n");
                    }
                    break;

                //----------------------------------------     Get current limit
                case 12:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_CURRENT_LIMIT, &current_limit, 1)) {
                        printf("Current limit: %d\n", current_limit);
                    } else {
                        printf("Unable to retrieve current limit\n");
                    }
                    break;

                //----------------------------------------     Get deflection control flag
                case 13:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_DEFLECTION_CONTROL, &aux_uchar, 1)) {
                        if(aux_uchar)
                            printf("Deflection control is ACTIVE\n");
                        else
                            printf("Deflection control is NOT ACTIVE\n");
                    }
                    else
                        printf("Unable to retrieve deflection control flag\n");

                    break;
                //----------------------------------------      Get maximum steps
                case 14:
                    if(!commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_POS, &aux_max_step_pos, 1) &&
                       !commGetParam(&comm_settings_t, BROADCAST_ID, PARAM_MAX_STEP_NEG, &aux_max_step_neg, 1)) {
                        printf("max_step_pos: %d\n", aux_max_step_pos);
                        printf("max_step_neg: %d\n", aux_max_step_neg);
                    } else {
                        printf("Unable to retrieve position limit flag\n");
                    }
                    break;

                default:
                    break;
            }

        }

    }

    return 1;
}

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

            file = fopen(QBMOVE_FILE, "w+");
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

int calibrate() {
    printf("Calibrating...");
    fflush(stdout);
    if(!commCalibrate(&comm_settings_t, BROADCAST_ID)) {
        printf("DONE\n");
        return 1;
    } else {
        printf("FAILED\n");
        return 0;
    }
}


int open_port() {
    FILE *file;
    char port[255];

    file = fopen(QBMOVE_FILE, "r");

    if (file == NULL) {
        printf("Error opening file %s\n", QBMOVE_FILE);
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
    usleep(500000);

    return 1;
}

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


void printVersion() {
    printf("==============================================\n");
    printf("qbparam version: %s\n", QBMOVEADMIN_VERSION);
    printf("==============================================\n");
}

void printMainMenu() {
    printf("g: getParam\n");
    printf("s: setParam\n");
    printf("m: initMemory\n");
    printf("c: calibrate\n");
    printf("==============================================\n");
}

void printParamMenu() {
    printf("----------------------------------------------\n");
    printf(" 1: ID\n");
    printf(" 2: PID params\n");
    printf(" 3: PID params current\n");
    printf(" 4: input mode\n");
    printf(" 5: control mode\n");
    printf(" 6: resolution\n");
    printf(" 7: measurements offset\n");
    printf(" 8: multipliers\n");
    printf(" 9: position limit flag\n");
    printf("10: position limits\n");
    printf("11: startup activation flag\n");
    printf("12: current limit\n");
    printf("13: deflection control flag\n");
    printf("14: Max steps\n");
    printf("----------------------------------------------\n");
}

/* END OF FILE */