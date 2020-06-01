#include <Arduino.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "water.h"
#define MAX_GAIN 2.0
#define GAIN_INTERVAL 100

AudioFileSourcePROGMEM *in;
AudioGeneratorWAV *wav;
AudioOutputI2S *out;

bool playing = false;
float gain = 0.0;
long last_gain_interval = 0;

void IRAM_ATTR on() { playing = true; }
void IRAM_ATTR off() { playing = false; }

void setup() {
  btStop();
  Serial.begin(115200);

  audioLogger = &Serial;
  in = new AudioFileSourcePROGMEM(water, sizeof(water));
  wav = new AudioGeneratorWAV();
  out = new AudioOutputI2S();

  out->SetGain(gain);

  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  attachInterrupt(18, on, FALLING);
  attachInterrupt(19, off, FALLING);
}

void loop() {
  if (playing || gain > 0.0) {
    if ((millis() - last_gain_interval) > GAIN_INTERVAL) {
      last_gain_interval = millis();
      Serial.printf("gain %lf\n", gain);
      if (playing && gain < MAX_GAIN) {
        gain += 0.1;
      }
      if (!playing) {
        gain -= 0.1;
      }
      out->SetGain(gain);
      if (gain <= 0.0 && !playing) {
        wav->stop();
        in->close();
      }
    }
    if (wav->isRunning()) {
      wav->loop();
    } else {
      wav->stop();
      in->close();
      in = new AudioFileSourcePROGMEM(water, sizeof(water));
      wav->begin(in, out);
    }
  }
}
