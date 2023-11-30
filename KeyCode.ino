// LCD Keypad Shield

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Array.h>
#include <Vector.h>


using namespace std; 

// DS1302:  CE pin    -> Arduino Digital 2
//          I/O pin   -> Arduino Digital 3
//          SCLK pin  -> Arduino Digital 4

// Variables pour boutons du LCD
int lcd_key     = 0;
int adc_key_in  = 0;

// Constantes
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Création de l'objet lcd (avec les différents ports numériques qu'il utilise)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
String passWord = "";
String passGuess = "";
// char passWord[];
// char passGuess[];
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns
int keyCount = 0;
bool falseCounter = false;
int cursor_x;


char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {35, 37, 39, 41}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {24, 26, 28, 30}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Setup for 16x2 LCD screen
  choosePassword();
  init_lcd(0);

}

void loop() {
  while (passGuess.length()<4) {  
    char key = keypad.getKey();
    if (key){
    Serial.println(key);
    lcd.print(key);
    passGuess += key; 
  }
  }

  if (passWord == passGuess) {
    init_lcd(2);
  }
  else {
    init_lcd(1);
    lcd_key = read_LCD_buttons(); 
    switch(lcd_key) {
      case btnNONE : {break;}
      case btnLEFT : {Serial.println("LEFT ");cursor_x -= 1;break;}
      case btnRIGHT : {Serial.println("RIGHT ");cursor_x +=1;break;}
      case btnSELECT : {
        Serial.println(" SELECT KEY PRESSED :");
        if (cursor_x == 8) {
          init_lcd(3);
        }
        else if (cursor_x == 6) {
          init_lcd(0);
        }
      }
    }  
  }
  delay(250);
}

void init_lcd (int restart_code) {
  lcd.clear();
  switch (restart_code) {

    case 0 : // Reset
      passGuess = "";
      cursor_x=8;
      lcd.setCursor(2,0);
      lcd.print("Enter Code : ");
      lcd.setCursor(6,1);
      lcd.blink();
      break;
    
    case 1 : // False Answer 
      lcd.setCursor(0,0);
      lcd.print("Wrong, Again ?");
      lcd.setCursor(6,1);
      lcd.print("Y N");
      lcd.setCursor(cursor_x,1);
      lcd.blink();
      break;


    case 2 : // Right answer 
      lcd.setCursor(0,0);
      lcd.print("Password Correct");
      delay(2000);
      init_lcd(3);
      break;

    case 3 : // Thanks for trying 
      lcd.setCursor(0,0);
      lcd.print("Thanks for");
      lcd.setCursor(4,1);
      lcd.print("Trying");
      break;
  }

}

void choosePassword() {
  passWord="";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password ?");
  lcd.setCursor(6,1);
  lcd.blink();
  while (passWord.length()<4){
  // while (strlen(passWord)<3) {
    char key = keypad.getKey();
    if (key){
      Serial.println(passWord);
      lcd.print(key);
      passWord += key;
      // strcat(passWord, key);
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Password Set");
  delay(2000);
  Serial.println("Password Set to : "+passWord);
}

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);   // Lecture du port analogique

 // Les valeurs qui suivent doivent être adaptées au shield
 if (adc_key_in > 790) return btnNONE;   // En principe 1023 quand aucune touche n'est pressée
 if (adc_key_in < 50)   return btnRIGHT;     // 0
 if (adc_key_in < 195)  return btnUP;        // 99
 if (adc_key_in < 380)  return btnDOWN;      // 255
 if (adc_key_in < 555)  return btnLEFT;      // 409
 if (adc_key_in < 790)  return btnSELECT;    // 640

 return btnNONE;
}