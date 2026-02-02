#include "GameModel.h"

GameModel::GameModel()
    : _nextCardId(1)
    , _bottomCard(nullptr) 
{
}


GameModel::~GameModel() {
    if (_bottomCard != nullptr) {
        _bottomCard->release();
        _bottomCard = nullptr;
    }
}

ValueMap GameModel::toValueMap() const {
    ValueMap map;

   
    ValueVector playfieldVec;
    for (auto& card : _playfieldCards) {
        playfieldVec.push_back(Value(card->toValueMap()));
    }
    map["playfieldCards"] = playfieldVec;

   
    ValueVector stackVec;
    for (auto& card : _stackCards) {
        stackVec.push_back(Value(card->toValueMap()));
    }
    map["stackCards"] = stackVec;

  
    if (_bottomCard) {
        map["bottomCard"] = _bottomCard->toValueMap();
    }
    map["nextCardId"] = _nextCardId;

    return map;
}

void GameModel::fromValueMap(const ValueMap& map) {
   
    _playfieldCards.clear();
    _stackCards.clear();
    if (_bottomCard) {
        _bottomCard->release();
        _bottomCard = nullptr;
    }

    // 主牌区反序列化
    ValueVector playfieldVec = map.at("playfieldCards").asValueVector();
    for (auto& val : playfieldVec) {
        CardModel* card = CardModel::create();
        card->fromValueMap(val.asValueMap());
        _playfieldCards.pushBack(card);
    }

    // 备用牌堆反序列化
    ValueVector stackVec = map.at("stackCards").asValueVector();
    for (auto& val : stackVec) {
        CardModel* card = CardModel::create();
        card->fromValueMap(val.asValueMap());
        _stackCards.pushBack(card);
    }

    // 底牌反序列化
    if (map.find("bottomCard") != map.end()) {
        _bottomCard = CardModel::create();
        _bottomCard->fromValueMap(map.at("bottomCard").asValueMap());
    }
    _nextCardId = map.at("nextCardId").asInt();
}

// 主牌区添加
void GameModel::addPlayfieldCard(CardModel* card) {
    if (!card) return;
    card->setCardId(_nextCardId++);
    _playfieldCards.pushBack(card); 
}

// 主牌区移除
void GameModel::removePlayfieldCard(int cardId) {
    for (auto it = _playfieldCards.begin(); it != _playfieldCards.end(); ++it) {
        if ((*it)->getCardId() == cardId) {
            _playfieldCards.erase(it); 
            break;
        }
    }
}

// 主牌区获取
CardModel* GameModel::getPlayfieldCard(int cardId) const {
    for (auto& card : _playfieldCards) {
        if (card->getCardId() == cardId) {
            return card;
        }
    }
    return nullptr; 
}

// 备用牌堆添加
void GameModel::addStackCard(CardModel* card) {
    if (!card) return;
    card->setCardId(_nextCardId++);

    _stackCards.pushBack(card);
}

// 备用牌堆弹出
CardModel* GameModel::popStackCard() {
    if (_stackCards.empty()) {
        return nullptr;
    }
    CardModel* card = _stackCards.back();
    _stackCards.popBack(); 
    card->retain();
    card->setFaceUp(true);
    return card;
}

// 底牌设置
void GameModel::setBottomCard(CardModel* card) {
    if (_bottomCard) {
        _bottomCard->release();
        _bottomCard = nullptr;
    }
    _bottomCard = card;
    if (_bottomCard) {
        _bottomCard->retain();
        _bottomCard->setFaceUp(true);
    }
}


