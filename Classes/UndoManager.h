#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "UndoModel.h"
#include "GameModel.h" 
USING_NS_CC;

class UndoManager {
public:
    UndoManager();
    ~UndoManager() {};

  
    void init(GameModel* gameModel) { _gameModel = gameModel; }

 
    void addUndoRecord(UndoModel* undoModel);

  
    bool undo(UndoModel* outUndoModel);

  
    bool hasUndoRecord() const { return !_undoRecords.empty(); }

private:
    Vector<UndoModel*> _undoRecords;
    GameModel* _gameModel;
 
    bool isStackCard(int cardId) const;
};

#endif // __UNDO_MANAGER_H__


