#include "axis.h"

struct hal_s_ {
    long                   report;        // What to report?
    unsigned char          debug_reg_on;  // with '\' as a command this turns ON
    int                    expected_data;
    volatile unsigned char new_gyro;
};
static struct hal_s_ hal = { 0 };

void gyro_data_ready_cb( void ) {
    hal.new_gyro = 1;
}
/*******************************************************************************/
#define PRINT_ACCEL         ( 0x01 )
#define PRINT_GYRO          ( 0x02 )
#define PRINT_RAW_GYRO      ( 0x04 )
#define PRINT_COMPASS       ( 0x08 )
#define PRINT_RAW_COMPASS   ( 0x10 )
#define PRINT_RV            ( 0x20 )
#define PRINT_GRV           ( 0x40 )
#define PRINT_ORIENT        ( 0x80 )
#define PRINT_LINEAR_ACCEL  ( 0x100 )
#define PRINT_GRAVITY       ( 0x200 )
#define PRINT_STEP_COUNTER  ( 0x400 )
#define PRINT_STEP_DETECTOR ( 0x800 )
#define PRINT_SMD           ( 0x1000 )
#define PRINT_GEOMAG        ( 0x2000 )
#define PRINT_PRESSURE      ( 0x8000 )
#define PRINT_CUBE_GRV      ( 0x10000 )
#define PRINT_CUBE_RV       ( 0x20000 )
#define PRINT_CUBE_GEOMAG   ( 0x40000 )
#define PRINT_LPQ           ( 0x80000 )
#define PRINT_BAC           ( 0x100000 )
#define PRINT_FLIP_PICKUP   ( 0x200000 )
#define PRINT_TILT          ( 0x400000 )
#define PRINT_PROXIMITY     ( 0x800000 )
#define PRINT_HRM           ( 0x1000000 )
#define PRINT_SHAKE         ( 0x2000000 )
#define PRINT_B2S           ( 0x4000000 )

// for ICM20648
#define PRINT_GES_GROUP ( PRINT_STEP_COUNTER | PRINT_STEP_DETECTOR | PRINT_SMD | PRINT_BAC | PRINT_FLIP_PICKUP | PRINT_TILT )

int        self_test_result = 0;
int        dmp_bias[ 9 ]    = { 0 };
static int self_test_done   = 0;

unsigned short accel_data_was_set       = 0;
unsigned short gyro_data_was_set        = 0;
unsigned short raw_gyro_data_was_set    = 0;
unsigned short compass_data_was_set     = 0;
unsigned short raw_compass_data_was_set = 0;
unsigned short quat6_data_was_set       = 0;
unsigned short quat9_data_was_set       = 0;
unsigned short rv_quat_was_set          = 0;
unsigned short gmrv_quat_was_set        = 0;
unsigned short bac_data_was_set         = 0;
unsigned short flip_data_was_set        = 0;
unsigned short tilt_data_was_set        = 0;

float       grv_float[ 4 ];
signed long cube_grv[ 4 ] = { 0, 0, 0, 0 };
float       accel_float[ 3 ];
float       rv_float[ 4 ];
signed long cube_rv[ 4 ] = { 0, 0, 0, 0 };
float       gyro_float[ 3 ];
float       gyro_raw_float[ 3 ];
float       gyro_bias_float[ 3 ];
long        previous_bias[ 3 ];
float       compass_float[ 3 ];
float       compass_raw_float[ 3 ];
float       compass_bias[ 3 ];
#if ( MEMS_CHIP == HW_ICM20648 )
float       gmrv_float[ 4 ];
signed long cube_gmrv[ 4 ] = { 0, 0, 0, 0 };
#endif
int accel_accuracy   = 0;
int gyro_accuracy    = 0;
int rv_accuracy      = 0;
int compass_accuracy = 0;
#if ( MEMS_CHIP == HW_ICM20648 )
int gmrv_accuracy = 0;
#endif

long long ts = 0;

signed long result_quat[ 4 ] = { 0, 0, 0, 0 };

int         a_average[ 3 ] = { 0, 0, 0 };
int         g_average[ 3 ] = { 0, 0, 0 };
#if defined MEMS_AUGMENTED_SENSORS
float       linAccFloat[ 3 ];
float       gravityFloat[ 3 ];
float       orientationFloat[ 3 ];
#endif

unsigned long steps               = 0;
uint16_t      bac_state           = 0;
uint8_t       tilt_state          = 0;
long          bac_ts              = 0;
uint16_t      flip_pickup         = 0;
uint8_t       step_detected       = 0;
float         current_output_rate = 5;

/** @brief Set of flags for BAC state */
#define BAC_DRIVE 0x01
#define BAC_WALK  0x02
#define BAC_RUN   0x04
#define BAC_BIKE  0x08
#define BAC_TILT  0x10
#define BAC_STILL 0x20

/* Change ACCEL_GYRO_CHIP_ADDR if necessary */
const unsigned char ACCEL_GYRO_CHIP_ADDR = 0x68;

/* Change ACCEL_GYRO_ORIENTATION according to actual mount matrix */
signed char ACCEL_GYRO_ORIENTATION[] = { 0, -1, 0, 1, 0, 0, 0, 0, 1 };

/* Change the COMPASS_SLAVE_ID to the correct ID of compass used. You can find the defines in inv_mems_hw_config.h*/
const unsigned char COMPASS_SLAVE_ID = HW_AK09916;

/* Change COMPASS_CHIP_ADDR to 0x0C for ICM20948 which uses internal AK09916 */
/* Change COMPASS_CHIP_ADDR to 0x0E for other AK09912/09911/09913/8963 */
const unsigned char COMPASS_CHIP_ADDR  = 0x0C;
const unsigned char PRESSURE_CHIP_ADDR = 0x00;

/* Change COMPASS_ORIENTATION according to actual mount matrix */
signed char COMPASS_ORIENTATION[] = { 0, -1, 0, 1, 0, 0, 0, 0, 1 };

/* Change SOFT_IRON_MATRIX if necessary (q30) */
long SOFT_IRON_MATRIX[] = { 1073741824, 0, 0, 0, 1073741824, 0, 0, 0, 1073741824 };

