#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
#include "CardView.h"
#include "GameModel.h"

USING_NS_CC;
using namespace ui;

class GameView : public Layer {
public:
    static GameView* create();
    virtual bool init() override;

    void initGameView(const GameModel& gameModel);

    // 视图管理方法
    void addPlayfieldCardView(CardView* cardView);
    void setBottomCardView(CardView* cardView);
    void addStackCardView(CardView* cardView);
    void removePlayfieldCardView(int cardId);
    

    // 动画方法
    void playMatchAnimation(int srcCardId, const Vec2& targetPos);
    void playUndoAnimation(int cardId, const Vec2& targetPos);

    // 回调设置接口
    void setUndoBtnCallback(const std::function<void()>& callback);
    void setCardClickCallback(const std::function<void(int)>& callback);


    Vector<CardView*> getPlayfieldCardViews() const { return _playfieldCardViews; }
    Vector<CardView*> getStackCardViews() const { return _stackCardViews; }
    CardView* getBottomCardView() const { return _bottomCardView; }

    // 通过ID获取卡牌视图
    CardView* getCardViewById(int cardId) {
        // 搜索主牌区
        for (auto& cardView : _playfieldCardViews) {
            if (cardView->getCardId() == cardId) {
                return cardView;
            }
        }

        // 搜索备用牌堆
        for (auto& cardView : _stackCardViews) {
            if (cardView->getCardId() == cardId) {
                return cardView;
            }
        }

        // 搜索底牌
        if (_bottomCardView && _bottomCardView->getCardId() == cardId) {
            return _bottomCardView;
        }

        return nullptr;
    }

    
    void* getCardClickCallbackTarget() { return _cardClickCallback.target<void(int)>(); }
    void* getUndoBtnCallbackTarget() { return _undoBtnCallback.target<void()>(); }


private:
    void createUndoButton();

   
    std::function<void()> _undoBtnCallback;
    std::function<void(int)> _cardClickCallback;
    Vector<CardView*> _playfieldCardViews;
    Vector<CardView*> _stackCardViews;
    CardView* _bottomCardView;
};

#endif // GAME_VIEW_H
