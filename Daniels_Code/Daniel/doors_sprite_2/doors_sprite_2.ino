#include "sym_door.h"
#include <TFT_eSPI.h>       
TFT_eSPI    tft = TFT_eSPI();         
TFT_eSprite d_spr = TFT_eSprite(&tft);  

void setup()
{

d_spr.setColorDepth(8); //wictig, sonst nimmt das Sprite zuviel Speicher ein und wird nicht angezeigt
tft.init();


  tft.fillScreen(TFT_BLACK);
}

void loop(void)
{

door_0();
delay(1000);
door_1();
delay(1000);
door_2();
delay(1000);
door_3();
delay (1000);
door_4();
delay(1000);
door_5();
delay(1000);
door_6();
delay(1000);
door_7();
delay(1000);
door_8();
delay(1000);
door_9();
delay(1000);
door_10();
delay(1000);
door_11();
delay(1000);
door_12();
delay(1000);
door_13();
delay(1000);
door_14();
delay(1000);
door_15();
delay(1000); 
door_16();
delay(1000);
door_17();
delay(1000);
door_18();
delay (1000);
door_19();
delay(1000);
door_20();
delay(1000);
door_21();
delay(1000);
door_22();
delay(1000);
door_23();
delay(1000);
door_24();
delay(1000);
door_25();
delay(1000);
door_26();
delay(1000);
door_27();
delay(1000);
door_28();
delay(1000);
door_29();
delay(1000);
door_30();
delay(1000); 
door_31();
delay(1000);
}
