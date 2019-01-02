#include <LiquidCrystal.h>
LiquidCrystal lcd(22, 23, 24, 25, 26, 27, 28);
#include "DHT.h"
#include <Wire.h>
#include <Time.h>
#include <SPI.h>
#include <Ethernet.h>
#include <DS1307RTC.h>
#include <SD.h>
 File dosya;
const int ESPinSecimi = 4; 
#define DHTPIN 2     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
}; 
EthernetClient client;
IPAddress ip(192,168,2,237);
EthernetServer server(80); 

void setup () {
  Serial.begin(9600); //Seri haberleşme,sıcaklığı ekrandan takip edeceğiz.
  
     while (!Serial) {
    ; 
  } 
  //ARDUINO IP ADRESI BELIRLENIYOR
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  
  Serial.print("Suan Arduino'nun IP addresi: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // Byte olarak ip adres seri ekrana yazdırılıyor
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
  Serial.println();

  //ARDUINO IP ADRESI BELIRENDI YAZDIRILDI



  //SERVER BASLATILIYOR BAGLANTI YAPILIYOR
  Ethernet.begin(mac, ip);
  server.begin();
  
 // SD KART KONTROL 
   if (!SD.begin(ESPinSecimi)) {   
    Serial.println("Kart hatasi. Baglantinizi kontrol ediniz..");
 }
  Serial.println("Kart Algilandi...");
  
  
  //RTC BASLATILIYOR
  
    while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("RTC AKTIF");
 
  Serial.println("-------------------");
  
// DHT SENSORLER BASLATILIYOR
  dht.begin(); 
  
// LCD EKRAN BASLATILIYOR 
  lcd.begin(20,4);
  lcd.clear();
}


void loop () {
  
 //RTC


 tmElements_t tm; // RTC TARIH HESAPLAMA PARAMETRESI


// SERIAL EKRANA TARIH ve SAAT BILGISI YAZDIRILIYOR
  if (RTC.read(tm)) {
    Serial.print("Saat = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print("       Tarih (GG/AA/YYYY) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("RTC durduruldu.  Set Time'ı calistirinzi");
      Serial.println("DOSYA/ORNEKLER/DS1307RTC/SETTIME");
      Serial.println();
    } else {
      Serial.println("RTC OKUNAMADI! BAGLANTIYI KONTROL EDINIZ");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);


//RTC 
  
 
  

  
  int h = dht.readHumidity();  // Nem Degeri okunuyor ve h degiskenine atanıyor
 
  int t = dht.readTemperature(); // Sıcaklık Degeri okunuyor  t degiskenine atanıyor




  EthernetClient client = server.available();  // Server ve Istemci Uygun mu ?
  if (client) {
    Serial.println("****Yeni Oturum acildi****");
    
    
    
    boolean currentLineIsBlank = true; 
    while (client.connected()) { // İstemci baglı oldugu surece
      if (client.available()) {  // Istemci uygunsa
       
       // Sayfayı görüntüle
       
       
        char c = client.read();  

        if (c == '\n' && currentLineIsBlank) {
         
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
            
         client.print("<center>"); 
         client.print("<b>");
         client.print("<font size='20pt' color='blue'>EGE  MYO  AKILLI  LABORATUVAR PROJESI</font>");
         client.print("<br/> ");
         
         
         client.print("<font size='5pt'>");
         client.print("Tarih: "); 
         client.print(tm.Day);
         client.print("/");
         client.print(tm.Month);
         client.print("/");
         client.print(tmYearToCalendar(tm.Year));
          
         client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
          
         client.print("Saat: ");
         client.print(tm.Hour);
         client.print(":");
         client.print(tm.Minute);
         client.print("<br/> ");
         client.print("<br/> ");
        client.print("</b>");
        
        client.print("</font>");
        
        
       client.print("<img src='http://i61.tinypic.com/20qkj9s.png'>");  // TEMP. RESIM
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       
       client.print("<img src='http://i61.tinypic.com/29xdzew.png'>"); //HUMUDITY RESIM 
       
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       
       client.print("<img src='http://i62.tinypic.com/2zhmhz7.png'>"); //LDR RESIM 
       client.print("</center>"); 
       
       client.print("<br/> ");
     
  
    
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); 
       
        client.print("<b><font size='5pt' color='red'>");
        client.print("SICAKLIK: ");
        client.println(t);
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
    
        client.print(" </font></b>");
        

        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp"); 
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
            client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp"); client.print("&nbsp");   
        client.print("<b><font size='5pt' color='red'>");
        client.print("NEM: ");
        client.println(h);
        client.print(" </font></b>");
           
        
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
        
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp"); 
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
        client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");client.print("&nbsp");
        client.print("<b><font size='5pt' color='red'>");
        client.print("ISIK: ");
        client.println(t);
        client.print(" </font></b>");
        client.print(" </br>");
        client.print(" </br>");
           client.print("<hr>");
        

    
          client.println("</html>");
          break;
        }
        if (c == '\n') {
         
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
         
          currentLineIsBlank = false;
        }
      }
    }
  
    delay(1);

    client.stop();  // OTURUM KAPATILDI
    
    
    Serial.println("-*-*-*- Oturum Kapatildi -*-*-*-");
  }



//LCD EKRANA YAZDIRMA ISLEMLERI


lcd.setCursor(0,0); //CURSOR 0.Sutun 0.Satıra konumlandırıldı 
lcd.print("S:"); 
lcd.print(tm.Hour); //Saat Yazdırıldı
lcd.print(":");
lcd.print(tm.Minute); //Dakika Yazdırıldı
lcd.setCursor(9,0); //CURSOR 9.Sutun 0.Satıra konumlandırıldı 
lcd.print("T:"); 
lcd.print(tm.Day); //Gün Yazdırıldı
lcd.print('/');
lcd.print(tm.Month); //Ay Yazdırıldı
lcd.print('/');
lcd.print(tmYearToCalendar(tm.Year)); //Yıl Yazdırıldı
 
 
lcd.setCursor(0,1); //CURSOR 0.Sutun 1.Satıra konumlandırıldı 
lcd.print("EGEMYO SMART PROJECT");
lcd.setCursor(7,3); //CURSOR 7.Sutun 3.Satıra konumlandırıldı 
lcd.print("LD:");
lcd.print(t); // TEMSILI IŞIK DEĞERİ YAZDIRILDI
lcd.setCursor(1,2); //CURSOR 1.Sutun 2.Satıra konumlandırıldı 
lcd.print("IP=>");
lcd.setCursor(5,2); //CURSOR 5.Sutun 2.Satıra konumlandırıldı 

// IP ADRESI Yazdırıldı
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
   lcd.print(Ethernet.localIP()[thisByte], DEC);
   if(thisByte!= 3){
    lcd.print(".");}
  }
  
  
lcd.setCursor(0,3);  //CURSOR 0.Sutun 3.Satıra konumlandırıldı 
lcd.print("H:");
lcd.print(h); // Nem Yazdırıldı
lcd.print("%");
lcd.setCursor(14,3); //CURSOR 14.Sutun 3.Satıra konumlandırıldı 
lcd.print("T:");
lcd.print(t); // SICAKLIK DEGERİ YAZDIRILDI
lcd.print((char)223);  // derece sembolü char koduyla ekrana yazdırıldı
lcd.print("C");
 delay(2000);
 
//SERI EKRANA BILGILER YAZILIYOR
 Serial.print("Nem: ");  
  Serial.print(h); // Nem Değeri Ekrana yazdırıldı
  Serial.print(" %\t");
  Serial.print("Sicaklik: "); 
  Serial.print(t); // Sıcaklık Değeri Ekrana yazdırıldı
  Serial.println(" *C ");
 
 delay (1000); //1sn aralıklarla yapacak


 // SD karta kaydedilecek dosya adı tanımlaması yapılıyor
 char ad[20]; 
sprintf(ad,"%02d%02d%02d.txt",tm.Day,tm.Month,(tmYearToCalendar(tm.Year)));


dosya = SD.open(ad, FILE_WRITE); // Dosya yok ise oluşturuluyor  varsa yazmak için gerekli ayarlamalar yapılıyor.
  if (SD.exists(ad)) { //Dosya yerindemi kontrol ediliyor.



//(TARİH;SAAT;SICAKLIK;NEM;IŞIK) formatında dosyaya veriler yazdırılıyor
// ÖRNEK >>>>  11/06/2015;15:30;25;46;25

dosya.print(tm.Day);
dosya.print("/");
dosya.print(tm.Month);
dosya.print("/");
dosya.print(tmYearToCalendar(tm.Year));
dosya.print(";");
print3digits(tm.Hour);
dosya.print(":");
print3digits(tm.Minute);
dosya.print(";");
dosya.print(t);
dosya.print(";");
dosya.print(h);
dosya.print(";");
dosya.print(t);
dosya.println(";");

Serial.print(ad);
Serial.println("'ye Kayit yapildi ");
     
   dosya.close(); //Dosya kapılıyor
    
    }

delay (1000); // VERI KAYIT EKRAN ve SERVER BILGILERININ NE KADAR SIK GUNCELLENECEGI BURADAN MİLİSANİYE OLARAK BELIRLENMEKTEDIR  (1000 ms = 1 saniye)

}


void print3digits(int number2) {   // Dosyaya saat verisi yazdırılırken saat 1:5 iken 01:05 şeklinde düzenliyor
  if (number2 >= 0 && number2 < 10) {
   
    dosya.print('0');
  }

  dosya.print(number2);
}

void print2digits(int number) { // Serial Ekrana saat verisi yazdırılırken saat 1:5 iken 01:05 şeklinde düzenliyor
  if (number >= 0 && number < 10) {
    Serial.write('0');

  }
  Serial.print(number);
}
