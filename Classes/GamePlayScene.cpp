//
//  GamePlayScene.cpp
//  NekoPeko2
//
//  Created by A12889 on 14/04/05.
//
//

#include "GamePlayScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
//using namespace std;

CCScene* GamePlayScene::scene()
{
    CCScene* scene = CCScene::create();
    GamePlayScene * gamePlayLayer = GamePlayScene::create();
    scene->addChild(gamePlayLayer);
    return scene;
}

bool GamePlayScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    currentTag = 0;
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    initForVariables();
    
    showGamePlayBackground();
    createTiles();
    
    return true;
}

void GamePlayScene::initForVariables()
{
    // 乱数初期化
    srand((unsigned)time(NULL));
    
    // タイルの一辺の長さを取得
    TileSprite* pTile = TileSprite::create(kTagBaseTile, kTileRed, kStatusNormal);
    tileSize = pTile->getContentSize().height;
    
    tileTypes.push_back(kTileRed);
    tileTypes.push_back(kTileBlue);
    tileTypes.push_back(kTileYellow);
    tileTypes.push_back(kTileGreen);
    tileTypes.push_back(kTilePink);
    
//    m_animating = false;
}

void GamePlayScene::showGamePlayBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    gamePlayBackground = CCSprite::create(kGamePlayBackgroundPng);
    
    gamePlayBackground->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    
    addChild(gamePlayBackground, kZOrderBackground, kTagBackground);
}

void GamePlayScene::createTiles()
{
    int tag = kTagBaseTile;
    for(int x = 0; x < kMaxTileX; x++ ) {
        std::vector<TileSprite *> tiles;
        for (int y = 0; y < kMaxTileY; y++) {
//            int tag = getTag(x, y);
            kTile tileType = (kTile)(rand() % kTileCount);
            TileSprite *tile = TileSprite::create(tag, tileType, kStatusNormal);
            tile->setPosition(getPosition(x, y));
            gamePlayBackground->addChild(tile, kZOrderTile, tag);
            tiles.push_back(tile);
            
            tag++;
        }
        tileFieldsVector.push_back(tiles);
    }
}

// 位置取得
CCPoint GamePlayScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = gamePlayBackground->getContentSize().width * 0.03;
    float offsetY = gamePlayBackground->getContentSize().height * 0.01;
    
    
    //    CCLOG("pointX:%f", (posIndexX + 0.5) * m_tileSize + offsetX);
    //    CCLOG("pointY:%f", (posIndexY + 0.5) * m_tileSize + offsetY);
    return CCPoint((posIndexX + 0.5) * tileSize + offsetX, (posIndexY + 0.5) * tileSize + offsetY);
}

// タグ取得
int GamePlayScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseTile + posIndexX * 100 + posIndexY;
}

void GamePlayScene::getTouchTileTag(cocos2d::CCPoint touchPoint, int &tag, kTile &tileType)
{
    for (int x = 0; x < kMaxTileX; x++)
    {
        for (int y = 0; y < kMaxTileY; y++)
        {
             currentTag = getTag(x, y);
            CCNode* node = gamePlayBackground->getChildByTag(currentTag);
            if (node && node->boundingBox().containsPoint(touchPoint)) {
                tag = currentTag;
                tileType = ((TileSprite *) node)->getTileType();
                
                return;
            }
        }
        
    }
}

GamePlayScene::TouchedTile GamePlayScene::getTouchedTile(CCPoint touchPoint)
{
    for (std::vector<std::vector<TileSprite*> >::const_iterator it = tileFieldsVector.begin(); it != tileFieldsVector.end(); ++it)
    {
        for (vector<TileSprite*>::const_iterator tile = (*it).begin(); tile != (*it).end(); ++tile)
        {
            if (currentTag != 0 && (*tile)->getTag() == currentTag)
            {
                continue;
            }
            
            if ((*tile)->boundingBox().containsPoint(touchPoint))
            {
                return TouchedTile((*tile)->getTag(), (*tile)->getTileType());
            }
        }
    }
    
    return TouchedTile(0, (new TileSprite())->getTileType());
}

