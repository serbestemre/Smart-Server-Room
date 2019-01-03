#include <LiquidCrystal.h>
LiquidCrystal lcd(22, 23, 24, 25, 26, 27, 28);
#include "DHT.h"
#include <Wire.h>
#include <Time.h>
#include <SPI.h>
#include <DS1307RTC.h>
#include <SD.h>
 File dosya;
const int ESPinSecimi = 4; //ES pull-up için seçiliyor
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
 
char ad[20];
 

void setup () {
  Serial.begin(9600); //Seri haberleşme,sıcaklığı ekrandan takip edeceğiz.
  
   if (!SD.begin(ESPinSecimi)) {
    Serial.println("Kart hatasi. Bağlantinizi kontrol ediniz..");
 }
  Serial.println("Kart Algilandi...");
  
  
  
  
  
  //RTC
  
    while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
  
  //rtc end
  
  
  dht.begin();
  lcd.begin(20,4);
 lcd.clear();

}


void loop () {
  
 //RTC


 tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);


//RTC 
  
 
  

  
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();

lcd.setCursor(0,0); // 
lcd.print("S:"); 
lcd.print(tm.Hour);
lcd.print(":");
lcd.print(tm.Minute);
lcd.setCursor(9,0); //
lcd.print("T:"); 
lcd.print(tm.Day);
lcd.print('/');
  lcd.print(tm.Month);
   lcd.print('/');
 lcd.print(tmYearToCalendar(tm.Year));
 
 
 
 
lcd.setCursor(0,1);
lcd.print("EGEMYO SMART PROJECT");
lcd.setCursor(6,2);
lcd.print("LD:");
lcd.print(t);
lcd.setCursor(1,3);




lcd.setCursor(0,3);
lcd.print("H:");
lcd.print(h);
lcd.setCursor(13,3);
lcd.print("T:");
lcd.print(t);

 delay(2000);
 

 Serial.print("Nem: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Sicaklik: "); 
  Serial.print(t);
  Serial.println(" *C ");
 
 delay (1000); //1sn aralıklarla yapacak




ad[20]=("%d%d%d.txt",tm.Day,tm.Month,tm.Year);

dosya = SD.open(ad, FILE_WRITE); // Dosya oluşturuluyor yok ise varsa gerekli ayarlamalar yapılıyor.

 
 dosya.print(tm.Day); //dosya'ya yazılıyor.
 dosya.print("/"); //dosya'ya yazılıyor.
 dosya.print(tm.Month); //dosya'ya yazılıyor.
 dosya.print("/"); //dosya'ya yazılıyor.
 dosya.print(tmYearToCalendar(tm.Year)); //dosya'ya yazılıyor.
  
 dosya.print("    "); //dosya'ya yazılıyor.
 
 
 
 print2digits(tm.Hour); 

 dosya.println(":"); //dosya'ya yazılıyor.
  print2digits(tm.Minute); 

  
  
  
  
   dosya.print("  Sıcaklık="); //dosya'ya yazılıyor.
   dosya.println(t); //dosya'ya yazılıyor.
   


   dosya.print("  Nem="); //dosya'ya yazılıyor.
   dosya.println(h); //dosya'ya yazılıyor.
   
   dosya.print("  Işık="); //dosya'ya yazılıyor.
   dosya.println(h); //dosya'ya yazılıyor.
   dosya.println();
   dosya.println();
   dosya.println();
   
   
   dosya.close();
    
   

delay (1000); //1sn aralıklarla yapacak








}


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');

    dosya.print('0');
  }
  Serial.print(number);

  dosya.print(number);
}
