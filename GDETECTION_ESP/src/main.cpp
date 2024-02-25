#include "main.h"

void setup(){
  delay(1000);

  setCpuFrequencyMhz(80);

  lg.setLogLevel(INFO);
  fcn.initPeripherals();

  fcn.flashLogger.begin();
  fcn.sdLogger.begin();

  lc.monitor();

  lg.info("Main setup finished", "main");
}

void loop(){

  if (moduleData.payloadState == READY) {
    fcn.blink();
  }

  if (moduleData.payloadState == WRITE) {
    fcn.measure();
  }

}