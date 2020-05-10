void checkBrightness() {
  if (LCD_BRIGHT == 11) {                      
    if (analogRead(PHOTO) < BRIGHT_THRESHOLD) {  
      analogWrite(BACKLIGHT, LCD_BRIGHT_MIN);
    } else {                                     
      analogWrite(BACKLIGHT, LCD_BRIGHT_MAX);
    }
  } else {
    analogWrite(BACKLIGHT, LCD_BRIGHT * LCD_BRIGHT * 2.5);
  }

  if (LED_BRIGHT == 11) {                        
    if (analogRead(PHOTO) < BRIGHT_THRESHOLD) {  
#if (LED_MODE == 0)
      LED_ON = (LED_BRIGHT_MIN);
#else
      LED_ON = (255 - LED_BRIGHT_MIN);
#endif
    } else {                                     
#if (LED_MODE == 0)
      LED_ON = (LED_BRIGHT_MAX);
#else
      LED_ON = (255 - LED_BRIGHT_MAX);
#endif
    }
  }
}

void modesTick() {
  button.tick();
  boolean changeFlag = false;
  if (button.isSingle()) {    

    if (mode >= 240) {
      podMode++;
      switch (mode) {
        case 252:             
          //         podMode++;
          if (podMode > 4) podMode = 0;
          LEDType = podMode;
          changeFlag = true;
          break;

        case 253:           
          //         podMode++;
          if (podMode > 11) podMode = 0;
          LCD_BRIGHT = podMode;
          checkBrightness();
          changeFlag = true;
          break;

        case 254:          
          //         podMode++;
          if (podMode > 11) podMode = 0;
          LED_BRIGHT = podMode;
          changeFlag = true;
          break;

        case 255:         
          //         podMode++;
          if (podMode > 15) podMode = 1;
          changeFlag = true;
          break;
      }
    } else {
      do {
        mode++;
        if (mode > 10) mode = 0;
#if (CO2_SENSOR == 0 && mode == 1)
        mode = 3;
#endif
      } while (((VIS_ONDATA & (1 << (mode - 1))) == 0) && (mode > 0)); 
      changeFlag = true;
    }
  }
  if (button.isDouble()) {                 
    if (mode > 0 && mode < 11) {      
      MAX_ONDATA = (int)MAX_ONDATA ^ (1 << (mode - 1));
    } else if (mode == 0)  {
      mode0scr++;
      if (CO2_SENSOR == 0 && mode0scr == 1) mode0scr++;
      if (mode0scr > 5) mode0scr = 0;         
    } else if (mode > 240) podMode = 1; 
    changeFlag = true;
  }

  if ((button.isTriple()) && (mode == 0)) { 
    mode = 255;
    podMode = 3;
    changeFlag = true;
  }

  if (button.isHolded()) {  
    //    if ((mode >=252) && (mode <= 254)) {
    //      mode = 255;
    //      podMode = 1;
    //    }
    switch (mode) {
      case 0:
        bigDig = !bigDig;
        break;
      case 252:      
        mode = 255;
        podMode = 1;
        break;
      case 253:      
        mode = 255;
        podMode = 1;
        break;
      case 254:     
        mode = 255;
        podMode = 1;
        break;
      case 255:       
        if (podMode == 2 || podMode == 1) mode = 0;                
        if (podMode >= 3 && podMode <= 5) mode = 255 - podMode + 2;  
        if (podMode >= 6 && podMode <= 17) VIS_ONDATA = VIS_ONDATA ^ (1 << (podMode - 6));  
        if (podMode == 1) {                                       
          if (EEPROM.read(2) != (MAX_ONDATA & 255)) EEPROM.write(2, (MAX_ONDATA & 255));
          if (EEPROM.read(3) != (MAX_ONDATA >> 8)) EEPROM.write(3, (MAX_ONDATA >> 8));
          if (EEPROM.read(4) != (VIS_ONDATA & 255)) EEPROM.write(4, (VIS_ONDATA & 255));
          if (EEPROM.read(5) != (VIS_ONDATA >> 8)) EEPROM.write(5, (VIS_ONDATA >> 8));
          if (EEPROM.read(6) != mode0scr) EEPROM.write(6, mode0scr);
          if (EEPROM.read(7) != bigDig) EEPROM.write(7, bigDig);
          if (EEPROM.read(8) != LED_BRIGHT) EEPROM.write(8, LED_BRIGHT);
          if (EEPROM.read(9) != LCD_BRIGHT) EEPROM.write(9, LCD_BRIGHT);
          if (EEPROM.read(10) != LEDType) EEPROM.write(10, LEDType);
          if (EEPROM.read(0) != 122) EEPROM.write(0, 122);
        }
        if (podMode < 6) podMode = 1;
        if (mode == 252) podMode = LEDType;     
        if (mode == 254) podMode = LED_BRIGHT;  
        if (mode == 253) podMode = LCD_BRIGHT; 
        break;
      default:
        mode = 0;
    }
    changeFlag = true;
  }

  if (changeFlag) {
    if (mode >= 240) {
      lcd.clear();
      lcd.createChar(1, BM);  
      lcd.createChar(2, IY);  
      lcd.createChar(3, DD);  
      lcd.createChar(4, II);  
      lcd.createChar(5, IA);  
      lcd.createChar(6, YY);  
      lcd.createChar(7, AA);  
      lcd.createChar(0, ZZ);  
      lcd.setCursor(0, 0);
    }
    if (mode == 255) {         
#if (WEEK_LANG == 1)
      lcd.print("HACTPO\2K\4:");            
#else
      lcd.print("Setup:");
#endif
      lcd.setCursor(0, 1);
      switch (podMode) {
        case 1:
#if (WEEK_LANG == 1)
          lcd.print("COXPAH\4T\1");   
#else
          lcd.print("Save");
#endif
          break;
        case 2:
#if (WEEK_LANG == 1)
          lcd.print("B\6XO\3");         
#else
          lcd.print("Exit");
#endif
          break;
        case 5:
#if (WEEK_LANG == 1)
          lcd.print("PE\10.\4H\3\4KATOPA");  
#else
          lcd.print("indicator mode");
#endif
          break;
        case 3:
#if (WEEK_LANG == 1)
          lcd.print("\5PK.\4H\3\4KATOPA"); 
#else
          lcd.print("indicator brt.");
#endif
          break;
        case 4:
#if (WEEK_LANG == 1)
          lcd.print("\5PK.\7KPAHA");  
#else
          lcd.print("Bright LCD");
#endif
          break;
      }
      if (podMode >= 6 && podMode <= 17) {
        lcd.createChar(8, FF);  
        lcd.createChar(7, GG);  
        lcd.createChar(5, LL);  
        lcd.setCursor(10, 0);
#if (WEEK_LANG == 1)
        lcd.print("\7PA\10\4KOB");       
#else
        lcd.print("Charts  ");
#endif
        lcd.setCursor(0, 1);
        if ((3 & (1 << (podMode - 6))) != 0) lcd.print("CO2 ");
        if ((12 & (1 << (podMode - 6))) != 0) {
#if (WEEK_LANG == 1)
          lcd.print("B\5,% ");
#else
          lcd.print("Hum,%");
#endif
        }
        if ((48 & (1 << (podMode - 6))) != 0) lcd.print("t\337 ");
        if ((192 & (1 << (podMode - 6))) != 0) {
          if (PRESSURE) lcd.print("p,rain ");
          else lcd.print("p,mmPT ");
        }
        if ((768 & (1 << (podMode - 6))) != 0) {
#if (WEEK_LANG == 1)
          lcd.print("B\6C,m  ");
#else
          lcd.print("hgt,m  ");
#endif
        }

        if ((1365 & (1 << (podMode - 6))) != 0) {
          lcd.createChar(3, CH);  
          lcd.setCursor(8, 1);
#if (WEEK_LANG == 1)
          lcd.print("\3AC:");
#else
          lcd.print("Hour:");
#endif
        } else {
          lcd.setCursor(7, 1);
#if (WEEK_LANG == 1)
          lcd.print("\3EH\1:");
#else
          lcd.print("Day: ");
#endif
        }
        if ((VIS_ONDATA & (1 << (podMode - 6))) != 0) {
#if (WEEK_LANG == 1)
          lcd.print("BK\5 ");
#else
          lcd.print("On  ");
#endif
        }
        else {
#if (WEEK_LANG == 1)
          lcd.print("B\6K\5");
#else
          lcd.print("Off ");
#endif
        }
      }
    }
    if (mode == 252) {                 
      LEDType = podMode;
      lcd.createChar(6, LL);  //Л
      lcd.createChar(3, DD);  //Д
      lcd.createChar(5, II);  //И
      lcd.createChar(8, ZZ);  //Ж
      lcd.setCursor(0, 0);
#if (WEEK_LANG == 1)
      lcd.print("PE\10.\4H\3\4KATOPA:");
#else
      lcd.print("indicator mode:");
#endif
      lcd.setCursor(0, 1);
      switch (podMode) {
        case 0:
          lcd.print("CO2   ");
          break;
        case 1:
#if (WEEK_LANG == 1)
          lcd.print("B\6A\10H.");        
#else
          lcd.print("Humid.");
#endif
          break;
        case 2:
          lcd.print("t\337     ");
          break;
        case 3:
#if (WEEK_LANG == 1)
          lcd.print("OCA\3K\5");       
#else
          lcd.print("rain  ");
#endif
          break;
        case 4:
#if (WEEK_LANG == 1)
          lcd.print("\3AB\6EH\5E");   
#else
          lcd.print("pressure");
#endif
          break;
      }

    }
    if (mode == 253) {                    
#if (WEEK_LANG == 1)
      lcd.print("\5PK.\7KPAHA:");// + String(LCD_BRIGHT * 10) + "%  ");
#else
      lcd.print("Bright LCD:");
#endif
      //lcd.setCursor(11, 0);
      if (LCD_BRIGHT == 11) {
#if (WEEK_LANG == 1)
        lcd.print("ABTO ");
#else
        lcd.print("Auto ");
#endif
      }
      else lcd.print(String(LCD_BRIGHT * 10) + "%");
    }
    if (mode == 254) {         
      
#if (WEEK_LANG == 1)
      lcd.print("\5PK.\4H\3\4K.:");// + String(LED_BRIGHT * 10) + "%  ");
#else
      lcd.print("indic.brt.:");
#endif
      //lcd.setCursor(15, 0);
      if (LED_BRIGHT == 11) {
#if (WEEK_LANG == 1)
        lcd.print("ABTO ");
#else
        lcd.print("Auto ");
#endif
      }
      else lcd.print(String(LED_BRIGHT * 10) + "%");
    }

    if (mode == 0) {
      lcd.clear();
      loadClock();
      drawSensors();
      if (DISPLAY_TYPE == 1) drawData();
    } else if (mode <= 10) {
      //lcd.clear();
      loadPlot();
      redrawPlot();
    }
  }
}

