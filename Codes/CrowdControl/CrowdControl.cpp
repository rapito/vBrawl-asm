//
// Created by Ilir on 2021-12-13.
//

#include "CrowdControl.h"
#include "EXIPacket.h"
#include "EffectItemHandler.h"
#include "EffectStatusHandler.h"
#include "EffectActionHandler.h"
#include "EffectGameHandler.h"
#include "Brawl/GF/gfPadSystem.h"

namespace FrameLogic {
    // setup global variables
    DebugData data = DebugData();
    DebugData* debugData = &data;
    Menu* myMenu;
    bool visible = false;

    const int MIN_PRELOADED_POKEMON_FRAMES = 60;
    const int MIN_PRELOADED_ASSIST_FRAMES = 120;
    int preloadedPokemonId = -1;
    int preloadedPokemonWaitFrames = 0;
    int preloadedPokemonAmount = 0;
    int preloadedAssistId = -1;
    int preloadedAssistWaitFrames = 0;
    int preloadedAssistAmount = 0;

    //hacky way to check if in game
    unsigned int getScene() {
        u32* ptr = (u32*) (0x805b4fd8 + 0xd4);
        ptr = (u32*) *ptr;
        if(ptr < (u32*)0xA0000000) {
            ptr = (u32*) *(ptr + (0x10 / 4));
            if(ptr != nullptr) {
                u32 scene = *(ptr + (8 / 4));
                return scene;
            }
        }
        return false;
    }

    // called when game starts
    void SendGameStatus(EXIStatus exiStatus) {
        EXIPacket responsePckt = EXIPacket(exiStatus, nullptr, 0);
        responsePckt.Send();
    }

