#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "GameModel.h"
#include "GameView.h"
#include "UndoManager.h"
#include "LevelConfigLoader.h"
#include "GameModelGenerator.h"
USING_NS_CC;

class GameController {
public:
    GameController();
    ~GameController();

    void init(GameView* gameView);
    void startGame(const std::string& levelFileName = "level1.json");
    void handleCardClick(int cardId);
    void handleUndoClick();

private:
    bool isStackCard(int cardId) const;
    void handleStackCardFlip(int cardId);
    void handlePlayfieldCardMatch(int cardId);


    GameModel _gameModel;
    GameView* _gameView;
    UndoManager _undoManager;
    GameModelGenerator* _modelGenerator;
};

#endif // __GAME_CONTROLLER_H__


