/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "GameController.h"
#include "GameView.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 2080);

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
  
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();



    std::string generalPath = "card_general.png";
    if (!FileUtils::getInstance()->isFileExist(generalPath)) {
        log("【错误】card_general.png不存在！请放入Resources目录，当前路径：%s",
            FileUtils::getInstance()->fullPathForFilename(generalPath).c_str());
    }
    else {
        log("【成功】找到card_general.png，路径：%s",
            FileUtils::getInstance()->fullPathForFilename(generalPath).c_str());
    }

    
    if (!glview) {
        glview = GLViewImpl::createWithRect("CardGame", Rect(0, 0, 1080, 2080), 1.0f);
        director->setOpenGLView(glview);
    }

    
    director->setDisplayStats(true); 
    director->setAnimationInterval(1.0f / 60);
    glview->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);

    
    auto gameView = GameView::create();
    auto gameController = new GameController();
    gameController->init(gameView);
    gameController->startGame("level1.json"); 

   
    auto scene = Scene::create();
    scene->addChild(gameView);
    director->runWithScene(scene);

    return true;
}


void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}