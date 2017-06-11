#include <rdm630.h>
#define NoOfReaders   4

String obj1 = "a";                         //object ids from each tag
String obj2 = "b";
String obj3 = "c";
String obj4 = "d";
String objects[] = {obj1,obj2,obj3,obj4}; //array to hold all object ids in correct order
//String s1;
//String s2;
//String s3;
//String s4;
String slots[NoOfReaders];                          //array to hold the currently place object id in each slot
//int led1 = 3;
//int led2 = 4;
//int led3 = 5;
//int led4 = 6;
int leds[] = {3,4,5,6};                     //array to hold pins of indicator leds used to signify which items are in the correct position
int lock = 7;


rdm630 slot1(9, 0);  //TX-pin of RDM630 connected to Arduino pin 9
rdm630 slot2(10, 1);  //TX-pin of RDM630 connected to Arduino pin 10
//rdm630 slot3(11,0);  //TX-pin of RDM630 connected to Arduino pin 11
//rdm630 slot4(12, 0);  //TX-pin of RDM630 connected to Arduino pin 12


void setup()
{
    Serial.begin(9600);           // start serial to PC
    slot1.begin();                //init all rfid readers
    slot2.begin();
    //slot3.begin();
    //slot4.begin();
    for (int i=0;i<NoOfReaders;i++){   // init led pins
      pinMode(leds[i],OUTPUT);
      Serial.println("init leds");
    }
    pinMode(lock,OUTPUT);
    for (int i=0;i<NoOfReaders;i++){
      digitalWrite(leds[i],LOW);       // set all leds off
    }
      digitalWrite(lock,LOW);
}

void loop()
{
    byte data[6];
    byte length;
    slot1.listen();
    if (slot1.isListening()){
      //Serial.println("slot 1 listening");
      //Serial.println(slot1.available());
    if(slot1.available()){              //scan slot one using software serial to see if a tag is present
        slot1.getData(data,length);
        Serial.println("Data valid");
        //for(int i=0;i<length;i++){
            //slots[0] += (data[i],HEX);
        //}
        unsigned long result = 
          ((unsigned long int)data[1]<<24) + 
          ((unsigned long int)data[2]<<16) + 
          ((unsigned long int)data[3]<<8) + 
          data[4];              
        Serial.print("decimal CardID: ");
        Serial.println(result);
    }}
    //delay(500);
    slot2.listen();
    if (slot2.isListening()){
      //Serial.println("slot 2 listening");
    if(slot2.available()){              //scan slot one using software serial to see if a tag is present
        slot2.getData(data,length);
        Serial.println("Data valid");
        //for(int i=0;i<length;i++){
            //slots[0] += (data[i],HEX);
        //}
        unsigned long result = 
          ((unsigned long int)data[1]<<24) + 
          ((unsigned long int)data[2]<<16) + 
          ((unsigned long int)data[3]<<8) + 
          data[4];              
        Serial.print("decimal CardID: ");
        Serial.println(result);
    }}
    checkpositions();
    resetscan();
}

void checkpositions(){              //for each slot see if the correct object is placed
  bool matched = true;
  for (int i=0;i<NoOfReaders;i++){
    if(objects[i]==slots[i]){
      digitalWrite(leds[i],HIGH);
    }
    else{
      digitalWrite(leds[i],LOW);
      matched = false;              //if at least one object is not in position, matched will be false
    }
  }
  if(matched){                      //if matched is true all objects have been placed
    unlock();
  }
}

void resetscan(){         //reset all slot variables to blank to prepare for next scan through rfid readers
  for (int i=0;i<NoOfReaders;i++){
    slots[i] = "";
  }
}

void unlock(){                //unlocks maglock (via relay) can insert code here to play sound or trigger other event as needed
  Serial.println("UNLOCKED");
  digitalWrite(lock,HIGH);
  delay(5000);
  digitalWrite(lock,LOW);
}
