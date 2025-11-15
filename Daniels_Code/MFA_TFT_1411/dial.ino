
#define SPRITESIZE  171          // Size of the Sprite (With and Height are equal) Do not make smaller than 111
#define SPRITEPIVOT 85           // Pivot point of the Sprite of the dial Sprite Should be abs(SPRITESIZE / 2)
//#define SPRITESIZE  251          // Size of the Sprite (With and Height are equal) Do not make smaller than 111
//#define SPRITEPIVOT 125           // Pivot point of the Sprite of the dial Sprite Should be abs(SPRITESIZE / 2)

void draw_dial(int X_Pos, int Y_Pos, float DRO_value, int DRO_digits, float scale_value, float scale_max_value, String line_1, String line_2){
  dial.fillSprite(TFT_TRANSPARENT);
  dial.setTextColor(TEXT_COLOR, BACK_COLOR);
  dial.fillCircle(SPRITEPIVOT, SPRITEPIVOT, SPRITEPIVOT - 1, BACK_COLOR);
  dial.drawCircle(SPRITEPIVOT, SPRITEPIVOT, SPRITEPIVOT - 1, TEXT_COLOR);
  // tl = 10 from sprite scale and spritepivot = 90 and we need 2 times fontHeight
  // pos = SPRITEPIVOT / 2 - tft.fontHeight(FONT2)
  dial.drawString(line_1, SPRITEPIVOT, SPRITEPIVOT / 2 - dial.fontHeight(FONT2), FONT2);
  dial.drawString(line_2, SPRITEPIVOT, SPRITEPIVOT / 2 , FONT2);
  
  // draw units 
  dial.drawString(LITER, SPRITEPIVOT, SPRITEPIVOT + SPRITEPIVOT / 2 - 2, FONT2);
  temp = dial.textWidth(HUNDERTKM, FONT2);
  dial.drawFastHLine(SPRITEPIVOT - temp / 2, SPRITEPIVOT + SPRITEPIVOT / 2 + dial.fontHeight(FONT2) / 2, temp, TEXT_COLOR);
  dial.drawString(HUNDERTKM, SPRITEPIVOT, SPRITEPIVOT + SPRITEPIVOT / 2 + tft.fontHeight(FONT2) + 2, FONT2);

  // average consumption is in the middle
  dtostrf(DRO_value, String(DRO_value).length() - 2, DRO_digits, TFT_String);
  dial.drawString(TFT_String, SPRITEPIVOT, SPRITEPIVOT, FONT4);

  // The needle (the small triangle) indicating the average consumption on the scale
  temp = 45 + (270 * DRO_value / scale_max_value);
  if (temp < 45){
    temp = 45;
  }
  if (temp > 315){
    temp = 315;
  }
  needle.pushRotated(&dial, temp, TFT_BLACK);

  // draw the arc showing actual consumption
  if(velocity_actual > 5){
    //360-45-45 = 270 = max_value
    //             X  = value
    scale_value_filtered = (0.8 * scale_value_filtered + 0.2 * scale_value);

    temp = 45 + (270 * scale_value_filtered / scale_max_value);
    if (temp < 45){
      temp = 45;
    }
    if (temp > 315){
      temp = 315;
    }
    // temp may be 45.1 leading to a draw of a full circle, so we check that to avoid drawing a complete circle
    if ((temp >= 46) && (velocity_actual > 5)){
      dial.drawSmoothArc(SPRITEPIVOT, SPRITEPIVOT, SPRITEPIVOT - 1, SPRITEPIVOT - SPRITEPIVOT / 10, 45, int(temp) , TFT_GREEN, TFT_BLACK, false);
    }
  }

  // include the scale
  scale.pushToSprite(&dial, 0, 0, TFT_BLACK);

  // finaly push the sprite
  dial.pushSprite(X_Pos, Y_Pos, TFT_TRANSPARENT);
}

