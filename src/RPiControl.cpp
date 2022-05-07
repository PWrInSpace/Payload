#include <RPiControl.hh>

 
 void RPiControl::resolveCommand(std::string command){
     int i;

     i =  (RPiCommands)3;


     switch(i){
         case CAM_REC:
        {
            Serial.println("CAM is on");
            RPiControl::recordOn();
            break;
        }
        case CAM_OFF:
        {
            Serial.println("CAM is off");
            RPiControl::recordOff();
            break;
        }
        case RPI_ON:
        {
            Serial.println("RPI on");
            RPiControl::raspberryPower();
            break;
        }
        case RPI_OFF:
        {
            Serial.println("RPI off");
            RPiControl::raspberryOff();
            break;
        }
        default:
        Serial.println("WRONG COMMAND");
        break;

     }
}


void RPiControl::init(){
    pinMode(RPI_PIN_16, OUTPUT);
    pinMode(RPI_PIN_18, OUTPUT);
    pinMode(RPI_PIN_22, OUTPUT);
    pinMode(RPI_POWER_PIN, OUTPUT);
}

void RPiControl::recordOn(){
    if(digitalRead(RPI_PIN_16)==0)
    digitalWrite(RPI_PIN_16,HIGH);
}

void RPiControl::recordOff(){
     if(digitalRead(RPI_PIN_16)==1)
    digitalWrite(RPI_PIN_16,LOW);
}

void RPiControl::raspberryPower(){
    if(digitalRead(RPI_POWER_PIN)==0)
    digitalWrite(RPI_POWER_PIN,HIGH);
}
void RPiControl::raspberryOff(){
    if(digitalRead(RPI_POWER_PIN)==1)
    digitalWrite(RPI_POWER_PIN,LOW);
}