//
//  BaseLayer.h
//  CampJam
//
//  Created by Bruno Assarisse on 23/07/13.
//
//

#ifndef __CampJam__BaseLayer__
#define __CampJam__BaseLayer__

#include "cocos2d.h"

using namespace cocos2d;

class BaseLayer : public Layer
{
public:
    
    ~BaseLayer();
    
    virtual bool init();
    
    double getCurrentTimeInSeconds();
    
	virtual void onEnterTransitionDidFinish();
    
    virtual void recursivelyPauseAllChildren(Node *node);
    virtual void recursivelyResumeAllChildren(Node *node);
    
    virtual void keyPressed(int keyCode);
    virtual void keyReleased(int keyCode);
    virtual void handleKey(int keyCode, bool pressed);
    
    virtual void buttonUp(bool pressed);
    virtual void buttonDown(bool pressed);
    virtual void buttonLeft(bool pressed);
    virtual void buttonRight(bool pressed);
    virtual void buttonA(bool pressed);
    virtual void buttonB(bool pressed);
    virtual void buttonStart(bool pressed);
    virtual void buttonSelect(bool pressed);
    virtual void buttonAny(bool pressed);
};


#endif /* defined(__CampJam__BaseLayer__) */
