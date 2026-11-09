# DIY Universal Component Tester (Arduino)

Developed by **YUSELLAB**

An automatic component tester built with Arduino Uno/Nano and a 16x2 I2C LCD display. It automatically identifies pinouts and measures values for resistors, capacitors, diodes, LEDs, BJTs, and MOSFETs across three test points (TP1, TP2, TP3).

## 🚀 Features
- **Auto-Detection:** Matrix scans pin combinations across TP1, TP2, and TP3.
- **Resistor Measurement:** Low & High range reading with differential ADC calibration (~1 Ohm to 2 MOhm).
- **Capacitor Measurement:** nF and uF ranges using RC time constant.
- **Diodes & LEDs:** Forward voltage ($V_f$) and polarity (Anode/Cathode) detection *(Supported on TP1 & TP2)*.
- **Transistors & MOSFETs:** Identifies NPN/PNP BJTs (Base, Collector, Emitter) and N-Ch/P-Ch MOSFETs (Gate, Drain, Source).

## 🔌 Wiring Diagram
### I2C LCD Display
- **VCC** -> 5V
- **GND** -> GND
- **SDA** -> A4
- **SCL** -> A5

### Test Pins (TP1, TP2, TP3)
- **TP1:** Arduino A0 | D8 via 680 Ohm | D9 via 470k Ohm
- **TP2:** Arduino A1 | D10 via 680 Ohm | D11 via 470k Ohm
- **TP3:** Arduino A2 | D12 via 680 Ohm | D13 via 470k Ohm

## ⚠️ Important Usage Notes
- **Diodes & LEDs:** Insert component into **Pin 1 (TP1)** and **Pin 2 (TP2)** for accurate detection and forward voltage measurement.
- **Resistors & Capacitors:** Can be connected across any pair (1-2, 2-3, 1-3).
- **Transistors & MOSFETs:** Connect using all three pins (1-2-3).
