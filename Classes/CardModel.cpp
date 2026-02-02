#include "CardModel.h"


CardModel* CardModel::create() {
    CardModel* pRet = new (std::nothrow) CardModel();
    
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

CardModel::CardModel()
    : _cardId(-1)          // 初始ID=-1（未分配）
    , _suit(CST_NONE)      // 初始无花色
    , _face(CFT_NONE)      // 初始无点数
    , _position(Vec2::ZERO)// 初始位置(0,0)
    , _isFaceUp(false)     // 初始背面朝上
{
}

ValueMap CardModel::toValueMap() const {
    ValueMap map;
    map["cardId"] = _cardId;
    map["suit"] = (int)_suit;
    map["face"] = (int)_face;
    map["posX"] = _position.x;
    map["posY"] = _position.y;
    map["isFaceUp"] = _isFaceUp;
    return map;
}

void CardModel::fromValueMap(const ValueMap& map) {
    _cardId = map.at("cardId").asInt();
    _suit = (CardSuitType)map.at("suit").asInt();
    _face = (CardFaceType)map.at("face").asInt();
    _position.x = map.at("posX").asFloat();
    _position.y = map.at("posY").asFloat();
    _isFaceUp = map.at("isFaceUp").asBool();
}

