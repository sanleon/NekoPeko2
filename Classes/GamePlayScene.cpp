//
//  GamePlayScene.cpp
//  NekoPeko2
//
//  Created by Jo Sungbum on 2014/03/21.
//
//

#include "GamePlayScene.h"
#include "SimpleAudioEngine.h"
#include "TileSprite.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

const char GamePlayScene::GAME_PLAY_BACKGROUND_PNG[] = "game-play-bg.png";

CCScene* GamePlayScene::scene()
{
    CCScene* scene = CCScene::create();
    GamePlayScene* gamePlayLayer = GamePlayScene::create();
    scene->addChild(gamePlayLayer);
    return scene;
}

bool GamePlayScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    // タップイベントを取得
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    initForVariables();
    
    // 背景表示
    showGamePlayBackground();
    
    showTile();
    
    return true;
}

void GamePlayScene::showGamePlayBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    m_game_play_background = CCSprite::create(GAME_PLAY_BACKGROUND_PNG);

    m_game_play_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_game_play_background, kZOrderBackground, kTagBackground);
}

void GamePlayScene::initForVariables()
{
    // 乱数初期化
    srand((unsigned)time(NULL));

    // タイルの一辺の長さを取得
    TileSprite* pTile = TileSprite::createWithTileType(kTileRed);
    m_tileSize = pTile->getContentSize().height;

    tileTypes.push_back(kTileRed);
    tileTypes.push_back(kTileBlue);
    tileTypes.push_back(kTileYellow);
    tileTypes.push_back(kTileGreen);
    tileTypes.push_back(kTilePink);
    
    m_animating = false;
}

// 位置取得
CCPoint GamePlayScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_game_play_background->getContentSize().width * 0.03;
    float offsetY = m_game_play_background->getContentSize().height * 0.01;

    
//    CCLOG("pointX:%f", (posIndexX + 0.5) * m_tileSize + offsetX);
//    CCLOG("pointY:%f", (posIndexY + 0.5) * m_tileSize + offsetY);
    return CCPoint((posIndexX + 0.5) * m_tileSize + offsetX, (posIndexY + 0.5) * m_tileSize + offsetY);
}

// タグ取得
int GamePlayScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseTile + posIndexX * 100 + posIndexY;
}

//　タイル表示
void GamePlayScene::showTile()
{
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            kTile  tileType = (kTile)(rand() % kTileCount);

            // 対応するタイル配列にタグを追加
            int tag = getTag(x, y);
            m_tileTags[tileType].push_back(tag);
            m_tags.push_back(tag);
            m_tileMap.insert(make_pair(tag, 1));

            // タイルを作成
            TileSprite *pTile = TileSprite::createWithTileType(tileType);
            pTile->setPosition(getPosition(x, y));
            m_game_play_background->addChild(pTile, kZOrderTile, tag);
            CCLog("Tile Type : %d, Tag : %d", tileType, tag);


        }
    }
}

bool GamePlayScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return !m_animating;
}

void GamePlayScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLog("ccTouchEnded");
    CCPoint touchPoint = m_game_play_background->convertTouchToNodeSpace(pTouch);

    int tag = 0;
    kTile tileType;
    getTouchTileTag(touchPoint, tag, tileType);

    removeTouchTile(tag, tileType);

    std::list<int>removedTiles;
    CCLog("Remove Tag:%d",tag);
    removedTiles.push_back(tag);

    movingTilesAnimation1(removedTiles);
    


    list<int>::iterator it;
    for (it = m_tags.begin(); it != m_tags.end(); ++it) {
        
//        CCLog("m_tags : %d", *it);
        
        for (int x = 4; x < kTileCount; x++) {
            m_animating = true;
            kTile searchTileType = (kTile)x;
            std::list<int> threeTiles = getSameColorTileTags(*it, searchTileType);
            if (threeTiles.size() > 2) {
                removeTiles(threeTiles, searchTileType);
                movingTilesAnimation1(threeTiles);
            }
                sleep(0.2);
        }
    }
    // TODO

    
