//
//  StopAudioOutputUnit.c
//  SimpleSineWavePlayer
//
//  Created by Panayotis Matsinopoulos on 25/7/21.
//

#include "StopAudioOutputUnit.h"
#include "CheckError.h"

void StopAudioOutputUnit(AudioUnit inAudioUnit) {
  CheckError(AudioOutputUnitStop(inAudioUnit), "Stopping the Output Audio Unit");
  CheckError(AudioUnitUninitialize(inAudioUnit), "Uninitializing the Output Audio Unit");
  CheckError(AudioComponentInstanceDispose(inAudioUnit), "Disposing the Output Audio Unit");
}
