#include "ProvizWiFi.h"
#include "Arduino.h"

#define DEBUG
/* you may have this defined somewhere else erase if you do */
#ifdef DEBUG
void print_debug(String msg){
    //Serial2.println(" [DEBUG] " + msg);
}
#endif

WiFi::WiFi(String accessPoint,String passwordAP, String serverIp, String port, int timeOut) : AP(accessPoint), PASSWORD_AP(passwordAP), SERVER_IP(serverIp), PORT(port), TIMEOUT(timeOut)
{
    /* this->AP = accessPoint; */
    /* this->PASSWORD_AP = passwordAP; */
    /* this->SERVER_IP = serverIp; */
    /* this->PORT = port; */
    /* this->TIMEOUT = timeout; */
}

bool WiFi::isProvizDataAvailable()
{
  readFromEEPROM(0,provizData);
  if(strcmp(provizData.getSecretKey(), "MiamiCSLLAB") == 0)
  {
  return true;
}
  else
  {
  return false;
}
}

void WiFi::loadDefaultProvizData(String deviceUUID)
{
  provizData.setProgrammed(false);
  provizData.setDeviceUUID(deviceUUID.c_str());
  provizData.initializeSecretKey();
  writeToEEPROM(0,provizData);

}
String WiFi::getDeviceUUID()
{
  readFromEEPROM(0,provizData);
  return provizData.getDeviceUUID();
}

bool WiFi::isProgrammed()
{
  readFromEEPROM(0,provizData);
  return provizData.isProgrammed();
}

void WiFi::setDeviceUUID(String deviceUUID)
{
  provizData.setDeviceUUID(deviceUUID.c_str());
  writeToEEPROM(0,provizData);
}

void WiFi::setProgrammed(bool isProgrammed)
{
  provizData.setProgrammed(isProgrammed);
  writeToEEPROM(0,provizData);
}

void WiFi::createTransparentModeTunnel()
{
  String command = "AT+CIPSEND";
  sendCommand(command,">");
}

void WiFi::sendMessage(String message)
{
  sendCommandAlone(message);
}

void WiFi::informServerForReset()
{
   sendMessage("RST+INFORM");
}

void WiFi::closeTransparentModeTunnel()
{
  Serial.print("+++");
  delay(1000);
}

boolean WiFi::connect2AP()
{
  sendCommand("AT+CWMODE_DEF=1","OK");
  String connectionStr = "AT+CWJAP_DEF=\"";
  connectionStr += AP;
  connectionStr += "\",\"";
  connectionStr += PASSWORD_AP;
  connectionStr += "\"";
  sendCommand("AT+CIPMODE=0","OK");
  sendCommand("AT+CIPMUX=0","OK");
  if(sendCommand(connectionStr,"OK") == true)
  {
    return true;
  }
  return false;
}

boolean WiFi::checkWifiShield()
{
    return sendCommand("AT","OK");
}

void WiFi::switchToNormalMode()
{
  sendCommand("AT+CIPMODE=0","OK");
}

void WiFi::sendCommandAlone(String command)
{makeEmptySerialLine();
Serial.print(command);
Serial.write('\015');
Serial.write('\012');
}
void WiFi::makeEmptySerialLine()
{
  while(Serial.available() >0)
  {Serial.readString();}
}

void WiFi::sendDataToServerinNormalMode(String command)
{
  if(isFinished){
    isFinished = false;
    String sendingCommand = "AT+CIPSEND=";
    sendingCommand += (strlen(command.c_str())+2);
    sendCommand(sendingCommand,"OK");
    sendCommandAlone(command+"\n");
    isFinished = true;
  }
}

bool WiFi::isServerWantsToProgram()
{
  bool isFinished = false;
  bool returnValue = false;
  String foundResult = "";
  sendDataToServerinNormalMode("isProgrammedRequest");
  char* firstOccurance;

  unsigned long time = millis();
  while((millis() < (time + TIMEOUT)))
  {
    String receivedString = receive();
    String isProgrammedText = "isProgrammed:";
    firstOccurance = strstr(receivedString.c_str(),isProgrammedText.c_str());
    if(firstOccurance != NULL)
    {
    firstOccurance +=13;
    break;
  }
  }
  if(firstOccurance != NULL)
  {
  while(firstOccurance != '#')
  {
    if(*firstOccurance == '\0')
    {
      returnValue = false;
      break;
    }
    else{
    foundResult += *firstOccurance;
    if(foundResult == "0")
    {
      returnValue = false;
    }
    else if(foundResult == "1"){
      returnValue = true;
      break;
    }
    firstOccurance++;
  }

  }
}
return returnValue;
}




boolean WiFi::sendCommand(String command,String responseStr)
{
    sendCommandAlone(command);
    boolean result = isReceived(responseStr);
    return result;
}

String WiFi::receive()
{
    String received = "";
        while(Serial.available() >0)
        {
            received.concat(Serial.readString());
        }
        return received;
    }


boolean WiFi::isReceived(String response)
{

    String received = "";
    unsigned long boundary = (millis() + TIMEOUT);
    while(millis()<boundary)
    {
      if(Serial.available()>0){
        received.concat(Serial.readString());
        if (received.indexOf(response) > -1)
        {  return true;
        }
      }
    }
    return false;
}

String WiFi::getIpAddress()
{
    sendCommand("AT+CIFSR","OK");
    String receivedString = receive();
    return receivedString;
}

void WiFi::reset()
{
  sendCommand("AT+RST","ready");
}

void WiFi::switchToTransparentMode()
{
  sendCommand("AT+CIPMODE=1","OK"); // UART-WIFI passthrough mode
}
bool WiFi::connect2Server()
{

    int port = PORT.toInt();
    String commandBuilder = "AT+CIPSTART=\"";
    commandBuilder += "TCP\",\"";
    commandBuilder += SERVER_IP;
    commandBuilder += "\",";
    commandBuilder += port;
    sendCommand(commandBuilder,"OK");
    if(sendCommand("AT+CIPSTATUS","STATUS:3") == true)
    {
        isConnected = true;
    }
    return isConnected;

}
