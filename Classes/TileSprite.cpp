//
// Created by Jo Sungbum on 2014/03/21.
//

#include "TileSprite.h"

void TileSprite::initNextPos()
{
    nextPosX = -1;
    nextPosY = -1;
}

//void TileSprite::setNextPos(int _nextPosX, int _nextPosY)
//{
//    nextPosX = _nextPosX;
//    nextPosY = _nextPosY;
//}

TileSprite::TileSprite()
{
    initNextPos();
}

TileSprite::~TileSprite()
{
}

//TileSprite* TileSprite::createWithTileType(kTile tileType)
//{
//    return create(tileType, kStatusNormal);
//}

//TileSprite* TileSprite::create(kTile tileType, kStatus status)
//{
//    TileSprite *tileSprite = new TileSprite();
//    tileSprite->setStatus(status);
//    
//    if (tileSprite && tileSprite->initWithTileType(tileType))
//    {
//        tileSprite->autorelease();
//        return tileSprite;
//    }
//    else
//    {
//        CC_SAFE_DELETE(tileSprite);
//        return NULL;
//    }
//}

TileSprite* TileSprite::create(int tag, kTile tileType, kStatus status)
{
    TileSprite *tileSprite = new TileSprite();
    
    if (tileSprite && tileSprite->init(tag, tileType, status))
    {
        return tileSprite;
    }
    else
    {
        CC_SAFE_DELETE(tileSprite);
        return NULL;
    }
}

//bool TileSprite::initWithTileType(kTile tileType)
//{
//    if (!CCSprite::initWithFile(getTileImageFileName(tileType)))
//    {
//        return false;
//    }
//
//    tileType = tileType;
//
//    return true;
//}

bool TileSprite::init(int _tag, kTile _tileType, kStatus _status)
{
    if (!CCSprite::initWithFile(getTileImageFileName(_tileType)))
    {
        return false;
    }
    
    tileType = _tileType;
    tag = _tag;
    status = _status;
    
    
    return true;
}

const char* TileSprite::getTileImageFileName(kTile tileType)
{
    switch (tileType) {
        case kTileRed:
            return "red-tile.png";

        case kTileBlue:
            return "blue-tile.png";

        case kTileYellow:
            return "yellow-tile.png";

        case kTileGreen:
            return "green-tile.png";

        case kTilePink:
            return "pink-tile.png";

        default:
            CCAssert(false, "invalid tileType");
            return "";
    }
}
