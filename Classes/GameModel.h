#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CardModel.h"
#include "UndoModel.h"
USING_NS_CC;

class GameModel {
public:
    GameModel();
   
    ~GameModel();

    ValueMap toValueMap() const;
    void fromValueMap(const ValueMap& map);

    void addPlayfieldCard(CardModel* card);
    void removePlayfieldCard(int cardId);
    CardModel* getPlayfieldCard(int cardId) const;
    Vector<CardModel*> getPlayfieldCards() const { return _playfieldCards; }

    void addStackCard(CardModel* card);
    CardModel* popStackCard();
    Vector<CardModel*>& getStackCards() { return _stackCards; }
    const Vector<CardModel*>& getStackCards() const { return _stackCards; }
    bool isStackEmpty() const { return _stackCards.empty(); }

    void setBottomCard(CardModel* card);
    CardModel* getBottomCard() const { return _bottomCard; }

private:
    Vector<CardModel*> _playfieldCards;
    Vector<CardModel*> _stackCards;
    CardModel* _bottomCard;
    int _nextCardId;
};

#endif // __GAME_MODEL_H__


