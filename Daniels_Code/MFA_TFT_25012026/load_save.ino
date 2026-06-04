// struct values_to_save {           // Data to be stored permanetly, 
//     int page; 				             // Page to display
//     int mode;                     // Mode is on of START, REFUEL, PERIOD
//     int deposit_last;             // last stored content of deposit, needed to check if car has been refueled
//     unsigned int km_start;        // distance driven since start           
//     unsigned int km_refuel;       // distance driven since refule           
//     unsigned int km_long_period;  // distance driven since last manual reset
//     float C_start;                // consumption since start ml
//     float C_refuel;               // consumption since refule ml
//     float C_long_period;          // consumption since last manual reset l
//     float C_25_km[25];            // consumption from the last 25 driven km in ml
//     float C_last_km;              // conssumption of the last km, needed to calculate the last 25 km average in ml
//                                   // need to store the value, as we may have driven only 800 m on shut down and we do want to loose the amount used 
//     unsigned int time_start;      // time since last start min
//     unsigned int time_refuel;     // time since last refuel min
//     unsigned int time_long_period;// time since last manual reset min
//     unsigned int velocity_max;    // may be playing only ;-)
// };

void save_Data(void){
  // setup preferences to store and read values "Data = Namespace, false = read and write"
  preferences.begin("Settings", false);
    preferences.putInt("page", Data.page);
    preferences.putInt("mode", Data.mode);
    preferences.putInt("deposit", Data.deposit_last);
    preferences.putUInt("vel_max", Data.velocity_max);
  preferences.end();

  preferences.begin("km", false);
    preferences.putUInt("start", Data.km_start);
    preferences.putUInt("refuel", Data.km_refuel);
    preferences.putUInt("period", Data.km_long_period);
  preferences.end();

  preferences.begin("time", false);
    preferences.putUInt("start", Data.time_start);
    preferences.putUInt("refuel", Data.time_refuel);
    preferences.putFloat("period", Data.time_long_period);
  preferences.end();

  preferences.begin("Consumption", false);
    preferences.putFloat("start", Data.C_start);
    preferences.putFloat("refuel", Data.C_refuel);
    preferences.putFloat("period", Data.C_long_period);
    preferences.putFloat("last_km", Data.C_last_km);
    for(int i=0; i < 25; i++) {
      itoa(i, TFT_String, 5);
      preferences.putFloat(TFT_String, Data.C_25_km[i]);
    }
  preferences.end();

  //Serial.println(F("Data saved"));

}

void load_Data(void){

  preferences.begin("Settings", false);
    Data.page = preferences.getInt("page", 0);
    Data.mode = preferences.getInt("mode", 1);
    Data.deposit_last = preferences.getInt("deposit", 0);
    //deposit = Data.deposit_last;
    Data.velocity_max = preferences.getUInt("vel_max", 0);
  preferences.end();

  preferences.begin("km", false);
    Data.km_start = preferences.getUInt("start", 0);
    Data.km_refuel = preferences.getUInt("refuel", 0);
    Data.km_long_period = preferences.getUInt("period", 0);
  preferences.end();

  preferences.begin("time", false);
    Data.time_start = preferences.getUInt("start", 0);
    Data.time_refuel = preferences.getUInt("refuel", 0);
    Data.time_long_period = preferences.getFloat("period", 0.0);
  preferences.end();

  preferences.begin("Consumption", false);
    Data.C_start = preferences.getFloat("start", 0.0);
    Data.C_refuel = preferences.getFloat("refuel", 0.0);
    Data.C_long_period = preferences.getFloat("period", 0.0);
    Data.C_last_km = preferences.getFloat("last_km", 0.0);

    C_last_25_km = 0;
    for(int i=0; i < 25; i++) {
      itoa(i, TFT_String, 5);
      Data.C_25_km[i] = preferences.getFloat(TFT_String, 0.0);
      if(Data.C_25_km[i] != 0){
        C_last_25_km += Data.C_25_km[i];
      }
    }
    
  preferences.end();

    //print_Data();
  //Serial.println(F("Data loaded"));
}