void redrawPlot() {
  lcd.clear();
#if (DISPLAY_TYPE == 1)      
  switch (mode) {            
    case 1: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Hour, "c ", "hr", mode);
      break;
    case 2: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Day, "c ", "da", mode);
      break;
    case 3: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humHour, "h%", "hr", mode);
      break;
    case 4: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humDay, "h%", "da", mode);
      break;
    case 5: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tempHour, "t\337", "hr", mode);
      break;
    case 6: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tempDay, "t\337", "da", mode);
      break;
    //    case 7: drawPlot(0, 3, 15, 4, RAIN_MIN, RAIN_MAX, (int*)rainHour, "r ", "hr", mode);
    //      break;
    //    case 8: drawPlot(0, 3, 15, 4, RAIN_MIN, RAIN_MAX, (int*)rainDay, "r ", "da", mode);
    //      break;
    case 7: drawPlot(0, 3, 15, 4, PRESS_MIN, PRESS_MAX, (int*)pressHour, "p ", "hr", mode);
      break;
    case 8: drawPlot(0, 3, 15, 4, PRESS_MIN, PRESS_MAX, (int*)pressDay, "p ", "da", mode);
      break;
    case 9: drawPlot(0, 3, 15, 4, ALT_MIN, ALT_MAX, (int*)altHour, "m ", "hr", mode);
      break;
    case 10: drawPlot(0, 3, 15, 4, ALT_MIN, ALT_MAX, (int*)altDay, "m ", "da", mode);
      break;
  }
  
