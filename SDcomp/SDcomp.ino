bool SDState{false};
File LogFile;

bool trySDCard(void)
{
  // sanity check: maybe the SD card is actually here?
  LogFile = SD.open("test.txt", FILE_WRITE);
  delay(10);
  if (LogFile) {
    LogFile.close();
    return true;
  }
  int8_t counter{10};
  while (counter && !SDState) {
    SDState = SD.begin(TFT_CARD_CS);
    Serial.print(counter);
    Serial.print(" ");
    counter--;
    delay(100);
  }
  Serial.print("SD: ");
  Serial.println(SDState);
  return SDState;
}

void setup() {
  SDState = SD.begin(TFT_CARD_CS);
  Serial.begin(115200);
}

void loop() {
  // NB. Simplified a lot (exitMenuMessage(string) is my custom function defined elsewhere)
  switch (state) {
  case SD_RETRY:
    exitMenuMessage(trySDCard() ? "found SD card" : "no SD card");
    break;
  }
}