// This will create the needed sprites to display without flicker
void create_sprites(void){

  // create dial sprite
  dial.setColorDepth(8);
  dial.createSprite(SPRITESIZE, SPRITESIZE);
  dial.setPivot(SPRITEPIVOT, SPRITEPIVOT);
  dial.setTextColor(TEXT_COLOR, BACK_COLOR);
  dial.setTextDatum(MC_DATUM);
  
  // create the scale
  scale.setColorDepth(8);
  scale.createSprite(SPRITESIZE, SPRITESIZE);
  scale.setPivot(SPRITEPIVOT, SPRITEPIVOT);
  scale.fillSprite(TFT_BLACK);
  // Draw ticks every 15 degrees from -135 to +135 degrees
  int counter = 0;
  for (int i = -135; i < 136; i += 22) {

    // Long scale tick length
    int tl = SPRITESIZE / 18;

    // Coordinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (SPRITEPIVOT - 2) + SPRITEPIVOT;
    uint16_t y0 = sy * (SPRITEPIVOT - 2) + SPRITEPIVOT;
    uint16_t x1 = sx * (SPRITEPIVOT - tl) + SPRITEPIVOT;
    uint16_t y1 = sy * (SPRITEPIVOT - tl) + SPRITEPIVOT;

    //Draw ticks
    if (counter % 3 == 0){
      scale.drawLine(x0, y0, x1, y1, TFT_RED);
    }
    else{
      scale.drawLine(x0, y0, x1, y1, TFT_YELLOW);
    }
    counter += 1;
  }

  // create the small triangle in sprite to act as a needle
  needle.setColorDepth(1);
  needle.setBitmapColor(TFT_RED, TFT_BLACK);
  needle.createSprite(SPRITESIZE / 10, SPRITESIZE / 10);
  needle.setPivot(SPRITEPIVOT / 10, SPRITESIZE / 10 - SPRITEPIVOT + SPRITESIZE / 18); //Pivot is calculatet from top left corner
  needle.fillSprite(TFT_BLACK);
  needle.fillTriangle(SPRITESIZE / 20, SPRITESIZE / 10 - 1, 1, 1, SPRITESIZE / 10 - 1, 1, TFT_RED);

  int width = 36;
  int height = 158;
  // create a left bar sprite
  left_bar.setColorDepth(8);
  left_bar.createSprite(width, height);
  left_bar.fillSprite(TFT_BLACK);
  // Draw 10 tiks
  for (int i = 0; i < 130; i += int(height / 10)){
    left_bar.drawFastHLine(0, 19 + i, 6, TEXT_COLOR);
  }

  // create a right bar sprite
  right_bar.setColorDepth(8);
  right_bar.createSprite(width, height);
  right_bar.fillSprite(TFT_BLACK);
  // Draw 10 tiks
  for (int i = 0; i < 130; i += int(height / 10)){
    right_bar.drawFastHLine(width - 6, 19 + i, 6, TEXT_COLOR);
  }


  
  // create a value box will be done in function, as it may be of diferent sizes
  // see update_values.ino draw_value_box
  // box.setColorDepth(8);
  // box.createSprite(width, height);
}