#else                 
  switch (mode) {
    case 1: drawPlot(0, 1, 12, 2, CO2_MIN, CO2_MAX, (int*)co2Hour, "c", "h", mode);
      break;
    case 2: drawPlot(0, 1, 12, 2, CO2_MIN, CO2_MAX, (int*)co2Day, "c", "d", mode);
      break;
    case 3: drawPlot(0, 1, 12, 2, HUM_MIN, HUM_MAX, (int*)humHour, "h", "h", mode);
      break;
    case 4: drawPlot(0, 1, 12, 2, HUM_MIN, HUM_MAX, (int*)humDay, "h", "d", mode);
      break;
    case 5: drawPlot(0, 1, 12, 2, TEMP_MIN, TEMP_MAX, (int*)tempHour, "t", "h", mode);
      break;
    case 6: drawPlot(0, 1, 12, 2, TEMP_MIN, TEMP_MAX, (int*)tempDay, "t", "d", mode);
      break;
    //    case 7: drawPlot(0, 1, 12, 2, RAIN_MIN, RAIN_MAX, (int*)rainHour, "r", "h", mode);
    //      break;
    //    case 8: drawPlot(0, 1, 12, 2, RAIN_MIN, RAIN_MAX, (int*)rainDay, "r", "d", mode);
    //      break;
    case 7: drawPlot(0, 1, 12, 2, PRESS_MIN, PRESS_MAX, (int*)pressHour, "p", "h", mode);
      break;
    case 8: drawPlot(0, 1, 12, 2, PRESS_MIN, PRESS_MAX, (int*)pressDay, "p", "d", mode);
      break;
    case 9: drawPlot(0, 1, 12, 2, ALT_MIN, ALT_MAX, (int*)altHour, "m", "h", mode);
      break;
    case 10: drawPlot(0, 1, 12, 2, ALT_MIN, ALT_MAX, (int*)altDay, "m", "d", mode);
      break;
  }