//    if (tag != 0) {
//        vector<kTile>::iterator typeIt = tileTypes.begin();
//        while (typeIt != tileTypes.end())
//        {
//            list<int>::iterator tagsIt = m_tileTags[*typeIt].begin();
//            //        int baseTag = *it;
//            //        sameColorTileTags.push_back(baseTag);
//            list<int> sameColorTileTagsRight;
//            list<int> sameColorTileTagsLeft;
//            list<int> sameColorTileTagsTop;
//            list<int> sameColorTileTagsDown;
//            while (tagsIt != m_tileTags[*typeIt].end())
//            {
//                sameColorTileTagsRight = getRightSameColorTiles(m_tileTags[*typeIt], *tagsIt);
//                sameColorTileTagsLeft = getLeftSameColorTiles(m_tileTags[*typeIt], *tagsIt);
//                sameColorTileTagsTop = getTopSameColorTiles(m_tileTags[*typeIt], *tagsIt);
//                sameColorTileTagsDown = getDownSameColorTiles(m_tileTags[*typeIt], *tagsIt);
//                 CCLog("tagsIt : %d", *tagsIt);
//                list<int>::iterator it;
//                for (it = sameColorTileTagsRight.begin(); it != sameColorTileTagsRight.end(); ++it) {
//                    CCLog("same color tags : %d", *it);
//                }
//                if (sameColorTileTagsRight.size() > 2) {
//                    removeTiles(sameColorTileTagsRight, tileType);
//                    
//                    movingTilesAnimation1(sameColorTileTagsRight);
//                    
//                }
//
//                tagsIt++;
//            }
//            typeIt++;
//        }
////        list<int> sameColorTileTags = getSameColorTileTags();
//    }
    
}

void GamePlayScene::getTouchTileTag(cocos2d::CCPoint touchPoint, int &tag, kTile &tileType)
{
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            int currentTag = getTag(x, y);
            CCNode* node = m_game_play_background->getChildByTag(currentTag);
            if (node && node->boundingBox().containsPoint(touchPoint)) {
                tag = currentTag;
                tileType = ((TileSprite *) node)->getTileType();

                return;
            }
        }
        
    }
}

void GamePlayScene::removeTouchTile(int tileTag, kTile tileType)
{
    removeTile(tileTag, tileType);
}

void GamePlayScene::removeTile(int tileTag, kTile tileType)
{
    bool first = true;
    
    m_tileTags[tileType].remove(tileTag);
    
    CCNode* tile = m_game_play_background->getChildByTag(tileTag);
    if (tile)
    {
        CCScaleTo* scale = CCScaleTo::create(REMOVING_TIME, 0);
        
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removingTile));
        
        CCFiniteTimeAction* seq = CCSequence::create(scale, func, NULL);
        
        CCFiniteTimeAction* action;
        
        if (first)
        {
            action = CCSpawn::create(seq, NULL, NULL);
            
            first = false;
        }
        else
        {
            action = seq;
        }
        tile->runAction(action);
    }
}

void GamePlayScene::removeTiles(std::list<int> tileTags, kTile tileType)
{
    list<int>::iterator it;
    for (it = tileTags.begin(); it != tileTags.end(); ++it) {
        CCLog("Rmove Tag : %d, Tile Type : %d", *it, tileType);
        removeTile(*it, tileType);
    }
}

