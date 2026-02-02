#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "CardModel.h"
#include "CommonUtils.h"
USING_NS_CC;

typedef std::function<void(int cardId)> CardClickCallback;

class CardView : public Sprite {
public:
    
    static CardView* create(CardModel* cardModel);
    virtual bool init(CardModel* cardModel);

    void setCardClickCallback(const CardClickCallback& callback) { _clickCallback = callback; }
    void updateView(CardModel* cardModel); 
    void playMoveAnimation(const Vec2& targetPos, float duration = 0.5f);
    int getCardId() const { return _cardId; }


private:
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

    int _cardId;
    CardClickCallback _clickCallback;
    EventListenerTouchOneByOne* _touchListener;
};

#endif // __CARD_VIEW_H__