void reset_Data(int mode){
// ToDo : how to reset the from start values after one hour 

// Idee in case0x280 evaluate_can_messages gelöst - Echte Uhrzeit erforderlich? oder Zeit speichern?)

  // If the time past from last stop is over one hour, we reset the start values


 // Serial.print("User reset off ");
 // Serial.print("\t");

  if(mode == START){
 //   Serial.println("START");
    Data.C_start = 0.0;
    Data.km_start = 0;
    C_last = 0.0;
    Data.C_last_km = 0.0;
    Data.time_start = 0;
//    String(F("R_START")).toCharArray(TFT_String, 12);
  }
  else if (mode == REFUEL){
 //   Serial.println("REFUEL");
    Data.C_refuel = 0.0;
    Data.km_refuel = 0;
    Data.time_refuel = 0;
    C_last = 0.0;
    Data.C_last_km = 0.0;
//    String(F("R_REFUEL")).toCharArray(TFT_String, 12);
  }
  else if (mode == PERIOD){
//    Serial.println("PERIOD");
    Data.C_long_period = 0.0;
    Data.km_long_period = 0;
    Data.time_long_period = 0;
    C_last = 0.0;
    Data.C_last_km = 0.0;
//    String(F("R_PERIOD")).toCharArray(TFT_String, 12);
  }
  else{
    //Serial.println(mode);
    return;
  }
//  draw_small_value_box(320, 460, 200, 40, TFT_String);
  //save_Data();
}

