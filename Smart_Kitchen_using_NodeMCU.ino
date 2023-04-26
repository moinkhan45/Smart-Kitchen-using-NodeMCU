#define BLYNK_TEMPLATE_ID "TMPLSmSEfHQj"
#define BLYNK_DEVICE_NAME "Smart Kitchen"
#define BLYNK_AUTH_TOKEN "WR0IeHXLMFDix-vU0WURUzrAKuEaIAfo"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

 
char auth[] = BLYNK_AUTH_TOKEN;    // You should get Auth Token in the Blynk App.
char ssid[] = "OnePlus 10R 5G";                           // Your WiFi credentials.
char pass[] = "One@123";
 
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define DHTTYPE DHT11     // DHT 11
#define DHTPIN D3
 
int relay_fan = D5;
#define relay_light D6
#define relay_fridge D7
#define relay_oven D8
 
#define buzzer_alarm D0
#define pir_human D4
 
 
int alarm_status;
int pir_status = 0;
 
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 SimpleTimer timer;
 
int gas_sensor = A0; // smoke sensor is connected with the analog pin A0 
int output_value; 
 
void setup()
{
  Serial.begin(115200);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  Blynk.begin(auth, ssid, pass);


 
  pinMode(pir_human, INPUT);
  pinMode(buzzer_alarm, OUTPUT);
 pinMode(relay_fan, OUTPUT);
  pinMode(relay_light, OUTPUT);
  pinMode(relay_fridge, OUTPUT);
  pinMode(relay_oven, OUTPUT);
 
  digitalWrite(buzzer_alarm, LOW);
 
  digitalWrite(relay_fan,LOW);
  digitalWrite(relay_light, HIGH);
  digitalWrite(relay_fridge, HIGH);
  digitalWrite(relay_oven, HIGH);
  delay(100);
}
 
 
void loop()
{
  Blynk.run();
  timer.run();
  output_value= analogRead(gas_sensor);
  //output_value = map(output_value,1023,10,0,100);
  //MQ135 gasSensor = MQ135(A0);
  //float air_quality = gasSensor.getPPM();
 
  float t = dht.readTemperature();
  float h = dht.readHumidity();
 
  pir_status = digitalRead(pir_human);
  alarm_status = digitalRead(buzzer_alarm);
if (pir_status == 1)
  {
    Serial.println("Person Detected");
  }
  else if (pir_status == 0)
  {
    Serial.println("No One in Room");
  }
 
 
  if (output_value > 500)
  {
    digitalWrite(buzzer_alarm, HIGH);
    digitalWrite(relay_fan,LOW );
    Serial.println("Buzzer Status: ON");
    Serial.println("Exhaust Fan: ON");
  }
  else
  {
    digitalWrite(buzzer_alarm, LOW);
    digitalWrite(relay_fan, HIGH);
    Serial.println("Buzzer Status: OFF");
    Serial.println("Exhaust Fan: OFF");
  }
 
  Serial.print("Air Quality: ");
Serial.print(output_value);
  Serial.println(" PPM");
 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
 
  Serial.println();
  Serial.println("**********");
  Serial.println();
 
  Blynk.virtualWrite(V1, t);             // For Temperature
  Blynk.virtualWrite(V2, h);             // For Humidity
  Blynk.virtualWrite(V3, output_value);   // For Gas
  Blynk.virtualWrite(V4, alarm_status);  // For Alarm & Exhaust Fan
  Blynk.virtualWrite(V5, pir_status);    // For Human Detection
 
  display.clearDisplay();
  display.setCursor(0, 0); //oled display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Air Quality Index");
  display.setCursor(0, 20); //oled display
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(output_value);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" PPM");
  display.display();
  delay(1500);
 
  display.clearDisplay();
 
  //display temperature
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  //display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %");
 
  display.display();
  delay(1500);
}
