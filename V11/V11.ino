#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include "Arduino.h"
#include <SoftReset.h>
#include <EEPROM.h>

#define PIN_BUZZER        7
#define BACKLIGHT_PIN     13

#define SERIESRESISTOR 1000   
#define THERMISTORPIN A0 
 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'.','0','E','D'}
};

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 4096;

const int BUFSIZE = 30;
char buf[BUFSIZE];
String myString; 
char myStringChar[BUFSIZE];

byte colPins[COLS] = {28,26,24,22}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {36,34,32,30}; //connect to the row pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 RTC;

String strGender="";
String strAge="";
String strHeight="";
String strWeight="";
String strWeist="";

String strDate="";
String v_date;

char *strWeistTmp="";
    
int menu_id=0;
int menu_b=0;

int v_gender=0;
int v_age=0;
float v_Height=0;
float v_Weight=0;
float v_Weist=0;
  
void setup()
{
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, HIGH );

  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  //lcd.begin(20,4); 
  lcd.begin(); 

  lcd.clear();
  lcd.setCursor(0,0); lcd.print(" Wearable Body Fat");
  lcd.setCursor(0,1); lcd.print("     Calculator");
  lcd.setCursor(0,2); lcd.print("*** Developed By ***");  
  lcd.setCursor(0,3); lcd.print("Dinesh Kaushal- PIBT");  
  delay(5000);
  
  RTC.adjust(DateTime(__DATE__, __TIME__));
  
  strGender="";
  strAge="";
  strHeight="";
  strWeight="";
  strWeist="";
  
  menu_id=0;
  v_age=0;
  v_Height=0;
  v_Weight=0;
  v_Weist=0;

  lcd.clear();
  lcd.setCursor(0,0); lcd.print("A. Start Process");
  lcd.setCursor(0,1); lcd.print("B. Check Status");  
  lcd.setCursor(0,2); lcd.print("C. Clear Data");  
  lcd.setCursor(0,3); lcd.print("D. Re-start System");    
}

