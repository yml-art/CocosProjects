#include "GameController.h"

GameController::GameController()
    : _gameView(nullptr)
    , _modelGenerator(GameModelGenerator::getInstance())
{
    _undoManager.init(&_gameModel);
}

GameController::~GameController() {
    GameModelGenerator::destroyInstance();
}

void GameController::init(GameView* gameView) {
    _gameView = gameView;

    if (_gameView) {
        _gameView->setCardClickCallback([this](int cardId) {
            this->handleCardClick(cardId);
            });

        _gameView->setUndoBtnCallback([this]() {
            this->handleUndoClick();
            });

        log("GameController初始化完成");
    }
}

void GameController::startGame(const std::string& levelFileName) {
    LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig(levelFileName);
    log("加载关卡配置：主牌区数量=%d，备用牌堆数量=%d",
        levelConfig.getPlayfieldConfigs().size(),
        levelConfig.getStackConfigs().size());

    _gameModel = _modelGenerator->generateGameModel(levelConfig);

    if (_gameView) {
        _gameView->initGameView(_gameModel);
    }
}


void GameController::handleCardClick(int cardId) {
    log("处理卡牌点击 ID=%d", cardId);

    if (isStackCard(cardId)) {
        log("点击的是备用牌堆卡牌");
        handleStackCardFlip(cardId);
    }
    else {
        log("点击的是主牌区卡牌");
        handlePlayfieldCardMatch(cardId);
    }
}

void GameController::handleUndoClick() {
    log("执行回退操作");

    UndoModel* undoModel = UndoModel::create();

    if (_undoManager.undo(undoModel)) {
        log("回退成功，类型=%d", undoModel->getOperateType());

        if (_gameView) {
            // 获取需要回退的卡牌
            CardModel* srcCard = undoModel->getSrcCard();
            CardModel* destCard = undoModel->getDestCard();

            // 执行反向平移动画
            if (srcCard) {
                Vec2 srcOriginalPos = undoModel->getSrcCardOriginalPos();
                CardView* srcCardView = _gameView->getCardViewById(srcCard->getCardId());

                if (srcCardView) {
                    srcCardView->playMoveAnimation(srcOriginalPos);
                    log("回退源卡牌动画: 卡牌%d 到 (%.1f,%.1f)",
                        srcCard->getCardId(), srcOriginalPos.x, srcOriginalPos.y);
                }
            }

            if (destCard) {
                Vec2 destOriginalPos = undoModel->getDestCardOriginalPos();
                CardView* destCardView = _gameView->getCardViewById(destCard->getCardId());

                if (destCardView) {
                    destCardView->playMoveAnimation(destOriginalPos);
                    log("回退目标卡牌动画: 卡牌%d 到 (%.1f,%.1f)",
                        destCard->getCardId(), destOriginalPos.x, destOriginalPos.y);
                }
            }

            // 延迟更新整个视图
            auto delay = DelayTime::create(0.6f);
            auto update = CallFunc::create([this, undoModel]() {
                // 直接在这里恢复状态
                if (undoModel->getOperateType() == OT_MATCH_CARD) {
                    CardModel* srcCard = undoModel->getSrcCard();
                    CardModel* destCard = undoModel->getDestCard();

                    if (srcCard && destCard) {
                        // 将匹配的牌放回桌面
                        srcCard->setPosition(undoModel->getSrcCardOriginalPos());
                        srcCard->setFaceUp(true);
                        _gameModel.addPlayfieldCard(srcCard);

                        // 恢复原底部牌
                        destCard->setPosition(undoModel->getDestCardOriginalPos());
                        destCard->setFaceUp(true);
                        _gameModel.setBottomCard(destCard);

                        log("♦️3 已放回桌面原位置");
                    }
                }

                _gameView->initGameView(_gameModel);
                });
            _gameView->runAction(Sequence::create(delay, update, nullptr));
        }
    }
    else {
        log("提示：无回退记录可回退！");
    }
}


bool GameController::isStackCard(int cardId) const {
    
    const auto& stackCards = _gameModel.getStackCards();
    for (auto& card : stackCards) {
        if (card->getCardId() == cardId) {
            return true;
        }
    }
    return false;
}

