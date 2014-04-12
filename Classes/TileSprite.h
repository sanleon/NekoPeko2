//
// Created by Jo Sungbum on 2014/03/21.
//


#ifndef __TileSprite_H_
#define __TileSprite_H_

#include "cocos2d.h"
#include "Config.h"

using namespace cocos2d;

class TileSprite : public cocos2d::CCSprite {
protected:
    const char* getTileImageFileName(kTile tileType);

public:
    CC_SYNTHESIZE(kTile, tileType, TileType);
    CC_SYNTHESIZE(int, nextPosX, NextPosX);
    CC_SYNTHESIZE(int, nextPosY, NextPosY);
    CC_SYNTHESIZE(int, tag, Tag);
    CC_SYNTHESIZE(kStatus, status, Status);
    
    void initNextPos();
//    void setNextPos(int _nextPosX, int _nextPosY);

    TileSprite();
    virtual ~TileSprite();
//    virtual bool initWithTileType(kTile tileType);
    virtual bool init(int _tag, kTile _tileType, kStatus _status);
//    static TileSprite* createWithTileType(kTile tileType);
//    static TileSprite* create(kTile tileType, kStatus status);
    static TileSprite* create(int tag, kTile tileType, kStatus status);
};


#endif //__TileSprite_H_
