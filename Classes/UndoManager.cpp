
#include "UndoManager.h"
UndoManager::UndoManager()
    : _gameModel(nullptr)
{
}

// 1. 添加回退记录
void UndoManager::addUndoRecord(UndoModel* undoModel) {
    
    if (undoModel == nullptr || _gameModel == nullptr) {
        log("警告：UndoModel 或 GameModel 为空，无法添加回退记录");
        return;
    }

    // 记录操作前的原位置
    if (undoModel->getSrcCard()) {
        undoModel->setSrcCardOriginalPos(undoModel->getSrcCard()->getPosition());
    }

    if (undoModel->getDestCard()) {
        undoModel->setDestCardOriginalPos(undoModel->getDestCard()->getPosition());
    }

    // 记录操作类型（根据卡牌位置判断）
    if (undoModel->getSrcCard() && undoModel->getDestCard()) {
        // 判断是否备用牌堆的卡牌（翻牌操作）
        bool isSrcStackCard = false;
        auto stackCards = _gameModel->getStackCards();
        for (auto& card : stackCards) {
            if (card->getCardId() == undoModel->getSrcCard()->getCardId()) {
                isSrcStackCard = true;
                break;
            }
        }

        if (isSrcStackCard) {
            undoModel->setOperateType(OT_FLIP_CARD);
        }
        else {
            undoModel->setOperateType(OT_MATCH_CARD);
        }
    }

   
    _undoRecords.pushBack(undoModel);

    log("添加回退记录：操作类型=%d，源卡牌ID=%d，目标卡牌ID=%d",
        undoModel->getOperateType(),
        undoModel->getSrcCard() ? undoModel->getSrcCard()->getCardId() : -1,
        undoModel->getDestCard() ? undoModel->getDestCard()->getCardId() : -1);
}

// 2. 执行回退操作：恢复游戏状态+安全释放记录
bool UndoManager::undo(UndoModel* outUndoModel) {
    // 三重校验：避免非法操作（无记录/无模型/无输出指针）
    if (_undoRecords.empty()) {
        log("提示：无回退记录，无法执行回退");
        return false;
    }

    if (_gameModel == nullptr) {
        log("错误：GameModel 未绑定，无法修改游戏状态");
        return false;
    }

    if (outUndoModel == nullptr) {
        log("错误：输出参数 outUndoModel 为空，无法返回回退数据");
        return false;
    }

    // 步骤1：取出最近的回退记录
    UndoModel* lastRecord = _undoRecords.back();

   
    outUndoModel->setOperateType(lastRecord->getOperateType());
    outUndoModel->setSrcCard(lastRecord->getSrcCard());   
    outUndoModel->setDestCard(lastRecord->getDestCard()); 
    outUndoModel->setSrcCardOriginalPos(lastRecord->getSrcCardOriginalPos());
    outUndoModel->setDestCardOriginalPos(lastRecord->getDestCardOriginalPos());

    // 步骤2：按操作类型执行回退逻辑
    CardModel* srcCard = lastRecord->getSrcCard();   // 源卡牌（操作时的"新牌"）
    CardModel* destCard = lastRecord->getDestCard(); // 目标卡牌（操作前的"旧牌"）

    switch (lastRecord->getOperateType()) {
        // 回退"翻牌操作"：备用牌→底牌 → 恢复为 旧底牌+备用牌放回堆
    case OT_FLIP_CARD:
        // 恢复底牌为操作前的旧牌（destCard = 旧底牌）
        if (destCard != nullptr) {
            destCard->setPosition(lastRecord->getDestCardOriginalPos());
            destCard->setFaceUp(true);
            _gameModel->setBottomCard(destCard);
        }

        // 将操作时的新牌（srcCard = 原备用牌）放回备用牌堆
        if (srcCard != nullptr) {
            srcCard->setPosition(lastRecord->getSrcCardOriginalPos());
            srcCard->setFaceUp(false); // 备用牌默认背面朝上
            _gameModel->addStackCard(srcCard);
        }
        break;

        // 回退"匹配操作"：主牌→底牌 → 恢复为 旧底牌+主牌放回主牌区
    case OT_MATCH_CARD:
        // 恢复底牌为操作前的旧牌（destCard = 旧底牌）
        if (destCard != nullptr) {
            destCard->setPosition(lastRecord->getDestCardOriginalPos());
            destCard->setFaceUp(true);
            _gameModel->setBottomCard(destCard);
        }

        // 将操作时的主牌（srcCard = 原主牌区卡牌）放回主牌区
        if (srcCard != nullptr) {
            srcCard->setPosition(lastRecord->getSrcCardOriginalPos());
            srcCard->setFaceUp(true);
            _gameModel->addPlayfieldCard(srcCard);
        }
        break;

    default:
        log("警告：未知操作类型，无法回退");
        break;
    }

    // 从记录中移除
    _undoRecords.popBack();

    log("回退成功：操作类型=%d，源卡牌ID=%d，目标卡牌ID=%d",
        outUndoModel->getOperateType(),
        srcCard ? srcCard->getCardId() : -1,
        destCard ? destCard->getCardId() : -1);

    return true; // 回退成功
}
