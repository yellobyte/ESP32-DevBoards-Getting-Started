/*
  This sketch reads the version number of hte ESP32 chip

  2017-07-17 Andreas Spiess

*/

#include "soc/efuse_reg.h"

int getChipRevision()
{
  return (REG_READ(EFUSE_BLK0_RDATA3_REG) >> (EFUSE_RD_CHIP_VER_RESERVE_S)&&EFUSE_RD_CHIP_VER_RESERVE_V) ;
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.print("REG_READ(EFUSE_BLK0_RDATA3_REG) ");
  Serial.println(REG_READ(EFUSE_BLK0_RDATA3_REG), BIN);

  Serial.print("EFUSE_RD_CHIP_VER_RESERVE_S ");
  Serial.println(EFUSE_RD_CHIP_VER_RESERVE_S, BIN);

  Serial.print("EFUSE_RD_CHIP_VER_RESERVE_V ");
  Serial.println(EFUSE_RD_CHIP_VER_RESERVE_V, BIN);

   Serial.println();

  Serial.print("Chip Revision (official version): ");
  Serial.println(getChipRevision());
  
  Serial.print("Chip Revision from shift Opration ");
  Serial.println(REG_READ(EFUSE_BLK0_RDATA3_REG) >> 15, BIN);
}

// the loop function runs over and over again forever
void loop() {
}
