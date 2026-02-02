#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "CommonDef.h"
USING_NS_CC;

class CardConfig : public Ref {
public:
    
    static CardConfig* create();

    
    CardFaceType getFace() const { return _face; }
    void setFace(CardFaceType face) { _face = face; }

    CardSuitType getSuit() const { return _suit; }
    void setSuit(CardSuitType suit) { _suit = suit; }

    Vec2 getPosition() const { return _position; }
    void setPosition(const Vec2& pos) { _position = pos; }

protected:
    CardConfig()
        : _face(CFT_NONE)
        , _suit(CST_NONE)
        , _position(Vec2::ZERO)
    {
    };
    ~CardConfig() {};

private:
    CardFaceType _face;     // 点数
    CardSuitType _suit;     // 花色
    Vec2 _position;         // 初始位置
};


class LevelConfig {
public:
  
    Vector<CardConfig*> getPlayfieldConfigs() const { return _playfieldConfigs; }
    void addPlayfieldConfig(CardConfig* config) { _playfieldConfigs.pushBack(config); }

   
    Vector<CardConfig*> getStackConfigs() const { return _stackConfigs; }
    void addStackConfig(CardConfig* config) { _stackConfigs.pushBack(config); }

private:
    Vector<CardConfig*> _playfieldConfigs; 
    Vector<CardConfig*> _stackConfigs;     
};

#endif // __LEVEL_CONFIG_H__