void print_Data (void){
  Serial.print("Page = ");
  Serial.println(Data.page);
  Serial.print("Mode = ");
  Serial.println(Data.mode);
  Serial.print("Deposit_last = ");
  Serial.println(Data.deposit_last);
  Serial.print("Max Velocity = ");
  Serial.println(Data.velocity_max);

  Serial.print("km_start = ");
  Serial.println(Data.km_start);
  Serial.print("km_refuel = ");
  Serial.println(Data.km_refuel);
  Serial.print("km_period = ");
  Serial.println(Data.km_long_period);

  Serial.print("time start = ");
  Serial.println(Data.time_start);
  Serial.print("time refuel = ");
  Serial.println(Data.time_refuel);
  Serial.print("time_period = ");
  Serial.println(Data.time_long_period);

  Serial.print("C_Start = ");
  Serial.println(Data.C_start);
  Serial.print("C_Refuel = ");
  Serial.println(Data.C_refuel);
  Serial.print("C_period = ");
  Serial.println(Data.C_long_period);
  Serial.print("C_last_km = ");
  Serial.println(Data.C_last_km);

  for(int i=0; i < 25; i++) {
    Serial.print("C_km[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(Data.C_25_km[i]);
  }

  Serial.print("C_last_25 km = ");
  Serial.println(C_last_25_km);

}

/*
//////////////////
////////////////// Variante für unterschiedliche Speicherstände Memory 1 und 0-> nicht erforderlich aktuell

String getMemoryPrefix() {
  return String("m") + String(currentMemory) + "_";
}


void save_Data(void) {
  String prefix = getMemoryPrefix();   // z.B. "m0_" oder "m1_"

  // Settings
  preferences.begin("Settings", false);
    preferences.putInt(   (prefix + "page").c_str(),        Data.page);
    preferences.putInt(   (prefix + "mode").c_str(),        Data.mode);
    preferences.putInt(   (prefix + "deposit").c_str(),     Data.deposit_last);
    preferences.putUInt(  (prefix + "vel_max").c_str(),     Data.velocity_max);
  preferences.end();

  // km
  preferences.begin("km", false);
    preferences.putUInt(  (prefix + "start").c_str(),       Data.km_start);
    preferences.putUInt(  (prefix + "refuel").c_str(),      Data.km_refuel);
    preferences.putUInt(  (prefix + "period").c_str(),      Data.km_long_period);
  preferences.end();

  // time
  preferences.begin("time", false);
    preferences.putUInt(  (prefix + "start").c_str(),       Data.time_start);
    preferences.putUInt(  (prefix + "refuel").c_str(),      Data.time_refuel);
    preferences.putFloat( (prefix + "period").c_str(),      Data.time_long_period);  // ← war vorher falsch als UInt
  preferences.end();

  // Consumption
  preferences.begin("Consumption", false);
    preferences.putFloat( (prefix + "start").c_str(),       Data.C_start);
    preferences.putFloat( (prefix + "refuel").c_str(),      Data.C_refuel);
    preferences.putFloat( (prefix + "period").c_str(),      Data.C_long_period);
    preferences.putFloat( (prefix + "last_km").c_str(),     Data.C_last_km);

    for(int i = 0; i < 25; i++) {
      String key = prefix + "C25_" + String(i);     // Besser lesbar als nur Zahl
      preferences.putFloat(key.c_str(), Data.C_25_km[i]);
    }
  preferences.end();

  //Serial.println("Data saved for memory " + String(currentMemory));
}

// ──────────────────────────────────────────────────────────────

void load_Data(void) {
  String prefix = getMemoryPrefix();   // z.B. "m0_" oder "m1_"

  // Settings
  preferences.begin("Settings", false);
    Data.page         = preferences.getInt(   (prefix + "page").c_str(),    0);
    Data.mode         = preferences.getInt(   (prefix + "mode").c_str(),    1);
    Data.deposit_last = preferences.getInt(   (prefix + "deposit").c_str(), 0);
    Data.velocity_max = preferences.getUInt(  (prefix + "vel_max").c_str(), 0);
  preferences.end();

  // km
  preferences.begin("km", false);
    Data.km_start       = preferences.getUInt((prefix + "start").c_str(),  0);
    Data.km_refuel      = preferences.getUInt((prefix + "refuel").c_str(), 0);
    Data.km_long_period = preferences.getUInt((prefix + "period").c_str(), 0);
  preferences.end();

  // time
  preferences.begin("time", false);
    Data.time_start       = preferences.getUInt( (prefix + "start").c_str(),  0);
    Data.time_refuel      = preferences.getUInt( (prefix + "refuel").c_str(), 0);
    Data.time_long_period = preferences.getFloat((prefix + "period").c_str(), 0.0);
  preferences.end();

  // Consumption
  preferences.begin("Consumption", false);
    Data.C_start       = preferences.getFloat((prefix + "start").c_str(),    0.0);
    Data.C_refuel      = preferences.getFloat((prefix + "refuel").c_str(),   0.0);
    Data.C_long_period = preferences.getFloat((prefix + "period").c_str(),   0.0);
    Data.C_last_km     = preferences.getFloat((prefix + "last_km").c_str(),  0.0);

    C_last_25_km = 0.0f;
    for(int i = 0; i < 25; i++) {
      String key = prefix + "C25_" + String(i);
      Data.C_25_km[i] = preferences.getFloat(key.c_str(), 0.0f);
      if(Data.C_25_km[i] != 0) {
        C_last_25_km += Data.C_25_km[i];
      }
    }
  preferences.end();

  //Serial.println("Data loaded for memory " + String(currentMemory));
}

// ──────────────────────────────────────────────────────────────

void reset_Data(int mode) {
  // Reset betrifft immer nur den aktuellen Memory!
  // (weil Data die aktuelle Struktur ist)

  Serial.print("User reset off - mode ");
  Serial.println(mode);

  if(mode == START) {
    Data.C_start     = 0.0;
    Data.km_start    = 0;
    Data.time_start  = 0;
    Data.C_last_km   = 0.0;
    C_last           = 0;
    // Optional: Ringpuffer für START zurücksetzen?
    // for(int i=0; i<25; i++) Data.C_25_km[i] = 0.0;
  }
  else if(mode == REFUEL) {
    Data.C_refuel    = 0.0;
    Data.km_refuel   = 0;
    Data.time_refuel = 0;
    Data.C_last_km   = 0.0;
    C_last           = 0;
  }
  else if(mode == PERIOD) {
    Data.C_long_period    = 0.0;
    Data.km_long_period   = 0;
    Data.time_long_period = 0.0;
    Data.C_last_km        = 0.0;
    C_last                = 0;
  }
  else {
    return;
  }

  save = true;  // Wichtig: Änderungen werden beim nächsten Loop-Zyklus gespeichert
}

// ──────────────────────────────────────────────────────────────

void print_Data(void) {
  Serial.println("--- Data (Memory " + String(currentMemory) + ") ---");
  Serial.print("Page          = "); Serial.println(Data.page);
  Serial.print("Mode          = "); Serial.println(Data.mode);
  Serial.print("Deposit_last  = "); Serial.println(Data.deposit_last);
  Serial.print("Max Velocity  = "); Serial.println(Data.velocity_max);

  Serial.print("km_start      = "); Serial.println(Data.km_start);
  Serial.print("km_refuel     = "); Serial.println(Data.km_refuel);
  Serial.print("km_period     = "); Serial.println(Data.km_long_period);

  Serial.print("time start    = "); Serial.println(Data.time_start);
  Serial.print("time refuel   = "); Serial.println(Data.time_refuel);
  Serial.print("time_period   = "); Serial.println(Data.time_long_period);

  Serial.print("C_Start       = "); Serial.println(Data.C_start, 1);
  Serial.print("C_Refuel      = "); Serial.println(Data.C_refuel, 1);
  Serial.print("C_period      = "); Serial.println(Data.C_long_period, 3);
  Serial.print("C_last_km     = "); Serial.println(Data.C_last_km, 1);

  Serial.print("C_last_25_km  = "); Serial.println(C_last_25_km, 1);

  Serial.println("Last 25 km:");
  for(int i = 0; i < 25; i++) {
    if (i % 5 == 0 && i > 0) Serial.println();
    Serial.print("  "); Serial.print(i); Serial.print(": "); 
    Serial.print(Data.C_25_km[i], 1); Serial.print("  ");
  }
  Serial.println("\n-----------------------------");
}
*/