std::list<int> GamePlayScene::getRightSameColorTiles(std::list<int> tileTagList, int baseTag)
{
    list<int> sameTileTagList;
    int rightTags[] = {
        baseTag + 100, baseTag+200, baseTag+300, baseTag+400, baseTag+500, baseTag+600,
    };
    for (int i = 0; i < sizeof(rightTags) / sizeof(rightTags[0]); i++) {
        if (!hasSameColorTile(sameTileTagList, rightTags[i])) {
            if (hasSameColorTile(tileTagList, rightTags[i])) {
                sameTileTagList.push_back(rightTags[i]);
            }
        }
    }
    return sameTileTagList;

}
//std::list<int> GamePlayScene::getLeftSameColorTiles(std::list<int> tileTagList, int baseTag)
//{
//    list<int> sameTileTagList;
//    int leftTags[] = {
//        baseTag - 100, baseTag-200, baseTag-300, baseTag-400, baseTag-500, baseTag-600,
//    };
//    for (int i = 0; i < sizeof(leftTags) / sizeof(leftTags[0]); i++) {
//        if (!hasSameColorTile(sameTileTagList, leftTags[i])) {
//            if (hasSameColorTile(tileTagList, leftTags[i])) {
//                sameTileTagList.push_back(leftTags[i]);
//            }
//        }
//    }
//    return sameTileTagList;
//    
//}
//std::list<int> GamePlayScene::getTopSameColorTiles(std::list<int> tileTagList, int baseTag)
//{
//    list<int> sameTileTagList;
//    int topTags[] = {
//        baseTag +1, baseTag+2, baseTag+3, baseTag+4, baseTag+5, baseTag+6,
//    };
//    for (int i = 0; i < sizeof(topTags) / sizeof(topTags[0]); i++) {
//        if (!hasSameColorTile(sameTileTagList, topTags[i])) {
//            if (hasSameColorTile(tileTagList, topTags[i])) {
//                sameTileTagList.push_back(topTags[i]);
//            }
//        }
//    }
//    return sameTileTagList;
//}
//std::list<int> GamePlayScene::getDownSameColorTiles(std::list<int> tileTagList, int baseTag)
//{
//    list<int> sameTileTagList;
//    int downTags[] = {
//            baseTag -1, baseTag-2, baseTag-3, baseTag-4, baseTag-5, baseTag-6,
//    };
//    for (int i = 0; i < sizeof(downTags) / sizeof(downTags[0]); i++) {
//        if (!hasSameColorTile(sameTileTagList, downTags[i])) {
//            if (hasSameColorTile(tileTagList, downTags[i])) {
//                sameTileTagList.push_back(downTags[i]);
//            }
//        }
//    }
//    return sameTileTagList;
//}

//list<int> GamePlayScene:: getSameColorTileTags()
//{
//    list<int> sameColorTileTags;
//   
//    vector<kTile>::iterator typeIt = tileTypes.begin();
//    while (typeIt != tileTypes.end())
//    {
//        list<int>::iterator tagsIt = m_tileTags[*typeIt].begin();
//        //        int baseTag = *it;
//        //        sameColorTileTags.push_back(baseTag);
//        while (tagsIt != m_tileTags[*typeIt].end())
//        {
//            
//            CCLog("tag %d", *tagsIt);
//            int tags[] = {
//                *tagsIt,
//                *tagsIt + 100, *tagsIt+200, *tagsIt+300, *tagsIt+400, *tagsIt+500, *tagsIt+600,
//                *tagsIt - 100, *tagsIt-200, *tagsIt-300, *tagsIt-400, *tagsIt-500, *tagsIt-600,
//                *tagsIt + 1, *tagsIt + 2, *tagsIt + 3, *tagsIt + 4, *tagsIt + 5,
//                *tagsIt - 1, *tagsIt - 2, *tagsIt - 3, *tagsIt - 4, *tagsIt - 5,
//            };
//            
//            int rightTags[] = {
//                *tagsIt,
//                *tagsIt + 100, *tagsIt+200, *tagsIt+300, *tagsIt+400, *tagsIt+500, *tagsIt+600,
//            };
//            for (int i = 0; i < sizeof(rightTags) / sizeof(rightTags[0]); i++) {
//                if (!hasSameColorTile(sameColorTileTags, rightTags[i])) {
//                    if (hasSameColorTile(m_tileTags[*typeIt], rightTags[i])) {
//                        sameColorTileTags.push_back(rightTags[i]);
//                    }
//                }
//            }
//            return sameColorTileTags;
//            tagsIt++;
//        }
//
//        typeIt++;
//        
//    }

//    //    list<int>::iterator it = sameColorTileTags.begin();
//    //    list<int>::iterator it = m_tileTags[tileType].begin();
//    list<int> sameColorTileTags;
//    
//    
//    list<int>::iterator tagsIt;
//    
////    sameColorTileTags.push_back(*tagsIt);
//    for (tagsIt = m_tags.begin(); tagsIt != m_tags.end(); ++tagsIt) {
//        int baseTag = *tagsIt;
//        vector<kTile>::iterator typeIt = tileTypes.begin();
//        while (typeIt != tileTypes.end())
//        {
//            list<int>::iterator it = m_tileTags[*typeIt].begin();
//            //        int baseTag = *it;
//            //        sameColorTileTags.push_back(baseTag);
//            while (it != m_tileTags[*typeIt].end())
//            {
//                int tags[] = {
//                    baseTag + 100, baseTag+200, baseTag+300, baseTag+400, baseTag+500, baseTag+600,
//                    baseTag - 100, baseTag-200, baseTag-300, baseTag-400, baseTag-500, baseTag-600,
//                    baseTag + 1, baseTag + 2, baseTag + 3, baseTag + 4, baseTag + 5,
//                    baseTag - 1, baseTag - 2, baseTag - 3, baseTag - 4, baseTag - 5,
//                };
//                for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++) {
//                    CCLog("it : %d", *it);
//                    CCLog("tags[i] : %d", tags[i]);
//                    if (!hasSameColorTile(sameColorTileTags, tags[i])) {
//                        if (hasSameColorTile(m_tileTags[*typeIt], tags[i])) {
//                            sameColorTileTags.push_back(tags[i]);
//                        }
//                    }
//                }
//            }
//        }
//
//    }
    
