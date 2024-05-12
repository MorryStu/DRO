#define clk 23
#define dat 4
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <SPI.h>
#include <AS5X47.h>
  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8
#define ST77XX_BLACK   0x0000
#define ST77XX_GRAY    0x8410
#define ST77XX_WHITE   0xFFFF
#define ST77XX_RED     0xF800
#define ST77XX_ORANGE  0xFA60
#define ST77XX_YELLOW  0xFFE0  
#define ST77XX_DENIM   0x03EE
#define ST77XX_GREEN   0x07E0
#define ST77XX_DARKGREEN   0x2ce1
#define ST77XX_GREEN   0x07E0
#define ST77XX_CYAN    0x07FF
#define ST77XX_AQUA    0x04FF
#define ST77XX_BLUE    0x001F
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_PINK    0xF8FF 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
boolean data[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //24 bit boolean array for storing the received data
boolean mm_in = 0; //0 for mm, 1 for inch
int received_bit_location = 0;
float measured_value = 0.0;
int slaveSelectPin = 7;// Define where the CSN Pin in connected. 
AS5X47 as5047p(slaveSelectPin);// Start connection to the sensor.

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
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setCursor(80,100 );
  tft.setTextSize(4);
  tft.print("T.I.S.M");
  delay(2000); 
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.fillRect(0, 0, 320, 46, ST77XX_YELLOW);
  tft.fillRect(0, 50, 320, 46, ST77XX_RED);
  tft.fillRect(0, 98,320, 46, ST77XX_CYAN);
  tft.fillRect(0, 147, 320, 46, ST77XX_DENIM);
  tft.fillRect(0, 195, 320,46, ST77XX_WHITE);
  
 }
void loop() {
if (mm_in == 1){ // if it's in the inch mode
Serial.print(measured_value,4); Serial.println(" in");
} else {
Serial.print(measured_value,2); Serial.println(" mm"); 
}
float angle = as5047p.readAngle();
 tft.invertDisplay(false);
  delay(1);
  tft.setFont();
  tft.setCursor(20, 10);
  tft.setTextColor(ST77XX_RED, ST77XX_YELLOW);
  tft.setTextSize(3);
  tft.print(measured_value,3);
  tft.setCursor(140, 20);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextSize(1);
  tft.print(" Depth mm");
  tft.setFont();
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_YELLOW, ST77XX_RED);
  tft.setCursor(20, 60);
  tft.print(measured_value*2,3);
  tft.setCursor(140, 70);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextSize(1);
  tft.print(" Diam mm");
  tft.setTextColor(ST77XX_DARKGREEN, ST77XX_CYAN);
  tft.setFont();
  tft.setCursor(20, 110);
  tft.setTextSize(3);
  tft.print(measured_value/25.400,3);
  tft.setCursor(140, 120);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextSize(1);
  tft.print(" Depth Inch");
  tft.setTextColor(ST77XX_GREEN, ST77XX_DENIM);
  tft.setTextSize(1);
  tft.setFont();
  tft.setTextSize(3);
  tft.setCursor(20, 156);
  tft.print(measured_value*2/25.400,3);
  tft.setCursor(140, 165);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextSize(1);
  tft.print(" Diam Inch");
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.setCursor(150, 220);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextSize(1);
  tft.print("Comp Angle");
  tft.setFont();
tft.setTextSize(3);
tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
tft.setCursor(20,200);
tft.print(angle);


  
  
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
