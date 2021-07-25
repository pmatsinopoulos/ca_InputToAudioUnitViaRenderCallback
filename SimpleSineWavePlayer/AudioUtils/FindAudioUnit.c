//
//  FindAudioUnit.c
//  SimpleSineWavePlayer
//
//  Created by Panayotis Matsinopoulos on 25/7/21.
//

#include "FindAudioUnit.h"

AudioComponent FindAudioUnit(AudioComponentDescription iAudioComponentDescription) {
  AudioComponent outputAudioUnit = AudioComponentFindNext(NULL, &iAudioComponentDescription);
  if (!outputAudioUnit) {
    fprintf(stderr, "Cannot get the audio unit matching given description\n");
    exit(1);
  }
  return outputAudioUnit;
}