void loop()
{
  
    DateTime now = RTC.now();
    
    v_date=String(now.year(), DEC);
    v_date=v_date + "-" + String(now.month(), DEC);
    v_date=v_date + "-" + String(now.day(), DEC);
    
    //Serial.println("Date:" + v_date);
    
  char customKey = customKeypad.getKey();
  if (customKey != NO_KEY)
  {
    Serial.println(customKey);
    
    if(customKey=='A')
    {
      menu_id++;
      beep(1000);
  
      if(menu_id==1)
      {
        strGender="";
        
        Serial.println("Gender");
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Enter Details:");
        lcd.setCursor(0,1); lcd.print("--------------------");
        lcd.setCursor(0,2); lcd.print("Enter Gender:");
        lcd.setCursor(13,2); lcd.blink();
      }
      else if(menu_id==2)
      {
        strAge="";
        
        Serial.println("Age");
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Enter Details:");
        lcd.setCursor(0,1); lcd.print("--------------------");
        lcd.setCursor(0,2); lcd.print("Enter Age   :");
        lcd.setCursor(13,2); lcd.blink();
      }    
     else if(menu_id==3)
      {
        strHeight="";
        
        Serial.println("Height");
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Enter Details:");
        lcd.setCursor(0,1); lcd.print("--------------------");
        lcd.setCursor(0,2); lcd.print("Enter Height:");
        lcd.setCursor(13,2); lcd.blink();
      }    
     else if(menu_id==4)
      {
        strWeight="";
        
        Serial.println("Weight");
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Enter Details:");
        lcd.setCursor(0,1); lcd.print("--------------------");
        lcd.setCursor(0,2); lcd.print("Enter Weight:");
        lcd.setCursor(13,2); lcd.blink();
      }
     else if(menu_id==5)
      { 
        beep(1000);
        
        strWeist="";
        v_Weist=0;
        
        Serial.println("Weist");
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Calliberating");
        lcd.setCursor(0,1); lcd.print("Weist Size");        
        lcd.setCursor(0,2); lcd.print("--------------------");
        lcd.setCursor(0,3); lcd.print("Please wait .....");
        lcd.setCursor(13,2); lcd.noBlink();
        delay(3000);  
        
        v_Weist = analogRead(THERMISTORPIN);

        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Calliberating");
        lcd.setCursor(0,1); lcd.print("Weist Size");        
        lcd.setCursor(0,2); lcd.print("--------------------");
        lcd.setCursor(0,3); lcd.print("Completed .....");
        lcd.setCursor(13,2); lcd.noBlink();
        delay(2000);

        v_Weist = round(((SERIESRESISTOR / v_Weist)*25)+2);
        strWeist=String(v_Weist);
        //dtostrf(v_Weist, 5, 2, strWeist);
           
        beep(1000);
        menu_id=0;
        
        lcd.clear();      
        lcd.noBlink();
        
        Serial.println(strGender);
        
        if(strGender=="1")
        {
          lcd.setCursor(0,0); lcd.print("Gen:Male");
          lcd.setCursor(12,0); lcd.print("Age:" + strAge);
          lcd.setCursor(0,1); lcd.print("Hgh:" + String(strHeight) + "ft");
          lcd.setCursor(12,1); lcd.print("Wgh:" + String(strWeight) + "kg");
          lcd.setCursor(0,2); lcd.print("Wst:" + String(strWeist) + "in"); 
                   
          delay(1000);
                    
          String v_data= v_date + "," + "MALE" + "," + strAge + "," + strHeight + "," + strWeight + "," + strWeist;
          Serial.println("Data to write:" + v_data);
          
          v_data.toCharArray(myStringChar, BUFSIZE);
          strcpy(buf, myStringChar);
          eeprom_write_string(30, buf); 
                    
          lcd.setCursor(0,3); lcd.print("<< record: stored >>");
        
        }
        else if(strGender=="0")
        {
          lcd.setCursor(0,0); lcd.print("Gen:Female");
          lcd.setCursor(12,0); lcd.print("Age:" + strAge);
          lcd.setCursor(0,1); lcd.print("Hgh:" + String(strHeight) + "ft");
          lcd.setCursor(12,1); lcd.print("Wgh:" + String(strWeight) + "kg");
          lcd.setCursor(0,2); lcd.print("Wst:" + String(strWeist) + "in"); 
                   
          delay(1000);
                    
          String v_data= v_date + "," + "FEMALE" + "," + strAge + "," + strHeight + "," + strWeight + "," + strWeist;
          Serial.println("Data to write:" + v_data);
          
          v_data.toCharArray(myStringChar, BUFSIZE);
          strcpy(buf, myStringChar);
          eeprom_write_string(30, buf); 
                    
          lcd.setCursor(0,3); lcd.print("<< record: stored >>");
        }
        else
        {        
          lcd.clear();
          lcd.setCursor(0,1); lcd.print("Invalid Gender Entry");
          lcd.setCursor(0,2); lcd.print(" **Return to main**");
        }
      }
    }
    else if(customKey=='B')
    {
       menu_b++;

       if(menu_b==1)
       {
           lcd.clear();
           lcd.setCursor(0,0); lcd.print("********************");
           lcd.setCursor(0,1); lcd.print("    Health Record");
           lcd.setCursor(0,2); lcd.print("    Calculation");
           lcd.setCursor(0,3); lcd.print("********************");
       } 
       
       if(menu_b==2)
       {
         eeprom_read_string(30, buf, BUFSIZE);
         Serial.println("Read 30 address:"); Serial.println(buf); 
         
         String v_reply=buf;
         
         int index1 = v_reply.indexOf(',',0);
         int index2 = v_reply.indexOf(',', index1+1);
         int index3 = v_reply.indexOf(',', index2+1);
         int index4 = v_reply.indexOf(',', index3+1);
         int index5 = v_reply.indexOf(',', index4+1);
                
         strDate=v_reply.substring(0, index1);
         strGender=v_reply.substring(index1+1, index2);
         strAge=v_reply.substring(index2+1, index3);
         strHeight=v_reply.substring(index3+1, index4);
         strWeight=v_reply.substring(index4+1, index5);    
         strWeist=v_reply.substring(index5+1);
  
          //Age Conversion
          v_age= strAge.toInt();
          
          //Gender Conversion
          //v_gender=strGender.toInt();
          
          //Height conversion from String to Float
          char char_Height[strHeight.length() + 1];
          strHeight.toCharArray(char_Height, strHeight.length() + 1);
          v_Height=atof(char_Height);
          
          //Weight conversion from String to Float
          char char_Weight[strWeight.length() + 1];
          strWeight.toCharArray(char_Weight, strWeight.length() + 1);
          v_Weight=atof(char_Weight);
          
          //Waist conversion from String to Float
           strWeist.toCharArray(strWeistTmp, 5);
           dtostrf(v_Weist, 5, 2, strWeistTmp);
         
           lcd.clear();
           lcd.setCursor(0,0); lcd.print("Date:" + strDate);
           lcd.setCursor(0,1); lcd.print("Gen:" + strGender);
           lcd.setCursor(12,1); lcd.print("Age:" + strAge);
           lcd.setCursor(0,2); lcd.print("Hgh:" + String(strHeight) + "ft");
           lcd.setCursor(12,2); lcd.print("Wgh:" + String(strWeight) + "kg");
           lcd.setCursor(0,3); lcd.print("Wst:" + String(strWeist) + "in"); 
       }
       else if(menu_b==3)
       { 
         eeprom_read_string(30, buf, BUFSIZE);
         Serial.println("Read 30 address:"); Serial.println(buf); 
         
         String v_reply=buf;
         
         int index1 = v_reply.indexOf(',',0);
         int index2 = v_reply.indexOf(',', index1+1);
         int index3 = v_reply.indexOf(',', index2+1);
         int index4 = v_reply.indexOf(',', index3+1);
         int index5 = v_reply.indexOf(',', index4+1);
                
         strDate=v_reply.substring(0, index1);
         strGender=v_reply.substring(index1+1, index2);
         strAge=v_reply.substring(index2+1, index3);
         strHeight=v_reply.substring(index3+1, index4);
         strWeight=v_reply.substring(index4+1, index5);    
         strWeist=v_reply.substring(index5+1);
  
          //Age Conversion
          v_age= strAge.toInt();
          
          //Gender Conversion
          //v_gender=strGender.toInt();
          
          //Height conversion from String to Float
          char char_Height[strHeight.length() + 1];
          strHeight.toCharArray(char_Height, strHeight.length() + 1);
          v_Height=atof(char_Height);
          
          //Weight conversion from String to Float
          char char_Weight[strWeight.length() + 1];
          strWeight.toCharArray(char_Weight, strWeight.length() + 1);
          v_Weight=atof(char_Weight);
          
          //Waist conversion from String to Float
           strWeist.toCharArray(strWeistTmp, 5);
           dtostrf(v_Weist, 5, 2, strWeistTmp);

          String v_bmi_status;
          float mcf = pow((v_Height * 12)*0.025,2); // metric conversion factor
        
          float bmi= v_Weight/mcf; //body mass index
          Serial.println("BMI:" + String(round(bmi)) + "kg/m2");  
          
          //bmi chart
          if(strGender=="FEMALE")
          {
            if(bmi>31.5)
            {
              v_bmi_status="Ser. Obesty";
            }
            else if(bmi>27.5)
            {
              v_bmi_status="Obesty";
            }
            else
            {
              v_bmi_status="Normal";
            }
          }
          else if(strGender=="MALE")
          {
           if(bmi>33)
            {
              v_bmi_status="Ser. Obesty";
            }
            else if(bmi>28.5)
            {
              v_bmi_status="Obesty";
            }
            else
            {
              v_bmi_status="Normal";
            }
          }
          Serial.println("BMI Status:" + v_bmi_status);
  
          lcd.clear();
          lcd.setCursor(0,0); lcd.print("Health Info - BMI");
          lcd.setCursor(0,1); lcd.print("--------------------");
          lcd.setCursor(0,2); lcd.print("Value:" + String(bmi) + " kg/m2");
          lcd.setCursor(0,3); lcd.print("Status:" + v_bmi_status);
       }
       else if(menu_b==4)
       {  
         eeprom_read_string(30, buf, BUFSIZE);
         Serial.println("Read 30 address:"); Serial.println(buf); 
         
         String v_reply=buf;
         
         int index1 = v_reply.indexOf(',',0);
         int index2 = v_reply.indexOf(',', index1+1);
         int index3 = v_reply.indexOf(',', index2+1);
         int index4 = v_reply.indexOf(',', index3+1);
         int index5 = v_reply.indexOf(',', index4+1);
                
         strDate=v_reply.substring(0, index1);
         strGender=v_reply.substring(index1+1, index2);
         strAge=v_reply.substring(index2+1, index3);
         strHeight=v_reply.substring(index3+1, index4);
         strWeight=v_reply.substring(index4+1, index5);    
         strWeist=v_reply.substring(index5+1);
        
          //Age Conversion
          v_age= strAge.toInt();
          
          //Gender Conversion
          //v_gender=strGender.toInt();
          
          //Height conversion from String to Float
          char char_Height[strHeight.length() + 1];
          strHeight.toCharArray(char_Height, strHeight.length() + 1);
          v_Height=atof(char_Height);
          
          //Weight conversion from String to Float
          char char_Weight[strWeight.length() + 1];
          strWeight.toCharArray(char_Weight, strWeight.length() + 1);
          v_Weight=atof(char_Weight);
          
          //Waist conversion from String to Float
           strWeist.toCharArray(strWeistTmp, 5);
           v_Weist=atof(strWeistTmp);

          float WtHR= ((v_Weist*2.5) / ((v_Height*12)*2.5)*100);
          
          Serial.println("WHtr:" + String(WtHR));
          
          
          String v_whtr_status;
          
          //WHtR chart
          if(strGender=="FEMALE")
          {
            if(WtHR>58)
            {
              v_whtr_status="Ext. Weist";
            }
            else if(WtHR>54)
            {
              v_whtr_status="Ser. Weist";
            }
            else if(WtHR>49)
            {
              v_whtr_status="Over Weist";
            }
            else if(WtHR>42)
            {
              v_whtr_status="Healthy";
            }
            else if(WtHR>35)
            {
              v_whtr_status="Ext. Slim";
            }
            else
            {
              v_whtr_status="Abn. Slim";
            }
          }
          else if(strGender=="MALE")
          {
            if(WtHR>63)
            {
              v_whtr_status="Ext. Weist";
            }
            else if(WtHR>58)
            {
              v_whtr_status="Ser. Weist";
            }
            else if(WtHR>53)
            {
              v_whtr_status="Over Weist";
            }
            else if(WtHR>43)
            {
              v_whtr_status="Healthy";
            }
            else if(WtHR>35)
            {
              v_whtr_status="Ext. Slim";
            }
            else
            {
              v_whtr_status="Abn. Slim";
            }
          }

          Serial.println("Weist to Hight Ratio:" + String(round(WtHR)) + "%         ");          
          Serial.println("Waist Status:" + v_whtr_status);

          lcd.clear();
          lcd.setCursor(0,0); lcd.print("Health Info - WHtR");
          lcd.setCursor(0,1); lcd.print("--------------------");
          lcd.setCursor(0,2); lcd.print("Ratio:" + String(WtHR) + "%");
          lcd.setCursor(0,3); lcd.print("Status:" + v_whtr_status);    
       }
       
       
       else if(menu_b==5)
       {  
         eeprom_read_string(30, buf, BUFSIZE);
         Serial.println("Read 30 address:"); Serial.println(buf); 
         
         String v_reply=buf;
         
         int index1 = v_reply.indexOf(',',0);
         int index2 = v_reply.indexOf(',', index1+1);
         int index3 = v_reply.indexOf(',', index2+1);
         int index4 = v_reply.indexOf(',', index3+1);
         int index5 = v_reply.indexOf(',', index4+1);
                
         strDate=v_reply.substring(0, index1);
         strGender=v_reply.substring(index1+1, index2);
         strAge=v_reply.substring(index2+1, index3);
         strHeight=v_reply.substring(index3+1, index4);
         strWeight=v_reply.substring(index4+1, index5);    
         strWeist=v_reply.substring(index5+1);
        
          //Age Conversion
          v_age= strAge.toInt();
          
          //Gender Conversion
          //v_gender=strGender.toInt();
          
          //Height conversion from String to Float
          char char_Height[strHeight.length() + 1];
          strHeight.toCharArray(char_Height, strHeight.length() + 1);
          v_Height=atof(char_Height);
          
          //Weight conversion from String to Float
          char char_Weight[strWeight.length() + 1];
          strWeight.toCharArray(char_Weight, strWeight.length() + 1);
          v_Weight=atof(char_Weight);
          
          //Waist conversion from String to Float
           strWeist.toCharArray(strWeistTmp, 5);
           v_Weist=atof(strWeistTmp);        

          //Body Fat
          float v_fat; 
          String v_fat_status;
          
          if(strGender=="MALE")
          {
            v_fat = 100*(-98.42 + 4.15* v_Weist - 0.082 * (v_Weight/0.45)) / (v_Weight/0.45);
          }
          else if(strGender=="FEMALE")
          {
            v_fat = 0 * 100*(-76.76 +4.15*v_Weist - 0.082*v_Weight)/v_Weight; 
          }
                    
          if(v_age>60)
          {
            if(strGender=="FEMALE")
            { 
              if(v_fat>=42)
              {
                v_fat_status="Ext. Level";
              }
              else if(v_fat>=36)
              {
                v_fat_status="Over Level";
              }
              else if(v_fat>=24)
              {
                v_fat_status="Healthy";
              }
              else
              {
                v_fat_status="Und. Level";
              }      
            }
            else if(strGender=="MALE")
            {
              if(v_fat>=30)
              {
                v_fat_status="Ext. Level";
              }
              else if(v_fat>=25)
              {
                v_fat_status="Over Level";
              }
              else if(v_fat>=13)
              {
                v_fat_status="Healthy";
              }
              else
              {
                v_fat_status="Und. Level";
              } 
            }
          }
          else if(v_age>40)
          {
            if(strGender=="FEMALE")
            {
              if(v_fat>=40)
              {
                v_fat_status="Ext. Level";
              }
              else if(v_fat>=35)
              {
                v_fat_status="Over Level";
              }
              else if(v_fat>=33)
              {
                v_fat_status="Healthy";
              }
              else
              {
                v_fat_status="Und. Level";
              }      
            }
            else if(strGender=="MALE")
            {
              if(v_fat>=28)
              {
                v_fat_status="Ext. Level";
              }
              else if(v_fat>=22)
              {
                v_fat_status="Over Level";
              }
              else if(v_fat>=11)
              {
                v_fat_status="Healthy";
              }
              else
              {
                v_fat_status="Und. Level";
              } 
            }
          }
          else if(v_age>20)
          {
            if(strGender=="FEMALE")
            {
              if(v_fat>=39)
              {
                v_fat_status="Ext. Level";
              }
              else if(v_fat>=33)
              {
                v_fat_status="Over Level";
              }
              else if(v_fat>=21)
              {
                v_fat_status="Healthy";
              }
              else
              {
                v_fat_status="Und. Level";
              }      
            }
            else if(strGender=="MALE")
            {
              if(v_fat>=25)
              {
                v_fat_status="Ext. Level";
              }
              else if(v_fat>=19)
              {
                v_fat_status="Over Level";
              }
              else if(v_fat>=8)
              {
                v_fat_status="Healthy";
              }
              else
              {
                v_fat_status="Und. Level";
              } 
            }    
          }
          
          Serial.println("Fat:" + String(round(v_fat)) + "%");
          Serial.println("Level:" + v_fat_status);
                    
          lcd.clear();
          lcd.setCursor(0,0); lcd.print("Health Info - FAT");
          lcd.setCursor(0,1); lcd.print("--------------------");
          lcd.setCursor(0,2); lcd.print("Ratio:" + String(v_fat) + "%");
          lcd.setCursor(0,3); lcd.print("Status:" + v_fat_status);
          
          menu_b=1;
       }
    }    
    else if(customKey=='C')
    {
        lcd.clear(); lcd.noBlink();
        lcd.setCursor(0,1); lcd.print("**  Clearing Data  **");
      
        Serial.print("Clear data"); 
        eeprom_clearAll();  
        Serial.print("Clear data done");   

        soft_restart();
  
    }
    else if(customKey=='D')
    {       
        lcd.clear(); lcd.noBlink();
        lcd.setCursor(0,1); lcd.print("** System Restart **");
        delay(2000);      
        soft_restart();
    }     
      
    if (customKey=='1' || customKey=='2' || customKey=='3' || customKey=='4' || customKey=='5' || customKey=='6' || customKey=='7' || customKey=='8' || customKey=='9' || customKey=='0' || customKey=='.')
    {  
        lcd.print(customKey);   
         
        if (menu_id==1)  
        {
          strGender += customKey;                 
        }  
  
        if (menu_id==2)
        {
          strAge += customKey;            
        }        
  
        if (menu_id==3)
        {
          strHeight += customKey;           
        }
  
        if (menu_id==4)
        {
          strWeight += customKey;           
        }
    }     
  }
}

