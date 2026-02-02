#include "VisibleRect.h"

Rect VisibleRect::getVisibleRect()
{
    auto winSize = Director::getInstance()->getWinSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    return Rect(origin.x, origin.y, winSize.width, winSize.height);
}

Vec2 VisibleRect::left()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x, visibleRect.origin.y + visibleRect.size.height / 2);
}

Vec2 VisibleRect::right()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x + visibleRect.size.width, visibleRect.origin.y + visibleRect.size.height / 2);
}

Vec2 VisibleRect::top()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x + visibleRect.size.width / 2, visibleRect.origin.y + visibleRect.size.height);
}

Vec2 VisibleRect::bottom()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x + visibleRect.size.width / 2, visibleRect.origin.y);
}

Vec2 VisibleRect::center()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x + visibleRect.size.width / 2, visibleRect.origin.y + visibleRect.size.height / 2);
}

Vec2 VisibleRect::leftTop()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x, visibleRect.origin.y + visibleRect.size.height);
}

Vec2 VisibleRect::rightTop()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x + visibleRect.size.width, visibleRect.origin.y + visibleRect.size.height);
}

Vec2 VisibleRect::leftBottom()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x, visibleRect.origin.y);
}

Vec2 VisibleRect::rightBottom()
{
    auto visibleRect = getVisibleRect();
    return Vec2(visibleRect.origin.x + visibleRect.size.width, visibleRect.origin.y);
}