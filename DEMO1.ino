#include <LiquidCrystal_I2C.h>
#include <Keypad.h>  

#define GAS_VALUE_MAX 400
#define GAS_VALUE_MIN 0

int flamesensor = A1;  
int gassensor = A0;
int led = 13;
int buzzer = 12;

String key_input;//Store the char input from keypad
uint THRESHOLD = GAS_VALUE_MAX;//Value default 
//uint dust_sensor_read;//Read dust density from sensor GP2Y101AU0F
bool state = true;

 const byte ROWS = 4; /* four rows */  
 const byte COLS = 4; /* four columns */  
 char hexaKeys[ROWS][COLS] = {  
  {'1','2','3','A'},  
  {'4','5','6','B'},  
  {'7','8','9','C'},  
  {'*','0','#','D'}  
  };  
 char keypressed;                 //Where the keys are stored it changes very often
 byte rowPins[ROWS] = {9,8,7,6}; /* connect to the row pinouts of the keypad */  
 byte colPins[COLS] = {5,4,3,2}; /* connect to the column pinouts of the keypad */  
 Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);   
 LiquidCrystal_I2C lcd(0x27,20,2);

void setup() {
  Serial.begin(115200);
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(4 ,0);
  lcd.print("Xin Chao");  
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  delay(5000); 
  lcd.clear();
  lcd.print("MACH BAO CHAY!!!"); 
}

void loop() {
  int FlameValue = analogRead(flamesensor);
  int GasValue = analogRead(gassensor);
    Serial.print("Gas detect  :");
    Serial.println((int)GasValue);
    Serial.print("Flame detect:");
    Serial.println((int)FlameValue);
    delay(1000); 
   if(FlameValue<300){
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Phat hien lua");
      lcd.setCursor(2,1);
      lcd.print("HAY KIEM TRA!");
      digitalWrite(led, HIGH);
      tone_buzzer (); 
      delay(5000);
      }
  
  if(GasValue>THRESHOLD){
      lcd.clear();
      lcd.setCursor(1 ,0);
      lcd.print("Phat hien khoi");
      lcd.setCursor(2,1);
      lcd.print("HAY KIEM TRA!");
      digitalWrite(led, HIGH);
      tone_buzzer (); 
      delay(5000);
      }  

   char key = customKeypad.getKey(); // Ký tự nhập vào sẽ gán cho biến Key
   
   switch(key){
   case '#':
   //lcd.clear();
   //lcd.setCursor(2,0);
   //lcd.print("Change Value");
   //lcd.setCursor(0,1);
   //lcd.print("GasSensor: ");
   ChangeValue();
   delay(5000);
   }
   
   switch(key){
   case '*': 
   sensor_value();
   delay(5000);
   }
    
   switch(key){
   case 'B': 
   lcd.clear();
   lcd.print("MACH BAO CHAY!!!");
   }

   switch(key){
   case 'C':
   lcd.clear();
   lcd.print("Da Tat Bao Dong");
   digitalWrite(led, LOW);   
   delay(5000);
   sensor_value();
   }
   
   switch(key){
   case 'D': 
   lcd.clear();
   lcd.setCursor(1,0);
   lcd.print("Test Canh Bao!");
   digitalWrite(led, HIGH);
   tone_buzzer ();
   delay(5000);
   digitalWrite(led, LOW);
   lcd.clear();
   lcd.print("MACH BAO CHAY!!!");  
  }
}

void sensor_value() {
    // read the value of the Flame Sensor:
  int FlameValue = analogRead(flamesensor);
  int GasValue = analogRead(gassensor);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Flame detect:");
    lcd.print((int)FlameValue);
    lcd.setCursor(0,1);
    lcd.print("Gas detect  :");
    lcd.print((int)GasValue);

}

void ChangeValue(){
  unsigned char i=10;
  lcd.clear();
  NOTE:
    lcd.setCursor(0,0);//Col 0 Row 0
    lcd.print("Change Value");
    lcd.setCursor(0,1);//Col 0 Row 1
    lcd.print("GasSensor:");
  while(1){
   // char key = KEYPAD_SCAN();
    char key = customKeypad.getKey();
    if(key!=0){
      switch(key){
        case '1':        case '2':        case '3':        case '4':        case '5':        
        case '6':        case '7':        case '8':        case '9':        case '0':
          lcd.setCursor(0,1);//Col 0 Row 1
          lcd.print("GasSensor:");
          lcd.setCursor(i,1);//Col i Row 1
          lcd.print(key);
          key_input+=key;
          i++;
          break;
        case 'A'://Confirm Threshold
          if(key_input.length() > 0 && key_input.length() <4){
              THRESHOLD = key_input.toInt();//Convert String to Int, New THRESHOLD   
              if( THRESHOLD <=700){
                key_input ="";//Clear key_input  
                state = true;
                lcd.clear();
                lcd.setCursor(0,0);//Col 0 Row 0
                lcd.print("Success");
                lcd.setCursor(0,1);//Col 0 Row 0
                lcd.print("Press B to back"); 
              } 
             /* else{
                lcd.clear();
                lcd.setCursor(0,0);//Col 0 Row 0
                lcd.print("Wrong"); 
                key_input ="";//Clear key_input
                lcd.setCursor(0,1);//Col 0 Row 1
                lcd.print("Input again");
                delay(1000);
                goto NOTE;  
              } */
          }
         /* else{
              lcd.clear();
              lcd.setCursor(0,0);//Col 0 Row 0
              lcd.print("Wrong"); 
              key_input ="";//Clear key_input
              lcd.setCursor(0,1);//Col 0 Row 1
              lcd.print("Input again");
              delay(1000);
              goto NOTE;
          } */
          break;
        case 'B'://Black to LCD Display value Sensor
          lcd.clear();
          lcd.print("MACH BAO CHAY!!!");
          digitalWrite(led, LOW);   
          goto EXIT_ADJUST;
          break;
      }
    } 
  }
  EXIT_ADJUST:;
}

void tone_buzzer() {
for(int i=600; i<1200; i++){
  delay(5);
  tone(buzzer, i);
  }
  for(int i=1200; i<600; i--){
  delay(5);
  tone(buzzer, i);
  }
}
