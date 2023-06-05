#include "localComms.h"

bool LocalComms::monitor(){

  Wire.begin(0);
  Wire.onReceive(LocalComms::receiveEvent);
  Wire.onRequest(LocalComms::requestEvent);

  return true;
}

void LocalComms::receiveEvent(int bytes) {
  int status = Wire.read();

  if(status == AWAIT){
    moduleData.payloadState = AWAIT;
  } else if(status == READY){
    moduleData.payloadState = READY;
  } else if(status == WRITE){
    moduleData.payloadState = WRITE;
  }
  lg.info("Received state '"+String(payloadInternalStateNames[moduleData.payloadState])+"' ("+moduleData.payloadState+")", "local-comms");
}

void LocalComms::requestEvent() {
  // Todo
}