bool GamePlayScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void GamePlayScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//    if (currentTag != 0)
//    {
//        background->removeChildByTag(tmpCurrentTag);
//        CCNode* current = background->getChildByTag(currentTag);
//        current->setVisible(true);
//        background->reorderChild(current, kZOrderBlock);
//    }
//    
//    currentTag  = 0;
//    previousTag = 0;
    
    CCPoint touchPoint = gamePlayBackground->convertTouchToNodeSpace(pTouch);
//    
//    int tag = 0;
//    kTile tileType;
//    getTouchTileTag(touchPoint, tag, tileType);
//
//    
//    removeTouchTile(tag, tileType);

    TouchedTile tTile = getTouchedTile(touchPoint);
    
    removeTouchTile(tTile.tag, tTile.type);
    
 //    scheduleOnce(schedule_selector(GamePlayScene::removeTileAnimation), 0.5f);
    

}

void GamePlayScene::removeTouchTile(int tileTag, kTile tileType)
{
    PositionIndex pi = getPositionIndex(tileTag);
    tileFieldsVector[pi.x][pi.y]->setStatus(kStatusMarked);
    
    
    CCDelayTime* delay = CCDelayTime::create(1.6f);
    CCCallFuncN* removeTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTiles));
    CCCallFuncN* removeTilesActionFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTilesAction));
    CCCallFuncN* moveTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::moveTiles));
    CCCallFuncN* appendTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::appendTiles));
    CCCallFuncN* prependRemoveTilesFunction = CCCallFuncN::create(this,  callfuncN_selector(GamePlayScene::prependRemoveTiles));
    CCCallFuncN* removeTileAnimationFunction = CCCallFuncN::create(this,  callfuncN_selector(GamePlayScene::removeTileAnimation));
    CCCallFuncN* animationStartFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::animationStart));
    CCCallFuncN* animationEndFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::animationEnd));
    CCFiniteTimeAction* tileActionSequence = CCSequence::create(animationStartFunction, removeTilesFunction, removeTilesActionFunction, moveTilesFunction, delay, appendTilesFunction, prependRemoveTilesFunction, removeTileAnimationFunction, animationEndFunction, NULL);
    
    
    this->runAction(tileActionSequence);
//
////    
////    removeTiles();
////    
////    removeTilesAction();
////    
////    appendTiles();
////    
////    prependRemoveTiles();
////    
////    removeTileAnimation();
//
}

//void GamePlayScene::removeTile(int tileTag, kTile tileType)
//{
//    bool first = true;
//    
//    CCNode* tile = gamePlayBackground->getChildByTag(tileTag);
//    if (tile)
//    {
//        CCScaleTo* scale = CCScaleTo::create(0.2f, 0);
//        
//        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTilesActionAnimation));
//        
//        CCFiniteTimeAction* seq = CCSequence::create(scale, func, NULL);
//        
//        CCFiniteTimeAction* action;
//        
//        if (first)
//        {
//            action = CCSpawn::create(seq, NULL, NULL);
//            
//            first = false;
//        }
//        else
//        {
//            action = seq;
//        }
//        tile->runAction(action);
//    }
//}

void GamePlayScene::removeTilesActionAnimation(CCNode* node)
{
    TileSprite* tileSprite = (TileSprite*)node;
    tileSprite->removeFromParentAndCleanup(true);
    tileSprite->release();
    tileSprite = TileSprite::create(tileSprite->getTag(), tileSprite->getTileType(), kStatusRemoved);
}

void GamePlayScene::removeTilesAction()
{
   bool isFirst = true;
    for (vector<TileSprite*>::const_iterator it = removedTilesVector.begin(); it != removedTilesVector.end(); ++it)
    {
//        CCLOG("NexTPosx : %d", (*it)->getTag());
//
        int x = (*it)->getNextPosX();
        int y = (*it)->getNextPosY();
        
        CCScaleTo* shrinkTile    = CCScaleTo::create(0.2f, 0);
        CCCallFuncN* removeTilesActionAnimationFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTilesActionAnimation));
        CCFiniteTimeAction* shrinkTileSequence = CCSequence::create(shrinkTile, removeTilesActionAnimationFunction, NULL);
        
