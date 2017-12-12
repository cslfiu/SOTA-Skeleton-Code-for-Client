#include "string.h"
class ProvizData
{
private:
  char deviceUUID[37];
  bool programmed;
  char secretKey[12]; //MiamiCSLLAB

public:
  ProvizData();
  ~ProvizData();
  char* getDeviceUUID(){return deviceUUID;}
  bool isProgrammed(){return programmed;}
  char* getSecretKey(){return secretKey;}
  void initializeSecretKey(){
    secretKey[0] = 'M';
    secretKey[1] = 'i';
    secretKey[2] = 'a';
    secretKey[3] = 'm';
    secretKey[4] = 'i';
    secretKey[5] = 'C';
    secretKey[6] = 'S';
    secretKey[7] = 'L';
    secretKey[8] = 'L';
    secretKey[9] = 'A';
    secretKey[10] = 'B';
    secretKey[11] = '\0';

  }
  void setProgrammed(bool isDeviceProgrammed){programmed = isDeviceProgrammed;}
  void setDeviceUUID(char* deviceId){
    for(int i =0; i<36;i++)
    {
      deviceUUID[i] = deviceId[i];
    }
    deviceUUID[36] = '\0';

  }
};
