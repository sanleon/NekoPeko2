//
// Created by Jo Sungbum on 2014/03/21.
//


#ifndef __TileSprite_H_
#define __TileSprite_H_

#include "cocos2d.h"
#include "Config.h"

class TileSprite : public cocos2d::CCSprite {
protected:
    const char* getTileImageFileName(kTile tileType);

public:
    CC_SYNTHESIZE_READONLY(kTile, m_tileType, TileType);
    CC_SYNTHESIZE_READONLY(int, m_nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, m_nextPosY, NextPosY);
    
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);

    TileSprite();
    virtual ~TileSprite();
    virtual bool initWithTileType(kTile tileType);
    static TileSprite* createWithTileType(kTile tileType);
};


#endif //__TileSprite_H_