#define INV_TST_LEN 256
char tst[ INV_TST_LEN ] = { 0 };

#define MAX_BUF_LENGTH ( 18 )

enum packet_type_e { PACKET_TYPE_ACCEL, PACKET_TYPE_GYRO, PACKET_TYPE_QUAT, PACKET_TYPE_COMPASS = 7 };

//----------------------------------------------------------------

void send_data_packet( char packet_type, void *data ) {
    long *ldata = ( long * )data;
    char  buf[ MAX_BUF_LENGTH ], length = 0;

    memset( buf, 0, MAX_BUF_LENGTH );
    buf[ 0 ] = '$';
    buf[ 1 ] = packet_type;

    switch ( packet_type ) {
    case PACKET_TYPE_ACCEL:
    case PACKET_TYPE_GYRO:
    case PACKET_TYPE_COMPASS:
        buf[ 2 ]  = ( char )( ldata[ 0 ] >> 24 );
        buf[ 3 ]  = ( char )( ldata[ 0 ] >> 16 );
        buf[ 4 ]  = ( char )( ldata[ 0 ] >> 8 );
        buf[ 5 ]  = ( char )ldata[ 0 ];
        buf[ 6 ]  = ( char )( ldata[ 1 ] >> 24 );
        buf[ 7 ]  = ( char )( ldata[ 1 ] >> 16 );
        buf[ 8 ]  = ( char )( ldata[ 1 ] >> 8 );
        buf[ 9 ]  = ( char )ldata[ 1 ];
        buf[ 10 ] = ( char )( ldata[ 2 ] >> 24 );
        buf[ 11 ] = ( char )( ldata[ 2 ] >> 16 );
        buf[ 12 ] = ( char )( ldata[ 2 ] >> 8 );
        buf[ 13 ] = ( char )ldata[ 2 ];
        length    = 14;
        break;
    case PACKET_TYPE_QUAT:
        buf[ 2 ]  = ( char )( ldata[ 0 ] >> 24 );
        buf[ 3 ]  = ( char )( ldata[ 0 ] >> 16 );
        buf[ 4 ]  = ( char )( ldata[ 0 ] >> 8 );
        buf[ 5 ]  = ( char )ldata[ 0 ];
        buf[ 6 ]  = ( char )( ldata[ 1 ] >> 24 );
        buf[ 7 ]  = ( char )( ldata[ 1 ] >> 16 );
        buf[ 8 ]  = ( char )( ldata[ 1 ] >> 8 );
        buf[ 9 ]  = ( char )ldata[ 1 ];
        buf[ 10 ] = ( char )( ldata[ 2 ] >> 24 );
        buf[ 11 ] = ( char )( ldata[ 2 ] >> 16 );
        buf[ 12 ] = ( char )( ldata[ 2 ] >> 8 );
        buf[ 13 ] = ( char )ldata[ 2 ];
        buf[ 14 ] = ( char )( ldata[ 3 ] >> 24 );
        buf[ 15 ] = ( char )( ldata[ 3 ] >> 16 );
        buf[ 16 ] = ( char )( ldata[ 3 ] >> 8 );
        buf[ 17 ] = ( char )ldata[ 3 ];
        length    = 18;
        break;
    }
    UART_Write( UART0, ( uint8_t * )buf, length );
}

inv_error_t set_output_rates( float rate ) {
    unsigned short base_rate = 1125;
    inv_error_t    result    = 0;

    if ( hal.report & PRINT_GES_GROUP ) {
        // Set 56Hz x N to feed 56Hz data to algo in DMP
        if ( rate >= 225 )
            rate = 225;
        else if ( rate >= 102 )
            rate = 112.5;
        else if ( rate >= 51 )
            rate = 56.5;
    }

    if ( hal.report & PRINT_ACCEL ) result |= inv_set_odr( ANDROID_SENSOR_ACCELEROMETER, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_GYRO ) result |= inv_set_odr( ANDROID_SENSOR_GYROSCOPE, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_RAW_GYRO ) result |= inv_set_odr( ANDROID_SENSOR_GYROSCOPE_UNCALIBRATED, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_GRV ) result |= inv_set_odr( ANDROID_SENSOR_GAME_ROTATION_VECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_CUBE_GRV ) result |= inv_set_odr( ANDROID_SENSOR_GAME_ROTATION_VECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_STEP_DETECTOR ) result |= inv_set_odr( ANDROID_SENSOR_STEP_DETECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_SMD ) result |= inv_set_odr( ANDROID_SENSOR_WAKEUP_SIGNIFICANT_MOTION, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_STEP_COUNTER ) result |= inv_set_odr( ANDROID_SENSOR_STEP_COUNTER, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_LINEAR_ACCEL ) result |= inv_set_odr( ANDROID_SENSOR_LINEAR_ACCELERATION, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_GRAVITY ) result |= inv_set_odr( ANDROID_SENSOR_GRAVITY, ( unsigned short )( base_rate / rate ) );
    if ( ( hal.report & PRINT_COMPASS ) || ( hal.report & PRINT_RAW_COMPASS ) ) result |= inv_set_odr( ANDROID_SENSOR_GEOMAGNETIC_FIELD, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_RAW_COMPASS ) result |= inv_set_odr( ANDROID_SENSOR_MAGNETIC_FIELD_UNCALIBRATED, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_RV ) result |= inv_set_odr( ANDROID_SENSOR_ROTATION_VECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_GEOMAG ) result |= inv_set_odr( ANDROID_SENSOR_GEOMAGNETIC_ROTATION_VECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_BAC ) result |= inv_set_odr( ANDROID_SENSOR_ACTIVITY_CLASSIFICATON, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_TILT ) result |= inv_set_odr( ANDROID_SENSOR_WAKEUP_TILT_DETECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_FLIP_PICKUP ) result |= inv_set_odr( ANDROID_SENSOR_FLIP_PICKUP, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_CUBE_RV ) result |= inv_set_odr( ANDROID_SENSOR_ROTATION_VECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_CUBE_GEOMAG ) result |= inv_set_odr( ANDROID_SENSOR_GEOMAGNETIC_ROTATION_VECTOR, ( unsigned short )( base_rate / rate ) );
    if ( hal.report & PRINT_ORIENT ) result |= inv_set_odr( ANDROID_SENSOR_ORIENTATION, ( unsigned short )( base_rate / rate ) );
    inv_reset_dmp_odr_counters();
    dmp_reset_fifo();
    return result;
}

