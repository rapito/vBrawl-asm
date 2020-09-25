//
// Created by johno on 9/24/2020.
//

#ifndef PROJECTMCODES_REPLAYPLAYER_H
#define PROJECTMCODES_REPLAYPLAYER_H


#include <Wii/FILE.h>
#include "ReplayEvent.h"
#include "Containers/vector.h"
#include "AsyncFileIO.h"


class ReplayPlayer {
public:
    ReplayPlayer(ReplayEvent* buffer, u32 bufferSize);

    //only works at begining of game of course
    ReplayGameStartEvent* getGameStartEvent();
    ReplayFrameStartEvent* getFrameStartEvent();
    ReplayPreFrameFighterEvent* getFighterEventFromPort(int port, bool createIfNotFound=false);

    void clear();
    void loadEventsToVector();

    void openReplayFile(FILE* file);

    void loadNextFrame();

    void close();

    vector<ReplayEvent*> events;

    AsyncFileIO fileIO;
};


#endif //PROJECTMCODES_REPLAYPLAYER_H
