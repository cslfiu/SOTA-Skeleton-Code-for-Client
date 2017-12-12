#ifndef PROVIZWIFI_H
#define PROVIZWIFI_H

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
#include "ProvizData.h"


struct proviz_data{
  char* deviceUUID;
  bool isProgrammed;
};

class WiFi{
    public:
        WiFi(String accessPoint,String passwordAP, String serverIp, String port, int timeOut);
        boolean checkWifiShield();
        void sendCommandAlone(String command);
        boolean sendCommand(String command,String responseStr);
        String receive();
        boolean isReceived(String response);
        String getIpAddress();
        void reset();
        void sendMessage(String message);
        void informServerForReset();
        void sendDataToServerinNormalMode(String command);
        boolean connect2AP();
        void switchToNormalMode();
        void switchToTransparentMode();
        bool connect2Server();
        String getDeviceUUID();
        void setDeviceUUID(String);
        bool isProgrammed();
        void setProgrammed(bool);
        void loadDefaultProvizData(String deviceUUID);
        bool isProvizDataAvailable();
        bool isServerWantsToProgram();
        void closeTransparentModeTunnel();
        void createTransparentModeTunnel();

    private:
        boolean isDebug;
        String AP;
        String PASSWORD_AP;
        ProvizData provizData;
        String SERVER_IP;
        bool isFinished = true;
        String PORT;
        int TIMEOUT;
        char packetEndChar = "\r\n";
        int i = 0;
        boolean isStartedWell = false;
        boolean isConnected = false;
        void makeEmptySerialLine();
        template <class T> int writeToEEPROM(int ee, const T& value)
        {
            const byte* p = (const byte*)(const void*)&value;
            unsigned int i;
            for (i = 0; i < sizeof(value); i++){
                  EEPROM.write(ee++, *p++);
                }
            return i;
        }

        template <class T> int readFromEEPROM(int ee, T& value)
        {
            byte* p = (byte*)(void*)&value;
            unsigned int i;
            for (i = 0; i < sizeof(value); i++)
                  *p++ = EEPROM.read(ee++);
            return i;
        }
};
#endif