#endif
}

void readSensors() {
  bme.takeForcedMeasurement();
  dispTemp = bme.readTemperature();
  dispHum = bme.readHumidity();
  dispAlt = ((float)dispAlt * 1 + bme.readAltitude(SEALEVELPRESSURE_HPA)) / 2; 
  dispPres = (float)bme.readPressure() * 0.00750062;
#if (CO2_SENSOR == 1)
  dispCO2 = mhz19.getPPM();
#else
  dispCO2 = 0;
#endif
}

void drawSensors() {
#if (DISPLAY_TYPE == 1)
  if (mode0scr != 2) {                   
    lcd.setCursor(0, 2);
    if (bigDig) {
      if (mode0scr == 1) lcd.setCursor(15, 2);
      if (mode0scr != 1) lcd.setCursor(15, 0);
    }
    lcd.print(String(dispTemp, 1));
    lcd.write(223);
  } else {
    drawTemp(dispTemp, 0, 0);
  }

  if (mode0scr != 4) {      
    lcd.setCursor(5, 2);
    if (bigDig) lcd.setCursor(15, 1);
    lcd.print(" " + String(dispHum) + "% ");
  } else {
    drawHum(dispHum, 0, 0);
  }

#if (CO2_SENSOR == 1)
  if (mode0scr != 1) {   

    if (bigDig) {
      lcd.setCursor(15, 2);
      lcd.print(String(dispCO2) + "p");
    } else {
      lcd.setCursor(11, 2);
      lcd.print(String(dispCO2) + "ppm ");
    }
  } else {
    drawPPM(dispCO2, 0, 0);
  }
#endif

  if (mode0scr != 3) {        
    lcd.setCursor(0, 3);
    if (bigDig && mode0scr == 0) lcd.setCursor(15, 3);
    if (bigDig && (mode0scr == 1 || mode0scr == 2)) lcd.setCursor(15, 0);
    if (bigDig && mode0scr == 4) lcd.setCursor(15, 1);
    if (!(bigDig && mode0scr == 1)) lcd.print(String(dispPres) + "mm");
  } else {
    drawPres(dispPres, 0, 0);
  }

  if (mode0scr != 5) {           
  } else {                     
    drawAlt(dispAlt, 0, 0);
  }

  if (!bigDig) {                         
    lcd.setCursor(5, 3);
    lcd.print(" rain     ");
    lcd.setCursor(11, 3);
    if (dispRain < 0) lcd.setCursor(10, 3);
    lcd.print(String(dispRain) + "%");
  }

  if (mode0scr != 0) {      
    lcd.setCursor(15, 3);
    if (hrs / 10 == 0) lcd.print(" ");
    lcd.print(hrs);
    lcd.print(":");
    if (mins / 10 == 0) lcd.print("0");
    lcd.print(mins);
  } else {
    drawClock(hrs, mins, 0, 0); //, 1);
  }
#else


  if (!bigDig) {         
    lcd.setCursor(0, 0);
    lcd.print(String(dispTemp, 1));
    lcd.write(223);
    lcd.setCursor(6, 0);
    lcd.print(String(dispHum) + "% ");

#if (CO2_SENSOR == 1)
    lcd.print(String(dispCO2) + "ppm");
    if (dispCO2 < 1000) lcd.print(" ");
#endif

    lcd.setCursor(0, 1);
    lcd.print(String(dispPres) + " mm  rain ");
    lcd.print(String(dispRain) + "% ");
  } else {           
    switch (mode0scr) {
      case 0:
        drawClock(hrs, mins, 0, 0);
        break;
      case 1:
#if (CO2_SENSOR == 1)
        drawPPM(dispCO2, 0, 0);
#endif
        break;
      case 2:
        drawTemp(dispTemp, 2, 0);
        break;
      case 3:
        drawPres(dispPres, 2, 0);
        break;
      case 4:
        drawHum(dispHum, 0, 0);
        break;
      case 5:
        drawHum(dispAlt, 0, 0);
        break;
    }
  }
#endif
}