//    vector<kTile>::iterator typeIt = tileTypes.begin();
//    while (typeIt != tileTypes.end())
//    {
//        list<int>::iterator it = m_tileTags[*typeIt].begin();
////        int baseTag = *it;
////        sameColorTileTags.push_back(baseTag);
//        while (it != m_tileTags[*typeIt].end())
//        {
//
//            int tags[] = {
//                 *it + 100, *it+200, *it+300, *it+400, *it+500, *it+600,
//                 *it - 100, *it-200, *it-300, *it-400, *it-500, *it-600,
//                 *it + 1, *it + 2, *it + 3, *it + 4, *it + 5,
//                 *it - 1, *it - 2, *it - 3, *it - 4, *it - 5,
//            };
//
////            CCLog("sizeof tags : %lu", sizeof(tags));
////            CCLog("sizeof tags[0] : %lu", sizeof(tags[0]));
////            CCLog("sizeof / tags : %lu", sizeof(tags)/sizeof(tags[0]));
//            for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++) {
//                CCLog("it : %d", *it);
//            CCLog("tags[i] : %d", tags[i]);
//                if (!hasSameColorTile(sameColorTileTags, tags[i])) {
//                    if (hasSameColorTile(m_tileTags[*it1], tags[i])) {
//                        sameColorTileTags.push_back(tags[i]);
//                    }
//                }
//            }
//            
//            it++;
//            
//        }
//        
//        it1++;
//    }
    
//    return sameColorTileTags;
//}

list<int> GamePlayScene::getSameColorTileTags(int baseTag, kTile tileType)
{
    list<int> sameColorTileTags;
    sameColorTileTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorTileTags.begin();
    //    list<int>::iterator it = m_tileTags[tileType].begin();
    
    
    while (it != sameColorTileTags.end())
    {
        
        //                int tags[] = {
        //                    *it + 100, *it + 200, *it + 300, *it + 400, *it + 500, *it + 600,
        //                    *it - 100, *it - 200, *it - 300, *it - 400, *it - 500, *it - 600,
        //                    *it + 1, *it + 2, *it + 3, *it + 4, *it + 5,
        //                    *it - 1, *it - 2, *it - 3, *it - 4, *it - 5,
        //                 };
        int tags[] = {
            *it + 100,
            *it - 100,
//            *it + 1,
//            *it - 1,
        };
        
        for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++) {
            if (!hasSameColorTile(sameColorTileTags, tags[i])) {
                if (hasSameColorTile(m_tileTags[tileType], tags[i])) {
                    sameColorTileTags.push_back(tags[i]);
                }
            }
        }
        
        it++;
        
    }
    
//        it1++;
//    }
    
    return sameColorTileTags;
}

bool GamePlayScene::hasSameColorTile(list<int> tileTagList, int searchTileTag)
{
    list<int>::iterator it;
//     CCLog("searchTileTag : %d", searchTileTag);
    for(it = tileTagList.begin(); it != tileTagList.end(); ++it)
    {
        if (*it == searchTileTag)
        {
//                    CCLog("it: %d, searchTileTag : %d",*it, searchTileTag);
            return true;
        }
    }
    return false;
}

void GamePlayScene::removingTile(cocos2d::CCNode *tile)
{
    tile->removeFromParentAndCleanup(true);
    
    std::list<int> newTiles = addNewTile();
    
    movingTilesAnimation1(newTiles);
    


}

GamePlayScene::PositionIndex GamePlayScene::getPositionIndex(int tag)
{
    int pos1_x = (tag - kTagBaseTile) / 100;
    int pos1_y = (tag - kTagBaseTile) % 100;

    return PositionIndex(pos1_x, pos1_y);
}


