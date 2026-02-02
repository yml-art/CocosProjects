
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

// 花色类型
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 0：梅花
    CST_DIAMONDS,   // 1：方块
    CST_HEARTS,     // 2：红桃
    CST_SPADES,     // 3：黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 点数类型
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,        // 0：A（对应图片后缀1）
    CFT_TWO,        // 1：2（对应图片后缀2）
    CFT_THREE,      // 2：3（对应图片后缀3）
    CFT_FOUR,       // 3：4（对应图片后缀4）
    CFT_FIVE,       // 4：5
    CFT_SIX,        // 5：6
    CFT_SEVEN,      // 6：7
    CFT_EIGHT,      // 7：8
    CFT_NINE,       // 8：9
    CFT_TEN,        // 9：10
    CFT_JACK,       // 10：J
    CFT_QUEEN,      // 11：Q
    CFT_KING,       // 12：K
    CFT_NUM_CARD_FACE_TYPES
};

// 操作类型（用于回退）
enum OperateType {
    OT_NONE = -1,
    OT_FLIP_CARD,   // 翻牌替换（备用牌→底牌）
    OT_MATCH_CARD   // 匹配替换（主牌→底牌）

};


#endif // __COMMON_DEF_H__