void plotSensorsTick() {
  if (testTimer(hourPlotTimerD, hourPlotTimer)) {
    for (byte i = 0; i < 14; i++) {
      tempHour[i] = tempHour[i + 1];
      humHour[i] = humHour[i + 1];
      pressHour[i] = pressHour[i + 1];
      //      rainHour[i] = rainHour[i + 1];
      altHour[i] = altHour[i + 1];
      co2Hour[i] = co2Hour[i + 1];
    }
    tempHour[14] = dispTemp;
    humHour[14] = dispHum;
    pressHour[14] = dispPres;
    //    rainHour[14] = dispRain;
    altHour[14] = dispAlt;
    co2Hour[14] = dispCO2;

    if (PRESSURE) pressHour[14] = dispRain;
    else pressHour[14] = dispPres;
  }

  if (testTimer(dayPlotTimerD, dayPlotTimer)) {
    long averTemp = 0, averHum = 0, averPress = 0, averAlt = 0, averCO2 = 0; //, averRain = 0

    for (byte i = 0; i < 15; i++) {
      averTemp += tempHour[i];
      averHum += humHour[i];
      averPress += pressHour[i];
      //      averRain += rainHour[i];
      averAlt += altHour[i];
      averCO2 += co2Hour[i];
    }
    averTemp /= 15;
    averHum /= 15;
    averPress /= 15;
    //    averRain /= 15;
    averAlt /= 15;
    averCO2 /= 15;

    for (byte i = 0; i < 14; i++) {
      tempDay[i] = tempDay[i + 1];
      humDay[i] = humDay[i + 1];
      pressDay[i] = pressDay[i + 1];
      //      rainDay[i] = rainDay[i + 1];
      altDay[i] = altDay[i + 1];
      co2Day[i] = co2Day[i + 1];
    }
    tempDay[14] = averTemp;
    humDay[14] = averHum;
    pressDay[14] = averPress;
    //    rainDay[14] = averRain;
    altDay[14] = averAlt;
    co2Day[14] = averCO2;
  }

  if (testTimer(predictTimerD, predictTimer)) {
    long averPress = 0;
    for (byte i = 0; i < 10; i++) {
      bme.takeForcedMeasurement();
      averPress += bme.readPressure();
      delay(1);
    }
    averPress /= 10;

    for (byte i = 0; i < 5; i++) {                  
      pressure_array[i] = pressure_array[i + 1];    
    }
    pressure_array[5] = averPress;               
    sumX = 0;
    sumY = 0;
    sumX2 = 0;
    sumXY = 0;
    for (int i = 0; i < 6; i++) {
      //sumX += time_array[i];
      sumX += i;
      sumY += (long)pressure_array[i];
      //sumX2 += time_array[i] * time_array[i];
      sumX2 += i * i;
      //sumXY += (long)time_array[i] * pressure_array[i];
      sumXY += (long)i * pressure_array[i];
    }
    a = 0;
    a = (long)6 * sumXY;              
    a = a - (long)sumX * sumY;
    a = (float)a / (6 * sumX2 - sumX * sumX);
    delta = a * 6;     
    dispRain = map(delta, -250, 250, 100, -100);   
    //Serial.println(String(pressure_array[5]) + " " + String(delta) + " " + String(dispRain));   
  }
}

