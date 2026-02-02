#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "CommonDef.h"
USING_NS_CC;


class CardModel : public Ref {
public:

    static CardModel* create();

    
    ValueMap toValueMap() const;
    void fromValueMap(const ValueMap& map);

    
    int getCardId() const { return _cardId; }
    void setCardId(int id) { _cardId = id; }

    CardSuitType getSuit() const { return _suit; }
    void setSuit(CardSuitType suit) { _suit = suit; }

    CardFaceType getFace() const { return _face; }
    void setFace(CardFaceType face) { _face = face; }

    Vec2 getPosition() const { return _position; }
    void setPosition(const Vec2& pos) { _position = pos; }


    bool isFaceUp() const { return _isFaceUp; }
    void setFaceUp(bool isUp) { _isFaceUp = isUp; }

protected:
    
    CardModel();
    ~CardModel() {};

private:
    int _cardId;          // 唯一ID
    CardSuitType _suit;   // 花色
    CardFaceType _face;   // 点数
    Vec2 _position;       // 位置
    bool _isFaceUp;       // 是否翻面
};

#endif // __CARD_MODEL_H__


