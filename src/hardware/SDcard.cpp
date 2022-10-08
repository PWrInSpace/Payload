#include "../include/hardware/SDcard.h"
#include <string.h>
#include "../include/pinout.h"

SDCard::SDCard(SPIClass &_spi, uint8_t _cs) : spi(_spi), cs(_cs), __mySD(SDFS(FSImplPtr(new VFSImpl()))){};

bool SDCard::init()
{
  // spi = SPIClass(HSPI);
  // spi.begin(sck, miso, mosi, cs);
  spi.setClockDivider(SPI_CLOCK_DIV2);
  digitalWrite(LED_SD_GOOD, LOW);
  
  if (!SD.begin(cs, spi))
  { // idk xD

    for (int i = 0; i++; i < 3)
    {
      digitalWrite(LED_SD_GOOD, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED_SD_GOOD, LOW);
      vTaskDelay(500/portTICK_PERIOD_MS);
    }
    return false;
  }
  digitalWrite(LED_SD_GOOD, HIGH);
  return true;
}

bool SDCard::write(String path, const String &dataFrame)
{
  File file = SD.open(path, "a");
  if (file == 0x00)
  {
    Serial.print("Open error: ");
    Serial.println(path);

    SD.end();
    if (!SD.begin(cs, spi))
    {
      Serial.println("Begin error ");
      return false;
    }
    file = SD.open(path, "a");
    if (file == 0x00)
    {
      Serial.println("Open v2");
      return false;
    }
    Serial.println("Udalo sie");
  }
  if (file)
  {
    if (!file.write((uint8_t *)dataFrame.c_str(), dataFrame.length()))
    {
      file.close();
      return false;
    }
  }
  else
  {
    return false;
  }

  file.close();

  return true;
}

bool SDCard::write(String path, char *dataFrame)
{
  File file = SD.open(path, "a");
  if (file == 0x00)
  {
    Serial.print("Open error: ");
    Serial.println(path);

    SD.end();
    if (!SD.begin(cs, spi))
    {
      Serial.println("Begin error ");
      return false;
    }
    file = SD.open(path, "a");
    if (file == 0x00)
    {
      Serial.println("Open v2");
      return false;
    }
    Serial.println("Udalo sie");
  }

  if (file)
  {
    if (!file.write((uint8_t *)dataFrame, strlen(dataFrame)))
    {
      file.close();
      return false;
    }
  }
  else
  {
    return false;
  }

  file.close();

  return true;
}

bool SDCard::fileExists(String path)
{
  return SD.exists(path);
}