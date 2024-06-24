#include "Wire.h"
#define I2C_TEMP_ADDRESS 0x40
char dtaUart[15];
char dtaLen = 0;
uint8_t Data[100] = {0};
uint8_t buff[100] = {0};
#include "Wire.h"
#define I2C_SLAVE_ADDRESS 0x55

#define QUANTITY_BYTES_TO_RECEIVE 1     // 1 to 16
#define STOP true                       // true - Release the bus / false - keep the connection alive

uint8_t uint8_to_send = 46;             // Send a number - 0 and 255
uint8_t buf[4] = {0};
uint16_t data, data1;
float temp;
float hum;


void setup()
{
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Wire.begin();
}

void loop()
{

  readReg(0x00, buf, 4);
  data = buf[0] << 8 | buf[1];
  data1 = buf[2] << 8 | buf[3];
  temp = ((float)data * 165 / 65535.0) - 40.0;
  hum =  ((float)data1 / 65535.0) * 100;
  Serial.print("temp(C):");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("hum(%RH):");
  Serial.println(hum);

/////////////////// WRITE TO SLAVE /////////////////////////////////////
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(temp);
  uint8_t error = Wire.endTransmission(true);
//////////////////// END WRITE TO SLAVE ////////////////////////////////

//////////////////////////////////// RECEIVE FROM SLAVE //////////////////////////////////////////
  uint8_t bytesReceived = Wire.requestFrom(I2C_SLAVE_ADDRESS, QUANTITY_BYTES_TO_RECEIVE, STOP);
  uint8_t uint8_from_slave = Wire.read(); // Note below..
  Serial.println(bytesReceived);
  Serial.println(uint8_from_slave);
////////////////////////////////// RECEIVE FROM SLAVE /////////////////////////////////////////

  delay(500);                          //Pause for some time
}




uint8_t readReg(uint8_t reg, const void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    Serial.println("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  Wire.beginTransmission(I2C_TEMP_ADDRESS);
  Wire.write(&reg, 1);
  if ( Wire.endTransmission() != 0) {
    return 0;
  }
  delay(20);
  Wire.requestFrom(I2C_TEMP_ADDRESS, (uint8_t) size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  return size;
}
