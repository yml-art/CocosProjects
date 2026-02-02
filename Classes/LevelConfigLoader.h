#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "cocos2d.h"
#include "LevelConfig.h"
#include "json/document.h"
USING_NS_CC;


class LevelConfigLoader {
public:
    static LevelConfig loadLevelConfig(const std::string& levelFileName);

private:
    static void parseCardConfig(const rapidjson::Value& jsonVal, CardConfig* config);
};

#endif // __LEVEL_CONFIG_LOADER_H__
