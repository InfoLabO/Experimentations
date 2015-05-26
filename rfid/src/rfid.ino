#include <SoftwareSerial.h>

SoftwareSerial rfid(7,8);

uint8_t recv_data[32];

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  rfid.begin(19200);
  delay(10);
  halt();
  //delay(1000);
  //modif();
}

/*
void modif() {
  Serial.println("\nTrying to write...");
  //const uint8_t b0[16] = {0x0B,0x11,0xB5,0x9E,0x31,0x08,0x04,0x00,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69};
  //B2 40 9E 87 EB 88 4 0 C1 85 14 94 51 70 34 11
  const uint8_t b0[16] = {0x88,0x04,0x56,0x93,0x31,0x08,0x04,0x00,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69};
  if (select()) {
    if (authenticate(0)) {
      write_block(0,(uint8_t*)b0);
      delay(1000);
      halt();
    }
  }
  Serial.println();
}*/

void loop() {
  if (select()) {
    for (uint8_t i=0; i<1; ++i) {
      Serial.print(i);
      Serial.print(" ");
      authenticate(i);
      read_block(i);
    }
    Serial.println();
  }
  delay(1000);
}


// Communication serial avec le module
void send_data(uint8_t datac, uint8_t* data) {
  uint8_t csum = datac;
  for (uint8_t i=0; i<datac; ++i) csum += data[i];
  rfid.write((uint8_t)0xff);
  rfid.write((uint8_t)0x00);
  rfid.write(datac);
  rfid.write(data,datac);
  rfid.write(csum);
}

uint8_t read_data() {
  if (!rfid.available()) return 0xff;
  recv_data[0] = rfid.read();
  if (recv_data[0]==0xff) {
    for(uint8_t i=1; i<3; ++i) recv_data[i] = rfid.read();
    for (uint8_t i=0; i<=recv_data[2]; ++i) recv_data[i+3] = rfid.read();
    return recv_data[2];
  }
}

uint8_t blocking_read() {
  uint8_t s;
  do {
    delay(10);
    s = read_data();
  }
  while (s==0xff);
  return s;
}


// Commandes du module
void halt() {
  const uint8_t halt_cmd[1] = {0x93};
  send_data(1,(uint8_t*)halt_cmd);
  blocking_read();
  Serial.print("Halt response code: ");
  Serial.println(recv_data[4],HEX);
}

bool select() {
  const uint8_t select_cmd[1] = {0x83};
  send_data(1,(uint8_t*)select_cmd);
  uint8_t s = blocking_read();
  if (s==6) {
    if (recv_data[4]==0x01) Serial.println("Mifare Ultralight tag");
    else if (recv_data[4]==0x02) Serial.println("Mifare Standard 1K tag");
    else if (recv_data[4]==0x03) Serial.println("Mifare Classic 4K tag");
    else Serial.println("Unknown tag type");
    Serial.print(recv_data[8], HEX);
    Serial.print(" ");
    Serial.print(recv_data[7], HEX);
    Serial.print(" ");
    Serial.print(recv_data[6], HEX);
    Serial.print(" ");
    Serial.print(recv_data[5], HEX);
    Serial.print(" ");
    Serial.println();
    return true;
  }
  return false;
}

bool authenticate(uint8_t block) {
  uint8_t auth_cmd[3] = {0x85,block,0xff};
  send_data(3,(uint8_t*)auth_cmd);
  blocking_read();
  Serial.print("Auth response code: ");
  Serial.println(recv_data[4],HEX);
  return (recv_data[4]==0x4c);
}

void read_block(uint8_t block) {
  uint8_t read_cmd[2] = {0x86,block};
  send_data(2,(uint8_t*)read_cmd);
  blocking_read();
  if (recv_data[2]==18) {
    for (uint8_t i=0; i<16; ++i) {
      Serial.print(recv_data[5+i],HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void write_block(uint8_t block, uint8_t* data) {
  uint8_t write_cmd[18];
  write_cmd[0] = 0x89;
  write_cmd[1] = block;
  for (uint8_t i=0; i<16; ++i) write_cmd[i+2] = data[i];
  send_data(18,(uint8_t*)write_cmd);
  blocking_read();
  if (recv_data[2]==18) Serial.println("Successfully wrote!");
  else {
    Serial.print("Write command failed. Error code: ");
    Serial.println(recv_data[4],HEX);
  }
}
