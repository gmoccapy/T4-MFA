void update_time(void){
  Data.time_start += ((millis() - lastMillis)) / 1000; // sec
  // one minute has passed_vel
  if((Data.time_start - time_last) > 59){
    Data.time_refuel += 1; // min
    Data.time_long_period += 1; // min
    time_last = Data.time_start;
  }
// DEBUG  
  if(temp_page == 6){
    tft.setTextDatum(MC_DATUM);
    temp = (shutdown_timer + 3600000 - millis()) / 1000 ;
    dtostrf(temp, 4, 0, TFT_String);
    draw_value_box(250, 134, 50, TFT_String);
  }
}

void update_volt(void){
  // Get the Voltage value from PIN 4 from ESP32 coresponds to ADC1_CH3
  temp = analogRead(PIN_Volt);      

  // Check if over or undervoltage, to turn on the LED
  if((temp > 4000) || (temp < 1000)){
    Serial.println(temp);
    batterie = true;
    check_led = true;
  }
  else{
    batterie = false;
  }

  // ignition is on! K15 with 12 V
  if (temp > 1000){
    // get rid of noise influence ; high pass filtering 
    // take 80% of old value and 20% of new value to smooth the display
    volt = (0.9 * volt) + (0.1 * temp);

    // ignition came back bevor one hour has past, so reset timer
    // and redraw page, as we have blanked it out Loosing K15
    if (shutdown_timer != 0){
      shutdown_timer = 0;
      if (temp_page != Data.page){
        temp_page = Data.page;
        DrawSelected(Data.page);
        check_LED();
      }
    }
    // if we had an power off, we need to put Hold high
    if (digitalRead(PIN_STAY_ON) == false){
      digitalWrite(PIN_STAY_ON, 1);
    }
  }
  // no Current on K15, so switch off display (better sayed just draw black screen)
  else{
    volt = temp;
    if ((shutdown_timer == 0) && (digitalRead(PIN_STAY_ON) == true)){
      shutdown_timer = millis();
      temp_page = 6;
      DrawSelected(temp_page);
    }
  } 
}

void update_values(void){
  // max 6 info lines
  // Line 1 = 72
  // Line 2 = 138
  // Line 3 = 204
  // Line 4 = 270
  // Line 5 = 337
  // Line 6 = 404

//DEBUG:
//Serial.println("This is update values");

  tft.setTextDatum(MR_DATUM); // middle right
  switch(Data.page){

    case 0:
      //draw_value_volt(70);
      draw_value_cruise_control(70);

      //Serial.println(warnings);
      if ((warnings) || (batterie)){
        // do nothing
      }
      else{
        // // draw_dial needs 181 pixel hight, value may be changes with #define SPRITESIZE and SPRITEPIVOT
        // // C_last_25_km = 1755.65 ml/25km = 70.226 ml/km => 0,070226 l/km = 7.0226 l/100km 
        temp = C_last_25_km / 250.0;  // ml/25 km and 100km gives factor 250     
        draw_dial(75, 108, temp, 1, C_actual, 24.0, F("Ab"), F("Start"));
      }

      // draw oil temp scale
      draw_bar("Öl", "`C", 15, 115, temp_oil, 0, 160, 60, 130, 140);

      // draw volt scale
      draw_bar("V", "V", 269, 115, volt * VOLT_FAKTOR, 0, 18, 11, 15, -1);

      draw_value_range(316);

      //draw_value_average_consumption(372, Data.mode);
      draw_value_actual_consumption(372);

      draw_value_out_temp(428);

      break;
    
    // since refuel
    case 1:
      // Line 1 = 72
      //draw_value_oil_temp(72);
      draw_value_trip(72, REFUEL);
      // Line 2 = 138
      draw_value_actual_consumption(138);
      // Line 3 = 204
      draw_value_range(204);
      // Line 4 = 270
      draw_value_average_consumption(270, REFUEL);
      // Line 5 = 337
      //draw_value_time(337, REFUEL);
      //draw_value_out_temp(337, PERIOD);
      draw_value_speed(337);
      // Line 6 = 404
      draw_value_average_velocity(404, REFUEL);
      break;
    
    // long period
    case 2:
      // Line 1 = 72
      //draw_value_oil_temp(72);
      draw_value_trip(72, PERIOD);
      // Line 2 = 138
      draw_value_actual_consumption(138);
      // Line 3 = 204
      draw_value_range(204);
      // Line 4 = 270
      draw_value_average_consumption(270, PERIOD);
      // Line 5 = 337
      draw_value_time(337, PERIOD);
      // Line 6 = 404
      draw_value_average_velocity(404, PERIOD);

      dtostrf(Data.velocity_max, 3, 0, TFT_String);
      draw_small_value_box(75, 72, 50, 30, TFT_String);

      break;
    
    // dial test
    case 3:
      // Line 1 = 72  ()
      draw_value_cruise_control(72);

      // Line 2 = 138
      // Line 3 = 204
      // Line 4 = 270
      // are used by the dial

      // draw_dial needs 181 pixel hight, value may be changes with #define SPRITESIZE and SPRITEPIVOT
      // C_last_25_km = 1755.65 ml/25km = 70.226 ml/km => 0,070226 l/km = 7.0226 l/100km 
      temp = C_last_25_km / 250.0;  // ml/25 km and 100km gives factor 250

      draw_dial(75, 118, temp, 1, C_actual, 24.0, F("Ab"), F("Start"));

      // Line 5 = 337 (range)
      draw_value_range(337);
      // Line 6 = 359 (time or oil)
      //draw_value_time(359, START);
      draw_value_oil_temp(404);
      break;

    // debug
    case 4:
      //updateDebug();
      break;

    // doors
    case 5:
      break;

    default: break;
  }
}

