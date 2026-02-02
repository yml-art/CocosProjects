#ifndef __GAME_MODEL_GENERATOR_H__
#define __GAME_MODEL_GENERATOR_H__

#include "cocos2d.h"
#include "GameModel.h"
#include "LevelConfig.h"
USING_NS_CC;

class GameModelGenerator {
public:
    static GameModelGenerator* getInstance();
    static void destroyInstance(); 

    GameModel generateGameModel(const LevelConfig& levelConfig);

private:
    GameModelGenerator() {};
    ~GameModelGenerator() {};
    static GameModelGenerator* _instance;
};

#endif // __GAME_MODEL_GENERATOR_H__


