// variables.h
#ifndef VARIABLES_H
#define VARIABLES_H

// defining the following, reduced the ram space because "100km" has not to be coded on several places 
#define HUNDERTKM "100km"
#define LITER     "l"
#define KM        "km"
#define HOUR      "h"
#define MINUTES   "min"
#define DEGREE    "`C"
#define RPM       "U"
#define CAR_NO    "MULTIVAN"


// define case for differnt calculation
enum memory{
  NOTHING,             // nichts zu tun
  START,               // values since start
  REFUEL,              // values since refuel
  PERIOD               // values long period
};

// position variables and colors to display Infos 5:6:5 Format 1.Letter = red 2 and 3 are Green and 4 is blue
#define NIGHT_TEXT_COLOR  0x64b8 //0x041F war etwas grell->    0x126F(org VW ?sehr dunkel, am Tag)
#define DAY_TEXT_COLOR    TFT_WHITE
#define BACK_COLOR        TFT_BLACK
#define SYM_NA_COLOR      0x4a4a   //Symbol not active color default: TFT_BLACK

// Voltage divider
// R1 = 220k ; R2 = 33k
// as U2 = (Uges * R2) / (R1 + R2)
// at 12 V we get U2 = 1.565217391 V
// 3.3V = 4095
// 1.565217391V = 1943
// factor 1.07 because reading was wrong
#define VOLT_FAKTOR (3.3 / 4095.0 ) * ((220.0 + 33.0 ) / 33.0) * 1.07


static float last_angle = -999.0f;        // compass letzte Richtung
static bool first_draw = true;

volatile uint8_t pending_reset = NOTHING;

volatile bool driving = false;

// some variables need to be declared volatile as they shared over Core(0) and Core(1)
// variables used only in core(0) are declared in evaluate_CAN_messages

unsigned long motor_off_time = 0;  // Zeitpunkt Motor aus

// Alle CAN-Werte + Berechnungen → volatile!
volatile float C_actual_l_per_h       = 0.0; // l/h – direkt aus 0x480, geglättet
volatile float C_actual_l_per_100km   = 0.0; // l/100km – automatisch umgeschaltet bei >5 km/h
volatile float scale_value_filtered   = 0.0;
volatile float temp_out               = 0.0;  
volatile int   temp_oil               = 0;
volatile int   deposit                = 0;
volatile unsigned int velocity_actual = 0;
volatile unsigned int velocity_cruise_control = 0;


volatile bool    flag_new_km            = false;   // 0x520: neuer km
volatile bool    flag_new_consumption   = false;   // 0x480: neue µl-Daten
volatile uint16_t raw_consumption_current = 0;     // aktueller Rohwert µl
volatile uint16_t C_last_raw    = 0xFFFF;          // letzter gültiger Rohwert


float            consumption_buffer       = 0.0f;  // gepufferte ml für update_values()


volatile unsigned long coolant_warning_start = 0;
volatile unsigned long oil_pressure_warning_start = 0;

bool ignition_on = false;  

float C_actual_filtered      = 0.0;  

float C_dial_display_value = 0.0f;

// Voltage calculated from analog in PIN 32
float volt = 0;                       

unsigned long shutdown_timer = 0;          	// time for update values in msec
unsigned long shutdown_time = 600000;       // Shutdown time - 600000 = 10 minutes

uint16_t TEXT_COLOR = DAY_TEXT_COLOR;       // with this we are able to change color with light state
uint16_t temp_color;                        // we use this temporäry color to let LED blink


//DEBUG
int maxpage = 3;


// DEBUG : Need to get suitable places for the LED, May be we hide the dial to show them
int Icon_Pos_Coolant[2]     = { 70, 110};     // position of LED icon    coolant      red
int Icon_Pos_BrakePads[2]   = {140, 110};     // position of LED icon    brakepads    red
int Icon_Pos_Petrol[2]      = {210, 110};     // position of LED icon    petrol       orange
int Icon_Pos_Oil[2]         = { 70, 165};     // position of LED icon    oil          red / orange
int Icon_Pos_BrakeSystem[2] = {140, 165};     // position of LED icon    brakepads    orange
int Icon_Pos_WasherFluid[2] = {210, 165};     // position of LED icon    washer_fluid orange
int Icon_Pos_Light[2]       = { 70, 220};     // position of LED icon    light        green
int Icon_Pos_Door[2]        = {140, 220};     // position of LED icon    door         orange
int Icon_Pos_Battery[2]    =  {210, 220};     // position of LED icon    battery     red
//int Icon_Pos_warn[2]   = {19, 428-25};      // position of LED icon     warning       orange  hier Position ersetzt Snowflake also weiter links
int Icon_Pos_warn[2]   = {77, 428-25};        // position rechts von snowflake
//int Icon_Pos_Petrol[2]      = { 19, 372 - 25};   // 210 / 165 position of LED icon    petrol       orange//316
int Icon_Pos_Snowflake[2]   = {19, 428-25};   // position of Temperature out warning
//int Icon_Pos_Snowflake[2]   = {80, 428-25}; //position of Temperature out warning // nach reckts versetzt
//int Icon_Pos_Compass[2] = {80, 70 -25}      // die position compass ist in updateValues direkt angegeben


int temp_page;                        // temporary page to show door open and others

char TFT_String[12];                  // convert values to string to print on the screen

// variables for MCP handling
bool PIN_INT_state = true;

