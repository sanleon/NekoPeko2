//
//  GamePlayScene.h
//  NekoPeko2
//
//  Created by Jo Sungbum on 2014/03/21.
//
//  MAIN Puzzleを表示
//

#ifndef __NekoPeko2__GamePlayScene__
#define __NekoPeko2__GamePlayScene__

#include "cocos2d.h"
#include "Config.h"

// TODO Refac
#define MAX_BLOCK_X 6
#define MAX_BLOCK_Y 5
#define REMOVING_TIME 0.1f
#define MOVING_TIME 0.2f

class GamePlayScene : public cocos2d::CCLayer
{
private:
    static const char GAME_PLAY_BACKGROUND_PNG[];

protected:
    enum kTag
    {
        kTagBackground = 1,
        kTagBaseTile = 10000,
    };

    enum kZOrder
    {
        kZOrderBackground,
        kZOrderTile,
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

    cocos2d::CCSprite* m_game_play_background;
    void showGamePlayBackground();

    float m_tileSize;
    
    std::list<int> m_tags;
    
    std::map<int, int> m_tileMap;
    // タイルの種類ことのグループ
    std::map<kTile, std::list<int> > m_tileTags;
    
    void initForVariables();
    void showTile();
    //　ポジションを取得する
    cocos2d::CCPoint getPosition(int posIndexX, int posIndexY);
    // ポジションからタグを取得する
    int getTag(int posIndexX, int posIndexY);
    
    void getTouchTileTag(cocos2d::CCPoint touchPoint, int &tag, kTile &tileType);
    std::vector<kTile> tileTypes;

//    std::list<int> getSameColorTileTags(int baseTag, kTile tileTypes);
    std::list<int> getSameColorTileTags(int baseTag, kTile tileTypes);

    std::list<int> getSameColorTileTags();
    void removeTouchTile(int tileTag, kTile tileType);
    void removeTile(int tileTag, kTile tileType);
    void removeTiles(std::list<int> tileTags, kTile tileType);
    bool hasSameColorTile(std::list<int> tileTagList, int searchTileTag);
    
    void removingTile(cocos2d::CCNode* tile);


    PositionIndex getPositionIndex(int tag);
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> tiles);
    void moveBlock();
    void movingTilesAnimation1(std::list<int> tiles);
    
    
    std::list<int> getRightSameColorTiles(std::list<int> tileTagList, int baseTag);
    std::list<int> getLeftSameColorTiles(std::list<int> tileTagList, int baseTag);
    std::list<int> getTopSameColorTiles(std::list<int> tileTagList, int baseTag);
    std::list<int> getDownSameColorTiles(std::list<int> tileTagList, int baseTag);
    
    
    std::list<int> addNewTile();
    std::list<int> getBlankTileTags();
    
    bool m_animating;
    void movedBlocks();
    

public:
    virtual bool init();
    static cocos2d::CCScene *scene();
    CREATE_FUNC(GamePlayScene);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
};

#endif /* defined(__NekoPeko2__GamePlayScene__) */
