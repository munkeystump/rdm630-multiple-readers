#include <rdm630.h>
#define nreader  4

rdm630 rfid1(9, 0);  //TX-pin of RDM630 connected to Arduino pin 9
rdm630 rfid2(10, 0);
rdm630 rfid3(11, 0);
rdm630 rfid4(12, 0);

boolean busy = false; //not really sure if I need this
rdm630 allreaders[4] = {rfid1, rfid2, rfid3, rfid4}; //array containing each rfid to be polled
String tag[nreader]; // array to store tags of currently placed tags
int lock = 7; //pin for lock (not yet implemented fully)

String objects[4] = {"160457ba0","1603442510","1707bc02f0","1708b8280"}; //tagIDs of correct objects to be placed

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial Started");
}

void loop()
{
  if (!busy) {
    for (int reader = 0 ; reader < nreader; reader ++) {    //cycle through each reader beginning software serial, reading tag then ending software serial
      allreaders[reader].begin();
      tag[reader] = readTag(reader);
      allreaders[reader].end();
    }

  }
  if(isitsolved(tag,objects)){                              //check to see if all objects are placed
    Serial.println("Puzzle Solved");
    unlock();
  }
}

String readTag(int r) {
  byte data[6];
  byte length;
  String tagID;
  for (int i = 0 ; i < 1000 ; i++) {
    if (allreaders[r].available()) {
      busy = true;
      allreaders[r].getData(data, length);
      Serial.print("Data valid on reader ");
      Serial.println(r + 1);
      for (int i = 0; i < length; i++) {
        tagID += String(data[i], HEX);
      }
      Serial.println(tagID);
      busy = false;
      return tagID;
    }
  }
  //return "";
}

boolean isitsolved(String t[nreader], String o[nreader]) {
  boolean solved = true;
  Serial.println("================================");
  for (int j = 0 ; j < nreader; j ++) {
    if (t[j] != o[j]) {
      solved = false;
    }
    else{
    Serial.print(t[j]);           //debugging to see which tags/objects are detected in correct positions
    Serial.print(" - ");
    Serial.println(o[j]);
  }}
  return solved;
}

void unlock() {
    Serial.println("Lock Open");
    digitalWrite(lock, LOW);
    delay(5000);
    digitalWrite(lock, HIGH);
  }
