//
//  GamePlayScene.h
//  NekoPeko2
//
//  Created by A12889 on 14/04/05.
//
//

#ifndef __NekoPeko2__GamePlayScene__
#define __NekoPeko2__GamePlayScene__

#include "cocos2d.h"
#include "Config.h"
#include "TileSprite.h"

using namespace std;
using namespace cocos2d;

const int kMaxTileX = 6;
const int kMaxTileY = 5;
const float kRemovingTime = 0.1f;
const float kMovingTime = 0.3f;
const int kTilePurgeThreshold = 3;

const char* const kGamePlayBackgroundPng = "game-play-bg.png";

enum kTag
{
    kTagRemoveTile = 0,
    kTagBackground = 1,
    kTagBaseTile = 10000,
};

enum kZOrder
{
    kZOrderBackground,
    kZOrderTile,
};

class GamePlayScene : public CCLayer
{
private:
    
    struct TouchedTile
    {
        TouchedTile(int _tag, kTile _type)
        {
            tag  = _tag;
            type = _type;
        }
        int    tag;
        kTile type;
    };
    
    struct PositionIndex
    {
        PositionIndex(int x1, int y1)
        {
            x = x1;
            y = y1;
        }
        int x;
        int y;
    };
    

    float tileSize;
    
    int currentTag;
    int tmpCurrentTag;
    int previousTag;
    
    void initForVariables();
    void showTile();
    
    
    CCSprite *gamePlayBackground;
    CCPoint getPosition(int posIndexX, int posIndexY);
    PositionIndex getPositionIndex(int tag);
    // ポジションからタグを取得する
    int getTag(int posIndexX, int posIndexY);
    
    void getTouchTileTag(CCPoint touchPoint, int &tag, kTile &tileType);
    
    void showGamePlayBackground();
    
    vector<PositionIndex> horizontalTileCounting(int left, int top, kTile tile_type);
    vector<PositionIndex> crossVerticalTileCounting(int left, int top, kTile tile_type);
    vector<PositionIndex> verticalTileCounting(int left, int top, kTile tile_type);
    vector<PositionIndex> crossHorizontalTileCounting(int left, int top, kTile tile_type);
    int crossTileCounting(int left, int top, kTile tile_type);

    CCNode* createParticle(const char* plistFile, const CCPoint &tagPoint);
    void removingParticle(CCNode* particle);
    
    void createTiles();
    void prependRemoveTiles();
    
    void removeTilesActionAnimation(CCNode* node);
    void removeTilesAction();
    void removeTiles();
    
    void removeTile();
    
    void moveTiles();
    void appendTiles();
    void recursiveRemoveTiles();
    void removeTileAnimation();
    void removeOneTileAnimation();
    void animationStart();
    void animationEnd();
    
    TouchedTile getTouchedTile(CCPoint touchPoint);
    
   void removeTouchTile(int tileTag, kTile tileType);
//    
//    void removeTile(int tileTag, kTile tileType);
    
protected:

public:
    
//    GamePlayScene();
    
    vector<kTile> tileTypes;
    vector<vector<TileSprite*> > tileFieldsVector;
    
    map<kTile, int> removedTileTypesCounter;
    vector<TileSprite*> removedTilesVector;
    vector<int> removedTags;
  
    
    virtual bool init();
    static CCScene *scene();
    CREATE_FUNC(GamePlayScene);
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
};



#endif /* defined(__NekoPeko2__GamePlayScene__) */
