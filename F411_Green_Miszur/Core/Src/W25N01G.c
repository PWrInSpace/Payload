#include "W25N01G.h"

#define SPI_TIMEOUT 1000

uint8_t rxBuffer[3100];

int _cs;
enum chipModels _model;
int _dieSelect;

void spiSendData(uint8_t* buf, uint32_t len) {

	HAL_GPIO_WritePin(NAND_CS_GPIO_Port, NAND_CS_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi1, buf, rxBuffer, len, SPI_TIMEOUT);
	HAL_GPIO_WritePin(NAND_CS_GPIO_Port, NAND_CS_Pin, 1);
	memcpy(buf, rxBuffer, len);
}

void spiSendCommandData(char* cmdbuf, uint8_t cmdbufSize, uint8_t* buf, uint32_t len) {

	HAL_GPIO_WritePin(NAND_CS_GPIO_Port, NAND_CS_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) cmdbuf, rxBuffer, cmdbufSize, SPI_TIMEOUT);
	HAL_SPI_TransmitReceive(&hspi1, buf, rxBuffer, len, SPI_TIMEOUT);
	memcpy(buf, rxBuffer, len);
	HAL_GPIO_WritePin(NAND_CS_GPIO_Port, NAND_CS_Pin, 1);
}

void sendData(char * buf, uint32_t len){
  spiSendData((uint8_t*) buf, len);
}

int flashBegin(){

  //reset();

  uint8_t jedec[5] = {W25N_JEDEC_ID, 0x00, 0x00, 0x00, 0x00};
  sendData((char*) jedec, sizeof(jedec));
  if(jedec[2] == WINBOND_MAN_ID){
    if((uint16_t)(jedec[3] << 8 | jedec[4]) == W25N01GV_DEV_ID){
      setStatusReg(W25N_PROT_REG, 0x00);
      _model = W25N01GV;
      return 0;
    }
    if((uint16_t)(jedec[3] << 8 | jedec[4]) == W25M02GV_DEV_ID){
      _model = W25M02GV;
      dieSelect(0);
      setStatusReg(W25N_PROT_REG, 0x00);
      dieSelect(1);
      setStatusReg(W25N_PROT_REG, 0x00);
      dieSelect(0);
      return 0;
    }
  }
  return 1;
}

void reset(){
  //TODO check WIP in case of reset during write
  char buf[] = {W25N_RESET};
  sendData(buf, sizeof(buf));
  HAL_Delay(1);
}

int dieSelect(char die){
  //TODO add some type of input validation
  char buf[2] = {W25M_DIE_SELECT, die};
  sendData(buf, sizeof(buf));
  _dieSelect = die;
  return 0;
}

int dieSelectOnAdd(uint32_t pageAdd){
  if(pageAdd > getMaxPage()) return 1;
  return dieSelect(pageAdd / W25N01GV_MAX_PAGE);
}

char getStatusReg(char reg){
  char buf[3] = {W25N_READ_STATUS_REG, reg, 0x00};
  sendData(buf, sizeof(buf));
  return buf[2];
}

void setStatusReg(char reg, char set){
  char buf[3] = {W25N_WRITE_STATUS_REG, reg, set};
  sendData(buf, sizeof(buf));
}

uint32_t getMaxPage(){
  if (_model == W25M02GV) return W25M02GV_MAX_PAGE;
  if (_model == W25N01GV) return W25N01GV_MAX_PAGE;
  return 0;
}

void writeEnable(){
  char buf[] = {W25N_WRITE_ENABLE};
  sendData(buf, sizeof(buf));
}

void writeDisable(){
  char buf[] = {W25N_WRITE_DISABLE};
  sendData(buf, sizeof(buf));
}

int blockErase(uint32_t pageAdd){
  if(pageAdd > getMaxPage()) return 1;
  dieSelectOnAdd(pageAdd);
  char pageHigh = (char)((pageAdd & 0xFF00) >> 8);
  char pageLow = (char)(pageAdd);
  char buf[4] = {W25N_BLOCK_ERASE, 0x00, pageHigh, pageLow};
  block_WIP();
  writeEnable();
  sendData(buf, sizeof(buf));
  return 0;
}

int bulkErase(){
  int error = 0;
  for(uint32_t i = 0; i < getMaxPage(); i+=64){
    if((error = blockErase(i)) != 0) return error;
  }
  return 0;
}

