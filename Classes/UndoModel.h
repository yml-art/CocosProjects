#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CardModel.h"
USING_NS_CC;

class UndoModel : public Ref {
public:
    static UndoModel* create();

   
    Vec2 getSrcCardOriginalPos() const { return _srcCardOriginalPos; }
    void setSrcCardOriginalPos(const Vec2& pos) { _srcCardOriginalPos = pos; }
    Vec2 getDestCardOriginalPos() const { return _destCardOriginalPos; }
    void setDestCardOriginalPos(const Vec2& pos) { _destCardOriginalPos = pos; }

   
    OperateType getOperateType() const { return _operateType; }
    void setOperateType(OperateType type) { _operateType = type; }
    CardModel* getSrcCard() const { return _srcCard; }
    void setSrcCard(CardModel* card) {
        if (_srcCard) _srcCard->release();
        _srcCard = card;
        if (_srcCard) _srcCard->retain();
    }
    CardModel* getDestCard() const { return _destCard; }
    void setDestCard(CardModel* card) {
        if (_destCard) _destCard->release();
        _destCard = card;
        if (_destCard) _destCard->retain();
    }

protected:
    UndoModel();
    ~UndoModel() {
        if (_srcCard) _srcCard->release();
        if (_destCard) _destCard->release();
    };

private:
    OperateType _operateType;       // 操作类型（翻牌/匹配）
    CardModel* _srcCard;            // 源卡牌（被点击的牌：♥A/♦3/♠2）
    CardModel* _destCard;           // 目标卡牌（被替换的底牌：♣4/♦3/♥A）
    Vec2 _srcCardOriginalPos;       // 源卡牌原位置（回退时要移回的位置）
    Vec2 _destCardOriginalPos;      // 目标卡牌原位置（回退时恢复的位置）
};

#endif // __UNDO_MODEL_H__