//        CCScaleTo* burst   = CCScaleTo::create(0.3f, 2.5f);
//        CCScaleTo* clear   = CCScaleTo::create(0.3f, 1);
//        CCCallFuncN* removingParticleFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removingParticle));
//        CCFiniteTimeAction* shrinkParticalSequence = CCSequence::create(burst, clear, removingParticleFunction, NULL);
//        CCNode* partial = createParticle("lizi.plist", tileFieldsVector[x][y]->getPosition());
        
//        CCSize bgSize = background->getContentSize();
//        CCLabelBMFont* red_label = (CCLabelBMFont*)background->getChildByTag(kTagRedScore);
//        CCLabelBMFont* blue_label = (CCLabelBMFont*)background->getChildByTag(kTagBlueScore);
//        CCLabelBMFont* yellow_label = (CCLabelBMFont*)background->getChildByTag(kTagYellowScore);
//        CCLabelBMFont* green_label = (CCLabelBMFont*)background->getChildByTag(kTagGreenScore);
//        CCLabelBMFont* gray_label = (CCLabelBMFont*)background->getChildByTag(kTagGrayScore);
        
//        red_label->setString(ccsf("%04d", (removedTileTypesCounter[kTileRed]) / kTilePurgeThreshold));
//        blue_label->setString(ccsf("%04d", (removedTileTypesCounter[kTileBlue]) / kTilePurgeThreshold));
//        yellow_label->setString(ccsf("%04d", (removedTileTypesCounter[kTileYellow]) / kTilePurgeThreshold));
//        green_label->setString(ccsf("%04d", (removedTileTypesCounter[kTileGreen]) / kTilePurgeThreshold));
//        gray_label->setString(ccsf("%04d", (removedTileTypesCounter[kTileGray]) / kTilePurgeThreshold));
        
        if (isFirst) {
//            CCPlaySE* playSe = CCPlaySE::create(kSERemoveTile);
            shrinkTileSequence = CCSpawn::create(shrinkTileSequence, NULL);
            isFirst = false;
        }
        
        tileFieldsVector[x][y]->runAction(shrinkTileSequence);
//        partial->runAction(shrinkParticalSequence);
        
    }
}

//cocos2d::CCNode* GamePlayScene::createParticle(const char *plistFile, const cocos2d::CCPoint &tagPoint)
//{
//    CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create(plistFile);
//    pParticle->setPosition(tagPoint);
//    gamePlayBackground->addChild(pParticle, kZOrderPartical);
//    
//    return pParticle;
//}
//
//void GamePlayScene::removingParticle(cocos2d::CCNode *particle)
//{
//    particle->removeFromParentAndCleanup(true);
//}

void GamePlayScene::removeTiles()
{
    for (int x = 0; x < kMaxTileX; ++x) {
        for (int y = 0; y < kMaxTileY; ++y) {
            if (tileFieldsVector[x][y]->getStatus() == kStatusMarked) {
                TileSprite* removedTile = TileSprite::create(tileFieldsVector[x][y]->getTag(), tileFieldsVector[x][y]->getTileType(), tileFieldsVector[x][y]->getStatus());
                removedTile->setNextPosX(x);
                removedTile->setNextPosY(y);
                removedTilesVector.push_back(removedTile);
//                removedTileTypesCounter[tileFieldsVector[x][y]->getTileType()] += 1;
                
            }
        }
    }
}



