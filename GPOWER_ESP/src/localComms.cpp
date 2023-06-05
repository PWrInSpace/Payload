#include "localComms.h"

void sendStatus(){
    Wire.begin();

    Wire.beginTransmission(0);
    Wire.write(moduleData.payloadState);
    lg.info("Sending state '"+String(payloadInternalStateNames[moduleData.payloadState])+"' ("+moduleData.payloadState+")", "local-comms");
    
    Wire.endTransmission();
    
    Wire.end();
}