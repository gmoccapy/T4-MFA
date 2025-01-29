// TFT_Setup
// Pin definition and settings see USER_SETUP_ID 11
// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS    5  
// #define TFT_DC    2  
// #define TFT_RST  16  // Reset pin (could connect to RST pin)
// #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
//                      // Pin is marked as EN 

// set up can system
// Default for ESP32
#define CAN_RX		21
#define CAN_TX		22

// MFA Control PIN
// MODE Switch momentary button
#define PIN_Mode 13
// RESET Switch momentary button
#define PIN_Reset 14
// MEMORY Switch button
#define PIN_Memory 16

// Analog and digital PIN
// Voltage PIN ; IN
#define PIN_Volt 32 
// Stay On PIN ; OUT
#define PIN_STAY_ON 12 
// Trunk PIN ; IN
#define PIN_TRUNK 4
// Oil Presure PIN ; IN
#define PIN_OIL_PRESURE 17
// Door right PIN ; IN
#define PIN_R_DOOR 26
// Door Sliding PIN ; IN
#define PIN_S_DOOR 27
// Wiper Water Warning PIN ; IN
#define PIN_WIPER_WATER 33
// Brake Pad PIN ; IN
#define PIN_BRAKE_PAD 34
// Motor Cap PIN ; IN
#define PIN_MOTOR_CAP 35
// Oil Level PIN ; IN
#define PIN_OIL_LEVEL 39

// we get from Can Bus, so we do not need the following PIN
// Door Left PIN ; IN
// #define PIN_L_DOOR 25





