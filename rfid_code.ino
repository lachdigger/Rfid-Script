#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>

#define RST_PIN 9
#define SS_PIN 8


byte readCard[4];
String MasterTag = "";  // enter a uid on your card to indentify. To Find out scan your card and print out the tag on your lcd.
String tagID = "";

//String cards = [];
int population;

char server[] = "enter your ip here";
IPAddress ip([enter your ip here]); 
EthernetClient client; 

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2. If you don't have one keep it how it is.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //Parameters: (rs, enable, d4, d5, d6, d7) 




void setup() 
{

  Serial.begin(9600);
  delay(1000);
  // Initiating
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  lcd.begin(16, 2); // LCD screen

  lcd.clear();
  lcd.print("Station Pop: ");
  lcd.print(population);
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card>>");

  if(Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);
  }
}

void loop() 
{
  //lcd.print("Scan Your Card>>");
  
  while (getID()) 
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (tagID == MasterTag) 
    {
      lcd.print(" logged in");
      population += 1;
    }
    else
    {
      lcd.print(" logged out");
      population -= 1;
    } 
      //lcd.print(tagID);
      lcd.setCursor(0, 1);
      lcd.print(" St, Pop : ");
      lcd.print(population);
      Sending_To_phpmyadmindatabase();
      delay(2000);
      lcd.clear();
      //lcd.print(" Access Control ");
      lcd.print(" Station Pop: ");
      lcd.print(population);
      lcd.setCursor(0, 1);
      lcd.print("Scan Your Card>>");
  }
}

void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    // http://localhost/testcode/dht.php?population=
    Serial.print("GET /testcode/dht.php?population=");
    client.print("GET /testcode/dht.php?population=");//YOUR URL
    client.print(population);
    Serial.println(population);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: [your ip]");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println(client.connect(server, 80));
    Serial.println(client.available());
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    //return;
  }
 }

//Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