void GameController::handleStackCardFlip(int cardId) {
    log("执行翻牌替换 - 卡牌ID: %d", cardId);

    // 获取当前底牌和备用牌
    CardModel* oldBottomCard = _gameModel.getBottomCard();
    if (!oldBottomCard) {
        log("错误：当前没有底牌");
        return;
    }

    // 从备用牌堆找到被点击的卡牌
    CardModel* newBottomCard = nullptr;
    auto& stackCards = _gameModel.getStackCards();

    // 方法1：使用索引
    int eraseIndex = -1;
    for (int i = 0; i < stackCards.size(); ++i) {
        CardModel* card = stackCards.at(i);
        if (card->getCardId() == cardId) {
            newBottomCard = card;
            newBottomCard->retain();
            eraseIndex = i;
            break;
        }
    }

    if (!newBottomCard || eraseIndex == -1) {
        log("错误：未找到备用牌堆中的卡牌 ID=%d", cardId);
        return;
    }

    //  记录原位置（用于回退）
    Vec2 newBottomOriginalPos = newBottomCard->getPosition();
    Vec2 oldBottomOriginalPos = oldBottomCard->getPosition();

    log("翻牌替换: 旧底牌=%d[%d], 新底牌=%d[%d]",
        oldBottomCard->getCardId(), oldBottomCard->getFace(),
        newBottomCard->getCardId(), newBottomCard->getFace());

  
    stackCards.erase(eraseIndex);

    //  设置新底牌位置（平移到顶部牌位置）
    newBottomCard->setPosition(oldBottomOriginalPos);
    newBottomCard->setFaceUp(true);

    // 更新模型
    
    _gameModel.setBottomCard(newBottomCard);
    newBottomCard->release(); 

    // 创建回退记录
    UndoModel* undoModel = UndoModel::create();
    undoModel->setOperateType(OT_FLIP_CARD);
    undoModel->setSrcCard(newBottomCard);
    undoModel->setDestCard(oldBottomCard);
    undoModel->setSrcCardOriginalPos(newBottomOriginalPos);
    undoModel->setDestCardOriginalPos(oldBottomOriginalPos);
    _undoManager.addUndoRecord(undoModel);

    // 执行平移动画并更新视图
    if (_gameView) {
        // 找到被点击的卡牌视图
        CardView* clickedCardView = nullptr;

        // 更高效的查找方式：直接通过ID查找
        clickedCardView = _gameView->getCardViewById(cardId);

        // 如果没找到，在备用牌堆视图中查找
        if (!clickedCardView) {
            auto stackCardViews = _gameView->getStackCardViews();
            for (auto& view : stackCardViews) {
                if (view->getCardId() == cardId) {
                    clickedCardView = view;
                    break;
                }
            }
        }

        if (clickedCardView) {
           
            auto moveAction = MoveTo::create(0.6f, oldBottomOriginalPos);
            auto easeAction = EaseSineOut::create(moveAction); 

            // 添加缩放或淡入效果
            auto fadeAction = FadeIn::create(0.3f);
            auto spawnAction = Spawn::create(easeAction, fadeAction, nullptr);

            // 动画完成后的回调
            auto updateAction = CallFunc::create([this, cardId, clickedCardView]() {
                log("翻牌动画完成: 卡牌%d", cardId);

              
                _gameView->initGameView(_gameModel);

                
                });

            
            clickedCardView->runAction(Sequence::create(spawnAction, updateAction, nullptr));

            log("执行翻牌平移动画: 卡牌%d 从(%.1f,%.1f) 到 (%.1f,%.1f)",
                cardId, newBottomOriginalPos.x, newBottomOriginalPos.y,
                oldBottomOriginalPos.x, oldBottomOriginalPos.y);
        }
        else {
            log("警告：未找到卡牌%d的视图，直接更新界面", cardId);

            
            if (auto scene = Director::getInstance()->getRunningScene()) {
                // 创建临时卡牌精灵用于动画
                auto tempSprite = Sprite::create("card_back.png");
                if (tempSprite) {
                    tempSprite->setPosition(newBottomOriginalPos);
                    tempSprite->setScale(0.8f);
                    scene->addChild(tempSprite, 1000);

                    // 动画
                    auto moveAction = MoveTo::create(0.4f, oldBottomOriginalPos);
                    auto fadeAction = FadeOut::create(0.2f);
                    auto removeAction = CallFunc::create([tempSprite]() {
                        tempSprite->removeFromParent();
                        });

                    tempSprite->runAction(Sequence::create(
                        moveAction,
                        DelayTime::create(0.1f),
                        fadeAction,
                        removeAction,
                        nullptr
                    ));
                }
            }

            
            _gameView->initGameView(_gameModel);
        }
    }
}

