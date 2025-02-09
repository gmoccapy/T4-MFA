// TFT_Setup
// Pin definition and settings see USER_SETUP_ID 11
// #define TFT_MISO 19 not used, commented out in user_setup
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS    5  
// #define TFT_DC    2  
// #define TFT_RST  -1  // Set TFT_RST to -1 as display RESET is connected to 
                        // ESP32 board RST, Pin is marked as EN 

// set up can system
// Default for ESP32
#define CAN_RX		21    // Green connector PIN 19
#define CAN_TX		22    // Green connector PIN 20

// MFA Control PIN
// MODE Switch momentary button
#define PIN_MODE 13     // Green connector PIN 24
// RESET Switch momentary button
#define PIN_RESET 14    // Green connector PIN 25
// MEMORY Switch button
#define PIN_MEMORY 16   // Green connector PIN 23

// Analog and digital PIN
// Voltage PIN ; IN
#define PIN_Volt 39
// Stay On PIN ; OUT
#define PIN_STAY_ON 12 
// Trunk PIN ; IN
#define PIN_TRUNK 4
// Door right PIN ; IN
#define PIN_R_DOOR 26
// Door Sliding PIN ; IN
#define PIN_S_DOOR 27
// Motor Cap PIN ; IN Green connector PIN 22
#define PIN_MOTOR_CAP 35
// Oil Presure PIN ; IN
#define PIN_OIL_PRESURE 17
// Oil Level PIN ; IN
#define PIN_OIL_LEVEL 25
// Brake Pad PIN ; IN Green connector PIN 7
#define PIN_BRAKEPADS 34
// Brake System PIN ; IN Blue connector PIN 29
#define PIN_BRAKESYSTEM 33
// Coolant PIN ; IN
#define PIN_coolant 0        
// Wiper Water Warning PIN ; IN Green connector PIN 6
#define PIN_WASHER_FLUID 32

// Oil Level PWM signal
#define PIN_OIL_PWM 19

// we get from Can Bus, so we do not need the following PIN
// Door Left
// petrol warning