void draw_value_box(int Y_Pos, int width, int height, char *value) {
  const int X_Pos = 222;

  TFT_eSprite box = TFT_eSprite(&tft);
  box.setColorDepth(8);

  // SICHERHEIT: Falls createSprite fehlschlägt → kein Leak + Notfall-Anzeige
  if (!box.createSprite(width, height)) {
    // RAM voll oder Fehler → direkter Draw ohne Sprite
    tft.setTextColor(TEXT_COLOR, BACK_COLOR);
    tft.setTextDatum(MR_DATUM);
    tft.setFreeFont(&FreeSansBold18pt7b);
    tft.drawString(value, X_Pos - 4, Y_Pos, 7);
    return;
  }

  box.fillSprite(BACK_COLOR);

  #ifdef DEBUG_TFT
    box.drawRect(0, 0, width, height, TFT_RED);
  #endif

  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MR_DATUM);
  box.setFreeFont(&FreeSansBold18pt7b);

  box.drawString(value, width - 1, height / 2);

  box.pushSprite(X_Pos - width, Y_Pos - height / 2, TFT_TRANSPARENT);

  // WICHTIG: deleteSprite() wird IMMER aufgerufen → KEIN LEAK!
  box.deleteSprite();
}
/*
void draw_value_box(int Y_Pos, int width, int height, char *value){
  int X_Pos = 222;
  // create value box sprite
  box.setColorDepth(8);
  box.createSprite(width, height);
  box.fillSprite(BACK_COLOR);
#ifdef DEBUG_TFT
  box.drawRect(0, 0, width, height, TFT_RED);
#endif
  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MR_DATUM);
  box.drawString(value, width - 1, height / 2, FONT7);
  box.pushSprite(X_Pos - width, Y_Pos - height / 2, TFT_TRANSPARENT);
  box.deleteSprite();
}
*/
void draw_small_value_box(int X_Pos, int Y_Pos, int width, int height, char *value){
  // create value box sprite
  box.setColorDepth(8);
  box.createSprite(width, height);
  box.fillSprite(BACK_COLOR);
  box.drawRect(0, 0, width, height, TFT_RED);
  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MR_DATUM);
  box.drawString(value, width - 1, height / 2, FONT4);
  box.pushSprite(X_Pos - width, Y_Pos - height / 2, TFT_TRANSPARENT);
  box.deleteSprite();
}


//  Draw a linear meter on the screen
void draw_bar(String label, String unit, int X_Pos, int Y_Pos, float value, int min_value, int max_value, int lower_limit, int upper_limit, int max_limit){

  int width = 36;
  int height = 158;

  temp = map(value, min_value, max_value, 0, 120); // 120 = Hight of 158 - 19 - 19 (label and value)

  temp_color = TEXT_COLOR;

  if ((value < lower_limit) || (value > upper_limit)){
    temp_color = TFT_ORANGE;
    if(max_limit < 0){
      temp_color = TFT_RED;
    }
  }
  if ((value > max_limit) && (max_limit > 0)){
    temp_color = TFT_RED;
  }
  if (batterie == true){
    if (Data.time_start % 2 == 0){
      temp_color = TFT_RED;
    }
    else{
      temp_color = TEXT_COLOR;
    }
  }
  box.setColorDepth(8);
  box.createSprite(width, height);
  box.fillSprite(BACK_COLOR);

  // Draw 10 tiks 
  if(X_Pos < 160){
    left_bar.pushToSprite(&box, 0, 0, TFT_BLACK);
  }
  else{
    right_bar.pushToSprite(&box, 0, 0, TFT_BLACK);
  }
  // example for Oil Temp
  // MAX Oil Temp = 160°C and bar is 158 - 19 - 19  Pixel hight
  // 88°C is then 120 x 88 / 160
  if(X_Pos < 160){
    box.fillRect(12, height - 19 - temp, width - 20, temp, temp_color);
  }
  else{
    box.fillRect(8, height - 19 - temp, width - 20, temp, temp_color);
  }

  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MC_DATUM);
  box.setFreeFont(&FreeSansBold12pt7b);
 // box.setFreeFont(&FreeArial12full);
  box.drawString(label, width / 2, 19 / 2, GFXFF);
  
  if(value-(int)value == 0){
    dtostrf(value, 3, 0, TFT_String);
  }
  else{
    dtostrf(value, 3, 1, TFT_String);
  }
  box.setTextDatum(MC_DATUM);
  String sum = TFT_String + unit;
  box.drawString(sum, width / 2, height - 19 / 2, FONT2);

  box.pushSprite(X_Pos , Y_Pos, TFT_TRANSPARENT);
  box.deleteSprite();

}
