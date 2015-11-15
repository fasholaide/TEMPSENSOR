#include <LiquidCrystal.h>
#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>

// Set pins.
#define LCD_LIGHT_PIN A4 // The number of the pin where anode of the display backlight is.
#define TEMP_PIN A0 // The number of the pin for reading values for temperature
#define LED_PIN 13 // The number of the pin for powering the LED light when temperature is high
#define ARRAY_SIZE 15
#define COMMAND_TEXT 0xF
#define TARGET_DEFAULT 0xF


// Initialize the LiquidCrystal library with the numbers of the interface pins.
// LiquidCrystal(rs, e, d4, d5, d6, d7)
LiquidCrystal lcd(8, 9, 5, 6, 7, 3);
//Initialize the AndroidAccessory Library
AndroidAccessory acc("Manufacturer", "Model", "Description", "Version", "URI", "Serial");
char tempMessage[ARRAY_SIZE - 5] = {'M', 'E', 'L', 'I', 'A', 'N', 'D', 'O', 'U', ':'};
byte rcvmsg[255];
byte sntmsg[3 + ARRAY_SIZE];



void setup(){
  Serial.begin(115200);
  //Power on the acc
  acc.powerOn();

  // Set the Temperature pin as an input.
  pinMode(TEMP_PIN, INPUT);
  // Set the LED light pin as an output.
  pinMode(LED_PIN, OUTPUT);
  // Set the push-button pin as an input.
  pinMode(BUTTON_PIN, INPUT);
  // Set the lcd display backlight anode pin as an output.
  pinMode(LCD_LIGHT_PIN, OUTPUT);
  // Set the lcd display backlight anode pin to low - lcd light off.
  digitalWrite(LCD_LIGHT_PIN, LOW);
  // Set the lcd number of columns and rows.
  lcd.begin(30, 2);
  // Print the message to the lcd.
  lcd.setCursor(0, 0); // First row.
  lcd.print("Temp in Celcius: ");
  lcd.display();
}

//Function to calculate Temperature from the values gotten form the pin
float getTempC(){
  float raw = analogRead(TEMP_PIN);
  float percent = raw/ 1023.0;
  float volts = percent * 5.0;
  return volts * 100.0;
}

void lightLCDWithTemp(float temperature){ 
 digitalWrite(LCD_LIGHT_PIN, HIGH);
 lcd.setCursor(3, 1); // Second row.
 lcd.print(temperature);
 lcd.display();
}

void printToSerial(float temperature){
  Serial.print("LOG:The temperature is ");
  Serial.print(temperature);
  Serial.print("\n");
}

void loop(){
  float temperature = getTempC();
  lightLCDWithTemp(temperature);
  if(temperature > 25){
    digitalWrite(LED_PIN, HIGH);
    if(acc.isConnected()){
      char array[5];
      dtostrf(temperature, 4, 2, array);
      sntmsg[0] = COMMAND_TEXT;
      sntmsg[1] = TARGET_DEFAULT;
      sntmsg[2] = ARRAY_SIZE;
      int x = ARRAY_SIZE - 5;
      
      for(int i = 0; i < x; i++){
        sntmsg[i + 3] = tempMessage[i];
      }
     
      sntmsg[x + 3] = array[0];
      sntmsg[x + 4] = array[1];
      sntmsg[x + 5] = array[2];
      sntmsg[x + 6] = array[3];
      sntmsg[x + 7] = array[4];
  
      acc.write(sntmsg, 3 + ARRAY_SIZE);
      lightLCDWithTemp(temperature);
      printToSerial(temperature);
      //Serial.println("Hello Android I can send to you now");
      delay(250);
    }
  }
  else {
    digitalWrite(LED_PIN, HIGH);
  }
}
