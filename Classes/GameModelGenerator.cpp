#include "GameModelGenerator.h"

GameModelGenerator* GameModelGenerator::_instance = nullptr;

GameModelGenerator* GameModelGenerator::getInstance() {
    if (_instance == nullptr) {
        _instance = new GameModelGenerator();
    }
    return _instance;
}

void GameModelGenerator::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}
GameModel GameModelGenerator::generateGameModel(const LevelConfig& levelConfig) {
    GameModel gameModel;

    // 1. 生成主牌区卡牌
    auto playfieldConfigs = levelConfig.getPlayfieldConfigs();
    for (auto& config : playfieldConfigs) {
        CardModel* card = CardModel::create(); 
        card->setSuit(config->getSuit());      
        card->setFace(config->getFace());
        card->setPosition(config->getPosition());
        card->setFaceUp(true);
        gameModel.addPlayfieldCard(card); 
    }

    // 2. 生成备用牌堆卡牌
    auto stackConfigs = levelConfig.getStackConfigs();
    for (auto& config : stackConfigs) {
        CardModel* card = CardModel::create();
        card->setSuit(config->getSuit());
        card->setFace(config->getFace());
        card->setPosition(config->getPosition());
       
        card->setFaceUp(true);
        gameModel.addStackCard(card);
    }

   
    if (!gameModel.isStackEmpty()) {
        CardModel* bottomCard = gameModel.popStackCard();
        gameModel.setBottomCard(bottomCard);
    }

    return gameModel;
}