boolean dotFlag;
void clockTick() {
  dotFlag = !dotFlag;
  if (dotFlag) {          
    secs++;
    if (secs > 59) {       
      secs = 0;
      mins++;
      if (mins <= 59 && mode == 0) {
        drawSensors();     
      }
    }
    if (mins > 59) {        
      // loadClock();        
      now = rtc.now();
      secs = now.second();
      mins = now.minute();
      hrs = now.hour();
      if (mode == 0) drawSensors();
      if (hrs > 23) hrs = 0;
      if (mode == 0 && DISPLAY_TYPE) drawData();
    }
    if ((DISP_MODE != 0 && mode == 0) && DISPLAY_TYPE == 1 && !bigDig) { 
      lcd.setCursor(15, 1);
      if (secs < 10) lcd.print(" ");
      lcd.print(secs);
    }
  }

  if (mode == 0) {                                

    if (!bigDig && powerStatus != 255 && DISPLAY_TYPE == 1) {      

      if (analogRead(A1) > 900 || analogRead(A0) < 300 || (analogRead(A1) < 300 && analogRead(A0) < 300)) powerStatus = 0;
      else powerStatus = (constrain((int)analogRead(A0) * 5.2 / 1023.0, 3.0, 4.2) - 3.0) / ((4.2 - 3.0) / 6.0) + 1;

      if (powerStatus) {
        for (byte i = 2; i <= 6; i++) {    
          if ((7 - powerStatus) < i) DC[i] = 0b11111;
          else DC[i] = 0b10001;
        }
        lcd.createChar(6, DC);
      } else lcd.createChar(6, AC);

      if (mode0scr != 1) lcd.setCursor(19, 2);
      else lcd.setCursor(19, 0);
      if (!dotFlag && powerStatus == 1) lcd.write(32);
      else lcd.write(6);
    }
    
    byte code;
    if (dotFlag) code = 165;
    else code = 32;
    if (mode0scr == 0 && (bigDig && DISPLAY_TYPE == 0 || DISPLAY_TYPE == 1)) {   
      if (bigDig && DISPLAY_TYPE == 1) lcd.setCursor(7, 2);
      else lcd.setCursor(7, 0);
      lcd.write(code);
      lcd.setCursor(7, 1);
      lcd.write(code);
    }
    else {
#if (DISPLAY_TYPE == 1)
      if (code == 165) code = 58;
      lcd.setCursor(17, 3);
      lcd.write(code);
#endif
    }
  }

  if ((dispCO2 >= blinkLEDCO2 && LEDType == 0 || dispHum <= blinkLEDHum && LEDType == 1 || dispTemp >= blinkLEDTemp && LEDType == 2) && !dotFlag) setLEDcolor(0); 
  else setLED();
}

boolean testTimer(unsigned long & dataTimer, unsigned long setTimer) {
  if (millis() - dataTimer >= setTimer) {
    dataTimer = millis();
    return true;
  } else {
    return false;
  }
}
