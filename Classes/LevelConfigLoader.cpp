
#include "LevelConfigLoader.h"

LevelConfig LevelConfigLoader::loadLevelConfig(const std::string& levelFileName) {
    LevelConfig config;

    
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(levelFileName);
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(filePath);
    if (jsonContent.empty()) {
        log("错误：未找到关卡配置文件 -> %s", levelFileName.c_str());
        return config;
    }

   
    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());
    if (doc.HasParseError()) {
        log("错误：JSON解析失败，错误码=%d，偏移量=%u", doc.GetParseError(), doc.GetErrorOffset());
        return config;
    }

    
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfieldArr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); ++i) {
            CardConfig* cardConfig = CardConfig::create(); // 指针创建
            parseCardConfig(playfieldArr[i], cardConfig);
            config.addPlayfieldConfig(cardConfig);
        }
    }

   
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stackArr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArr.Size(); ++i) {
            CardConfig* cardConfig = CardConfig::create();
            parseCardConfig(stackArr[i], cardConfig);
            config.addStackConfig(cardConfig);
        }
    }

    return config;
}


void LevelConfigLoader::parseCardConfig(const rapidjson::Value& jsonVal, CardConfig* config) {
    if (!config) return; 

   
    if (jsonVal.HasMember("CardFace") && jsonVal["CardFace"].IsInt()) {
        config->setFace((CardFaceType)jsonVal["CardFace"].GetInt());
    }

    
    if (jsonVal.HasMember("CardSuit") && jsonVal["CardSuit"].IsInt()) {
        config->setSuit((CardSuitType)jsonVal["CardSuit"].GetInt());
    }

  
    if (jsonVal.HasMember("Position") && jsonVal["Position"].IsObject()) {
        const rapidjson::Value& posObj = jsonVal["Position"];
        Vec2 pos(
            posObj.HasMember("x") ? posObj["x"].GetInt() : 0,
            posObj.HasMember("y") ? posObj["y"].GetInt() : 0
        );
        config->setPosition(pos);
    }
}


