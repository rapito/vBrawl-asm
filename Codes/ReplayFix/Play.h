//
// Created by johno on 9/24/2020.
//

#ifndef PROJECTMCODES_PLAY_H
#define PROJECTMCODES_PLAY_H

#include <Brawl/muReplayTask.h>
#include "ReplayPlayer.h"
#include "Wii/mtRand.h"
#include "Debug.h"
#include "Brawl/gfPadSystem.h"
#include "Brawl/GameFrame.h"
#include "PlaybackFileSyncer.h"
#include "Brawl/ipSwitch.h"
#include "Brawl/gfFileIOManager.h"



void playGameStart();


void playFileLoad(gfFileIORequest* fileIoRequest);


void playFrameStart();


void startPlayback();

void stopPlayback();


void playFighters();

void setGameInfoForReplay(muReplayTask& replayTask);

#define REPLAY_BUFFER_START ((u8*) 0x91301c00)

#endif //PROJECTMCODES_PLAY_H