void GamePlayScene::moveTiles()
{
    map<int, TileSprite*> moveTileList;
    for (vector<TileSprite*>::const_iterator it = removedTilesVector.begin(); it != removedTilesVector.end(); ++it) {
        CCLOG("tag : %d", (*it)->getTag());
        PositionIndex pi = getPositionIndex((*it)->getTag());
        int x = pi.x;
        for (int y = pi.y; y < kMaxTileY-1; ++y) {
            tileFieldsVector[x][y] = tileFieldsVector[x][y+1];
            tileFieldsVector[x][y]->retain();
            if (tileFieldsVector[x][y]->getStatus() == kStatusNormal) {
                TileSprite* tileSprite = tileFieldsVector[x][y];
                tileSprite->setNextPosY(y);
                tileSprite->setNextPosX(x);
                moveTileList.insert(make_pair(tileSprite->getTag(), tileSprite));
                
            }
        }
        tileFieldsVector[x][kMaxTileY-1] = TileSprite::create(kTagRemoveTile,kTileRed, kStatusRemoved);
    }
//    while (it != removedTilesVector.end()) {
//
//
//        
//        it++;
//    }
//    for (it = removedTilesVector.begin(); it != removedTilesVector.end(); ++it)
//    {
//            }
    
    for (map<int, TileSprite*>::const_iterator it = moveTileList.begin(); it != moveTileList.end(); ++it) {
        CCDelayTime* delay = CCDelayTime::create(0.8f);
        CCFiniteTimeAction* move = CCMoveTo::create(0.4f, getPosition((*it).second->getNextPosX(), (*it).second->getNextPosY()));
        CCSequence* action = CCSequence::createWithTwoActions(delay,move);
        (*it).second->runAction(action);
    }
}

void GamePlayScene::appendTiles()
{
    for (int x = 0; x < kMaxTileX; ++x)
    {
        for (int y = 0; y < kMaxTileY; ++y)
        {
            if (tileFieldsVector[x][y]->getStatus() == kStatusRemoved)
            {
                int tag = removedTilesVector.back()->getTag();
                removedTilesVector.pop_back();
                
                kTile tileType   = (kTile)(rand() % kTileCount);
                TileSprite* tile = TileSprite::create(tag, tileType, kStatusNormal);
                tile->setPosition(getPosition(x, kMaxTileY));
                gamePlayBackground->addChild(tile, kZOrderTile, tag);
                
                CCActionInterval* move = CCMoveTo::create(0.4f, getPosition(x, y));
                CCActionInterval* easeInOut = CCEaseInOut::create(move, 1.25f);
                tile->runAction(easeInOut);
                
                tileFieldsVector[x][y] = tile;
            }
        }
    }
}

void GamePlayScene::recursiveRemoveTiles()
{
    bool isMarked = false;
    prependRemoveTiles();
    for (int x = 0; x < kMaxTileX; ++x)
    {
        for (int y = 0; y < kMaxTileY; ++y)
        {
            if (tileFieldsVector[x][y]->getStatus() == kStatusMarked)
            {
                isMarked = true;
            }
        }
    }
    
    if (isMarked) {
        scheduleOnce(schedule_selector(GamePlayScene::removeTileAnimation), 0.5f);
    }
}

void GamePlayScene::removeOneTileAnimation()
{
//    CCDelayTime* delay = CCDelayTime::create(1.6f);
//    CCCallFuncN* removeTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTiles));
//    CCCallFuncN* removeTilesActionFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTilesAction));
//    CCCallFuncN* moveTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::moveTiles));
//    CCCallFuncN* appendTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::appendTiles));
//    CCCallFuncN* animationStartFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::animationStart));
//    CCCallFuncN* animationEndFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::animationEnd));
//    CCFiniteTimeAction* tileActionSequence = CCSequence::create(animationStartFunction, removeTilesFunction, removeTilesActionFunction, moveTilesFunction, delay, appendTilesFunction, animationEndFunction, NULL);
//    
//    
//    this->runAction(tileActionSequence);
}

