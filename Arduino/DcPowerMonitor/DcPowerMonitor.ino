/*
DC Power Monitor
Brett Deaton - 2022

Print to serial port the instantaneous power and cumulative energy
transferred through a DC circuit (of scale ~10 V, ~1 A).

The external wiring is a small shunt resistor (r_1 ~ 1 ohm) in
series with the source  (S) and load (L) and a large voltage
divider (having resistance r_2a + r_2b ~ 100 kohm, r_2a < r_2b)
in parallel with the source, as shown in the diagram below.

  +--------+----------------+
  |        /                |
  |        \ r_2b           |
  |        /                |
 (S)       |               (L)
  |        /                |
  |        \ r_2a           |
  |        /          r_1   |
  +--------+----------\/\---+
  
To compute power, the arduino combines two voltage measurements:
across the shunt resistor (v_1 < 5 V) and across the smaller of
the two resistors in the voltage divider parallel to the
source (v_2a < 5 V).

Connect test leads to appropriate analog pin (1 or 2), and
connect negative leads to grd.
*/

const int PIN_V1 = A1;
const int PIN_V2A = A2;
const float V_REF = 5.0; // reference voltage (arduino uno default)
const int N_RES = pow(2,10)-1; // resolution of analogRead()
const float R_1 = 1.1;
const float R_2A = 1e4;
const float R_2B = 1e5;
const float V_2_SCALE = (R_2A+R_2B)/R_2A; // v_2 = v_2a * v_2_scale
const float TIME_SCALE = 0.0000167; // time_min = time_ms * time_scale

float v_1 = 0;
float v_2a = 0;
float v_2 = 0;
float power = 0;
float energy = 0;
unsigned int time_now = 0;
unsigned int time_prev = 0;
unsigned int time_delta = 0;

void setup() {
  Serial.begin(9600);
  //Serial.println("V_2_SCALE: " + String(V_2_SCALE));
  Serial.println("power-W" + String(" energy-Wmin"));
}

void loop() {
  time_prev = time_now;
  time_now = millis();
  v_1 = analogRead(PIN_V1)*V_REF/N_RES;
  v_2a = analogRead(PIN_V2A)*V_REF/N_RES;
  v_2 = v_2a*V_2_SCALE;
  power = v_2 * v_1 / R_1;
  time_delta = time_now - time_prev;
  energy += power * time_delta * TIME_SCALE;
  Serial.println(power + String(" ") + String(energy));
  delay(1000);
}
