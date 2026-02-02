#include "UndoModel.h"

UndoModel* UndoModel::create() {
    UndoModel* pRet = new (std::nothrow) UndoModel();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

UndoModel::UndoModel()
    : _operateType(OT_NONE)
    , _srcCard(nullptr)
    , _destCard(nullptr)
    , _srcCardOriginalPos(Vec2::ZERO)  
    , _destCardOriginalPos(Vec2::ZERO)
{
}
