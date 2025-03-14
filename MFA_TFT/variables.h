// defining the following, reduced the ram space because "100km" has not to be coded on several places 
#define HUNDERTKM "100km"
#define LITER     "l"
#define KM        "km"
#define HOUR      "h"
#define MINUTES   "min"
#define DEGREE    "`C"
#define RPM       "U"
#define CAR_NO    "HI-SN 3463"

// define case for differnt calculation
enum memory{
  START,               // values from start
  REFUEL,              // values since refuel
  PERIOD               // values long period
};

// position variables and colors to display Infos 5:6:5 Format 1.Letter = red 2 and 3 are Green and 4 is blue
#define NIGHT_TEXT_COLOR 0x041F
#define DAY_TEXT_COLOR   TFT_WHITE
#define BACK_COLOR  TFT_BLACK

// Voltage divider
// R1 = 220k ; R2 = 33k
// as U2 = (Uges * R2) / (R1 + R2)
// at 12 V we get U2 = 1.565217391 V
// 3.3V = 4095
// 1.565217391V = 1943
// factor 1.1 because reading was wrong
#define VOLT_FAKTOR (3.3 / 4095.0 ) * ((220.0 + 33.0 ) / 33.0) * 1.12

float volt = 0;                       // Voltage calculated from analog in PIN 32
unsigned long shutdown_timer = 0;   	// time for update values in msec

uint16_t TEXT_COLOR = DAY_TEXT_COLOR;  // with this we are able to change color with light state

int Icon_Pos_Coolant[2]     = { 20,  50};   // position of LED icon    coolant
int Icon_Pos_BrakeSystem[2] = {135,  50};   // position of LED icon    brakepads 
int Icon_Pos_Petrol[2]      = {240,  50};   // position of LED icon    petrol
int Icon_Pos_Oil[2]         = { 20, 140};   // position of LED icon    oil 
int Icon_Pos_BrakePads[2]   = {135, 140};   // position of LED icon    brakepads 
int Icon_Pos_WasherFluid[2] = {240, 140};   // position of LED icon    washer_fluid
int Icon_Pos_Light[2]       = { 20, 210};   // position of LED icon    light
int Icon_Pos_Door[2]        = {135, 210};   // position of LED icon    door 
int Icon_Pos_Batterie[2]    = {240, 210};   // position of LED icon    batterie


// DEBUG:
// float debug = 0;
// float bitValue [8];

struct values_to_save {           // Data to be stored permanetly, 
    int page; 				            // Page to display
    int mode;                     // Mode is on of START, REFUEL, PERIOD
    int deposit_last;             // last stored content of deposit, needed to check if car has been refueled
    unsigned int km_start;        // distance driven since start           
    unsigned int km_refuel;       // distance driven since refule           
    unsigned int km_long_period;  // distance driven since last manual reset
    float C_start;                // consumption since start ml
    float C_refuel;               // consumption since refule ml
    float C_long_period;          // consumption since last manual reset l
    float C_25_km[25];            // consumption from the last 25 driven km in ml
    float C_last_km;              // conssumption of the last km, needed to calculate the last 25 km average in ml
                                  // need to store the value, as we may have driven only 800 m on shut down and we do want to loose the amount used 
    unsigned int time_start;      // time since last start min
    unsigned int time_refuel;     // time since last refuel min
    unsigned int time_long_period;// time since last manual reset min
    unsigned int velocity_max;    // may be playing only ;-)
};

//values_to_save Data
values_to_save Data;
// values_to_save Data = { 0,
//                         0, 
//                         0,
//                         0, 
//                         0, 
//                         0, 
//                         0.0, 
//                         0.0, 
//                         0.0, 
//                         { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },                       
//                         0.0, 
//                         0, 
//                         0, 
//                         0, 
//                         0
//                       };

int temp_page;                        // temporary page to show door open and others

float C_last_25_km;                   // consumption over the last 25 km in ml

char TFT_String[12];                  // convert values to string to print on the screen
float temp = 0.0;                     // holds temporarily different values 

// variables for the button pin
int IO_STATUS[16];
int PREVIOUS_IO_STATUS = 0;
unsigned int Mode_Button_pressed = 0;
bool Page_Switch_Done = true;
int reset = 0;                        // Set to 1 to reset from start, 2 to reset refuel and 3 to reset period

bool start = false;                   // is needed for initial screen display
bool motor_on	= false;                // Speed has been over 900 U/min than motor_on = true
bool save = false;				            // Save is set to true if motor_on = true, after saving the variable will be set to false to not save continiously
bool check_led = false;               // only if true, we will redraw the screen painting or deleting the LED icon
                                      // avoid redrawing the screen every llop cycle

bool light = false;                   // light is on or off
bool door = false;                    // left door is open (true) or closed (false) from can message
bool door_r = true;                   // right door is open (false) or closed (true) from IO Pull Up
bool door_s = true;                   // sliding door is open (false) or closed (true) from IO Pull Up
bool trunk = true;                    // trunk is open (false) or closed (true) from IO Pull Up
bool motor_cap = true;                // motor cap is open (false) or closed (true) from IO Pull Up
bool petrol = false;                  // reserve warning controlled by 0x320 can message
bool oil_presure = true;              // oil presure warning controlled by IO Pull Up 
bool oil_level = true;                // oil level warning controlled by IO Pull Up
bool washer_fluid = true;             // washer_fluid warning controlled IO Pull Up
bool coolant = true;                  // coolant warning controlled IO Pull Up
bool brakepads = true;                // Brakepads warning controlled IO Pull Up
bool brakesystem = true;              // Brakefluid warning controlled IO Pull Up
int warnings = false;                 // we count infos, warnings and critical infos

int speed;                            // rpm of the motor
unsigned int km_total = 0;            // total km driven by the vehicle / control the change of this value to recalculate values
                                      // must be 0 at start to not add km with fist CAN message

unsigned int C_motor_value;           // the value recieved from motor in µl
unsigned int C_last = 0;              // last saved consumption in µl
float C_actual;			        	        // consumption ml, from 0x480 calculated in l/h  may be displayed also as l/100km

int temp_oil;		    	                // Oil temp in °C
float temp_out;                       // outer temperatur in °C (multiplied by factor 10 to be able to use int)

unsigned long valueMillis;       	  	// time for update values in msec
unsigned long lastMillis;         		// time for calculation in msec
unsigned long time_last;      		    // time since last loop in sec, will be reseted every 60 Seconds and other times will be increased
unsigned long time_C_period = 0;      // time elepsed from consumption update ms
unsigned long time_C_period_last = 0; // time elepsed from last consumption update ms

int deposit = 0;        	            // content of deposit

unsigned int velocity_actual;	        // actual velocity in km/h
unsigned int velocity_cruise_control; // actual velocity in km/h
bool units_l_100_km = false;          // true if l/100km ; false if l/h ; start as false, as car is not moving at start 
                                      // as on start the value will be false, as car is not moving
                                      // we set this to true to have an initial change
