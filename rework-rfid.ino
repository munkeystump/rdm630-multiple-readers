#include <rdm630.h>
#define nreader  4

rdm630 rfid1(9, 0);  //TX-pin of RDM630 connected to Arduino pin 9
rdm630 rfid2(10, 0);
rdm630 rfid3(11, 0);
rdm630 rfid4(12, 0);

boolean busy = false;
rdm630 allreaders[4] = {rfid1, rfid2, rfid3, rfid4};
String tag[nreader];
int lock = 7;

String objects[4] = {"160457ba0","1603442510","1707bc02f0","1708b8280"};

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial Started");
}

void loop()
{
  if (!busy) {
    for (int reader = 0 ; reader < nreader; reader ++) {
      allreaders[reader].begin();
      tag[reader] = readTag(reader);
      allreaders[reader].end();
      //delay(500);
    }

  }
  if(isitsolved(tag,objects)){
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
    Serial.print(t[j]);
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
