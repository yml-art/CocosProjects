#include "LevelConfig.h"


CardConfig* CardConfig::create() {
    CardConfig* pRet = new (std::nothrow) CardConfig();
    if (pRet) {
        pRet->autorelease(); 
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

