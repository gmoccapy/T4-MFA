void evaluate_CAN_messages(){


  // we do not use the temp variable here, as it may be used also from the other kernel
  int bit;     // hold the bit value to evaluate
  int byte;    // The byte / int value to evaluate
  //float value; // the float value to evaluate
  unsigned int C_motor_value;           // the value recieved from motor in µl
  unsigned long time_C_period = 0;      // time elepsed from consumption update ms
  static unsigned int C_last_raw = 0xFFFF;          // Letzter Rohwert aus 0x480 (µl)
  static unsigned long time_C_period_last = 0;      // Zeitstempel für l/h


  switch (rxFrame.identifier)
  {
   // Message from KI 1
    case 0x320:
    {
      bit = bitRead(rxFrame.data[2], 7); 

      // deposit warning, low 
      if ((bit == 1) && (petrol == false)){
        petrol = true;
        check_led = true;
      }

    
      else if ((bit == 0) && (petrol == true)){
        petrol = false;
        check_led = true;
      }

      byte = rxFrame.data[2];

      // Check if we need to substract 128 from deposit value!
      // May be Byte 0 Bit 6 does contain the same value
      if(petrol == true){
        byte -= 128;
      }

      // with first start of ESP32 variable deposit_last will be initialized with value 0 
      // we use the first CAN message to set the correct value as we do not want to reset refuel Data
      if(Data.deposit_last == 0){
        deposit = byte;
        Data.deposit_last = deposit;
        //return;
      }

      if(byte != deposit){
        deposit = byte;
        
        // if the driver has refilled the deposit with at least
        // 5 Liter, we do reset the values since refuel 
        // the ammount of 5 liter will also handle changes of content due to driving
        
        if (deposit > (Data.deposit_last + 5) ){ 
        reset_Data(REFUEL);
      //   pending_reset = REFUEL; 
         save = true;
        }
        Data.deposit_last = deposit;
      }


// ToDo : Check witch value is best to use
// May be 0x420 Byte 0 Bit 0 does contain the same value
// we may use also an pysical in PIN connected to blue connector PIN 21
      // Check for opened door
      bit = bitRead(rxFrame.data[0], 0); 
      if ((bit == 1) && (door == false)){
        door = true;
        check_led = true;
      }
      else if ((bit == 0) && (door == true)){
        door = false;
        check_led = true;
      }

      break;
    }

   // Message from KI 2
    case 0x420:
    {
      temp_out = 0.5 * rxFrame.data[2] - 50;

      temp_oil = rxFrame.data[3] - 60;

      //kl_58d has only two values 0x00 = lights are off and 0x64 = lights are on
      byte = rxFrame.data[5];

      // check if light is on 
      // need to do the comparison to state variable to avoid writing to pin every cycle, 
      // as we only want to write to pin if state has changed
      if ((byte != 0) && (light == false)){
        //analogWrite(LED_Backlight, 100);
        light = true;
        check_led = true;
      }
      else if ((byte == 0) && (light == true)){
        //analogWrite(LED_Backlight, 330);
        light = false;
        check_led = true;
      }

      break;
    }

   // Message from KI 3
 case 0x520:
{
  float value = rxFrame.data[7] * 65536 + rxFrame.data[6] * 256 + rxFrame.data[5]; 

  if(km_total == 0){
    km_total = value;
    break;
  }

  if(value != km_total){
    km_total = value;
    Data.km_start += 1;
    Data.km_refuel += 1;
    Data.km_long_period += 1;

    // --- 1. Alten Wert in Ringpuffer schieben ---
    for (int i = 24; i > 0; i--) {
      Data.C_25_km[i] = Data.C_25_km[i-1];
    }

    // --- 2. AKTUELLEN km-Verbrauch eintragen ---
    Data.C_25_km[0] = Data.C_last_km;  // ml für diesen km

    // --- 3. WICHTIG: C_last_km ZURÜCKSETZEN! ---
    Data.C_last_km = 0.0;  // <--- DAS FEHLTE!

    // --- 4. C_last_25_km neu berechnen (nur gültige km) ---
    C_last_25_km = 0.0;
    int counter = 0;
    for(int i = 0; i < 25; i++) {
      if(Data.C_25_km[i] > 0.0) {  // nur gefahrene km zählen
        C_last_25_km += Data.C_25_km[i];
        counter++;
      }
    }

    // --- 5. Durchschnitt gefahrene km ---
    if (counter > 0) {
      C_last_25_km = C_last_25_km;  // bereits summiert
    } else {
      C_last_25_km = 0.0;
    }

    // Optional: Glättung oder Begrenzung
    if (counter < 25) {
      // Noch nicht 25 km gefahren → ggf. mit Startwert puffern
      // Oder einfach nur anzeigen, wenn mind. 1 km gefahren
    }

    // --- 6. Reichweite nur anzeigen ab 1 km ---
    // Wird in draw_value_range geprüft
  }
  break;
}

case 0x280:
{
  UMin = (rxFrame.data[2] + 256 * rxFrame.data[3]) / 4;

  // MOTOR AN
  if (UMin > 650 && !motor_on) {
    motor_on = true;

    // War der Motor länger als 2 Stunden aus ODER war der Strom weg?      (Strom weg ist immer nach Shutdown Timer der Fall?)            
    if (millis() - motor_off_time > 7200000UL || motor_off_time == 0) {   //nochmal durchdenken  (motoroff time wird in variabes initial auf 0 gesetzt)
      // "ab Start" + 25-km-Puffer löschen
      reset_Data(START);

      //   pending_reset = START; 

      //Data.C_start = 0.0;
      //Data.km_start = 0;
      //Data.time_start = 0;
    save = true;      
    }
  }
  // MOTOR AUS
  if (UMin < 600 && motor_on) {
    motor_on = false;
    motor_off_time = millis();  
    save = true;
  }
  break;
}


   // Message from Motor 2
    case 0x288:
    {
      velocity_actual = rxFrame.data[3] * 1.28;
  
      if(Data.velocity_max < velocity_actual){
        Data.velocity_max = velocity_actual;
      }
      velocity_cruise_control = rxFrame.data[4] * 1.28;

    static bool last_driving = false;
    if (velocity_actual > 6)  driving = true;               
    else if (velocity_actual < 4) driving = false;         
    else driving = last_driving;                            
    last_driving = driving;


// ToDo : 
// Check if we need to control not only on off, but also active and not active
// Check if we need to check for brake actions, 
// the value is stored in Byte 2 , Bit 0 and 1
// bitRead(rxFrame.data[2], 6) = GRA Status   00 = OFF ; 01 = CONTROLING ; 10 = ON ; 11 = ERROR
// bitRead(rxFrame.data[2], 7) = GRA Status
// bitRead(rxFrame.data[2], 0) = BRAKE Status 0 = NO_BRAKE ; 1 = BRAKE
// and from 0x280 we may need
// bitRead(rxFrame.data[0], 3) = CLUTCH Status 0 = PRESSED ; 1 = NOT_PRESSED

      // if      ((bitRead(rxFrame.data[2], 6) == 1) && (bitRead(rxFrame.data[2], 7) == 0)){
      //   // Cruise Control on
      //   String(F("Temp. on")).toCharArray(TFT_String, 12);
      // }
      // else if ((bitRead(rxFrame.data[2], 6) == 0) && (bitRead(rxFrame.data[2], 7) == 1)){
      //   // Cruise Control controling
      //   String(F("Temp. reg.")).toCharArray(TFT_String, 12);
      // }
      // else if ((bitRead(rxFrame.data[2], 6) == 1) && (bitRead(rxFrame.data[2], 7) == 1)){
      //   // Cruise Control error
      //   String(F("Temp. error")).toCharArray(TFT_String, 12);
      // }
      // else{
      //   // Cruise Control off
      //   String(F("Temp. off")).toCharArray(TFT_String, 12);
      // }
      // draw_small_value_box(300, 380, 280, 40, TFT_String);

      break;
    }

  //  // Message from Motor 3
  //   case 0x380:
  //   {
  //     break;
  //   }

  //  // Message from Motor 4 only 3 Bytes from here
  //   case 0x388:
  //   {
  //     bitValue[0] = rxFrame.data[0];
  //     bitValue[1] = rxFrame.data[1];
  //     bitValue[2] = rxFrame.data[2];
  //     break;
  //   }

  case 0x480:
{
  unsigned int current_raw = rxFrame.data[2] | (rxFrame.data[3] << 8);

  // === 1. INITIALISIERUNG ===
  if (C_last_raw == 0xFFFF) {
    C_last_raw = current_raw;
    time_C_period_last = millis();
    break;
  }

  // === 2. ECU RESET ERKENNEN ===
  if (current_raw < 100 && C_last_raw > 60000) {
    C_last_raw = current_raw;
    time_C_period_last = millis();
    break;
  }

  // === 3. DELTA BERECHNEN ===
  unsigned long delta_ul;
  if (current_raw >= C_last_raw) {
    delta_ul = current_raw - C_last_raw;
  } else {
    delta_ul = (65536UL - C_last_raw) + current_raw;
  }

  // === Störung filtern ===
  if (delta_ul > 30000) {  // >30ml pro Frame (~100 l/h bei 3ms) → unrealistisch
    C_last_raw = current_raw;
    break;
  }

  float delta_ml = delta_ul / 1000.0f;
  C_last_raw = current_raw;  // Jetzt erst aktualisieren!

  // === 4. VERBRÄUCHE HOCHZÄHLEN ===
  if (ignition_on) {
    Data.C_start       += delta_ml;
    Data.C_refuel      += delta_ml;
    Data.C_long_period += delta_ml / 1000.0f;
    Data.C_last_km     += delta_ml;  // für km-Berechnung
  }

  // === 5. AKTUELLER VERBRAUCH ===
  unsigned long now = millis();
  static float delta_sum = 0.0f;
  delta_sum += delta_ml;

  if (now - time_C_period_last >= 250) {
    unsigned long period_ms = now - time_C_period_last;
    time_C_period_last = now;

    if (period_ms > 50 && period_ms < 1000) {
      float lph = (delta_sum / 1000.0f) * (3600000.0f / period_ms);

      // Glättung
      static float smooth_lph = 0.0f;
      smooth_lph = 0.8f * smooth_lph + 0.2f * lph;
      C_actual_l_per_h = smooth_lph;
    }
    delta_sum = 0.0f;  // Reset für nächstes Fenster
  }

  // === 6. l/100km (nur bei Fahrt) ===
  if (driving && C_actual_l_per_h > 0.1f) {
    float l_per_100 = C_actual_l_per_h * 100.0f / velocity_actual;
    if (l_per_100 > 40.0f) l_per_100 = 40.0f;

    static float smooth_100 = 0.0f;
    smooth_100 = 0.9f * smooth_100 + 0.1f * l_per_100;
    C_actual_l_per_100km = smooth_100;

    // Dial nur bei Spritfluss und Geschwindigkeit
    if (delta_ml > 0.01f && velocity_actual > 10.0f) {
      C_dial_display_value = C_actual_l_per_100km;
    } else {
      C_dial_display_value = 0.0f;
    }
  } else {
    C_actual_l_per_100km = 0.0f;
    C_dial_display_value = 0.0f;
  }

  break;
}
    default: break;
  }
}