void GamePlayScene::removeTileAnimation()
{
    CCDelayTime* delay = CCDelayTime::create(1.6f);
    CCCallFuncN* removeTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTiles));
    CCCallFuncN* removeTilesActionFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::removeTilesAction));
    CCCallFuncN* moveTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::moveTiles));
    CCCallFuncN* appendTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::appendTiles));
    CCCallFuncN* recursiveRemoveTilesFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::recursiveRemoveTiles));
    CCCallFuncN* animationStartFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::animationStart));
    CCCallFuncN* animationEndFunction = CCCallFuncN::create(this, callfuncN_selector(GamePlayScene::animationEnd));
    CCFiniteTimeAction* tileActionSequence = CCSequence::create(animationStartFunction, removeTilesFunction,removeTilesActionFunction, moveTilesFunction, delay, appendTilesFunction, recursiveRemoveTilesFunction, animationEndFunction, NULL);
    
    this->runAction(tileActionSequence);
    
//    scheduleOnce(schedule_selector(GamePlayScene::removeTiles), 0.5f);
//    scheduleOnce(schedule_selector(GamePlayScene::removeTilesAction), 0.5f);
//    scheduleOnce(schedule_selector(GamePlayScene::moveTiles), 0.5f);
//    removeTiles();
//    removeTilesAction();
//    moveTiles();
//    appendTiles();
//    recursiveRemoveTiles();
    
}

void GamePlayScene::prependRemoveTiles()
{
    for (int x = 0; x < kMaxTileX; ++x)
    {
        for (int y = 0; y < kMaxTileY; ++y)
        {
            if (tileFieldsVector[x][y]->getTag() == 0) {
                continue;
            }
            
            int counter;
            counter = crossTileCounting(x, y, tileFieldsVector[x][y]->getTileType());
            if (counter >= kTilePurgeThreshold)
            {
                prependRemoveTiles();
            }
        }
    }
}

vector<GamePlayScene::PositionIndex> GamePlayScene::horizontalTileCounting(int left, int top, kTile tile_type) {
    vector<PositionIndex> flushPoints;
    if (tileFieldsVector[left][top]->getStatus() == kStatusMarked) {
        flushPoints.push_back(PositionIndex(left, top));
    }
    
    for (int i = left; i < kMaxTileX; ++i)
    {
        if (tileFieldsVector[i][top]->getStatus() == kStatusNormal && tileFieldsVector[i][top]->getTileType() == tile_type)
        {
            flushPoints.push_back(PositionIndex(i, top));
        }
        else
        {
            break;
        }
    }
    
    for (int i = left-1; i >= 0; --i)
    {
        if (tileFieldsVector[i][top]->getStatus() == kStatusNormal && tileFieldsVector[i][top]->getTileType() == tile_type)
        {
            flushPoints.push_back(PositionIndex(i, top));
        }
        else
        {
            break;
        }
    }
    return flushPoints;
}

//垂直方向に進みながら水平方向のブロックの数を数える
vector<GamePlayScene::PositionIndex> GamePlayScene::crossVerticalTileCounting(int left, int top, kTile tile_type)
{
    vector<PositionIndex> flushPoints;
    
    for (int i = top; i < kMaxTileY; ++i)
    {
        if (tileFieldsVector[left][i]->getTileType() == tile_type)
        {
            vector<PositionIndex> horizontalFlushPoints = horizontalTileCounting(left, i, tile_type);
            if (horizontalFlushPoints.size() >= kTilePurgeThreshold)
            {
                flushPoints.insert(flushPoints.end(), horizontalFlushPoints.begin(), horizontalFlushPoints.end());
            }
        }
        else
        {
            break;
        }
    }
    
    for (int i = top-1; i >= 0; --i)
    {
        if (tileFieldsVector[left][i]->getTileType() == tile_type)
        {
            vector<PositionIndex> horizontalFlushPoints = horizontalTileCounting(left, i, tile_type);
            if (horizontalFlushPoints.size() >= kTilePurgeThreshold)
            {
                flushPoints.insert(flushPoints.end(), horizontalFlushPoints.begin(), horizontalFlushPoints.end());
            }
        }
        else
        {
            break;
        }
    }
    return flushPoints;
}

