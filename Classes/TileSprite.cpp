//
// Created by Jo Sungbum on 2014/03/21.
//

#include "TileSprite.h"

void TileSprite::initNextPos()
{
    m_nextPosX = -1;
    m_nextPosY = -1;
}

void TileSprite::setNextPos(int nextPosX, int nextPosY)
{
    m_nextPosX = nextPosX;
    m_nextPosY = nextPosY;
}

TileSprite::TileSprite()
{
    initNextPos();
}

TileSprite::~TileSprite()
{
}

TileSprite* TileSprite::createWithTileType(kTile tileType)
{
    TileSprite *tileSprite = new TileSprite();
    if (tileSprite && tileSprite->initWithTileType(tileType))
    {
        tileSprite->autorelease();
        return tileSprite;
    }
    else
    {
        CC_SAFE_DELETE(tileSprite);
        return NULL;
    }
}

bool TileSprite::initWithTileType(kTile tileType)
{
    if (!CCSprite::initWithFile(getTileImageFileName(tileType)))
    {
        return false;
    }

    m_tileType = tileType;

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
