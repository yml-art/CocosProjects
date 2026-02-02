#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include "cocos2d.h"
#include "CommonDef.h"

USING_NS_CC;

class CommonUtils {
public:
   
    static std::string getSuitImagePath(CardSuitType suit);

    
    static std::string getNumberImagePath(CardSuitType suit, CardFaceType face);

   
    static bool isPointDiffOne(CardFaceType face1, CardFaceType face2);

    static std::string getFaceStr(CardFaceType face); // 新增：点数枚举转字符串
};


#endif // __COMMON_UTILS_H__

