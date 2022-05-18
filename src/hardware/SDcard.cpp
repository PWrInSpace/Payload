#include "../include/hardware/SDcard.h"
#include "sd_diskio.h"

bool SD_write(const String & path, const String & dataFrame){

    File file = SD.open(path, "a");  //opening the file each time since in case of a power outagge all datas will be lost 
    
    if(file) {

        if(!file.write((uint8_t *) dataFrame.c_str(), dataFrame.length())) {
            
            //mainDataFrame.sdErrorCounter++;
            file.close();
            return false;
        }
    }       
    else {   
        //mainDataFrame.sdErrorCounter++;
        return false;
    }
    
    file.close();

    return true;
}