void evaluate_CAN_messages(){


  // we do not use the temp variable here, as it may be used also from the other kernel
  int bit;     // hold the bit value to evaluate
  int byte;    // The byte / int value to evaluate
  float value; // the float value to evaluate

  //Auswertung der Daten
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
        Data.deposit_last = deposit;
        return;
      }

      if(byte != deposit){
        deposit = byte;
        
        // if the driver has refilled the deposit with at least
        // 5 Liter, we do reset the values since refuel 
        // the ammount of 5 liter will also handle changes of content due to driving
        if (deposit > (Data.deposit_last + 5) ){ 
          reset = REFUEL;   
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

      //kl_58d hase only two values 0x00 = lights are off and 0x64 = lights are on
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

      //total_km = rxFrame.data[7] << 8 << 8 + rxFrame.data[6] << 8 + rxFrame.data[5];
      value = rxFrame.data[7] * 65536 + rxFrame.data[6] * 256 + rxFrame.data[5]; 

      // after start km_total = 0, so we do not update driving values with first CAN message
      if(km_total == 0){
        km_total = value;
        break;
      }

      // we can do a complete recalculation of the petrol consumption, as we are now sure one km has been driven
      if(value != km_total){
        km_total = value;
        Data.km_start += 1;
        Data.km_refuel += 1;
        Data.km_long_period += 1;

        // delete oldest km consumption and add the newest one
        for (int i = 24; i > 0; i--) {
          Data.C_25_km[i] = Data.C_25_km[i-1];
        }
        Data.C_25_km[0] = Data.C_last_km; // in ml

        Data.C_last_km = 0;

        C_last_25_km = 0;
        int counter = 0;
        for(int i=0; i < 25; i++) {
          if(Data.C_25_km[i] != 0){
            C_last_25_km += Data.C_25_km[i];
            counter += 1;
          }
        }
        
        // recalculate if not all 25 km has been driven
        if ((counter < 24) && (counter != 0)){
          C_last_25_km = C_last_25_km / float(counter) * 25.0;
        }

      }
      break;
    }

   // Message from Motor 1
    case 0x280:
    {
      speed = (rxFrame.data[2] + 256 * rxFrame.data[3]) / 4;

      // Motor is running
      if ((speed > 900) && (motor_on == false)){
        motor_on = true;
      }
      // Motor has stoped, so save values to be reloaded on next start
      if ((speed < 700) && (motor_on == true)){
        motor_on = false;
        save = true; // we can not call saveData() from here, as we are on an other kernel!
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

   // Message from Motor 5
    case 0x480:
    {
      // get the actual value from Motor 5
      // µl since ignition (unsigned int C_motor_value) 
      C_motor_value = rxFrame.data[2] + rxFrame.data[3] * 256;

      // This will hapen, after motor stop and no ignition, but ESP has not shut down 
      // next value will be 0, so ignore that one to avoid miscalculation
      if(C_motor_value <= 1){
        C_last = C_motor_value;                // µl
        break;
      }

      // as new values are summited every 5 ms, the calculatet comsumption values are jumping a lot
      // so we do calculate them only every 300 ms to have more stable values
      if (millis() - time_C_period_last > 300){

        float delta = 0;                       // µl

        // check overflow of rxFrame.data[3]
        // Byte 3 jumps from 255 to 128 so we loose 128 * 256 µl
        if (C_motor_value < C_last){
          delta = C_motor_value + 32768 - C_last; 
        }
        else{
          delta = C_motor_value - C_last;
        }
        C_last = C_motor_value;                // µl

        delta = delta / 1000.0;                // µl in ml
        Data.C_last_km += delta;               // ml
        Data.C_start += delta;                 // ml
        Data.C_refuel += delta;                // ml
        Data.C_long_period += delta / 1000.0;  // ml in l

        time_C_period = millis() - time_C_period_last;
        time_C_period_last = millis();

        // delta = 0.147 ml ; time_C_period = 95 ms C_actual = 0.0015473 ml/ms = 5.57 l/h
        C_actual = (delta * 3600.0 / time_C_period);  // l/h
        C_actual = delta;
      }
      break;
    }

    default: break;
  }
}