// variables for the button pin
unsigned int Mode_Button_pressed = 0;
//unsigned int Memory_Button_pressed = 0;
unsigned int Reset_Button_pressed = 0;

//bool Page_Switch_Done = true;

bool battery = false;                // avoid flicker of battery symbol during startup and slow increasing voltage due to filter       
// we get from MCP port expander
bool coolant = false;                 // coolant warning controlled IO Pull Up
bool door_r = false;                  // right door is open or closed from IO Pull Up
bool door_s = false;                  // sliding door is open or closed from IO Pull Up
bool trunk = false;                   // trunk is open or closed from IO Pull Up
bool motor_cap = false;               // motor cap is open or closed from IO Pull Up
bool oil_pressure = false;            // oil pressure warning controlled by IO Pull Up 
bool oil_level = true;                // oil level warning controlled by IO Pull Up
bool washer_fluid = false;            // washer_fluid warning controlled IO Pull Up
bool brakepads = false;               // Brakepads warning controlled IO Pull Up
bool brakesystem = true;              // Brakefluid warning controlled IO Pull Up
bool temp_out_warning = false;        // We need to set a warning to avoid an update every cycle
bool unit_l_hour = false;              // bewusst falscher Wert! true if l/h ; false if l/100km ; start as true, as car is not moving at start 


int warnings = 0;                     // we count infos, warnings and critical infos

float temp = 0.0;                     // holds temporarily different values 

unsigned long valueMillis;       	  	// time for update values in msec
unsigned long lastMillis;         		// time for calculation in msec
unsigned long time_last;      		    // time since last loop in sec, will be reseted every 60 Seconds and other times will be increased


// this variable are used on both cores (core(1) = main loop and core(0) = evaluate can messages)
struct values_to_save {           // Data to be stored permanetly, 
    int page; 				            // core(1) Page to display
    int mode;                     // coer(1) Mode is on of START, REFUEL, PERIOD
    int deposit_last;             // both cores last stored content of deposit, needed to check if car has been refueled
    unsigned int km_start;        // both cores distance driven since start           
    unsigned int km_refuel;       // both cores distance driven since refule           
    unsigned int km_long_period;  // both cores distance driven since last manual reset
    float C_start;                // both cores consumption since start ml
    float C_refuel;               // both cores consumption since refule ml
    float C_long_period;          // both cores consumption since last manual reset l
    float C_25_km[25];            // both cores consumption from the last 25 driven km in ml
    float C_last_km;              // both cores conssumption of the last km, needed to calculate the last 25 km average in ml
                                  // need to store the value, as we may have driven only 800 m on shut down and we do want to loose the amount used 
    unsigned int time_start;      // core (1) time since last start min
    unsigned int time_refuel;     // core (1) time since last refuel min
    unsigned int time_long_period;// core (1) time since last manual reset min
    unsigned int velocity_max;    // both cores may be playing only ;-)
};

//values_to_save Data
values_to_save Data;
// DEBUG : Only for initializing the variables first time 
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


volatile unsigned int C_last = 0;              // last saved consumption in µl
volatile float C_last_25_km;                   // consumption over the last 25 km in ml

volatile bool start = false;                   // is needed for initial screen display
volatile bool motor_on	= false;               // Speed has been over 900 U/min than motor_on = true
volatile bool save = false;				             // Save is set to true if motor_on = true, after saving the variable will be set to false to not save continiously
volatile bool check_led = false;               // only if true, we will redraw the screen painting or deleting the LED icon
                                               // avoid redrawing the screen every llop cycle

// We get some LED information from Can BUS 
volatile bool light = false;                   // light is on or off from Can 0x420
volatile bool petrol = false;                  // reserve warning controlled by 0x320 can message
volatile bool door = false;                    // left door is open (true) or closed (false) from can message 0x320

volatile int UMin;                            // rpm of the motor
volatile unsigned int km_total = 0;            // total km driven by the vehicle / control the change of this value to recalculate values
                                               // must be 0 at start to not add km with fist CAN message

volatile float C_actual;			        	        // consumption ml, from 0x480 calculated in l/h  may be displayed also as l/100km
                                                
// GPS
unsigned long _GPS_time;          // UTC-Zeit in Sekunden seit Mitternacht
float _GPS_direction;             // Fahrtrichtung in Grad (0-360)
float _GPS_speed;                 // Geschwindigkeit in km/h
float _GPS_altitude;              // Höhe über Meeresspiegel, Korrekturfaktor von 46m wird für DEU angenommen
bool _GPS_valid;                  // Flag, GPS-Daten gültig
unsigned long _GPS_last_update;   // Letzter Update-Zeitstempel (millis())
int _GPS_timezone_offset = 1;     // Zeitzonen-Offset (+1 CET)

// GPS global
TinyGPSPlus gps;
// Serial für GPS
HardwareSerial GPS_Serial(2);  // UART2 auf ESP32

//KLine Auswertung   // Funktioniert so nicht! 
HardwareSerial KLine(1); //UART1
// kline.ino – nur 1 Pin: GPIO13 TX RX verbunden - funktioniert so nicht


unsigned long kline_last_init       = 0;
unsigned long kline_last_request    = 0;
unsigned long kline_last_byte_time  = 0;

bool          kline_active          = false;
float         oil_level_mm          = -1.0f;
float         boost_mbar            = -1.0f;

// Zustand für K-Line
enum { KLINE_IDLE, KLINE_WAIT_OIL, KLINE_WAIT_BOOST } kline_state = KLINE_IDLE;


#endif
