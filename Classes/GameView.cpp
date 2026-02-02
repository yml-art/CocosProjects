#include "GameView.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CardView.h"
#include "GameModel.h"

USING_NS_CC;
using namespace ui;




GameView* GameView::create() {
    GameView* view = new (std::nothrow) GameView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool GameView::init() {
    if (!Layer::init()) return false;

  
    _undoBtnCallback = nullptr;
    _cardClickCallback = nullptr;
    _bottomCardView = nullptr;

   
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto gameBg = LayerColor::create(Color4B(50, 100, 50, 255), visibleSize.width, visibleSize.height);
    this->addChild(gameBg, 0);

    createUndoButton(); 

    return true;
}


void GameView::initGameView(const GameModel& gameModel) {
    
    _playfieldCardViews.clear();
    _stackCardViews.clear();

    if (_bottomCardView) {
        _bottomCardView->removeFromParentAndCleanup(true);
        _bottomCardView = nullptr;
    }

    this->removeAllChildrenWithCleanup(true);

    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto gameBg = LayerColor::create(Color4B(50, 100, 50, 255), visibleSize.width, visibleSize.height);
    this->addChild(gameBg, 0);

    createUndoButton(); 

  
    auto playfieldCards = gameModel.getPlayfieldCards();
    for (auto& cardModel : playfieldCards) {
        if (!cardModel) continue;

        auto cardView = CardView::create(cardModel);
        cardView->setCardClickCallback(_cardClickCallback);

       
        Vec2 originalPos = cardModel->getPosition();
        Vec2 shiftedPos = Vec2(originalPos.x, originalPos.y + 400);
        cardView->setPosition(shiftedPos);

        this->addPlayfieldCardView(cardView);
    }

    
    auto bottomCardModel = gameModel.getBottomCard();
    if (bottomCardModel) {
        bottomCardModel->setPosition(Vec2(740, 500)); 
        auto bottomCardView = CardView::create(bottomCardModel);
        this->setBottomCardView(bottomCardView);
    }

    
    auto stackCards = gameModel.getStackCards();
    for (int i = 0; i < stackCards.size(); ++i) {
        auto cardModel = stackCards.at(i);
        if (!cardModel) continue;

        Vec2 stackPos = Vec2(300 + i * 150, 500); 
        cardModel->setPosition(stackPos);

        auto cardView = CardView::create(cardModel);
        cardView->setCardClickCallback(_cardClickCallback);
        this->addStackCardView(cardView);
    }
}


void GameView::addPlayfieldCardView(CardView* cardView) {
    if (cardView) {
        _playfieldCardViews.pushBack(cardView);
        this->addChild(cardView, 1);
    }
}

void GameView::setBottomCardView(CardView* cardView) {
    if (_bottomCardView) {
        _bottomCardView->removeFromParentAndCleanup(true);
        _bottomCardView = nullptr;
    }
    _bottomCardView = cardView;
    if (_bottomCardView) {
        this->addChild(_bottomCardView, 2); 
    }
}

void GameView::addStackCardView(CardView* cardView) {
    if (cardView) {
        _stackCardViews.pushBack(cardView);
        this->addChild(cardView, 1); 
    }
}

void GameView::removePlayfieldCardView(int cardId) {
    for (auto it = _playfieldCardViews.begin(); it != _playfieldCardViews.end(); ++it) {
        if ((*it)->getCardId() == cardId) {
            (*it)->removeFromParentAndCleanup(true);
            _playfieldCardViews.erase(it);
            break;
        }
    }
}


//  回退按钮创建
void GameView::createUndoButton() {
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 totalBtnPos = Vec2(visibleSize.width - 400, 200);

    auto btnBg = LayerColor::create(Color4B(100, 50, 50, 255), 150, 80);
    btnBg->setPosition(totalBtnPos);
    btnBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnBg, 10);

    auto undoText = Sprite::create("undo_text.png");
    if (undoText) {
        Size bgSize = btnBg->getContentSize();
        Vec2 bgCenter = Vec2(bgSize.width / 2, bgSize.height / 2);
        undoText->setPosition(bgCenter);
        undoText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        float scaleW = bgSize.width * 0.8 / undoText->getContentSize().width;
        float scaleH = bgSize.height * 0.8 / undoText->getContentSize().height;
        float fitScale = MIN(scaleW, scaleH);
        undoText->setScale(fitScale);
        btnBg->addChild(undoText);
    }

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [btnBg](Touch* touch, Event* event) {
        Vec2 touchPos = btnBg->convertTouchToNodeSpace(touch);
        Size bgSize = btnBg->getContentSize();
        Rect bgRect = Rect(0, 0, bgSize.width, bgSize.height);
        return bgRect.containsPoint(touchPos);
        };

    touchListener->onTouchEnded = [this](Touch* touch, Event* event) {
        log("回退按钮触摸触发");
        if (_undoBtnCallback) {
            _undoBtnCallback();
            log("回退按钮回调执行成功");
        }
        else {
            log("警告：回退按钮回调未绑定");
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnBg);
    log("回退按钮创建成功 - 位置: (%.1f, %.1f)", totalBtnPos.x, totalBtnPos.y);
}

// 动画方法
void GameView::playMatchAnimation(int srcCardId, const Vec2& targetPos) {
    for (auto& cardView : _playfieldCardViews) {
        if (cardView->getCardId() == srcCardId) {
            auto moveTo = MoveTo::create(0.5f, targetPos);
            auto removeFunc = CallFunc::create([this, srcCardId]() {
                this->removePlayfieldCardView(srcCardId);
                });
            cardView->runAction(Sequence::create(moveTo, removeFunc, nullptr));
            break;
        }
    }
}

void GameView::playUndoAnimation(int cardId, const Vec2& targetPos) {
    // 1. 主牌区卡牌回退
    for (auto& cardView : _playfieldCardViews) {
        if (cardView->getCardId() == cardId) {
            cardView->playMoveAnimation(targetPos);
            break;
        }
    }
    // 2. 底牌回退
    if (_bottomCardView && _bottomCardView->getCardId() == cardId) {
        _bottomCardView->playMoveAnimation(targetPos);
    }
    // 3. 备用牌堆卡牌回退
    for (auto& cardView : _stackCardViews) {
        if (cardView->getCardId() == cardId) {
            cardView->playMoveAnimation(targetPos);
            break;
        }
    }
}


void GameView::setUndoBtnCallback(const std::function<void()>& callback) {
    _undoBtnCallback = callback;
}

void GameView::setCardClickCallback(const std::function<void(int)>& callback) {
    _cardClickCallback = callback;
}

