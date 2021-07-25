//
//  CallBackData.h
//  SimpleSineWavePlayer
//
//  Created by Panayotis Matsinopoulos on 25/7/21.
//

#ifndef CallBackData_h
#define CallBackData_h

#include <AudioToolbox/AudioComponent.h>

typedef struct CallBackData {
  AudioUnit outputAudioUnit;
  double startingFrameCount;
} CallBackData;

#endif /* CallBackData_h */
