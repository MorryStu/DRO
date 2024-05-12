#define clk 2
#define dat 4
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <SPI.h>
  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
boolean data[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //24 bit boolean array for storing the received data
boolean mm_in = 0; //0 for mm, 1 for inch
int received_bit_location = 0;
float measured_value = 0.0;

void process_data();
long last_received = 0;

void setup() {
pinMode(clk,INPUT);
pinMode(dat,INPUT);
Serial.begin(9600);
attachInterrupt(digitalPinToInterrupt(clk), clk_ISR, FALLING);
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
  tft.init(240, 320);           // Init ST7789 240x320  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 240x320 TFT etc:
  Serial.println(F("Initialized"));
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setCursor(50,160 );
  tft.print("T.I.S.M");
  delay(1000); 
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
 }
void loop() {
if (mm_in == 1){ // if it's in the inch mode
Serial.print(measured_value,4); Serial.println(" in");
} else {
Serial.print(measured_value,2); Serial.println(" mm"); 
}
 tft.invertDisplay(false);
  delay(1);
  //tft.invertDisplay(false);
  //delay(500);
   tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);//, ST77XX_BLACK); 
  tft.setTextSize(1);
  tft.println(measured_value,2);
delay(100);
}

void process_data(){
measured_value = 0.0;
if(data[23] == 0){ //if it's in the mm mode
  mm_in = 0;
  //converting binary to decimal value
  for (int i = 0; i<=15; i++){
  measured_value += data[i]*pow(2,i)/100.0;
  }
    if (data[20] == 1){
    measured_value = measured_value * -1; //add the negative sign if it exists
    }
}

if(data[23] == 1){ //if it's in the inch mode
  mm_in = 1;
  //converting binary to decimal value
  for (int i = 1; i<=16; i++){
  measured_value += data[i]*pow(2,(i-1))/1000.0;
  }
    if (data[0]  == 1){
    measured_value += 0.0005; //add the 0.5 mil sign if it exists  
    }
    if (data[20] == 1){
    measured_value = measured_value * -1; //add the negative sign if it exists
    }
}
}

void clk_ISR(){
  if ( millis() - last_received > 2 ){
    received_bit_location = 0;
  }
  
  if (received_bit_location <= 23){
    data[received_bit_location] = !digitalRead(dat); //inverting the received data due to the hardware circuit level shifting
    received_bit_location++;
  } 
  
  if (received_bit_location == 24){
    received_bit_location = 0;
    process_data();
  }
  
  last_received = millis();
  //Serial.println(received_bit_location);
}