    // called every frame
    void onUpdateFrame() {

        // initializes the menu
        if (myMenu == nullptr) {
            myMenu = new Menu();
            Page* mainPage = new MainPage(myMenu, debugData);
            myMenu->nextPage(mainPage);

        }
        printer.setup();
        printer.drawBoundingBoxes(0);

        startNormalDraw();


        Message * message = &printer.message;
        message->zPos = 0;
        message->fontScaleX = 0.1;
        message->fontScaleY = 0.1;

        printer.lineHeight = 20 * message->fontScaleY;

        char buffer[200] = {};

        message->xPos = 1;
        message->yPos = 1;
        message->zPos = 0;

        setupDrawPrimitives();


        // halfword vector for sending into to the game
        // first entry is effect id (EFFECT_NOT_CONNECTED = 0, EFFECT_NONE = 1, EFFECT_UNKNOWN = 2, EFFECT_ACTUAL >= 3
        // second and above entries are parameters for the effect
        EXIPacket requestPckt = EXIPacket(5*sizeof(u16));
        requestPckt.Receive();
        u16* effectRequest = (u16*)requestPckt.source;

        EXIStatus exiStatus = STATUS_UNKNOWN;
        unsigned int scene = getScene();

        // TODO: Investigate crash during classic mode

        // for testing effects
        gfPadSystem *padSystem = PAD_SYSTEM;

        if (scene == SCENE_TYPE::SCENE_VS || scene == SCENE_TYPE::SCENE_TRAINING_MODE_MMS) {
            //visible = true;

            if (preloadedPokemonId >= 0) {
                preloadedPokemonWaitFrames++;
                if (preloadedPokemonWaitFrames > MIN_PRELOADED_POKEMON_FRAMES) {
                    effectItemSpawn(preloadedPokemonId, preloadedPokemonAmount);
                    preloadedPokemonId = -1;
                    preloadedPokemonWaitFrames = 0;
                    preloadedPokemonAmount = 0;
                }
            }

            if (preloadedAssistId >= 0) {
                preloadedAssistWaitFrames++;
                if (preloadedAssistWaitFrames > MIN_PRELOADED_ASSIST_FRAMES) {
                    effectItemSpawn(preloadedAssistId, preloadedAssistAmount);
                    preloadedAssistId = -1;
                    preloadedAssistWaitFrames = 0;
                    preloadedAssistAmount = 0;
                }
            }

            int numPlayers = FIGHTER_MANAGER->getEntryCount();

            switch (effectRequest[0]) {
                case EFFECT_GIVE_DAMAGE:
                    exiStatus = effectGameGiveDamage(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ITEM_SPAWN_REGULAR:
                    exiStatus = effectItemSpawn(effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_ITEM_SPAWN_POKEMON:
                    if (preloadedPokemonId < 0) {
                        exiStatus = effectItemPreloadPokemon(effectRequest[1]);
                        preloadedPokemonId = effectRequest[1];
                        preloadedPokemonAmount = effectRequest[2];
                    }
                    else exiStatus = RESULT_EFFECT_RETRY;
                    break;
                case EFFECT_ITEM_SPAWN_ASSIST:
                    if (preloadedAssistId < 0) {
                        exiStatus = effectItemPreloadAssist(effectRequest[1]);
                        preloadedAssistId = effectRequest[1];
                        preloadedAssistAmount = effectRequest[2];
                    }
                    else exiStatus = RESULT_EFFECT_RETRY;
                    break;
                case EFFECT_ITEM_ATTACH_GOOEY:
                    exiStatus = effectItemAttachGooey(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_METAL:
                    exiStatus = effectStatusGiveMetal(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_STATUS_CURRY:
                    exiStatus = effectStatusGiveCurry(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_HAMMER:
                    exiStatus = effectStatusGiveHammer(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_SUPERSTAR:
                    exiStatus = effectStatusGiveSuperStar(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_FLOWER:
                    exiStatus = effectStatusGiveFlower(numPlayers, effectRequest[1], 1, effectRequest[2],
                                                       effectRequest[3]);
                    break;
                case EFFECT_STATUS_HEART:
                    exiStatus = effectStatusGiveHeart(numPlayers, effectRequest[1], effectRequest[2], 1);
                    break;
                case EFFECT_STATUS_SLOW:
                    exiStatus = effectStatusGiveSlow(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4]);
                    break;
                case EFFECT_STATUS_MUSHROOM:
                    exiStatus = effectStatusGiveMushroom(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_STATUS_EQUIP:
                    exiStatus = effectStatusGiveEquip(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_SWAP:
                    exiStatus = effectStatusGiveSwap(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4]);
                    break;
                case EFFECT_STATUS_FINALSMASH:
                    exiStatus = effectStatusGiveFinalSmash(numPlayers, effectRequest[1], 1);
                    break;
                case EFFECT_STATUS_ACTION:
                    exiStatus = effectActionChangeForce(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_NOT_CONNECTED:
                case EFFECT_NONE:
                case EFFECT_UNKNOWN:
                    break;
                default:
                    exiStatus = RESULT_EFFECT_FAILURE;
                    break;
            }

            if (preloadedAssistId < 0) {//preloadedPokemonId < 0) {

                if (padSystem->pads[0].buttons.LeftDPad) {
                    //effectStatusGiveCurry(numPlayers, 0, 0);
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 1);
                    //effectActionChangeForce(numPlayers, 0, 0x10C);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 0);
                    //effectStatusGiveSwap(4, 0, 1, 0, 720);
                    //effectItemSpawn(0x0, 1); //0x78, 1); // 0x2A - Pokeball

                    //effectItemPreloadPokemon(0x69); // Deoxys
                    //preloadedPokemonId = 0x69;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0x9D); // Little Mac
                    //preloadedAssistId = 0x9D;
                    //preloadedAssistAmount = 1;

                    //effectItemAttachGooey(numPlayers, 0, 1);
                } else if (padSystem->pads[0].buttons.RightDPad) {
                    //effectStatusGiveCurry(numPlayers, 0, 1);
                    //effectItemSpawn(0x2A, 1); //0x78, 1); // 0x2A - Pokeball
                    //effectStatusGiveEquip(numPlayers, 0, 0x32);
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 0);
                    //effectActionChangeForce(numPlayers, 0, 0xC7);
                    //effectGameGiveDamage(numPlayers, 0, (int)15, 0);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 1);
                    //effectStatusGiveSwap(4, 0, 1, 1, 720);

                    //effectItemPreloadPokemon(0x6C); // Staryu
                    //preloadedPokemonId = 0x6C;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0x96); // Hammer Bro
                    //preloadedAssistId = 0x96;
                    //preloadedAssistAmount = 2;

                } else if (padSystem->pads[0].buttons.UpDPad) {
                    //effectItemPreloadPokemon(0x84); // Suicune
                    //preloadedPokemonId = 0x84;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0xA2); // Isaac
                    //preloadedAssistId = 0xA2;
                    //preloadedAssistAmount = 1;

                } else if (padSystem->pads[0].buttons.DownDPad) {
                    //effectItemPreloadPokemon(0x66); // Entei
                    //preloadedPokemonId = 0x66;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0xAA); // Stafy
                    //preloadedAssistId = 0xAA;
                    //preloadedAssistAmount = 1;
                } else if (padSystem->pads[0].buttons.Z) {
                    //effectItemPreloadPokemon(0x64); // Chickorita
                    //preloadedPokemonId = 0x64;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0xAF); // Waluigi
                    //preloadedAssistId = 0xAF;
                    //preloadedAssistAmount = 1;
                } else if (padSystem->pads[0].buttons.B) {
                    //effectItemPreloadPokemon(0x6E); // Hooh
                    //preloadedPokemonId = 0x6E;
                    //preloadedPokemonAmount = 1;

                    //effectAssistSpawn(0x94, 1); // // Knuckle Joe
                    //preloadedAssistId = 0x94;
                    //preloadedAssistAmount = 1;
                }
                else if (padSystem->pads[0].buttons.R) {
                    //effectItemPreloadPokemon(0x7B); // Metagross
                    //preloadedPokemonId = 0x7B;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0x9F); // Nintendog
                    //preloadedAssistId = 0xAA;
                    //preloadedAssistAmount = 1;
                }
            }

            int capacity = ((int (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->capacity)(&ITEM_MANAGER->itKindArrayList_vtable);
            for (int i = 0; i < capacity; i++) {
                debugData->loadedPkmn[i] = 0;
            }

            int size = ((int (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->size)(&ITEM_MANAGER->itKindArrayList_vtable);
            for (int i = 0; i < size; i++) {
                int* pkmnPtr = ((int* (*)(void* it, int i)) ITEM_MANAGER->itKindArrayList_vtable->at)(&ITEM_MANAGER->itKindArrayList_vtable, i);
                //int* pkmnPtr = ((int* (*)(void* it, int i)) ITEM_MANAGER->itArchiveArrayList_vtable->at)(&ITEM_MANAGER->itArchiveArrayList_vtable, i);
                if (pkmnPtr != nullptr) debugData->loadedPkmn[i] = *pkmnPtr;
            }

        }

        if (exiStatus >= 5) {
            EXIPacket responsePckt = EXIPacket(exiStatus, nullptr, 0);
            responsePckt.Send();
        }


        startNormalDraw();
        if (visible) {
            printer.setup();
            printer.start2D();

            message->fontScaleY = RENDER_SCALE_Y;
            message->fontScaleX = RENDER_SCALE_X;
            printer.lineHeight = 20 * message->fontScaleY;
            message->xPos = LEFT_PADDING;
            message->yPos = TOP_PADDING;
            myMenu->render(&printer, buffer);
        }

        startNormalDraw();



    }

    //void onEndMatch() {

    //}

    // called at the end of the game logic in a frame
    //void EndFrame() {

    //}

    SIMPLE_INJECTION(startGame, 0x806dd5f4, "mr r3, r19") { SendGameStatus(EXIStatus::STATUS_GAME_STARTED); } // when booting up
    SIMPLE_INJECTION(startMatch, 0x800dc590, "li r9, 0x2") { SendGameStatus(EXIStatus::STATUS_MATCH_STARTED); } // when starting match
    SIMPLE_INJECTION(endMatch, 0x806d4844, "li r4, 0") { SendGameStatus(EXIStatus::STATUS_MATCH_ENDED); } // when exiting match
    /*INJECTION("frameUpdate", 0x8001792c, R"(
    bl onUpdateFrame
    addi r3, r30, 280
)");*/

    SIMPLE_INJECTION(beginFrame, 0x80147394, "li r0, 0x1") { onUpdateFrame(); }
    //SIMPLE_INJECTION(endFrame,   0x801473a0, "li r0, 0x0") { EndFrame(); }


    INJECTION("forceVisMemPool", 0x80025dc8, R"(
    cmpwi r3, 69
)");

}