#pragma once
#ifndef VISIBLE_RECT_H
#define VISIBLE_RECT_H

#include "cocos2d.h"

USING_NS_CC;

class VisibleRect
{
public:
    static Rect getVisibleRect();
    static Vec2 left();
    static Vec2 right();
    static Vec2 top();
    static Vec2 bottom();
    static Vec2 center();
    static Vec2 leftTop();
    static Vec2 rightTop();
    static Vec2 leftBottom();
    static Vec2 rightBottom();
};

#endif