vector<GamePlayScene::PositionIndex> GamePlayScene::verticalTileCounting(int left, int top, kTile tile_type)
{
    vector<PositionIndex> flushPoints;
    if (tileFieldsVector[left][top]->getStatus() == kStatusMarked) {
        flushPoints.push_back(PositionIndex(left, top));
    }
    
    for (int i = top; i < kMaxTileY; ++i)
    {
        if (tileFieldsVector[left][i]->getStatus() == kStatusNormal && tileFieldsVector[left][i]->getTileType() == tile_type)
        {
            flushPoints.push_back(PositionIndex(left, i));
        }
        else
        {
            break;
        }
    }
    
    for (int i = top-1; i >= 0; --i)
    {
        if (tileFieldsVector[left][i]->getStatus() == kStatusNormal && tileFieldsVector[left][i]->getTileType() == tile_type)
        {
            flushPoints.push_back(PositionIndex(left, i));
        }
        else
        {
            break;
        }
    }
    return flushPoints;
}

//水平方向に進みながら垂直方向のブロックの数を数える
vector<GamePlayScene::PositionIndex> GamePlayScene::crossHorizontalTileCounting(int left, int top, kTile tile_type)
{
    vector<PositionIndex> flushPoints;
    for (int i = left; i < kMaxTileX; ++i)
    {
        if (tileFieldsVector[i][top]->getTileType() == tile_type)
        {
            vector<PositionIndex> verticalFlushPoints = verticalTileCounting(i, top, tile_type);
            if (verticalFlushPoints.size() >= kTilePurgeThreshold)
            {
                flushPoints.insert(flushPoints.end(), verticalFlushPoints.begin(), verticalFlushPoints.end());
            }
        }
        else
        {
            break;
        }
    }
    
    for (int i = left-1; i >= 0; --i)
    {
        if (tileFieldsVector[i][top]->getTileType() == tile_type)
        {
            vector<PositionIndex> verticalFlushPoints = verticalTileCounting(i, top, tile_type);
            if (verticalFlushPoints.size() >= kTilePurgeThreshold)
            {
                flushPoints.insert(flushPoints.end(), verticalFlushPoints.begin(), verticalFlushPoints.end());
            }
        }
        else
        {
            break;
        }
    }
    return flushPoints;
}

int GamePlayScene::crossTileCounting(int left, int top, kTile tile_type)
{
    vector<PositionIndex> crossHorizontalFlushPoints = crossHorizontalTileCounting(left, top, tile_type);
    vector<PositionIndex> crossVerticalFlushPoints   = crossVerticalTileCounting(left, top, tile_type);
    if (crossHorizontalFlushPoints.size() >= kTilePurgeThreshold)
    {
        for (vector<PositionIndex>::const_iterator it = crossHorizontalFlushPoints.begin(); it != crossHorizontalFlushPoints.end(); ++it)
        {
            tileFieldsVector[it->x][it->y]->setStatus(kStatusMarked);
        }
    }
    
    if (crossVerticalFlushPoints.size() >= kTilePurgeThreshold)
    {
        for (vector<PositionIndex>::const_iterator it = crossVerticalFlushPoints.begin(); it != crossVerticalFlushPoints.end(); ++it)
        {
            tileFieldsVector[it->x][it->y]->setStatus(kStatusMarked);
        }
    }
    // kTilePurgeThresholdを超えているかを知りたいだけなので、別に比較する意味はない・・・
    return max(crossHorizontalFlushPoints.size(), crossVerticalFlushPoints.size());
}

GamePlayScene::PositionIndex GamePlayScene::getPositionIndex(int tag)
{
    for (int x = 0; x < kMaxTileX; x++)
    {
        for (int y = 0; y < kMaxTileY; y++)
        {
            if (tileFieldsVector[x][y]->getTag() == tag)
            {
                return PositionIndex(x, y);
            }
        }
    }
    
    return PositionIndex(0, 0);
}

void GamePlayScene::animationStart()
{
    setTouchEnabled(false);
}

void GamePlayScene::animationEnd()
{
    setTouchEnabled(true);
}

