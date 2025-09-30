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

// MCP Interupt PIN connected to GPIO 33 in this case
#define INT_PIN 33

 // MFA Control PIN according to MCP Laypot
 // pins are numbert according to the following table
 //  0 = mcp.pin_A0
 //  1 = mcp.pin_A1
 //  2 = mcp.pin_A2
 //  3 = mcp.pin_A3
 //  4 = mcp.pin_A4
 //  5 = mcp.pin_A5
 //  6 = mcp.pin_A6
 //  7 = mcp.pin_A7
 //  8 = mcp.pin_B0
 //  9 = mcp.pin_B1
 // 10 = mcp.pin_B2
 // 11 = mcp.pin_B3
 // 12 = mcp.pin_B4
 // 13 = mcp.pin_B5
 // 14 = mcp.pin_B6
 // 15 = mcp.pin_B7

// MODE Switch momentary button
#define PIN_MODE 11
// MEMORY Switch selection switch
#define PIN_MEMORY 12
// RESET Switch momentary button
#define PIN_RESET 13
// Trunk PIN ; IN
#define PIN_TRUNK 10
// Door right PIN ; IN
#define PIN_R_DOOR 4
// Door Sliding PIN ; IN
#define PIN_S_DOOR 5
// Motor Cap PIN ; IN Green connector PIN 22
#define PIN_MOTOR_CAP 1
// Oil Presure PIN ; IN
#define PIN_OIL_PRESURE 8 
// Brake Pad PIN ; IN Green connector PIN 7
#define PIN_BRAKEPADS 0
// Brake System PIN ; IN Blue connector PIN 29
#define PIN_BRAKESYSTEM 2 
// Coolant PIN ; IN
#define PIN_COOLANT 9        
// Wiper Water Warning PIN ; IN Green connector PIN 6
#define PIN_WASHER_FLUID 3


// Analog and digital PIN
// Voltage PIN ; IN
#define PIN_Volt 39
// Stay On PIN ; OUT
#define PIN_STAY_ON 12 
// Oil Level PIN ; IN
#define PIN_OIL_LEVEL 25
// Oil Level PWM signal
#define PIN_OIL_PWM 19

// we get from Can Bus, so we do not need the following PIN
// Door Left
// petrol warning