void draw_value_actual_consumption(int Y_Pos){

  // draw units if we need to change l/h or L/100km
  // actual consumption not driving
  if((units_l_100_km == true) && (velocity_actual < 5)){
    drawUnits(Y_Pos, LITER, HOUR);
    units_l_100_km = false;
  }

  // actual consumption driving
  if((units_l_100_km == false) && (velocity_actual > 5)){
    drawUnits(Y_Pos, LITER, HUNDERTKM);
    units_l_100_km = true;
  }

  // C_actual is l/h
  if(velocity_actual > 5){
    // now want to display now l/100km not l/h as car is moving
    C_actual_filtered = (0.8 * C_actual_filtered + 0.2 * C_actual);
    temp = C_actual_filtered * 100.0 / velocity_actual;
  }
  else{
    temp = C_actual;
  }
  // only place to display 2 numbers and one digit
  if(temp > 99.0){
    temp = 99.0;
  }

  //drawActualConsumption(Y_Pos);
  dtostrf(temp, 4, 1, TFT_String);
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_time(int Y_Pos, int _case){
  switch(_case){

    case 1: // time from start in sec
      temp = Data.time_start;
      break;

    case 2: // time since refuel in min
      temp = Data.time_refuel * 60;
      break;

    case 3: // time long period in min
      temp = Data.time_long_period * 60;
      break;
  }

    // check if we are over 99 hours and 59 minutes
    // if so we have to strip minutes due to space reasons
    if(temp <= 359940){
      char hours[3];
      dtostrf((int(temp) / 3600), 2, 0, hours);
      char minutes[3];
      dtostrf((int(temp) / 60) % 60, 2, 0, minutes);
      // fill with leading zeros
      for(int i = 0; i < strlen(minutes); i++){
        if(minutes[i] == ' '){
          minutes[i] = '0';
        }
        else{
          break;
        }
      }
      strcpy(TFT_String, hours);
      strcat(TFT_String, ":");
      strcat(TFT_String, minutes);
    }
    else{
      dtostrf((int(temp) / 3600), 5, 0, TFT_String);
    } 
    draw_value_box(Y_Pos, 145, 50, TFT_String);
}

void draw_value_trip(int Y_Pos, int _case){
    
    switch(_case){

    case 1: // trip from start
      dtostrf(Data.km_start, 3, 0, TFT_String);
      break;
      
    case 2: // trip since refuel
      dtostrf(Data.km_refuel, 3, 0, TFT_String);
      break;
  
    case 3: // trip since refuel
      dtostrf(Data.km_long_period, 6, 0, TFT_String);
      break;
    }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_consumption(int Y_Pos, int _case){
  
  switch(_case){

    case 1: // consumption from start
      if(Data.C_start > 0){
        // C_from_start = 500 ml  
        temp = Data.C_start / 1000.0; // 1000ml / 1l
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;

    case 2: // consumption since refule
      if(Data.C_refuel > 0){
        // C_refuel = 500 ml 
        temp = Data.C_refuel / 1000.0; // 1000ml / 1l
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;

    case 3: // consumption long period
      if(Data.C_long_period > 0){
        // C_long_period = 86.38 l and km_long_oeriod = 1234 km => 86.38 l * 100.0 km / (1234 km) = 86.38 l / (1234 / 100) km 
        temp = Data.C_long_period; // l
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }
      break;  
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_average_consumption(int Y_Pos, int _case){
  
  switch(_case){

    case 1: // average consumption from start
      if(Data.km_start > 0){
        // C_start = 2100 ml and km_start = 35 km => 2100 ml / 35 km => 60 ml / 1 km => 6000 ml / 100 km => 6,0 l / 100 km 
        temp = (Data.C_start / float(Data.km_start)) / 10.0 ; // 1000 ml/l and 100 km gives factor 10
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;

    case 2: // average consumption since refule
      if(Data.km_refuel > 0){
        // C_refuel = 2100 ml and km_refuel = 35 km => 2100 ml / 35 km => 60 ml / 1 km => 6000 ml / 100 km => 6,0 l / 100 km 
        temp = (Data.C_refuel / float(Data.km_refuel)) / 10.0 ; // 1000 ml/l and 100 km gives factor 10
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;

    case 3: // average consumption long period
      if(Data.km_long_period > 0){
        // C_period = 2.100 l and km_period = 35 km => 2.1 l / 35 km => 0.06 l/km => 6.0 l/100 km  
        temp = (Data.C_long_period / float(Data.km_long_period)) * 100.0; // l / km *100
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }
      break;  
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_cruise_control(int Y_Pos){
  // cruise control
  uint color;
  if(velocity_cruise_control > 0){
    dtostrf(velocity_cruise_control, 3, 0, TFT_String);
    color = TFT_GREEN;
  }
  else{
    String(F("---")).toCharArray(TFT_String, 12);
    color = TEXT_COLOR;
  }  
  draw_value_box(Y_Pos, 134, 50, TFT_String);
  tft.drawXBitmap(19, Y_Pos - 25, sym_cruise_control, 50, 50, color);
}

void draw_value_average_velocity(int Y_Pos, int _case){
  
  switch(_case){

    case START: // average velocity from start
      if(Data.time_start > 0){
        // time_from_start in sec and km_from_start in km 
        temp = Data.km_start * 3600.0 / float(Data.time_start); // km/h
        dtostrf(temp, 4, 0, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;

    case REFUEL: // average velocity since refule
      if(Data.time_refuel > 0){
        // km_refuel = 35 km and time_refuel = 57 min => 35 km / 57 min = 0,614 km/min => 0,614 * 60 min/h = 36.8 km/h 
        temp = Data.km_refuel * 60.0 / float(Data.time_refuel); 
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;

    case PERIOD: // average velocity long period
      if(Data.time_long_period > 0){
        // km_period = 35 km and time_period = 57 min => 35 km / 57 min = 0,614 km/min => 0,614 * 60 min/h = 36.8 km/h 
        temp = Data.km_long_period * 60.0 / float(Data.time_long_period); 
        dtostrf(temp, 4, 1, TFT_String);
      }
      else{
        String(F("--.-")).toCharArray(TFT_String, 5);
      }  
      break;  
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_oil_temp(int Y_Pos){
  // oil temperatur
  if(temp_oil < 10){
    String(F("---")).toCharArray(TFT_String, 4);
  }
  else{
    dtostrf(temp_oil, 3, 0, TFT_String);
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_volt(int Y_Pos){
  // Get the Voltage value
  if(batterie == true){
    check_led = true;
    String(F("---")).toCharArray(TFT_String, 4);
  }
  else{
    dtostrf(volt * VOLT_FAKTOR, 6, 1, TFT_String);
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_out_temp(int Y_Pos){
  // out temp
  dtostrf(temp_out, 6, 1, TFT_String);
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_speed(int Y_Pos){
  // motor speed RPM, as we are very fast, not rounding lead to flickering numbers, so rount on 10'th
  temp = round(speed / 10.0); // speed = 1789 gives now 178.9 rounding that gives 179 
  dtostrf(temp * 10, 6, 0, TFT_String); // need to multiply by 10 to get 1790 RPM
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_value_range(int Y_Pos){
  // range
  // deposit = 53 l ; C_last_25_km = 9,73 ml/25km
  if(C_last_25_km > 0){
    temp = (deposit * 1000.0) / (C_last_25_km / 25.0);
    dtostrf(temp, 4, 0, TFT_String);
  }
  else{
    String(F("---")).toCharArray(TFT_String, 12);
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void updateDebug(void){
  // DEBUG
  // tft.setTextDatum(MR_DATUM);
  // dtostrf(bitValue[0], 6, 0, TFT_String);
  // draw_value_box(138, 134, 50, TFT_String);
  // dtostrf(bitValue[1], 6, 0, TFT_String);
  // draw_value_box(204, 134, 50, TFT_String);
  // dtostrf(bitValue[2], 6, 0, TFT_String);
  // draw_value_box(270, 134, 50, TFT_String);
  draw_value_cruise_control(404);
  dtostrf(deposit, 6, 0, TFT_String);
  draw_value_box(72, 200, 50, TFT_String);
  // draw_small_value_box(240, 1*24+12, 100, 24, TFT_String);

  // dtostrf(Data.time_start, 6, 0, TFT_String);
  // draw_small_value_box(240, 2*24+12, 100, 24, TFT_String);

  dtostrf(temp_oil, 6, 0, TFT_String);
  draw_value_box(138, 200, 50, TFT_String);
  // draw_small_value_box(240, 3*24+12, 100, 24, TFT_String);

  // dtostrf(Data.km_start, 6, 0, TFT_String);
  // draw_small_value_box(240, 4*24+12, 100, 24, TFT_String);

  dtostrf(C_last_25_km, 8, 0, TFT_String);
  // draw_small_value_box(240, 5*24+12, 100, 24, TFT_String);
  draw_value_box(204, 200, 50, TFT_String);

  // dtostrf(Data.time_refuel, 6, 0, TFT_String);
  // draw_small_value_box(240, 6*24+12, 100, 24, TFT_String);

  dtostrf(speed, 6, 0, TFT_String);
  // draw_small_value_box(240, 7*24+12, 100, 24, TFT_String);
  draw_value_box(270, 200, 50, TFT_String);

  // dtostrf(velocity_actual, 6, 0, TFT_String);
  // draw_small_value_box(240, 8*24+12, 100, 24, TFT_String);

  // dtostrf(velocity_cruise_control, 6, 0, TFT_String);
  // draw_small_value_box(240, 9*24+12, 100, 24, TFT_String);

  dtostrf(Data.C_last_km, 8, 0, TFT_String);
  // draw_small_value_box(240, 10*24+12, 100, 24, TFT_String);
  draw_value_box(337, 200, 50, TFT_String);

  // dtostrf(Data.C_start, 6, 1, TFT_String);
  // draw_small_value_box(240, 11*24+12, 100, 24, TFT_String);

  // if(velocity_actual > 5){
  //   tft.drawString(F("C_act [l/h]"), 0, 12*24+12, FONT4);
  // }
  // else{
  //   tft.drawString(F("C_act [l/dkm]"), 0, 12*24+12, FONT4);
  // } 
  // dtostrf(C_actual, 4, 4, TFT_String); // calculation l/h or l/100 km is done in read_CAN
  // draw_small_value_box(240, 12*24+12, 100, 24, TFT_String);
}