// 新しい位置をセット
void GamePlayScene::setNewPosition1(int tag, PositionIndex posIndex)
{
    TileSprite* tileSprite = (TileSprite*)m_game_play_background->getChildByTag(tag);
    int nextPosY = tileSprite->getNextPosY();
    if (nextPosY == -1)
    {
        nextPosY = posIndex.y;
    }

    tileSprite->setNextPos(posIndex.x, --nextPosY);
}

void GamePlayScene::searchNewPosition1(std::list<int> tiles)
{
    list<int>::iterator it1 = tiles.begin();
    while (it1 != tiles.end())
    {
        PositionIndex posIndex1 = getPositionIndex(*it1);

        // コマ種類のループ
        vector<kTile>::iterator it2 = tileTypes.begin();
        while (it2 != tileTypes.end())
        {
            list<int>::iterator it3 = m_tileTags[*it2].begin();
            while (it3 != m_tileTags[*it2].end())
            {
                PositionIndex posIndex2 = getPositionIndex(*it3);

                if (posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y)
                {
                    setNewPosition1(*it3, posIndex2);
                }

                it3++;
            }

            it2++;
        }

        it1++;
    }
}

void GamePlayScene::moveBlock()
{
    vector<kTile>::iterator it1 = tileTypes.begin();
    while (it1 != tileTypes.end())
    {
        list<int>::iterator it2 = m_tileTags[*it1].begin();
        while (it2 != m_tileTags[*it1].end())
        {
            TileSprite* tileSprite = (TileSprite*)m_game_play_background->getChildByTag(*it2);
            if(tileSprite) {
            int nextPosX = tileSprite->getNextPosX();
            int nextPosY = tileSprite->getNextPosY();
//            CCLog("tag: %d, nextPosX: %d, nextPosY : %d",*it2, nextPosX, nextPosY);

            if (nextPosX != -1 || nextPosY != -1)
            {
                int newTag = getTag(nextPosX, nextPosY);
                tileSprite->initNextPos();
                tileSprite->setTag(newTag);

                // タグ一覧の値値も新しいタグに変更する
                *it2 = newTag;

                CCMoveTo* move = CCMoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                tileSprite->runAction(move);
            }
            }

            it2++;
        }

        it1++;
    }
}


void GamePlayScene::movingTilesAnimation1(std::list<int> tiles)
{
    searchNewPosition1(tiles);

    moveBlock();
}

std::list<int> GamePlayScene::addNewTile()
{
    std::list<int> blankTileTags = getBlankTileTags();
    list<int>::iterator it = blankTileTags.begin();
    while (it != blankTileTags.end()) {
        kTile  tileType = (kTile)(rand() % kTileCount);
        m_tileTags[tileType].push_back(*it);
        
        // タイルを作成
        TileSprite *pTile = TileSprite::createWithTileType(tileType);
        PositionIndex pi = getPositionIndex(*it);
        pTile->setPosition(getPosition(pi.x, pi.y));
        CCLog("position index x : %d, y : %d", pi.x, pi.y);
        m_game_play_background->addChild(pTile, kZOrderTile, *it);
//        CCNode* tile = m_game_play_background->getChildByTag(*it);
//        bool first = true;
//        if (tile)
//        {
//            CCScaleBy* scale = CCScaleBy::create(REMOVING_TIME, 1);
//            
////            CCCallFuncN* func = CCCallFuncN::c;
//            
//            CCFiniteTimeAction* seq = CCSequence::create(scale,  NULL);
//            
//            CCFiniteTimeAction* action;
//            
//            if (first)
//            {
//                action = CCSpawn::create(seq, NULL, NULL);
//                
//                first = false;
//            }
//            else
//            {
//                action = seq;
//            }
//            tile->runAction(action);
//        }
        it++;
    }
    
    return blankTileTags;
}

std::list<int> GamePlayScene::getBlankTileTags()
{
    std::list<int> blankTileTags;
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            int tag = getTag(x, y);
//            CCLog("all Tag : %d", tag);
            TileSprite* tileSprite = (TileSprite*)m_game_play_background->getChildByTag(tag);
            if (!tileSprite) {
                CCLog("blank Tag : %d", tag);
                blankTileTags.push_back(tag);
            }
        }
    }
    return blankTileTags;
    
}

void GamePlayScene::movedBlocks()
{
    m_animating = false;
}