// ----------------------------------------------------------------------------
// Copyright (C)  qbrobotics. All rights reserved.
// www.qbrobotics.com
// ----------------------------------------------------------------------------

/**
* \file         handmove.c
*
* \brief        Command line tools to use the qbHand
*
* \details      With this file is possible to use and open/close the qbHand
*               via command line tools. It is also possible to read the measurements
*               from all sensors and calibrate the qbHand.
*
* \copyright    (C)  qbrobotics. All rights reserved.
*/

/**
* \mainpage     Handmove command line tools 
*
* \brief        This is an application that permits you to set up your qbHands
*               from the command line.
*
* \version      5.3.0
*
* \author       qbrobotics
*
* \date         June 01, 2016
*
* \details      This is an application that permits you to set up your qbHands
*               from the command line.
*
*               This application can be compiled for Unix systems like Linux and
*               Mac OS X and even for Windows.
*
* \copyright    (C)  qbrobotics. All rights reserved.
*/

//==================================================================     defines

//#define TEST_MODE


//=================================================================     includes

#include "../../qbAPI/src/qbmove_communications.h"
#include "definitions.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define sleep(x) Sleep(1000 * x)
#endif

//===============================================================     structures


static const struct option longOpts[] = {
    { "set_inputs", required_argument, NULL, 's' },
    { "get_measurements", no_argument, NULL, 'g' },
    { "activate", no_argument, NULL, 'a' },
    { "deactivate", no_argument, NULL, 'd' },
    { "ping", no_argument, NULL, 'p' },
    { "serial_port", no_argument, NULL, 't' },
    { "verbose", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { "set_zeros", no_argument, NULL, 'z'},
    { "get_currents", no_argument, NULL, 'c'},
    { "get_emg", no_argument, NULL, 'q'},
    { "bootloader", no_argument, NULL, 'b'},
    { "calibration", no_argument, NULL, 'k'},
    { "velocities", no_argument, NULL, 'i'},
    { "accelerations", no_argument, NULL, 'o'},
    { NULL, no_argument, NULL, 0 }
};

static const char *optString = "s:adgptvh?zkcqbkio";

/** Global structure used to store the measurements values
    and utility flags that are used to see if a specific command
    is received
*/

struct global_args {
    int device_id;                  ///< Id of the device driven. Default = 0 (Broadcast message)
    int flag_set_inputs;            ///< ./handmove -s option
    int flag_get_measurements;      ///< ./handmove -g option
    int flag_activate;              ///< ./handmove -a option
    int flag_deactivate;            ///< ./handmove -d option
    int flag_ping;                  ///< ./handmove -p option
    int flag_serial_port;           ///< ./handmove -t option
    int flag_verbose;               ///< ./handmove -v option
    int flag_set_zeros;             ///< ./handmove -z option
    int flag_get_currents;          ///< ./handmove -c option
    int flag_bootloader_mode;       ///< ./handmove -b option
    int flag_get_emg;               ///< ./handmove -q option
    int flag_calibration;           ///< ./handmove -k option
    int flag_get_velocities;        ///< ./handmove -i option
    int flag_get_accelerations;     ///< ./handmove -o option


    short int inputs[NUM_OF_MOTORS];                ///< Motor reference given to the device
    short int measurements[NUM_OF_SENSORS];         ///< Encoder measurements read from the device
    short int velocities[NUM_OF_SENSORS];           ///< Encoder rotational speed read from the device
    short int accelerations[NUM_OF_SENSORS];        ///< Encoder rotational acceleration read from the device
    short int measurement_offset[NUM_OF_SENSORS];   ///< Measurement offsets set to save the device zero position
    short int currents[NUM_OF_MOTORS];              ///< Motor currents read from the device
    short int emg[NUM_OF_EMGS];                     ///< Emg sensors values read from the device
    char filename[255];                             ///< Name of the file passed to the parser
    short int calib_speed;                          ///< Calibration speed
    short int calib_repetitions;                    ///< Calibration repetitions

    FILE* emg_file;                                 ///< File where the emg sensors values are saved
} global_args;


//==========================================================    global variables

/** Motor gear ratio
*/
float gear_ratio[NUM_OF_MOTORS];

/** comm_settings structure containing the serial port handle value
*/
comm_settings comm_settings_1;



//=====================================================     function definitions

void display_usage( void );

float** file_parser(char*, int*, int*);

void int_handler_2(int sig);

void int_handler_3(int sig);


//==============================================================================
//                                                                     main loop
//==============================================================================


/** main loop
 */
int main (int argc, char **argv)
{

    FILE *file;

    int  i = 0;             // global counters

    int  num_ports = 0;     // variables for COMM config
    char ports[10][255];
    int  aux_int;
    char port[255];

    char aux_string[10000]; // used to store PING reply
    int  aux[3];            // used to store input during set_inputs

    int  option;            // used for processing options
    int  longIndex = 0;

    char aux_char;

    int aux_input = 0;
    int aux_encoder_flag = 0;
    int aux_handle_ratio = 0;
    
    // initializations

    global_args.device_id               = 0;
    global_args.flag_serial_port        = 0;
    global_args.flag_ping               = 0;
    global_args.flag_verbose            = 0;
    global_args.flag_activate           = 0;
    global_args.flag_deactivate         = 0;
    global_args.flag_get_measurements   = 0;
    global_args.flag_set_inputs         = 0;
    global_args.flag_set_zeros          = 0;
    global_args.flag_bootloader_mode    = 0;

    //===================================================     processing options

    while ((option = getopt_long( argc, argv, optString, longOpts, &longIndex )) != -1)
    {
        switch (option)
        {
            case 's':
                sscanf(optarg,"%d,%d", &aux[0], &aux[1]);
                global_args.inputs[0] = (short int) aux[0];
                global_args.inputs[1] = (short int) aux[1];
                global_args.flag_set_inputs = 1;
                break;
            case 'g':
                global_args.flag_get_measurements = 1;
                break;
            case 'a':
                global_args.flag_activate = 1;
                break;
            case 'd':
                global_args.flag_deactivate = 1;
                break;
            case 't':
                global_args.flag_serial_port = 1;
                break;
            case 'p':
                global_args.flag_ping = 1;
                break;
            case 'v':
                global_args.flag_verbose = 1;
                break;
            case 'z':
                global_args.flag_set_zeros = 1;
                break;
            case 'c':
                global_args.flag_get_currents = 1;
                break;
            case 'b':
                global_args.flag_bootloader_mode = 1;
                break;
            case 'q':
                global_args.flag_get_emg = 1;
                break;
            case 'k':
                printf("Specify speed [0 - 200]: ");
                scanf("%d", &aux_int);
                global_args.calib_speed = (short int)aux_int;
                printf("Specify repetitions [0 - 32767]: ");
                scanf("%d", &aux_int);
                global_args.calib_repetitions = (short int)aux_int;
                global_args.flag_calibration = 1;
                break;
            case 'i':
                global_args.flag_get_velocities = 1;
                break;
            case 'o':
                global_args.flag_get_accelerations = 1;
                break;
            case 'h':
            case '?':
            default:
                display_usage();
                return 0;
                break;
        }
    }

    if((optind == 1) | (global_args.flag_verbose & (optind == 2)))
    {
        display_usage();
        return 0;
    }

    //==================================================     setting serial port

    if(global_args.flag_serial_port)
    {

        num_ports = RS485listPorts(ports);
        int tmp = -1;
        aux_int = -1;

        if(num_ports)
        {
            puts("\nChoose serial port 1:\n");
            for(i = 0; i < num_ports; ++i)
            {
                printf("[%d] - %s\n\n", i+1, ports[i]);
            }
            printf("Serial port: ");
            scanf("%d", &aux_int);

            if( aux_int && (aux_int <= num_ports) )
            {
                tmp = aux_int;
            }
            else puts("Choice not available");

        } else {
            puts("No serial port available.");
        }

        if (num_ports - 1 > 0)
        {
            puts("\nChoose serial port 2:\n");
            for(i = 0; i < num_ports; ++i)
            {
                printf("[%d] - %s ", i+1, ports[i]);
                if (i == tmp - 1) {
                    printf("[unavailable] ");
                }
                printf("\n\n");
            }
            printf("Serial port: ");
            scanf("%d", &aux_int);
        }
        if( aux_int && (aux_int <= num_ports) )
        {
            file = fopen(HANDMOVE_FILE, "w+");
            if (file == NULL) {
                printf("Cannot open qbmove.conf\n");
            }
            fprintf(file,"serialport %s\n",ports[tmp-1]);
            fclose(file);
        } else {
            puts("Choice not available");
        }

        return 0;
    }

    //==========================================     reading configuration files

    if(global_args.flag_verbose)
        puts("Reading configuration files.");

    file = fopen(HANDMOVE_FILE, "r");

    if (file == NULL) {
        printf("\nUnable to open %s\n", HANDMOVE_FILE);
        printf("Try run handmove -t\n\n");
        return 0;
    }

    fscanf(file, "serialport %s\n", port);

    fclose(file);


    if(global_args.flag_verbose)
        printf("Port: %s\n", port);


    file = fopen(MOTOR_FILE, "r");

    if (file == NULL) {
        printf("\nUnable to open %s\n", MOTOR_FILE);
        return 0;
    }

    fscanf(file, "gear_ratio_1 %f\n", &gear_ratio[0]);
    fscanf(file, "gear_ratio_2 %f\n", &gear_ratio[1]);

    fclose(file);

    if(global_args.flag_verbose)
        printf("Gear ratio 1: %f\nGear ratio 2: %f\n",
            gear_ratio[0], gear_ratio[1]);


    //==================================================     opening serial port

#ifndef TEST_MODE

    if(global_args.flag_verbose)
        puts("Connecting to serial port.");

    openRS485(&comm_settings_1, port);

    if(comm_settings_1.file_handle == INVALID_HANDLE_VALUE)
    {
        puts("Couldn't connect to the serial port.");

        if(global_args.flag_verbose)
            puts("Closing the application.");

        return 0;
    } else {
        if(global_args.flag_verbose)
            puts("Serial port connected");
    }


    //====================================================     gettind device id

    if (argc - optind == 1)
    {
        sscanf(argv[optind++],"%d",&global_args.device_id);
        if(global_args.flag_verbose)
            printf("Device ID:%d\n", global_args.device_id);
    }
    else if(global_args.flag_verbose)
        puts("No device ID was chosen. Running in broadcasting mode.");

#endif

    //=================================================================     ping

    // If ping... then DOESN'T PROCESS OTHER COMMANDS

    if(global_args.flag_ping)
    {
        if(global_args.flag_verbose)
            puts("Pinging serial port.");

        if(global_args.device_id) {
            commGetInfo(&comm_settings_1, global_args.device_id, INFO_ALL, aux_string);
        } else {
            RS485GetInfo(&comm_settings_1,  aux_string);
        }

        puts(aux_string);

        if(global_args.flag_verbose)
            puts("Closing the application.");

        return 0;
    }

    //===========================================================     set inputs

    if(global_args.flag_set_inputs)
    {
        if(global_args.flag_verbose)
            printf("Setting inputs to %d and %d.\n", global_args.inputs[0], global_args.inputs[1]);

        commSetInputs(&comm_settings_1, global_args.device_id, global_args.inputs);

    }

//=========================================================     get measurements

    if(global_args.flag_get_measurements) {
        while(1) {
            if(commGetParam(&comm_settings_1, BROADCAST_ID, PARAM_INPUT_MODE, &aux_input, 1) != -1 &&
                commGetParam(&comm_settings_1, BROADCAST_ID, PARAM_DOUBLE_ENC_ON_OFF, &aux_encoder_flag, 1) != -1 &&
                commGetParam(&comm_settings_1, BROADCAST_ID, PARAM_MOT_HANDLE_RATIO, &aux_handle_ratio, 1) !=-1 )
            {
                commGetMeasurements(&comm_settings_1, global_args.device_id, global_args.measurements);

                printf("measurements: ");
                for (i = 0; i < NUM_OF_SENSORS; i++) {
                    if(aux_input == INPUT_MODE_ENCODER3)
                        if(aux_encoder_flag) {
                            if(i == NUM_OF_SENSORS - 1)
                                printf("%d\t", global_args.measurements[i] * aux_handle_ratio);
                            else
                                printf("%d\t", global_args.measurements[i]);
                        }
                        else {
                            if (i == NUM_OF_SENSORS - 2) 
                                printf("%d\t", global_args.measurements[i] * aux_handle_ratio);
                            else
                                printf("%d\t", global_args.measurements[i]);
                        }
                    else
                        printf("%d\t", global_args.measurements[i]);
                }
                printf("\n");
                usleep(100000);
            }    
        }
    }

//=========================================================     get velocities

    if(global_args.flag_get_velocities) {
        while(1) {
            commGetVelocities(&comm_settings_1, global_args.device_id, global_args.velocities);
            printf("velocities: ");
            for (i = 0; i < NUM_OF_SENSORS; ++i) {
                printf("%hd\t", global_args.velocities[i]);
            }
            printf("\n");
            usleep(100000);
        }    
    }

//=========================================================     get accelerations

    if(global_args.flag_get_accelerations) {
        while(1) {
            commGetAccelerations(&comm_settings_1, global_args.device_id, global_args.accelerations);
            printf("accelerations: ");
            for (i = 0; i < NUM_OF_SENSORS; i++)
                printf("%hd\t", global_args.accelerations[i]);
            printf("\n");
            usleep(100000);
        }    
    }

//=========================================================     set bootloader mode

    if(global_args.flag_bootloader_mode) {

        printf("Entering bootloader mode\n");
        if(!commBootloader(&comm_settings_1, global_args.device_id)) {
            printf("DONE\n");
        } else {
            printf("An error occurred...\n");
        }

    }

//==============================================================     calibration

    if (global_args.flag_calibration) {

        printf("Speed: %d     Repetitions: %d\n", global_args.calib_speed, global_args.calib_repetitions);
        commHandCalibrate(&comm_settings_1, global_args.device_id, global_args.calib_speed, global_args.calib_repetitions);
        usleep(100000);

    }

//=============================================================     get_currents

    if(global_args.flag_get_currents) {

        if(global_args.flag_verbose)
            puts("Getting currents.");

        while(1) {
            commGetCurrents(&comm_settings_1, global_args.device_id,
                global_args.currents);

            printf("Current 1: %d\t Current 2: %d\n", global_args.currents[0], global_args.currents[1]);
            usleep(100000);
        }
    }


//==========================================================     get_emg

    struct timeval t_act, begin;
    struct timezone foo;
    int delta_t = 1500; //in microseconds

    i = 0;

    if(global_args.flag_get_emg) {
        if(global_args.flag_verbose) {
            puts("Getting emg signals.");
        }

        signal(SIGINT, int_handler_3);
        global_args.emg_file = fopen(EMG_SAVED_VALUES, "w");

        // Initialize begin time
        gettimeofday(&begin, &foo);

        while(1) {
            i++;

            // Getting EMG values
            commGetEmg(&comm_settings_1, global_args.device_id,
                global_args.emg);
            if(global_args.flag_verbose) {
                printf("Signal 1: %d\t Signal 2: %d\n", global_args.emg[0], global_args.emg[1]);
            }

#define REFERENCES

#ifdef REFERENCES

            // Getting current references
            commGetInputs(&comm_settings_1, global_args.device_id, global_args.inputs);
            fprintf(global_args.emg_file, "%d,%d,%d,%d\n", global_args.emg[0],
                    global_args.emg[1], global_args.inputs[0],
                    (int)timevaldiff(&begin, &t_act));

#else //current position

            // Getting current position
            commGetMeasurements(&comm_settings_1, global_args.device_id, global_args.measurements);
            fprintf(global_args.emg_file, "%d,%d,%d,%d\n", global_args.emg[0],
                    global_args.emg[1], global_args.measurements[0],
                    (int)timevaldiff(&begin, &t_act));

#endif

            // Wait for actual time to be grater than begin + i * delta_t
            while(1) {
                gettimeofday(&t_act, &foo);
                if (timevaldiff(&begin, &t_act) >= i * delta_t) {
                    break;
                }
            }

        }
    }


//=================================================================     activate

    if(global_args.flag_activate)
    {
        if(global_args.flag_verbose)
           puts("Turning QB Move on.\n");
        commActivate(&comm_settings_1, global_args.device_id, 1);

        commGetActivate(&comm_settings_1, global_args.device_id, &aux_char);

        printf("%c %d\n", aux_char, (int)aux_char);
    }

//===============================================================     deactivate

    if(global_args.flag_deactivate)
    {
        if(global_args.flag_verbose)
           puts("Turning QB Move off.\n");
        commActivate(&comm_settings_1, global_args.device_id, 0);
    }


    //============================================================     set zeros
    if(global_args.flag_set_zeros)
    {
        struct timeval t_prec, t_act;
        struct timezone foo;

        signal(SIGINT, int_handler_2);
        printf("Press CTRL-C to set Zero Position\n\n");
        printf("Press return to proceed\n");
        getchar();

        // Deactivate device to avoid motor movements
        commActivate(&comm_settings_1, global_args.device_id, 0);

        // Reset all the offsets
        for (i = 0; i < NUM_OF_SENSORS; i++) {
            global_args.measurement_offset[i] = 0;
        }

        commSetParam(&comm_settings_1, global_args.device_id,
            PARAM_MEASUREMENT_OFFSET, global_args.measurement_offset,
            NUM_OF_SENSORS);


        //Display current values until CTRL-C is pressed
        gettimeofday(&t_prec, &foo);
        gettimeofday(&t_act, &foo);
        while(1) {
            while (1) {
                gettimeofday(&t_act, &foo);
                if (timevaldiff(&t_prec, &t_act) >= 200000) {
                    break;
                }
            }
            commGetMeasurements(&comm_settings_1, global_args.device_id,
                    global_args.measurements);
            for (i = 0; i < NUM_OF_SENSORS; i++) {
                printf("%d\t", global_args.measurements[i]);
            }
            printf("\n");

            gettimeofday(&t_prec, &foo);
        }
    }

//==========================     closing serial port and closing the application


    closeRS485(&comm_settings_1);

    if(global_args.flag_verbose)
        puts("Closing the application.");

    return 0;
}


//==============================================================================
//                                                                   file_parser
//==============================================================================

/** Parse .csv file and return a pointer to a matrix of float dinamically
 *  allocated.  Remember to use free(pointer) in the caller
 */

float** file_parser( char* filename, int* deltat, int* num_values )
{
    FILE* filep;
    float** array = NULL;
    int i;
    filep = fopen(filename, "r");
    if (filep == NULL) perror ("Error opening file");
    else {
        //read first line
        fscanf(filep, "%d,%d", deltat, num_values);

        //alloc memory for the arrays
        array = (float**)malloc(2*sizeof(float*));
        array[0] = (float*)malloc(*num_values*sizeof(float));
        array[1] = (float*)malloc(*num_values*sizeof(float));

        //read num_values line of file and store them in array
        for(i=0; i<*num_values; i++) {
            fscanf(filep, "%f,%f", &array[0][i], &array[1][i]);
        }
    fclose(filep);
    }
    return array;
}

//==============================================================================
//                                                          CTRL-C interruptions
//==============================================================================

/** Handles the ctrl+c interruption to save the measurement offsets of the device
*/
void int_handler_2(int sig) {
    int i;
    printf("\n\nSetting zero position\n");

    //Set the offsets equal to minus current positions
    for (i = 0; i < NUM_OF_SENSORS; i++) {
        global_args.measurement_offset[i] = -global_args.measurements[i];
    }

    commSetParam(&comm_settings_1, global_args.device_id,
            PARAM_MEASUREMENT_OFFSET, global_args.measurement_offset,
            NUM_OF_SENSORS);

    commStoreParams(&comm_settings_1, global_args.device_id);

    sleep(1);

    // set motors to 0,0
    global_args.inputs[0] = 0;
    global_args.inputs[1] = 0;
    commSetInputs(&comm_settings_1, global_args.device_id, global_args.inputs);

    //Activate board
    commActivate(&comm_settings_1, global_args.device_id, 1);

    exit(1);
}

/** Handles the ctrl+c interruption to save the emg sensors measurements into a file
*/

void int_handler_3(int sig) {
    printf("Closing file and quitting application...\n");

    fclose(global_args.emg_file);

    printf("DONE\n");

    exit(1);
}



//==============================================================================
//                                                                 dysplay usage
//==============================================================================

/** Display program usage, and exit.
*/

void display_usage( void )
{
    puts("================================================================================");
    printf("handmove version: %s\n", HANDADMIN_VERSION);
    puts("================================================================================");
    puts("usage: handmove [id] [OPTIONS]" );
    puts("--------------------------------------------------------------------------------");
    puts("Options:");
    puts("");
    puts(" -s, --set_inputs <value,value>   Send reference input for motor (the second");
    puts("                                  value is ignored).");
    puts(" -g, --get_measurements           Get measurements from the encoders.");
    puts(" -c, --get_currents               Get motor current.");
    puts(" -i, --get_velocities             Get motor velocities");
    puts(" -o, --get_accelerations          Get motor accelerations");
    puts(" -q, --get_emg                    Get EMG values and save them in a file");
    puts("                                  defined in \"definitions.h\". Use -v option");
    puts("                                  to display values in the console too.");
    puts(" -a, --activate                   Activate the motor control.");
    puts(" -d, --deactivate                 Deactivate the motor control.");
    puts(" -z, --set_zeros                  Set zero position for all sensors.");
    puts("");
    puts(" -p, --ping                       Get info on the device.");
    puts(" -t, --serial_port                Set up serial port.");
    puts(" -b, --bootloader                 Enter bootloader mode.");
    puts(" -k, --calibration                Makes a series of opening and closing.");
    puts(" -v, --verbose                    Verbose mode.");
    puts(" -h, --help                       Shows this information.");
    puts("");
    puts("--------------------------------------------------------------------------------");
    puts("Examples:");
    puts("");
    puts("  handmove -p                       Get info on whatever device is connected.");
    puts("  handmove -t                       Set up serial port.");
    puts("  handmove 65 -s 10,10              Set inputs of device 65 to 10 and 10.");
    puts("  handmove 65 -g                    Get measurements from device 65.");
    puts("  handmove 65 -g -s 10,10           Set inputs of device 65 to 10");
    puts("                                    and 10, and get measurements.");
    puts("  handmove 65 -a                    Turn device 65 on.");
    puts("  handmove 65 -d                    Turn device 65 off.");
    puts("================================================================================");
    /* ... */
    exit( EXIT_FAILURE );
}

/* [] END OF FILE */