int handle_char_input( char c ) {
    switch ( c ) {
    case 'a':
        dmp_reset_fifo();
        hal.report ^= PRINT_ACCEL;
        inv_enable_sensor( ANDROID_SENSOR_ACCELEROMETER, !!( hal.report & PRINT_ACCEL ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "ACCEL....output toggled, now is: %s.\r\n", ( hal.report & PRINT_ACCEL ) ? "ON" : "OFF" );
        break;
    case 'g':
        dmp_reset_fifo();
        hal.report ^= PRINT_GYRO;
        inv_enable_sensor( ANDROID_SENSOR_GYROSCOPE, !!( hal.report & PRINT_GYRO ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "GYRO....output toggled, now is: %s.\r\n", ( hal.report & PRINT_GYRO ) ? "ON" : "OFF" );
    case 'G':
        dmp_reset_fifo();
        hal.report ^= PRINT_RAW_GYRO;
        inv_enable_sensor( ANDROID_SENSOR_GYROSCOPE_UNCALIBRATED, !!( hal.report & PRINT_RAW_GYRO ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "RAW GYRO....output toggled, now is: %s.\r\n", ( hal.report & PRINT_RAW_GYRO ) ? "ON" : "OFF" );

        break;
    case 'c':
        dmp_reset_fifo();
        hal.report ^= PRINT_COMPASS;
        if ( ( hal.report & PRINT_RAW_COMPASS ) || ( hal.report & PRINT_COMPASS ) )
            inv_enable_sensor( ANDROID_SENSOR_GEOMAGNETIC_FIELD, 1 );
        else
            inv_enable_sensor( ANDROID_SENSOR_GEOMAGNETIC_FIELD, 0 );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Compass....output toggled, now is: %s.\r\n", ( hal.report & PRINT_COMPASS ) ? "ON" : "OFF" );

        break;
    case 'C':
        dmp_reset_fifo();
        hal.report ^= PRINT_RAW_COMPASS;
        inv_enable_sensor( ANDROID_SENSOR_MAGNETIC_FIELD_UNCALIBRATED, !!( hal.report & PRINT_RAW_COMPASS ) );
        if ( ( hal.report & PRINT_RAW_COMPASS ) || ( hal.report & PRINT_COMPASS ) )
            inv_enable_sensor( ANDROID_SENSOR_GEOMAGNETIC_FIELD, 1 );  // to calculate bias in application layer
        else
            inv_enable_sensor( ANDROID_SENSOR_GEOMAGNETIC_FIELD, 0 );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Raw Compass....output toggled, now is: %s.\r\n", ( hal.report & PRINT_RAW_COMPASS ) ? "ON" : "OFF" );

        break;
    case 'r':
        dmp_reset_fifo();
        hal.report ^= PRINT_GRV;
        inv_enable_sensor( ANDROID_SENSOR_GAME_ROTATION_VECTOR, !!( hal.report & PRINT_GRV ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Game RV....output toggled, now is: %s.\r\n", ( hal.report & PRINT_GRV ) ? "ON" : "OFF" );

        break;
    case 'q':
        dmp_reset_fifo();
        hal.report ^= PRINT_CUBE_GRV;
        inv_enable_sensor( ANDROID_SENSOR_GAME_ROTATION_VECTOR, !!( hal.report & PRINT_CUBE_GRV ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Cube Game RV....output toggled, now is: %s.\r\n", ( hal.report & PRINT_CUBE_GRV ) ? "ON" : "OFF" );

        break;
    case 'd':
        dmp_reset_fifo();
        hal.report ^= PRINT_STEP_DETECTOR;
        inv_enable_sensor( ANDROID_SENSOR_STEP_DETECTOR, !!( hal.report & PRINT_STEP_DETECTOR ) );
        inv_reset_dmp_odr_counters();

        // pedometer always runs at half the rate of BAC, to run pedometer at 56Hz, run BAC at 112Hz as pedometer divider is always 2
        set_output_rates( current_output_rate );
        printf( "Step Detector....output toggled, now is: %s.\r\n", ( hal.report & PRINT_STEP_DETECTOR ) ? "ON" : "OFF" );

        break;
    case 'm':
        dmp_reset_fifo();
        hal.report ^= PRINT_SMD;
        inv_enable_sensor( ANDROID_SENSOR_WAKEUP_SIGNIFICANT_MOTION, !!( hal.report & PRINT_SMD ) );
        inv_reset_dmp_odr_counters();
        // pedometer always runs at half the rate of BAC, to run pedometer at 56Hz, run BAC at 112Hz as pedometer divider is always 2
        set_output_rates( current_output_rate );
        printf( "SMD....output toggled, now is: %s.\r\n", ( hal.report & PRINT_SMD ) ? "ON" : "OFF" );

        break;
    case 'p':
        dmp_reset_fifo();
        hal.report ^= PRINT_STEP_COUNTER;
        inv_enable_sensor( ANDROID_SENSOR_STEP_COUNTER, !!( hal.report & PRINT_STEP_COUNTER ) );
        inv_reset_dmp_odr_counters();
        // pedometer always runs at half the rate of BAC, to run pedometer at 56Hz, run BAC at 112Hz as pedometer divider is always 2
        set_output_rates( current_output_rate );
        printf( "Step Counter....output toggled, now is: %s.\r\n", ( hal.report & PRINT_STEP_COUNTER ) ? "ON" : "OFF" );

        break;
    case 'R':
        dmp_reset_fifo();
        hal.report ^= PRINT_RV;
        inv_enable_sensor( ANDROID_SENSOR_ROTATION_VECTOR, !!( hal.report & PRINT_RV ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "RV....output toggled, now is: %s.\r\n", ( hal.report & PRINT_RV ) ? "ON" : "OFF" );

        break;
    case 'e':
        dmp_reset_fifo();
        hal.report ^= PRINT_GEOMAG;
        inv_enable_sensor( ANDROID_SENSOR_GEOMAGNETIC_ROTATION_VECTOR, !!( hal.report & PRINT_GEOMAG ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "GeoMAG RV....output toggled, now is: %s.\r\n", ( hal.report & PRINT_GEOMAG ) ? "ON" : "OFF" );

        break;
    case 'b':
        dmp_reset_fifo();
        if ( !( hal.report & PRINT_BAC ) ) dmp_reset_bac_states();
        hal.report ^= PRINT_BAC;
        inv_enable_sensor( ANDROID_SENSOR_ACTIVITY_CLASSIFICATON, !!( hal.report & PRINT_BAC ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "BAC....output toggled, now is: %s.\r\n", ( hal.report & PRINT_BAC ) ? "ON" : "OFF" );

        break;
    case 'T':
        dmp_reset_fifo();
        hal.report ^= PRINT_TILT;
        inv_enable_sensor( ANDROID_SENSOR_WAKEUP_TILT_DETECTOR, !!( hal.report & PRINT_TILT ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Tilt....output toggled, now is: %s.\r\n", ( hal.report & PRINT_TILT ) ? "ON" : "OFF" );

        break;
    case 'F':
        dmp_reset_fifo();
        hal.report ^= PRINT_FLIP_PICKUP;
        inv_enable_sensor( ANDROID_SENSOR_FLIP_PICKUP, !!( hal.report & PRINT_FLIP_PICKUP ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Flip Pickup....output toggled, now is: %s.\r\n", ( hal.report & PRINT_FLIP_PICKUP ) ? "ON" : "OFF" );

        break;
    case '0':
        set_output_rates( 1 );
        current_output_rate = 1;
        break;
    case '1':
        set_output_rates( 5 );
        current_output_rate = 5;
        break;
    case '2':
        set_output_rates( 15 );
        current_output_rate = 15;
        break;
    case '3':
        set_output_rates( 30 );
        current_output_rate = 30;
        break;
    case '4':
        set_output_rates( 51 );
        current_output_rate = 51;
        break;
    case '5':
        set_output_rates( 56.5 );
        current_output_rate = 56.5;
        break;
    case '6':
        set_output_rates( 60 );
        current_output_rate = 60;
        break;
    case '7':
        set_output_rates( 102 );
        current_output_rate = 102;
        break;
    case '8':
        set_output_rates( 112.5 );
        current_output_rate = 112.5;
        break;
    case '9':
        set_output_rates( 225 );
        current_output_rate = 225;
        break;
    case 'Q':
        dmp_reset_fifo();
        hal.report ^= PRINT_CUBE_RV;
        inv_enable_sensor( ANDROID_SENSOR_ROTATION_VECTOR, !!( hal.report & PRINT_CUBE_RV ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Cube RV....output toggled, now is: %s.\r\n", ( hal.report & PRINT_CUBE_RV ) ? "ON" : "OFF" );

        break;
    case '@':
        dmp_reset_fifo();
        hal.report ^= PRINT_CUBE_GEOMAG;
        inv_enable_sensor( ANDROID_SENSOR_GEOMAGNETIC_ROTATION_VECTOR, !!( hal.report & PRINT_CUBE_GEOMAG ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Cube GeoMAG RV....output toggled, now is: %s.\r\n", ( hal.report & PRINT_CUBE_GEOMAG ) ? "ON" : "OFF" );

        break;
    case 'v':
        dmp_reset_fifo();
        hal.report ^= PRINT_GRAVITY;
        inv_enable_sensor( ANDROID_SENSOR_GRAVITY, !!( hal.report & PRINT_GRAVITY ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Gravity....output toggled, now is: %s.\r\n", ( hal.report & PRINT_GRAVITY ) ? "ON" : "OFF" );

        break;
    case 'l':
        dmp_reset_fifo();
        hal.report ^= PRINT_LINEAR_ACCEL;
        inv_enable_sensor( ANDROID_SENSOR_LINEAR_ACCELERATION, !!( hal.report & PRINT_LINEAR_ACCEL ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Linear Accel....output toggled, now is: %s.\r\n", ( hal.report & PRINT_LINEAR_ACCEL ) ? "ON" : "OFF" );

        break;
    case 'o':
        dmp_reset_fifo();
        hal.report ^= PRINT_ORIENT;
        inv_enable_sensor( ANDROID_SENSOR_ORIENTATION, !!( hal.report & PRINT_ORIENT ) );
        inv_reset_dmp_odr_counters();
        set_output_rates( current_output_rate );
        printf( "Orientation....output toggled, now is: %s.\r\n", ( hal.report & PRINT_ORIENT ) ? "ON" : "OFF" );

        break;
    case 'k':
        printf( "Getting the DMP biases...\r\n" );

        memset( dmp_bias, 0, sizeof( dmp_bias ) );
        dmp_get_bias( dmp_bias );
        printf( "DMP   Accel Bias: X=%d, Y=%d, Z=%d\r\n", dmp_bias[ 0 ], dmp_bias[ 1 ], dmp_bias[ 2 ] );
        printf( "DMP   Gyro  Bias: X=%d, Y=%d, Z=%d\r\n", dmp_bias[ 3 ], dmp_bias[ 4 ], dmp_bias[ 5 ] );

        break;
    case 't':
        if ( hal.report ) {
            printf( "Disable all sensors before running self-test\r\n" );

            break;
        }

        {
            // uint8_t d[2];
            // int ret, i;
            char pass_str[] = { "PASS" };
            char fail_str[] = { "FAIL" };

            printf( "Selftest...Started\r\n" );

            /* Perform self-test */
            self_test_result = inv_mems_run_selftest();
            printf( "Selftest...Done...Ret=%d\r\n", self_test_result );

            printf(
                "Result: Compass=%s, Accel=%s, Gyro=%s\r\n",
                ( self_test_result & 0x04 ) ? pass_str : fail_str,
                ( self_test_result & 0x02 ) ? pass_str : fail_str,
                ( self_test_result & 0x01 ) ? pass_str : fail_str );

            printf( "Accel Average (LSB@FSR 2g)\r\n" );

            printf( "\tX:%d Y:%d Z:%d\r\n", a_average[ 0 ], a_average[ 1 ], a_average[ 2 ] );

            printf( "Gyro Average (LSB@FSR 250dps)\r\n" );

            printf( "\tX:%d Y:%d Z:%d\r\n", g_average[ 0 ], g_average[ 1 ], g_average[ 2 ] );

            /* Nothing to do if FAIL on gyro and accel */
            if ( ( self_test_result & 0x03 ) != 0x03 ) break;

            /* Handle bias got by self-test */
            // for ICM20648/20948
            dmp_bias[ 0 ] = a_average[ 0 ] * ( 1 << 11 );  // Change from LSB to format expected by DMP
            dmp_bias[ 1 ] = a_average[ 1 ] * ( 1 << 11 );
            dmp_bias[ 2 ] = ( a_average[ 2 ] - 16384 ) * ( 1 << 11 );  // remove the gravity and scale (FSR=2 in selftest)
            int scale     = 2000 / 250;                                // self-test uses 250dps FSR, main() set the FSR to 2000dps
            dmp_bias[ 3 ] = g_average[ 0 ] * ( 1 << 15 ) / scale;
            dmp_bias[ 4 ] = g_average[ 1 ] * ( 1 << 15 ) / scale;
            dmp_bias[ 5 ] = g_average[ 2 ] * ( 1 << 15 ) / scale;

            printf( "Factory Cal - Accel DMP biases: \tX:%d Y:%d Z:%d\r\n", dmp_bias[ 0 ], dmp_bias[ 1 ], dmp_bias[ 2 ] );

            printf( "Factory Cal - Gyro DMP biases:  \tX:%d Y:%d Z:%d\r\n", dmp_bias[ 3 ], dmp_bias[ 4 ], dmp_bias[ 5 ] );

            /* Update bias on DMP memory */
            dmp_set_bias( dmp_bias );
            printf( "\r\nSetting the DMP biases with one-axis factory calibration values...done\r\n" );
            self_test_done = 1;
        }
        break;
    // case SHOW_COMMANDS:
    default:
        break;
    }
    return 1;
}
void process_sensor_output() {
    signed long long_quat[ 3 ] = { 0 };

    if ( hal.report & PRINT_ACCEL ) {
        if ( accel_data_was_set == 1 ) {
            if ( self_test_done && ( accel_accuracy == 0 ) )  // self-test is done already
                accel_accuracy = 1;                           // accuracy reaches intermediate level after one-axis factory cal--yd
            printf( "Accel Data\t %8.5f, %8.5f, %8.5f, %d, %lld\r\n", accel_float[ 0 ], accel_float[ 1 ], accel_float[ 2 ], accel_accuracy, ts );
            accel_data_was_set = 0;
        }
    }

    if ( hal.report & PRINT_GYRO ) {
        if ( gyro_data_was_set == 1 ) {
            if ( self_test_done && ( gyro_accuracy == 0 ) )  // self-test is done already
                gyro_accuracy = 1;                           // accuracy reaches intermediate level after one-axis factory cal--yd
            printf( "Gyro Data\t %7.5f, %7.5f, %7.5f, %d, %lld\r\n", gyro_float[ 0 ], gyro_float[ 1 ], gyro_float[ 2 ], gyro_accuracy, ts );
            gyro_data_was_set = 0;
        }
    }

    if ( hal.report & PRINT_RAW_GYRO ) {
        if ( raw_gyro_data_was_set == 1 ) {
            printf( "Raw Gyro Data\t %7.5f, %7.5f, %7.5f,%d, %lld\r\n", gyro_raw_float[ 0 ], gyro_raw_float[ 1 ], gyro_raw_float[ 2 ], 0, ts );
            printf( "Gyro Bias\t %7.5f, %7.5f, %7.5f\r\n", gyro_bias_float[ 0 ], gyro_bias_float[ 1 ], gyro_bias_float[ 2 ] );
            raw_gyro_data_was_set = 0;
        }
    }

    if ( quat6_data_was_set == 1 ) {
        if ( hal.report & PRINT_GRV ) {
            printf( "Game RV\t %7.5f, %7.5f, %7.5f, %7.5f, %lld\r\n", grv_float[ 0 ], grv_float[ 1 ], grv_float[ 2 ], grv_float[ 3 ], ts );
            // printf\(tst\);
        }
        if ( hal.report & PRINT_CUBE_GRV ) {
            dmp_get_6quaternion( long_quat );
            inv_compute_scalar_part( long_quat, result_quat );
            send_data_packet( PACKET_TYPE_QUAT, ( void * )result_quat );
        }
        if ( hal.report & PRINT_GRAVITY ) {
            printf( "Gravity\t %7.5f, %7.5f, %7.5f, %lld\r\n", gravityFloat[ 0 ], gravityFloat[ 1 ], gravityFloat[ 2 ], ts );
            // printf\(tst\);
        }
        if ( hal.report & PRINT_LINEAR_ACCEL ) {
            printf( "LinearAcc\t %7.5f, %7.5f, %7.5f, %lld\r\n", linAccFloat[ 0 ], linAccFloat[ 1 ], linAccFloat[ 2 ], ts );
            // printf\(tst\);
        }
        quat6_data_was_set = 0;
    }

    if ( hal.report & PRINT_COMPASS ) {
        if ( compass_data_was_set == 1 ) {
            printf( "Compass Data\t %7.5f, %7.5f, %7.5f,\t%d, %lld\r\n", compass_float[ 0 ], compass_float[ 1 ], compass_float[ 2 ], compass_accuracy, ts );
            compass_data_was_set = 0;
        }
    }

    if ( hal.report & PRINT_RAW_COMPASS ) {
        if ( raw_compass_data_was_set == 1 ) {
            printf( "Raw Compass Data\t %7.5f, %7.5f, %7.5f,\t%d, %lld\r\n", compass_raw_float[ 0 ], compass_raw_float[ 1 ], compass_raw_float[ 2 ], 0, ts );
            printf( "Compass Bias\t %7.3f, %7.3f, %7.3f\r\n", compass_bias[ 0 ], compass_bias[ 1 ], compass_bias[ 2 ] );
            raw_compass_data_was_set = 0;
        }
    }

    if ( quat9_data_was_set == 1 ) {
        if ( hal.report & PRINT_RV ) {
            printf( "RV\t %7.5f, %7.5f, %7.5f, %7.5f, %d, %lld\r\n", rv_float[ 0 ], rv_float[ 1 ], rv_float[ 2 ], rv_float[ 3 ], rv_accuracy, ts );
            // printf\(tst\);
        }
        if ( hal.report & PRINT_CUBE_RV ) {
            long temp_long_quat[ 3 ];
            dmp_get_9quaternion( long_quat );
            inv_convert_rotation_vector_1( long_quat, temp_long_quat );
            inv_compute_scalar_part( temp_long_quat, result_quat );
            send_data_packet( PACKET_TYPE_QUAT, ( void * )result_quat );
        }
        if ( hal.report & PRINT_ORIENT ) {
            printf( "Orientation\t %7.5f, %7.5f, %7.5f, %lld\r\n", orientationFloat[ 0 ], orientationFloat[ 1 ], orientationFloat[ 2 ], ts );
            // printf\(tst\);
        }
        quat9_data_was_set = 0;
    }

    if ( gmrv_quat_was_set == 1 ) {
        if ( hal.report & PRINT_GEOMAG ) {
            printf( "GeoMAG RV\t %7.5f, %7.5f, %7.5f, %7.5f, %d, %lld\r\n", gmrv_float[ 0 ], gmrv_float[ 1 ], gmrv_float[ 2 ], gmrv_float[ 3 ], gmrv_accuracy, ts );
            // printf\(tst\);
        }
        if ( hal.report & PRINT_CUBE_GEOMAG ) { send_data_packet( PACKET_TYPE_QUAT, ( void * )cube_gmrv ); }
        gmrv_quat_was_set = 0;
    }

    if ( hal.report & PRINT_BAC ) {
        if ( bac_data_was_set == 1 ) {
            printf( "BAC Ts:\t %ld\r\n", bac_ts );
            // printf\(tst\);
            if ( ( bac_state >> 8 ) & BAC_DRIVE ) printf( "\t Enter Drive\r\n" );
            if ( ( bac_state >> 8 ) & BAC_WALK ) printf( "\t Enter Walk\r\n" );
            if ( ( bac_state >> 8 ) & BAC_RUN ) printf( "\t Enter Run\r\n" );
            if ( ( bac_state >> 8 ) & BAC_BIKE ) printf( "\t Enter Bike\r\n" );
            if ( ( bac_state >> 8 ) & BAC_TILT ) printf( "\t Enter Tilt\r\n" );
            if ( ( bac_state >> 8 ) & BAC_STILL ) printf( "\t Enter Still\r\n" );
            if ( bac_state & BAC_DRIVE ) printf( "\t Exit Drive\r\n" );
            if ( bac_state & BAC_WALK ) printf( "\t Exit Walk\r\n" );
            if ( bac_state & BAC_RUN ) printf( "\t Exit Run\r\n" );
            if ( bac_state & BAC_BIKE ) printf( "\t Exit Bike\r\n" );
            if ( bac_state & BAC_TILT ) printf( "\t Exit tilt\r\n" );
            if ( bac_state & BAC_STILL ) printf( "\t Exit Still\r\n" );

            bac_data_was_set = 0;
            // bac_prev_ts = ts;
        }
    }

    if ( hal.report & PRINT_FLIP_PICKUP ) {
        if ( flip_data_was_set == 1 ) {
            if ( flip_pickup == 1 )
                printf( "\t Flip Detected\r\n" );
            else if ( flip_pickup == 2 )
                printf( "\t Pickup Detected\r\n" );
            flip_data_was_set = 0;
        }
    }

    if ( hal.report & PRINT_TILT ) {
        if ( tilt_data_was_set == 1 ) {
            if ( tilt_state == 2 )
                printf( "\t Tilt Started\r\n" );
            else if ( tilt_state == 1 )
                printf( "\t Tilt Ended\r\n" );
            tilt_data_was_set = 0;
        }
    }
    if ( hal.report & PRINT_STEP_DETECTOR ) {
        if ( step_detected == 1 ) printf( "Step Detected>>>>\r\n" );
        step_detected = 0;
    }
}
void fifo_handler() {
    short             int_read_back = 0;
    unsigned short    header = 0, header2 = 0;
    int               data_left_in_fifo                       = 0;
    short             short_data[ 3 ]                         = { 0 };
    signed long       long_data[ 3 ]                          = { 0 };
    signed long       long_quat[ 3 ]                          = { 0 };
    static mpu_time_t lastIrqTimeUs                           = 0;
    static mpu_time_t currentIrqTimeUs                        = 0;
    unsigned short    sample_cnt_array[ GENERAL_SENSORS_MAX ] = { 0 };
    int               sample_nb;

    // Process Incoming INT and Get/Pack FIFO Data
    inv_identify_interrupt( &int_read_back );
    if ( int_read_back & ( BIT_MSG_DMP_INT | BIT_MSG_DMP_INT_0 | BIT_MSG_DMP_INT_2 | BIT_MSG_DMP_INT_5 ) ) {
        // Read FIFO contents and parse it.
        unsigned short total_sample_cnt = 0;
        currentIrqTimeUs                = inv_get_tick_count();

        do {
            if ( inv_mems_fifo_swmirror( &data_left_in_fifo, &total_sample_cnt, sample_cnt_array ) ) break;
            sample_nb = total_sample_cnt;

            if ( lastIrqTimeUs != 0 && sample_nb ) ts = ( currentIrqTimeUs - lastIrqTimeUs ) / sample_nb;

            if ( sample_nb ) lastIrqTimeUs = currentIrqTimeUs;

            while ( total_sample_cnt-- ) {
                if ( inv_mems_fifo_pop( &header, &header2, &data_left_in_fifo ) ) break;
                if ( header & ACCEL_SET ) {
                    float scale;
                    accel_data_was_set = 1;
                    dmp_get_accel( long_data );
                    accel_accuracy = inv_get_accel_accuracy();
                    scale          = ( 1 << inv_get_accel_fullscale() ) * 2.f / ( 1L << 30 );  // Convert from raw units to g's
                    scale *= 9.80665f;                                                         // Convert to m/s^2
                    inv_convert_dmp3_to_body( long_data, scale, accel_float );
                }  // header & ACCEL_SET

                if ( header & GYRO_SET ) {
                    float       scale;
                    signed long raw_data[ 3 ]  = { 0 };
                    signed long bias_data[ 3 ] = { 0 };
                    raw_gyro_data_was_set      = 1;
                    dmp_get_raw_gyro( short_data );
                    scale = ( 1 << inv_get_gyro_fullscale() ) * 250.f / ( 1L << 15 );  // From raw to dps
                    scale *= ( float )M_PI / 180.f;                                    // Convert to radian.
                    raw_data[ 0 ] = ( long )short_data[ 0 ];
                    raw_data[ 1 ] = ( long )short_data[ 1 ];
                    raw_data[ 2 ] = ( long )short_data[ 2 ];
                    inv_convert_dmp3_to_body( raw_data, scale, gyro_raw_float );

                    // We have gyro bias data in raw units, scaled by 2^5
                    dmp_get_gyro_bias( short_data );
                    scale = ( 1 << inv_get_gyro_fullscale() ) * 250.f / ( 1L << 20 );  // From raw to dps
                    scale *= ( float )M_PI / 180.f;                                    // Convert to radian.
                    bias_data[ 0 ] = ( long )short_data[ 0 ];
                    bias_data[ 1 ] = ( long )short_data[ 1 ];
                    bias_data[ 2 ] = ( long )short_data[ 2 ];
                    inv_convert_dmp3_to_body( bias_data, scale, gyro_bias_float );

                    if ( hal.report & PRINT_GYRO ) {
                        // shift to Q20 to do all operations in Q20
                        raw_data[ 0 ] = raw_data[ 0 ] << 5;
                        raw_data[ 1 ] = raw_data[ 1 ] << 5;
                        raw_data[ 2 ] = raw_data[ 2 ] << 5;
                        inv_mems_dmp_get_calibrated_gyro( long_data, raw_data, bias_data );
                        inv_convert_dmp3_to_body( long_data, scale, gyro_float );
                        gyro_data_was_set = 1;
                    }
                    gyro_accuracy = inv_get_gyro_accuracy();
                }  // header & GYRO_SET

                if ( header & GYRO_CALIBR_SET ) {}  // header & GYRO_CALIBR_SET

                if ( header & CPASS_CALIBR_SET ) {
                    float scale;
                    compass_data_was_set = 1;
                    dmp_get_calibrated_compass( long_data );
                    compass_accuracy = inv_get_mag_accuracy();
                    scale            = 1.52587890625e-005f;  // COMPASS_CONVERSION
                    inv_convert_dmp3_to_body( long_data, scale, compass_float );
                }  // header & CPASS_CALIBR_SET

                if ( header & CPASS_SET ) {
                    // Raw compass [DMP]
                    dmp_get_raw_compass( long_data );
                    compass_raw_float[ 0 ]   = long_data[ 0 ] * 1.52587890625e-005f;
                    compass_raw_float[ 1 ]   = long_data[ 1 ] * 1.52587890625e-005f;
                    compass_raw_float[ 2 ]   = long_data[ 2 ] * 1.52587890625e-005f;
                    compass_bias[ 0 ]        = compass_raw_float[ 0 ] - compass_float[ 0 ];
                    compass_bias[ 1 ]        = compass_raw_float[ 1 ] - compass_float[ 1 ];
                    compass_bias[ 2 ]        = compass_raw_float[ 2 ] - compass_float[ 2 ];
                    raw_compass_data_was_set = 1;
                }  // header & CPASS_SET

                if ( header & QUAT6_SET ) {
                    quat6_data_was_set = 1;
                    dmp_get_6quaternion( long_quat );
                    inv_convert_rotation_vector( long_quat, grv_float );
                    long gravityQ16[ 3 ], temp_gravityQ16[ 3 ];
                    long linAccQ16[ 3 ];
                    long accelQ16[ 3 ];
                    /*Calculate Gravity*/
                    inv_convert_rotation_vector_1( long_quat, temp_gravityQ16 );
                    inv_mems_augmented_sensors_get_gravity( gravityQ16, temp_gravityQ16 );
                    gravityFloat[ 0 ] = inv_q16_to_float( gravityQ16[ 0 ] );
                    gravityFloat[ 1 ] = inv_q16_to_float( gravityQ16[ 1 ] );
                    gravityFloat[ 2 ] = inv_q16_to_float( gravityQ16[ 2 ] );
                    /*Calculate Linear Acceleration*/
                    accelQ16[ 0 ] = ( int32_t )( ( float )( accel_float[ 0 ] ) * ( 1ULL << 16 ) + ( ( accel_float[ 0 ] >= 0 ) - 0.5f ) );
                    accelQ16[ 1 ] = ( int32_t )( ( float )( accel_float[ 1 ] ) * ( 1ULL << 16 ) + ( ( accel_float[ 1 ] >= 0 ) - 0.5f ) );
                    accelQ16[ 2 ] = ( int32_t )( ( float )( accel_float[ 2 ] ) * ( 1ULL << 16 ) + ( ( accel_float[ 2 ] >= 0 ) - 0.5f ) );
                    inv_mems_augmented_sensors_get_linearacceleration( linAccQ16, gravityQ16, accelQ16 );
                    linAccFloat[ 0 ] = inv_q16_to_float( linAccQ16[ 0 ] );
                    linAccFloat[ 1 ] = inv_q16_to_float( linAccQ16[ 1 ] );
                    linAccFloat[ 2 ] = inv_q16_to_float( linAccQ16[ 2 ] );
                }  // header & QUAT6_SET

                /* 9axis orientation quaternion sample available from DMP FIFO */
                if ( header & QUAT9_SET ) {
                    long orientationQ16[ 3 ], temp_orientationQ16[ 3 ];
                    quat9_data_was_set = 1;
                    dmp_get_9quaternion( long_quat );
                    rv_accuracy = ( int )( ( float )inv_get_rv_accuracy() / ( float )( 1ULL << ( 29 ) ) );
                    inv_convert_rotation_vector( long_quat, rv_float );
                    if ( hal.report & PRINT_ORIENT ) {
                        inv_convert_rotation_vector_1( long_quat, temp_orientationQ16 );
                        inv_mems_augmented_sensors_get_orientation( orientationQ16, temp_orientationQ16 );
                        orientationFloat[ 0 ] = inv_q16_to_float( orientationQ16[ 0 ] );
                        orientationFloat[ 1 ] = inv_q16_to_float( orientationQ16[ 1 ] );
                        orientationFloat[ 2 ] = inv_q16_to_float( orientationQ16[ 2 ] );
                    }
                }  // header & QUAT9_SET

                /* 6axis AM orientation quaternion sample available from DMP FIFO */
                if ( header & GEOMAG_SET ) {
                    /* Read 6 axis quaternion out of DMP FIFO in Q30 and convert it to Android format */
                    dmp_get_gmrvquaternion( long_quat );
                    if ( hal.report & PRINT_CUBE_GEOMAG ) inv_compute_scalar_part( long_quat, cube_gmrv );
                    if ( hal.report & PRINT_GEOMAG ) {
                        inv_convert_rotation_vector( long_quat, gmrv_float );
                        /* Read geomagnetic rotation vector heading accuracy out of DMP FIFO in Q29*/
                        gmrv_accuracy = ( int )( ( float )inv_get_gmrv_accuracy() / ( float )( 1ULL << ( 29 ) ) );
                    }
                    gmrv_quat_was_set = 1;
                }

                /* Activity recognition sample available from DMP FIFO */
                if ( header2 & ACT_RECOG_SET ) {
                    /* Read activity type and associated timestamp out of DMP FIFO
                    activity type is a set of 2 bytes :
                    - high byte indicates activity start
                    - low byte indicates activity end */
                    dmp_get_bac_state( &bac_state );
                    dmp_get_bac_ts( &bac_ts );

                    if ( hal.report & PRINT_TILT ) {
                        /* Tilt information is inside BAC, so extract it out */
                        /* Check if bit tilt is set for activity start byte */
                        if ( ( bac_state >> 8 ) & BAC_TILT ) {
                            /* Start of tilt detected */
                            tilt_state = 2;
                        }
                        /* Check if bit tilt is set for activity end byte */
                        else if ( bac_state & BAC_TILT ) {
                            /* End of tilt detected */
                            tilt_state = 1;
                        }
                    }

                    if ( hal.report & PRINT_BAC ) bac_data_was_set = 1;
                    if ( hal.report & PRINT_TILT ) tilt_data_was_set = 1;
                }

                if ( header2 & FLIP_PICKUP_SET ) {
                    if ( hal.report & PRINT_FLIP_PICKUP ) {
                        dmp_get_flip_pickup_state( &flip_pickup );
                        flip_data_was_set = 1;
                    }
                }

                process_sensor_output();
            }  // total_sample_cnt

            if ( !data_left_in_fifo ) break;
        } while ( data_left_in_fifo );

        if ( int_read_back & BIT_MSG_DMP_INT_3 ) {
            if ( hal.report & PRINT_STEP_DETECTOR ) { printf( "Step Detected>>>>>>>\r\n" ); }
        }

        if ( header & PED_STEPDET_SET ) {
            if ( hal.report & PRINT_STEP_COUNTER ) {
                unsigned long        steps = 0;
                static unsigned long old_steps;
                dmp_get_pedometer_num_of_steps( &steps );
                if ( steps != old_steps ) {
                    printf( "\tStep Counter %d\r\n", ( int )steps );

                    old_steps = steps;
                }
            }
        }

        if ( int_read_back & BIT_MSG_DMP_INT_2 ) {
            if ( hal.report & PRINT_SMD ) printf( ">> SMD Interrupt *********\r\n" );
        }
    }  // int_read_back
}

uint32_t debugPrintf = 0;

void EINT024_IRQHandler( void ) {
    gyro_data_ready_cb();
    GPIO_CLR_INT_FLAG( PA, BIT6 );
}

static uint8_t testTxData32Byte[ 32 ] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                          0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

void debug_functions( void ) {
    printf( "Start AXIS Debug.\n" );

    inv_error_t result;

    CLK_SysTickDelay( 1000000 );

    inv_set_chip_to_body_axis_quaternion( ACCEL_GYRO_ORIENTATION, 0.0 );
    result = inv_initialize_lower_driver( SERIAL_INTERFACE_I2C, 0 );

    inv_set_slave_compass_id( 0x24 );

    if ( result ) { printf( "Could not initialize.\r\n" ); }
    else {
        printf( "initialized.\r\n" );
    }

    result = 0;

    uint8_t intPinCfg;

    result |= inv_read_mems_reg( REG_INT_PIN_CFG, 1, &intPinCfg );
    intPinCfg |= 0xC0;
    result |= inv_write_single_mems_reg( REG_INT_PIN_CFG, intPinCfg );

    if ( result ) { printf( "ERR SET INT PIN CFG.\n" ); }

    GPIO_EnableInt( PA, 6, GPIO_INT_FALLING );
    NVIC_EnableIRQ( EINT024_IRQn );

    handle_char_input( '9' );

    handle_char_input( 't' );
    handle_char_input( 'Q' );

    while ( 1 ) {
        if ( hal.new_gyro == 1 ) {
            hal.new_gyro = 0;
            fifo_handler();
        }
        RF24_SendPackat( testTxData32Byte, 32, 1 );
    };

    while ( 1 ) { __NOP(); }
}
