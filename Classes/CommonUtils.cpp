#include "CommonUtils.h"

std::string CommonUtils::getSuitImagePath(CardSuitType suit) {
    std::string suitFileName;
    switch (suit) {
    case CST_CLUBS:    suitFileName = "club.png";    break;
    case CST_DIAMONDS: suitFileName = "diamond.png"; break;
    case CST_HEARTS:   suitFileName = "heart.png";   break;
    case CST_SPADES:   suitFileName = "spade.png";   break;
    default:
        log("警告：无效花色，无法加载花色图");
        return "";
    }
   
    return StringUtils::format("suits/%s", suitFileName.c_str());
}


std::string CommonUtils::getNumberImagePath(CardSuitType suit, CardFaceType face) {
    std::string colorStr = (suit == CST_CLUBS || suit == CST_SPADES) ? "black" : "red";
    std::string faceStr;
    switch (face) {
    case CFT_ACE:   faceStr = "A";   break;
    case CFT_TWO:   faceStr = "2";   break;
    case CFT_THREE: faceStr = "3";   break;
    case CFT_FOUR:  faceStr = "4";   break;
    case CFT_FIVE:  faceStr = "5";   break;
    case CFT_SIX:   faceStr = "6";   break;
    case CFT_SEVEN: faceStr = "7";   break;
    case CFT_EIGHT: faceStr = "8";   break;
    case CFT_NINE:  faceStr = "9";   break;
    case CFT_TEN:   faceStr = "10";  break;
    case CFT_JACK:  faceStr = "J";   break;
    case CFT_QUEEN: faceStr = "Q";   break;
    case CFT_KING:  faceStr = "K";   break;
    default:
        log("警告：无效点数，无法加载点数图");
        return "";
    }
  
    return StringUtils::format("number/big_%s_%s.png", colorStr.c_str(), faceStr.c_str());
}

std::string CommonUtils::getFaceStr(CardFaceType face) {
    switch (face) {
    case CFT_ACE:   return "A";   break;
    case CFT_TWO:   return "2";   break;
    case CFT_THREE: return "3";   break;
    case CFT_FOUR:  return "4";   break;
    case CFT_FIVE:  return "5";   break;
    case CFT_SIX:   return "6";   break;
    case CFT_SEVEN: return "7";   break;
    case CFT_EIGHT: return "8";   break;
    case CFT_NINE:  return "9";   break;
    case CFT_TEN:   return "10";  break;
    case CFT_JACK:  return "J";   break;
    case CFT_QUEEN: return "Q";   break;
    case CFT_KING:  return "K";   break;
    default:        return "";    break;
    }
}


// 点数差1判断
bool CommonUtils::isPointDiffOne(CardFaceType face1, CardFaceType face2) {
    int f1 = static_cast<int>(face1);
    int f2 = static_cast<int>(face2);
    int diff = abs(f1 - f2);

    // 正常相邻或A和K相邻
    return diff == 1 || diff == 12;
}
