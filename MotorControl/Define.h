//Connection and message
#define BAUD 115200
#define STRING_COMMAND_LENGTH 2
#define NUMBER_COMMAND_LENGTH 4
#define COMPLETE_MESSAGE_LENGTH 3 * (STRING_COMMAND_LENGTH + NUMBER_COMMAND_LENGTH + 1)
#define NUMBER_COMMANDS_IN_MESSAGE 3

//Default position and motion
#define RA_PARK_POSITION 0
#define DEC_PARK_POSITION 0
#define MARGIN_FOR_SLEWING 10 //margin angle in degree

//Motor
#define RA_MOTOR_PIN1 40
#define RA_MOTOR_PIN2 41
#define DEC_MOTOR_PIN1 42
#define DEC_MOTOR_PIN2 43
#define RA_RATIO 40
#define DEC_RATIO 40

//Encoder
#define RA_ENCODER_PIN_A 20  //yellow cable
#define RA_ENCODER_PIN_B 21  //green cable
#define DEC_ENCODER_PIN_A 30 //yellow cable
#define DEC_ENCODER_PIN_B 31 //green cable
#define RA_INPUT_PER_TURN 256   //input per encoder turn on RA axis
#define DEC_INPUT_PER_TURN 256  //input per encoder turn on DEC axis

//DO NOT MODIFY! ALREADY CALCULATED VALUES
#define RA_INPUT_PER_TURN_RATIO RA_INPUT_PER_TURN * RA_RATIO
#define DEC_INPUT_PER_TURN_RATIO DEC_INPUT_PER_TURN * DEC_RATIO
float RA_DEGREE_EACH_INPUT = 360.0 / float(RA_INPUT_PER_TURN_RATIO);
float DEC_DEGREE_EACH_INPUT = 360.0 / float(DEC_INPUT_PER_TURN_RATIO);

//Messages
#define RA_COMMAND "RA"
#define DEC_COMMAND "DE"
#define TRACK_COMMAND "TR"
#define PARK_COMMAND "PA"
