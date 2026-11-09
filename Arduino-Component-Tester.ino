/*
  Universal Component Tester (DIY LCR Meter)
  Developer / Lab: YUSELLAB
  Reads: Resistors, Capacitors, Diodes, LEDs, BJTs (NPN/PNP), MOSFETs (N/P)
  Auto-detects pins (TP1, TP2, TP3)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// TP1, TP2, TP3 Pin Definitions
const int adcPin[3]  = {A0, A1, A2};
const int lowPin[3]  = {8, 10, 12};   // 680 ohm test resistors
const int highPin[3] = {9, 11, 13};   // 470k ohm test resistors

void setup() {
  lcd.init();
  lcd.backlight();
  
  // Opening Splash Screen
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("YUSELLAB");
  lcd.setCursor(0, 1);
  lcd.print("Component Tester");
  delay(2000);
  lcd.clear();
}

void resetPins() {
  for (int p = 8; p <= 13; p++) pinMode(p, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

// Discharge capacitors and reset gates to avoid false readings
void discharge() {
  resetPins();
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  delay(40);
  resetPins();
}

void loop() {
  discharge();

  // =============================================================
  // 0. TRANSISTOR TEST (NPN & PNP BJT)
  // =============================================================
  for (int b = 0; b < 3; b++) {
    int p1 = (b + 1) % 3;
    int p2 = (b + 2) % 3;

    // --- NPN BASE DETECTION ---
    discharge();
    pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], HIGH); 
    pinMode(adcPin[p1], OUTPUT); digitalWrite(adcPin[p1], LOW);  
    delay(5);
    int drop1 = analogRead(adcPin[b]);

    discharge();
    pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], HIGH); 
    pinMode(adcPin[p2], OUTPUT); digitalWrite(adcPin[p2], LOW);  
    delay(5);
    int drop2 = analogRead(adcPin[b]);

    if (drop1 > 50 && drop1 < 400 && drop2 > 50 && drop2 < 400) {
      discharge();
      pinMode(adcPin[p2], OUTPUT); digitalWrite(adcPin[p2], LOW);  
      pinMode(lowPin[p1], OUTPUT); digitalWrite(lowPin[p1], HIGH); 
      pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], HIGH); 
      delay(15);
      int valA = analogRead(adcPin[p1]);

      discharge();
      pinMode(adcPin[p1], OUTPUT); digitalWrite(adcPin[p1], LOW);  
      pinMode(lowPin[p2], OUTPUT); digitalWrite(lowPin[p2], HIGH); 
      pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], HIGH); 
      delay(15);
      int valB = analogRead(adcPin[p2]);

      int c_pin = (valA < valB) ? p1 : p2;
      int e_pin = (valA < valB) ? p2 : p1;

      lcd.clear();
      lcd.print("Type: NPN BJT");
      lcd.setCursor(0, 1);
      lcd.print("B:"); lcd.print(b + 1);
      lcd.print(" C:"); lcd.print(c_pin + 1);
      lcd.print(" E:"); lcd.print(e_pin + 1);
      delay(2000);
      return;
    }

    // --- PNP BASE DETECTION ---
    discharge();
    pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], LOW);  
    pinMode(adcPin[p1], OUTPUT); digitalWrite(adcPin[p1], HIGH); 
    delay(5);
    drop1 = analogRead(adcPin[b]);

    discharge();
    pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], LOW);  
    pinMode(adcPin[p2], OUTPUT); digitalWrite(adcPin[p2], HIGH); 
    delay(5);
    drop2 = analogRead(adcPin[b]);

    if (drop1 > 600 && drop1 < 970 && drop2 > 600 && drop2 < 970) {
      discharge();
      pinMode(adcPin[p2], OUTPUT); digitalWrite(adcPin[p2], HIGH); 
      pinMode(lowPin[p1], OUTPUT); digitalWrite(lowPin[p1], LOW);  
      pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], LOW);  
      delay(15);
      int valA = analogRead(adcPin[p1]);

      discharge();
      pinMode(adcPin[p1], OUTPUT); digitalWrite(adcPin[p1], HIGH); 
      pinMode(lowPin[p2], OUTPUT); digitalWrite(lowPin[p2], LOW);  
      pinMode(lowPin[b], OUTPUT);  digitalWrite(lowPin[b], LOW);  
      delay(15);
      int valB = analogRead(adcPin[p2]);

      int c_pin = (valA > valB) ? p1 : p2;
      int e_pin = (valA > valB) ? p2 : p1;

      lcd.clear();
      lcd.print("Type: PNP BJT");
      lcd.setCursor(0, 1);
      lcd.print("B:"); lcd.print(b + 1);
      lcd.print(" C:"); lcd.print(c_pin + 1);
      lcd.print(" E:"); lcd.print(e_pin + 1);
      delay(2000);
      return;
    }
  }

  // =============================================================
  // 0.2 MOSFET TEST (N-CH & P-CH)
  // =============================================================
  for (int g = 0; g < 3; g++) {
    int d = (g + 1) % 3;
    int s = (g + 2) % 3;

    // Check Gate insulation (must not draw current)
    discharge();
    pinMode(highPin[g], OUTPUT); digitalWrite(highPin[g], HIGH);
    delay(5);
    int gateVolt = analogRead(adcPin[g]);

    if (gateVolt > 850) { 
      
      // --- N-CH MOSFET ---
      discharge();
      pinMode(lowPin[g], OUTPUT); digitalWrite(lowPin[g], LOW);
      pinMode(adcPin[s], OUTPUT); digitalWrite(adcPin[s], LOW);
      pinMode(lowPin[d], OUTPUT); digitalWrite(lowPin[d], HIGH);
      delay(5);
      int valOff = analogRead(adcPin[d]);

      pinMode(lowPin[g], OUTPUT); digitalWrite(lowPin[g], HIGH);
      delay(10);
      int valOn = analogRead(adcPin[d]);

      if (valOff > 850 && valOn < 400) {
        lcd.clear();
        lcd.print("Type: N-CH MOS");
        lcd.setCursor(0, 1);
        lcd.print("G:"); lcd.print(g + 1);
        lcd.print(" D:"); lcd.print(d + 1);
        lcd.print(" S:"); lcd.print(s + 1);
        delay(2000);
        return;
      }

      // --- P-CH MOSFET ---
      discharge();
      pinMode(lowPin[g], OUTPUT); digitalWrite(lowPin[g], HIGH);
      pinMode(adcPin[s], OUTPUT); digitalWrite(adcPin[s], HIGH);
      pinMode(lowPin[d], OUTPUT); digitalWrite(lowPin[d], LOW);
      delay(5);
      valOff = analogRead(adcPin[d]);

      pinMode(lowPin[g], OUTPUT); digitalWrite(lowPin[g], LOW);
      delay(10);
      valOn = analogRead(adcPin[d]);

      if (valOff < 150 && valOn > 600) {
        lcd.clear();
        lcd.print("Type: P-CH MOS");
        lcd.setCursor(0, 1);
        lcd.print("G:"); lcd.print(g + 1);
        lcd.print(" D:"); lcd.print(d + 1);
        lcd.print(" S:"); lcd.print(s + 1);
        delay(2000);
        return;
      }
    }
  }

  // =============================================================
  // 1. TWO-PIN COMPONENT MATRIX SCANNER (Pins: 1-2, 2-3, 1-3)
  // =============================================================
  int pairs[3][2] = {{0, 1}, {1, 2}, {0, 2}};
  
  for (int i = 0; i < 3; i++) {
    int pA = pairs[i][0];
    int pB = pairs[i][1];

    // --- DIODE / LED TEST ---
    discharge();
    pinMode(adcPin[pB], OUTPUT); digitalWrite(adcPin[pB], LOW);
    pinMode(lowPin[pA], OUTPUT); digitalWrite(lowPin[pA], HIGH);
    delay(10);
    float vFwd = (analogRead(adcPin[pA]) * 5.0) / 1023.0;

    discharge();
    pinMode(adcPin[pA], OUTPUT); digitalWrite(adcPin[pA], LOW);
    pinMode(lowPin[pB], OUTPUT); digitalWrite(lowPin[pB], HIGH);
    delay(10);
    float vRev = (analogRead(adcPin[pB]) * 5.0) / 1023.0;

    // Strict one-way conduction for Diodes/LEDs
    if (vFwd < 4.2 && vRev > 4.5) { 
      lcd.clear();
      if (vFwd < 1.35) lcd.print("Type: Diode");
      else lcd.print("Type: LED");
      lcd.setCursor(0, 1);
      lcd.print(vFwd, 2); lcd.print("V ");
      lcd.print(pA+1); lcd.print("A-"); lcd.print(pB+1); lcd.print("K");
      delay(2000);
      return;
    }
    else if (vRev < 4.2 && vFwd > 4.5) { 
      lcd.clear();
      if (vRev < 1.35) lcd.print("Type: Diode");
      else lcd.print("Type: LED");
      lcd.setCursor(0, 1);
      lcd.print(vRev, 2); lcd.print("V ");
      lcd.print(pB+1); lcd.print("A-"); lcd.print(pA+1); lcd.print("K");
      delay(2000);
      return;
    }

    // --- RESISTOR TEST (With Differential ADC Reading) ---
    discharge();
    pinMode(adcPin[pB], OUTPUT); digitalWrite(adcPin[pB], LOW);
    pinMode(lowPin[pA], OUTPUT); digitalWrite(lowPin[pA], HIGH);
    delay(5);
    
    int rawHigh = analogRead(adcPin[pA]); // Voltage entering the resistor
    int rawLow = analogRead(adcPin[pB]);  // Internal pin resistance drop
    int diffVal = rawHigh - rawLow;       // True voltage drop across component

    if (diffVal < 3) {
      lcd.clear();
      lcd.print("Shorted ("); lcd.print(pA+1); lcd.print("-"); lcd.print(pB+1); lcd.print(")");
      delay(2000);
      return;
    }

    // Low Resistance Range (680R)
    if (rawHigh < 950) {
      float Rx = 700.0 * ((float)diffVal / (1023.0 - (float)rawHigh));
      lcd.clear();
      lcd.print("Resistor("); lcd.print(pA+1); lcd.print("-"); lcd.print(pB+1); lcd.print(")");
      lcd.setCursor(0, 1);
      if (Rx >= 1000.0) {
        lcd.print("R: "); lcd.print(Rx / 1000.0, 2); lcd.print(" kOhm");
      } else {
        lcd.print("R: "); lcd.print(Rx, 1); lcd.print(" Ohm");
      }
      delay(2000);
      return;
    } 

    // High Resistance Range (470k)
    discharge();
    pinMode(adcPin[pB], OUTPUT); digitalWrite(adcPin[pB], LOW);
    pinMode(highPin[pA], OUTPUT); digitalWrite(highPin[pA], HIGH);
    delay(5); 
    int rA = analogRead(adcPin[pA]);
    delay(15);
    int rB = analogRead(adcPin[pA]);

    bool isCapacitor = (rB > rA + 8); // Check if voltage is rising (charging)

    if (!isCapacitor && rB <= 1012) {
      int rLow = analogRead(adcPin[pB]);
      int rFark = rB - rLow;
      
      float Rx = 480500.0 * ((float)rFark / (1023.0 - (float)rB));
      lcd.clear();
      lcd.print("Resistor("); lcd.print(pA+1); lcd.print("-"); lcd.print(pB+1); lcd.print(")");
      lcd.setCursor(0, 1);
      if (Rx >= 1000000.0) {
        lcd.print("R: "); lcd.print(Rx / 1000000.0, 2); lcd.print(" MOhm");
      } else {
        lcd.print("R: "); lcd.print(Rx / 1000.0, 2); lcd.print(" kOhm");
      }
      delay(2000);
      return;
    }

    // --- CAPACITOR TEST ---
    discharge();
    pinMode(adcPin[pB], OUTPUT); digitalWrite(adcPin[pB], LOW);
    pinMode(highPin[pA], OUTPUT); digitalWrite(highPin[pA], HIGH);
    
    // nF Range (via 470k)
    unsigned long tStart = micros();
    while (analogRead(adcPin[pA]) < 647) {
      if (micros() - tStart > 800000) break; 
    }
    unsigned long duration = micros() - tStart;
    discharge();

    if (duration > 100 && duration < 800000) {
      float nF = ((float)duration / 470000.0) * 1000.0;
      lcd.clear();
      lcd.print("Capacitor("); lcd.print(pA+1); lcd.print("-"); lcd.print(pB+1); lcd.print(")");
      lcd.setCursor(0, 1);
      if (nF >= 1000.0) {
        lcd.print("C: "); lcd.print(nF / 1000.0, 2); lcd.print(" uF");
      } else {
        lcd.print("C: "); lcd.print(nF, 1); lcd.print(" nF");
      }
      delay(2000);
      return;
    }

    // uF Range (via 680R)
    discharge();
    pinMode(adcPin[pB], OUTPUT); digitalWrite(adcPin[pB], LOW);
    pinMode(lowPin[pA], OUTPUT); digitalWrite(lowPin[pA], HIGH);
    
    tStart = micros();
    while (analogRead(adcPin[pA]) < 647) {
      if (micros() - tStart > 1500000) break; 
    }
    duration = micros() - tStart;
    discharge();

    if (duration > 50 && duration < 1500000) {
      float uF = (float)duration / 700.0;
      lcd.clear();
      lcd.print("Capacitor("); lcd.print(pA+1); lcd.print("-"); lcd.print(pB+1); lcd.print(")");
      lcd.setCursor(0, 1);
      lcd.print("C: "); lcd.print(uF, 2); lcd.print(" uF");
      delay(2000);
      return;
    }
  }

  // =============================================================
  // IDLE OR UNKNOWN COMPONENT
  // =============================================================
  lcd.clear();
  lcd.print("No Component");
  delay(400);
}