void beep(unsigned char delayms)
{
  analogWrite(PIN_BUZZER, 20);
                    
  delay(delayms);   
  analogWrite(PIN_BUZZER,0);      
  delay(delayms);         
}  


boolean eeprom_is_addr_ok(int addr)
{
  return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}

boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes)
{
  int i;
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes))
  {
    return false;
  }
  
  for (i = 0; i < numBytes; i++)
  {
    EEPROM.write(startAddr + i, array[i]);
  }
  return true;
}

boolean eeprom_write_string(int addr, const char* string)
{
  int numBytes; // actual number of bytes to be written
  //write the string contents plus the string terminator byte (0x00)
  numBytes = strlen(string) + 1;
  return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}

boolean eeprom_read_string(int addr, char* buffer, int bufSize)
{
  byte ch; // byte read from eeprom
  int bytesRead; // number of bytes read so far
  if (!eeprom_is_addr_ok(addr))
  {
    return false;
  }

  if (bufSize == 0)
  {
    return false;
  }

  if (bufSize == 1)
  {
    buffer[0] = 0;
    return true;
  }

  bytesRead = 0;

  ch = EEPROM.read(addr + bytesRead);
  
  buffer[bytesRead] = ch;
  bytesRead++;

  while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) )
  {
    ch = EEPROM.read(addr + bytesRead);
    buffer[bytesRead] = ch; // store it into the user buffer
    bytesRead++; // increment byte counter
  }

  if ((ch != 0x00) && (bytesRead >= 1))
  {
    buffer[bytesRead - 1] = 0;
  }
  return true;
}

void eeprom_clearAll()
{
  for ( int i = 0 ; i < EEPROM.length() ; i++ )
  EEPROM.write(i, 0);
}

void eeprom_clear(int st)
{
  for ( int i = st ; i < BUFSIZE ; i++ )
  EEPROM.write(i, 0);
}