void GameController::handlePlayfieldCardMatch(int cardId) {
    CardModel* playfieldCard = _gameModel.getPlayfieldCard(cardId);
    CardModel* currentBottomCard = _gameModel.getBottomCard();

    if (!playfieldCard || !currentBottomCard) {
        log("错误：卡牌不存在");
        return;
    }

    if (!CommonUtils::isPointDiffOne(playfieldCard->getFace(), currentBottomCard->getFace())) {
        log("提示：点数不匹配（需差1），当前底牌点数=%d，点击卡牌点数=%d",
            currentBottomCard->getFace() + 1, playfieldCard->getFace() + 1);
        return;
    }

    // 记录原位置
    Vec2 playfieldOriginalPos = playfieldCard->getPosition();
    Vec2 bottomOriginalPos = currentBottomCard->getPosition();

    log("卡牌匹配: 桌面卡牌=%d[%d], 手牌区顶部牌=%d[%d]",
        playfieldCard->getCardId(), playfieldCard->getFace() + 1,
        currentBottomCard->getCardId(), currentBottomCard->getFace() + 1);

    
    currentBottomCard->retain();  
   
    playfieldCard->retain();

    // 从桌面牌区移除该卡牌
    _gameModel.removePlayfieldCard(cardId);  

    // 将桌面牌的位置设置为手牌区顶部牌的位置（平移动画目标位置）
    
    playfieldCard->setPosition(bottomOriginalPos);
    _gameModel.setBottomCard(playfieldCard);

    
    playfieldCard->release();  
    currentBottomCard->release();  

    

    // 创建回退记录
    UndoModel* undoModel = UndoModel::create();
    undoModel->setOperateType(OT_MATCH_CARD);
    undoModel->setSrcCard(playfieldCard);           // 源卡牌：被点击的桌面牌
    undoModel->setDestCard(currentBottomCard);      // 目标卡牌：原手牌区顶部牌
    undoModel->setSrcCardOriginalPos(playfieldOriginalPos);
    undoModel->setDestCardOriginalPos(bottomOriginalPos);

    _undoManager.addUndoRecord(undoModel);

   
    if (_gameView) {
      
        CardView* clickedCardView = _gameView->getCardViewById(cardId);

        if (clickedCardView) {
            // 执行平移动画到手牌区顶部牌位置
            auto moveAction = MoveTo::create(0.4f, bottomOriginalPos);
            auto easeMove = EaseSineOut::create(moveAction);

            // 添加匹配特效：缩放和旋转
            auto scaleUp = ScaleTo::create(0.1f, 1.1f);
            auto scaleDown = ScaleTo::create(0.1f, 1.0f);
            auto scaleSequence = Sequence::create(scaleUp, scaleDown, nullptr);

            // 轻微旋转效果
            auto rotateAction = RotateBy::create(0.3f, 180);
            auto easeRotate = EaseBackOut::create(rotateAction);

            // 组合动画
            auto spawnAction = Spawn::create(
                easeMove,
                easeRotate,
                scaleSequence,
                nullptr
            );

            // 动画完成后的回调
            auto completeAction = CallFunc::create([this, cardId, clickedCardView]() {
                log("匹配动画完成: 卡牌%d", cardId);

                // 更新视图
                _gameView->initGameView(_gameModel);

               
                });

            clickedCardView->runAction(Sequence::create(spawnAction, completeAction, nullptr));

            log("执行匹配平移动画: 卡牌%d 从(%.1f,%.1f) 到 (%.1f,%.1f)",
                cardId, playfieldOriginalPos.x, playfieldOriginalPos.y,
                bottomOriginalPos.x, bottomOriginalPos.y);
        }
        else {
          
            log("警告：未找到卡牌%d的视图，直接更新界面", cardId);
            _gameView->initGameView(_gameModel);
        }
    }
}