
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
    temp = 45 + (270 * scale_value / scale_max_value);
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

  // create a value box will be done in function, as it may be of diferent sizes
  // see update_values.ino draw_value_box
  // box.setColorDepth(8);
  // box.createSprite(width, height);
}

void draw_value_box(int Y_Pos, int width, int height, char *value){
  int X_Pos = 222;
  // create value box sprite
  box.setColorDepth(8);
  box.createSprite(width, height);
  box.fillSprite(BACK_COLOR);
  box.drawRect(0, 0, width, height, TFT_RED);
  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MR_DATUM);
  box.drawString(value, width - 1, height / 2, FONT7);
  box.pushSprite(X_Pos - width, Y_Pos - height / 2, TFT_TRANSPARENT);
  box.deleteSprite();
}

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
void draw_bar(char *label, int X_Pos, int Y_Pos, int value){

  int width = 36;
  int height = 158;

  temp = map(value, 0, 160, 0, 120); // 120 = Hight of 158 - 19 - 19 (label and value)

  uint32_t color = TFT_GREEN;

  if ((value < 60) || (value > 120)){
    color = TFT_ORANGE;
  }
  if (value > 130){
    color = TFT_RED;
  }
  
  box.setColorDepth(8);
  box.createSprite(width, height);
  box.fillSprite(BACK_COLOR);

  // Sourounding Frame
  box.drawRect(0, 0, width, height, TEXT_COLOR);

  // MAX Oil Temp = 160°C and bar is 158 - 19 - 19  Pixel hight
  // 88°C is then 120 x 88 / 160
  box.fillRect(8, height - 19 - temp, width - 11, temp, color);

  // Draw 10 tiks 
  for (int i = 0; i < 130; i += int(height / 10)){
    box.drawFastHLine(0, 19 + i, 6, TEXT_COLOR);
  }
  
  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MC_DATUM);
  box.setFreeFont(&FreeArial12full);
  box.drawString(label, width / 2, 19 / 2, GFXFF);
  
  box.drawString(F("`C"), width / 2, 28, FONT2);

  dtostrf(value, 3, 0, TFT_String);
  box.setTextDatum(MC_DATUM);
  box.drawString(TFT_String, width / 2, height - 19 / 2, FONT2);
  // box.setTextDatum(ML_DATUM);
  // box.drawString(F("`C"), width / 2 + 5, height - 19 / 2, FONT2);

  box.pushSprite(X_Pos , Y_Pos, TFT_TRANSPARENT);
  box.deleteSprite();

}