int loadProgData(uint16_t columnAdd, uint8_t* buf, uint32_t dataLen){
  if(columnAdd > (uint32_t)W25N_MAX_COLUMN) return 1;
  if(dataLen > (uint32_t)W25N_MAX_COLUMN - columnAdd) return 1;
  char columnHigh = (columnAdd & 0xFF00) >> 8;
  char columnLow = columnAdd & 0xff;
  char cmdbuf[3] = {W25N_PROG_DATA_LOAD, columnHigh, columnLow};
  block_WIP();
  writeEnable();
  spiSendCommandData(cmdbuf, sizeof(cmdbuf), buf, dataLen);

  /*SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
  digitalWrite(_cs, LOW);
  SPI.transfer(cmdbuf, sizeof(cmdbuf));
  SPI.transfer(buf, dataLen);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();*/
  return 0;
}

int loadProgDataPageAdd(uint16_t columnAdd, char* buf, uint32_t dataLen, uint32_t pageAdd){
  if(dieSelectOnAdd(pageAdd)) return 1;
  return loadProgData(columnAdd,(uint8_t*) buf, dataLen);
}

int loadRandProgData(uint16_t columnAdd, char* buf, uint32_t dataLen){
  if(columnAdd > (uint32_t)W25N_MAX_COLUMN) return 1;
  if(dataLen > (uint32_t)W25N_MAX_COLUMN - columnAdd) return 1;
  char columnHigh = (columnAdd & 0xFF00) >> 8;
  char columnLow = columnAdd & 0xff;
  char cmdbuf[3] = {W25N_RAND_PROG_DATA_LOAD, columnHigh, columnLow};
  block_WIP();
  writeEnable();

  spiSendCommandData(cmdbuf, sizeof(cmdbuf), (uint8_t*) buf, dataLen);
  return 0;
}

int loadRandProgDataPageAdd(uint16_t columnAdd, char* buf, uint32_t dataLen, uint32_t pageAdd){
  if(dieSelectOnAdd(pageAdd)) return 1;
  return loadRandProgData(columnAdd, buf, dataLen);
}

int ProgramExecute(uint32_t pageAdd){
  if(pageAdd > getMaxPage()) return 1;
  dieSelectOnAdd(pageAdd);
  char pageHigh = (char)((pageAdd & 0xFF00) >> 8);
  char pageLow = (char)(pageAdd);
  writeEnable();
  char buf[4] = {W25N_PROG_EXECUTE, 0x00, pageHigh, pageLow};
  sendData(buf, sizeof(buf));
  return 0;
}

int pageDataRead(uint32_t pageAdd){
  if(pageAdd > getMaxPage()) return 1;
  dieSelectOnAdd(pageAdd);
  char pageHigh = (char)((pageAdd & 0xFF00) >> 8);
  char pageLow = (char)(pageAdd);
  char buf[4] = {W25N_PAGE_DATA_READ, 0x00, pageHigh, pageLow};
  block_WIP();
  sendData(buf, sizeof(buf));
  return 0;

}

int read(uint16_t columnAdd, uint8_t* buf, uint32_t dataLen){
  if(columnAdd > (uint32_t)W25N_MAX_COLUMN) return 1;
  if(dataLen > (uint32_t)W25N_MAX_COLUMN - columnAdd) return 1;
  char columnHigh = (columnAdd & 0xFF00) >> 8;
  char columnLow = columnAdd & 0xff;
  char cmdbuf[4] = {W25N_READ, columnHigh, columnLow, 0x00};
  block_WIP();

  spiSendCommandData(cmdbuf, sizeof(cmdbuf), buf, dataLen);
  return 0;
}
//Returns the Write In Progress bit from flash.
int check_WIP(){
  char status = getStatusReg(W25N_STAT_REG);
  if(status & 0x01){
    return 1;
  }
  return 0;
}

int block_WIP(){
  //Max WIP time is 10ms for block erase so 15 should be a max.
  //unsigned long tstamp = uwTick;
  while(check_WIP()){
    //HAL_Delay(1);
    //if (uwTick > tstamp + 15) return 1;
	  ;
  }
  return 0;
}

int check_status(){
  return(getStatusReg(W25N_STAT_REG));
}

