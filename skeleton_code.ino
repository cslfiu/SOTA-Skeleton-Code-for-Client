/*
This code was generated by using Proviz Software.
All rights reserved.
Date 06/22/2017
*/
#include <ArduinoJson.h>
#include <ProvizWiFi.h>
#include <avr/wdt.h>

WiFi wifi("AP_NAME","","REMOTE_PROGRAMMER_IP_ADDRESS","PORT_NUMBER",5000);

void rebootProcedure()
{
  wifi.switchToTransparentMode();
  wifi.createTransparentModeTunnel();
  wifi.setProgrammed(true);
  triggerWDTReset();

}

void triggerWDTReset()
{
  wifi.informServerForReset();
  wdt_enable(WDTO_15MS);
}


void setup() {
  Serial.begin(115200);
  wifi.reset();
  if(wifi.connect2AP()){
    if(!wifi.connect2Server()){
      triggerWDTReset();
    }
  }
  else{
    triggerWDTReset();
  }
   /* You can place your variable initialization code into this function.
    *  This function is called once.
    */
}

void loop() {
  if (wifi.isReceived("reboot"))
  {
    rebootProcedure();
  }
   /* You can place your application code.
   *  This loop runs continously
   */

}
