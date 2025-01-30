// Create a SoftwareSerial object to communicate with the SIM800L module
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // SIM800L Tx & Rx connected to Arduino pins #3 & #2
String stringOne, stringTwo, stringThree;
String inputString = "";         // a string to hold incoming data
 int value1;
 char Mymessage[5];
 String ReadMsg;
 
 int value;

boolean bMsgReceivedFlag = false; 

boolean stringComplete = false; 
char incomingByte = 0;
String CellNumtemp, CellNum;
                                         
void setup()
{
  // Initialize serial communication with Arduino and the Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  // Initialize serial communication with Arduino and the SIM800L module
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); // Handshake test, should return "OK" on success
  updateSerial();
  delay(1000);
  mySerial.println("AT+CSQ"); // Signal quality test, value range is 0-31, 31 is the best.
  updateSerial();
  delay(1000);
  mySerial.println("AT+CCID"); // Read SIM information to confirm whether the SIM is inserted
  updateSerial();
  delay(1000);
  mySerial.println("AT+CREG?"); // Check if it's registered on the network
  updateSerial();
  delay(1000);
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial(); 
  delay(1000); 
  mySerial.println("AT+CMGS=\"+918698660302\""); // Change ZZ with the country code and xxxxxxxxxxx with the phone number to send an SMS to
  updateSerial();
  delay(1000);
  mySerial.print("hi"); // Change ZZ with the country code and xxxxxxxxxxx with the phone number to send an SMS to.
  updateSerial();
  delay(1000);
  mySerial.write(26); 
  delay(2000);
  mySerial.println("AT+CNMI=2,2,0,0,0"); // command to receive the SMS
  updateSerial();
  delay(2000);
   mySerial.println("AT+CNMI=2,2,0,0,0");
  updateSerial();
  delay(2000);
  inputString.reserve(200);   
  //CellNum = ("+918698660302");    
 // stringThree = CellNum;                                                                                                                                   
}

void loop()
{
   int i,uci,msg ; 
    updateSerial();
    if (mySerial.available() > 0) 
    {
        //Serial.println("2");       
        inputString = mySerial.readString();
        Serial.println(inputString);
        CellNumtemp = inputString.substring(inputString.indexOf("+91"));
        CellNum = CellNumtemp.substring(0,13); 
         i  = inputString.length();
         msg = 0;
         for(uci = 47; uci<=i; uci++)
         {
            stringOne += inputString[uci];
         }
         if(!bMsgReceivedFlag)
         {
           stringThree = CellNum;
           bMsgReceivedFlag = 1;
         }         
        Serial.println(stringOne); 
        delay(10);
        inputString = "";
        stringOne = ""; 
     }  
    
    while (Serial.available() > 0)
    { 
      ReadMsg = Serial.readString(); //Read the serial data and store in var
      /*************************************************************/
      // Serial.println(stringOne); 
      /****************************************************************/
      mySerial.println(ReadMsg);
     // mySerial.println("GOC_1");
       //mySerial.println(Serial.read());
      //value = atoi(Mymessage);
      if (ReadMsg )
      {    
          Send_SMS();
          delay(1000); 
      }
    }
}

void Send_SMS()
{   
  mySerial.println("AT+CMGS=\""+stringThree +"\"\r");
  delay(1000);
  //The text of the message to be sent.
  mySerial.print("PLC Data: ");
  
  mySerial.println(ReadMsg);
  //mySerial.print(11);
  delay(2000);
  mySerial.write(0x1A);
  delay(1000);
  updateSerial();
  ReadMsg = "";
}

void updateSerial()
{
  while (Serial.available()) 
  {
    mySerial.write(Serial.read()); // Forward data from Serial to Software Serial Port
  }
  while (mySerial.available()) 
  {
    Serial.write(mySerial.read()); // Forward data from Software Serial to Serial Port    
  }  
}
