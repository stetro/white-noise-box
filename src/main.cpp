#include <Arduino.h>
#include <WiFi.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include "noise.h"

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;

void setup() {
  WiFi.mode(WIFI_OFF);
  btStop();
  Serial.begin(115200);
  delay(500);
  Serial.printf("starting ...");

  file = new AudioFileSourcePROGMEM(noise, sizeof(noise));
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
}

void loop() {
  Serial.println("ON");
  delay(1000);
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else {
    Serial.printf("WAV done\n");
    delay(1000);
  }
}