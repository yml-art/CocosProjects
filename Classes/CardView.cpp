#include "CardView.h"
#include "CommonUtils.h"

USING_NS_CC;

CardView* CardView::create(CardModel* cardModel) {
    CardView* view = new (std::nothrow) CardView();
    if (view && view->init(cardModel)) {
        view->autorelease();
        return view;
    }
    delete view;
    view = nullptr;
    return nullptr;
}

bool CardView::init(CardModel* cardModel) {
    if (!Sprite::init() || !cardModel) return false;

    _cardId = cardModel->getCardId();
    this->setContentSize(Size(200, 300)); 
    this->updateView(cardModel);

    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

    return true;
}


void CardView::updateView(CardModel* cardModel) {
    if (!cardModel) return;

    
    this->removeAllChildrenWithCleanup(true);

    if (cardModel->isFaceUp()) {
        // ----------- 卡牌正面布局--------------
        
        std::string generalPath = "card_general.png"; 
        Sprite* generalSprite = Sprite::create(generalPath);
        if (generalSprite) {
            generalSprite->setAnchorPoint(Vec2::ZERO); 
          
            generalSprite->setScale(
                200.0f / generalSprite->getContentSize().width,
                300.0f / generalSprite->getContentSize().height
            );
            this->addChild(generalSprite, 0); 
        }
        else {
            log("警告：卡牌正面底色图加载失败，路径=%s", generalPath.c_str());
            
            auto grayBg = LayerColor::create(Color4B(240, 240, 240, 255), 200, 300);
            this->addChild(grayBg, 0);
        }

       
        CardSuitType suit = cardModel->getSuit();
        std::string suitPath = CommonUtils::getSuitImagePath(suit);
        Sprite* suitSprite = Sprite::create(suitPath);
        if (suitSprite) {
            suitSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT); 
            suitSprite->setPosition(Vec2(200 - 20, 300 - 20)); 
            suitSprite->setScale(0.5f); 
            // 按花色设置颜色（梅花/黑桃=黑，方块/红桃=红）
            if (suit == CST_CLUBS || suit == CST_SPADES) {
                suitSprite->setColor(Color3B::BLACK);
            }
            else {
                suitSprite->setColor(Color3B::RED);
            }
            this->addChild(suitSprite, 2); 
        }
        else {
            log("警告：花色图标加载失败，路径=%s", suitPath.c_str());
        }

        
        CardFaceType face = cardModel->getFace();
        std::string bigNumberPath = CommonUtils::getNumberImagePath(suit, face);
        Sprite* bigNumberSprite = Sprite::create(bigNumberPath);
        if (bigNumberSprite) {
            bigNumberSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE); 
            bigNumberSprite->setPosition(Vec2(100, 150)); 
            bigNumberSprite->setScale(1.2f); 
            this->addChild(bigNumberSprite, 1); 
        }
        else {
            log("警告：大点数图加载失败，路径=%s", bigNumberPath.c_str());
        }

       
        std::string smallNumberPath = StringUtils::format(
            "number/small_%s_%s.png",
            (suit == CST_CLUBS || suit == CST_SPADES) ? "black" : "red",
            CommonUtils::getFaceStr(face).c_str() // 
        );
        Sprite* smallNumberSprite = Sprite::create(smallNumberPath);
        if (smallNumberSprite) {
            smallNumberSprite->setAnchorPoint(Vec2::ZERO); 
            smallNumberSprite->setPosition(Vec2(10, 300 - 10 - smallNumberSprite->getContentSize().height)); // 左上角偏移10像素
            smallNumberSprite->setScale(0.4f); 
            this->addChild(smallNumberSprite, 2); 
        }
        else {
            log("警告：小点数图加载失败，路径=%s", smallNumberPath.c_str());
        }

    }
    else {
        std::string backPath = "card_back_blue.png";
        Sprite* backSprite = Sprite::create(backPath);
        if (backSprite) {
            backSprite->setAnchorPoint(Vec2::ZERO);
            backSprite->setScale(200.0f / backSprite->getContentSize().width, 300.0f / backSprite->getContentSize().height);
            backSprite->setPosition(Vec2::ZERO);
            this->addChild(backSprite, 0);
            log("背面渲染 - 成功加载底图：%s", backPath.c_str());
        }
        else {
           
            log("背面渲染 - 底图缺失，使用蓝色备用背景");
            auto blueBack = LayerColor::create(Color4B(0, 0, 255, 255), 200, 300);
            this->addChild(blueBack, 0);
        }
       
    }

    
    this->setPosition(cardModel->getPosition());
}

bool CardView::onTouchBegan(Touch* touch, Event* event) {
    if (!touch || !event) return false;
    Vec2 touchPos = this->convertTouchToNodeSpace(touch);
    Rect cardRect = Rect(0, 0, 200, 300); // 统一卡牌碰撞区域
    return cardRect.containsPoint(touchPos);
}

void CardView::onTouchEnded(Touch* touch, Event* event) {
    if (!touch || !event || !_clickCallback) return;
    _clickCallback(_cardId);
}

void CardView::playMoveAnimation(const Vec2& targetPos, float duration) {
    this->stopAllActions();
    auto moveTo = MoveTo::create(duration, targetPos);
    this->runAction(moveTo);
}


