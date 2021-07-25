//
//  main.c
//  SimpleSineWavePlayer
//
//  Created by Panayotis Matsinopoulos on 25/7/21.
//

#include <stdio.h>
#include <AudioToolbox/AudioToolbox.h>
#include <unistd.h>
#include <math.h>
#include "CallBackData.h"
#include "CheckError.h"
#include "StopAudioOutputUnit.h"
#include "FindAudioUnit.h"

#define SINE_FREQUENCY 880.0
#define SAMPLE_FREQUENCY 44100.0

OSStatus SineWaveRenderProc (void *inRefCon,
                             AudioUnitRenderActionFlags *ioActionFlags,
                             const AudioTimeStamp *inTimeStamp,
                             UInt32 inBusNumber,
                             UInt32 inNumberFrames,
                             AudioBufferList *ioData) {
  
  CallBackData *pCallBackData = (CallBackData *)inRefCon;
  double frameInPeriod = pCallBackData->startingFrameCount;
  
  // This is the number of samples/frames we need to cover a period/cycle
  // of the SINE waveform, if we sample/frame SAMPLE_FREQUENCY samples/frames
  // per second.
  double sinePeriodLengthInFrames = SAMPLE_FREQUENCY / SINE_FREQUENCY;
                                                                                                               
  int frame = 0;
  // we have to fill in all the frames that the callback caller specifies.
  for (frame = 0; frame < inNumberFrames; frame++) {
    // I go to the first buffer and the value of `mData` and save it into a `Float32 *` var.
    // Hence +data+ is a pointer to the beginning of the audio data buffer.
    Float32 *data = (Float32 *)ioData->mBuffers[0].mData;
    
    // We populate the current frame audio data value/sample with the result of `sin` function
    Float32 sampleValue = (Float32)sin(2 * M_PI * (frameInPeriod / sinePeriodLengthInFrames));
    (data)[frame] = sampleValue;
    
    // copy to right channel too
    data = (Float32*)ioData->mBuffers[1].mData;
    (data)[frame] = sampleValue;
   
    frameInPeriod += 1.0;
    
    if (frameInPeriod > sinePeriodLengthInFrames) {
      frameInPeriod = 0; // returns it back to 0?
    }
  } // for ()
  
  pCallBackData->startingFrameCount = frameInPeriod;
  
  return noErr;
}

void BuildAudioUnitDescription(AudioComponentDescription *oAudioComponentDescription) {
  oAudioComponentDescription->componentType = kAudioUnitType_Output;
  oAudioComponentDescription->componentSubType = kAudioUnitSubType_DefaultOutput;
  oAudioComponentDescription->componentManufacturer = kAudioUnitManufacturer_Apple;
}

void CreateAndConnectOutputUnit(CallBackData *callBackData) {
  AudioComponentDescription outputAudioUnitDescription = {0};
  
  BuildAudioUnitDescription(&outputAudioUnitDescription);
    
  AudioComponent outputAudioUnit = FindAudioUnit(outputAudioUnitDescription);
    
  CheckError(AudioComponentInstanceNew(outputAudioUnit,
                                       &callBackData->outputAudioUnit),
             "Instantiating the Output Audio Unit");
  
  AURenderCallbackStruct input;
  input.inputProc = SineWaveRenderProc;
  input.inputProcRefCon = callBackData;
  CheckError(AudioUnitSetProperty(callBackData->outputAudioUnit,
                                  kAudioUnitProperty_SetRenderCallback,
                                  kAudioUnitScope_Input,
                                  0,
                                  &input,
                                  sizeof(input)),
             "Setting the Output Audio Unit property for rendering callback");
  
  CheckError(AudioUnitInitialize(callBackData->outputAudioUnit),
             "Audio Unit Initialize");
}

int main(int argc, const char * argv[]) {
  CallBackData callBackData = {0};
  
  CreateAndConnectOutputUnit(&callBackData);
  
  // Since we do not work with AU Graph, we start directly the audio output unit
  CheckError(AudioOutputUnitStart(callBackData.outputAudioUnit),
             "Starting the Output Audio Unit");
  
  sleep(5);
cleanup:
  StopAudioOutputUnit(callBackData.outputAudioUnit);
  
  printf("Bye!\n");
  
  return 0;
}
