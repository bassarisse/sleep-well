#include "GamePlay.h"

#include "SimpleAudioEngine.h"
#include "B2DebugDraw/B2DebugDrawLayer.h"
#include "GameObject.h"
#include "Entities/Player.h"
#include "Entities/Enemies/BadNeuron.h"
#include "Entities/Static/Nerve.h"
#include "Entities/EnemySpawnPoint.h"
#include "Entities/CollectableSpawnPoint.h"
#include "Entities/Abstract/Pulse.h"
#include "Scenes/GameoverScene.h"
#include "Scenes/LevelTransition.h"

#include "GameState.h"

USING_NS_CC;

using namespace CocosDenshion;

GamePlay::~GamePlay() {
    
    delete _contactListener;
    
    CC_SAFE_RELEASE(_player);
	CC_SAFE_RELEASE(_pauseLayer);

    for(std::vector<GameObject *>::size_type i = 0; i < _gameObjects.size(); i++) {
        
        GameObject *deadObject = _gameObjects[i];
        
        deadObject->release();
        deadObject = NULL;
    }
    
    for(std::vector<GameObject *>::size_type i = 0; i < _staticObjects.size(); i++) {
        _staticObjects[i]->release();
    }
    
    for(std::vector<EnemySpawnPoint *>::size_type i = 0; i < _enemySpawnPoints.size(); i++) {
        _enemySpawnPoints[i]->release();
    }
    
    for(std::vector<CollectableSpawnPoint *>::size_type i = 0; i < _collectableSpawnPoints.size(); i++) {
        _collectableSpawnPoints[i]->release();
    }
    
}

Scene* GamePlay::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GamePlay::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePlay::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    
    srand(time(NULL));
    
    int level = GameState::getInstance()->getActLevel();
    
    _winSize = Director::getInstance()->getWinSize();
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //Point origin = Director::getInstance()->getVisibleOrigin();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("general.plist");
    
    _contactListener = new BAContactListener();
    _mainLayer = Layer::create();
    _mainBatchNode = SpriteBatchNode::create("general.png");
    _tiledMap = TMXTiledMap::create("main.tmx");
    _isTouching = false;
    _touchLocation = Point(0, 0);
    _gameTime = 0;
    
	_tiledMap->getLayer("main")->getTexture()->setAliasTexParameters();
    _mainBatchNode->getTexture()->setAliasTexParameters();
    
    _mainLayer->addChild(_tiledMap);
    _mainLayer->addChild(_mainBatchNode);
    
    this->addChild(_mainLayer);
    
    TMXObjectGroup *collisionGroup = _tiledMap->getObjectGroup("collision");
    TMXObjectGroup *gameObjectsGroup = _tiledMap->getObjectGroup("objects");
    
    b2Vec2 gravity = b2Vec2(0.0f, kGravity - level / 2.0f);
    
    _world = new b2World(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    _world->SetContactListener(_contactListener);
    
    Array *gameObjects = gameObjectsGroup->getObjects();
    Object *gameObject;
    
	//Allocating object here so that we can pass its reference to all enemies.
	//Actual initialization occurs when it's TMX entity is found below
	_player = new Player();
    
    int enemyCount = 0;

    CCARRAY_FOREACH(gameObjects, gameObject) {
        Dictionary *objectProperties = (Dictionary *)gameObject;
        
        String *type = (String *)objectProperties->objectForKey("type");
        if (!type)
            continue;
        
        if (type->compare("Player") == 0)
		{
            _player->init(_world, objectProperties, this);
            _mainBatchNode->addChild(_player->getNode());
            //_gameObjects.push_back(player);
		}
		else if (type->compare("EnemySpawnPoint") == 0)
        {
            EnemySpawnPoint *spawnPoint = new EnemySpawnPoint();
            spawnPoint->init(this, objectProperties);
            _enemySpawnPoints.push_back(spawnPoint);
            
        }
		else if (type->compare("CollectableSpawnPoint") == 0)
        {
            CollectableSpawnPoint *spawnPoint = new CollectableSpawnPoint();
            spawnPoint->init(this, objectProperties);
            _collectableSpawnPoints.push_back(spawnPoint);
            
        } else {
            
            GameObjectType gameObjectType = GameObjectTypeUnknown;
            
            if(type->compare("BadNeuron") == 0 && enemyCount < level * 0.75f) {
                gameObjectType = GameObjectTypeBadNeuron;
                enemyCount++;
            }
            
            if (gameObjectType != GameObjectTypeUnknown)
                this->createGameObject(gameObjectType, objectProperties);

        }
        
    }
    
    Array *collisions = collisionGroup->getObjects();
    Object *collision;
    CCARRAY_FOREACH(collisions, collision) {
        Dictionary *objectProperties = (Dictionary *)collision;
        
        float x = floatValue(objectProperties->objectForKey("x")) / PTM_RATIO;
        float y = floatValue(objectProperties->objectForKey("y")) / PTM_RATIO;
        
        String *width = (String *)objectProperties->objectForKey("width");
        Array *points = (Array *)objectProperties->objectForKey("polylinePoints");
        if (!points)
            points = (Array *)objectProperties->objectForKey("points");
        Object *aObject;
        
        if (width->compare("") != 0) {
            
            float halfwidth = floatValue(width) / 2.0f / PTM_RATIO;
            float halfheight = floatValue(objectProperties->objectForKey("height")) / 2.0f / PTM_RATIO;
            
            x += halfwidth;
            y += halfheight;
            
            b2PolygonShape shape;
            shape.SetAsBox(halfwidth, halfheight);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            
            String *type = (String *)objectProperties->objectForKey("type");
            if (type) {
                
                GameObject *gameObject = NULL;
                if(type->compare("Nerve") == 0) {
                    gameObject = new Nerve();
                    ((Nerve *)gameObject)->init(_world, objectProperties, _player);
                }
                
                if (gameObject) {
                    fixtureDef.userData = gameObject;
                    _staticObjects.push_back(gameObject);
                }
                
            }
            
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.position.Set(x, y);
            b2Body *body = _world->CreateBody(&bodyDef);
            
            body->CreateFixture(&fixtureDef);
            
        } else if (points) {
            
            std::vector<b2Vec2> verts;
            
            CCARRAY_FOREACH(points, aObject) {
                Dictionary *dict = (Dictionary *)aObject;
                
                float vertX = floatValue(dict->objectForKey("x")) / PTM_RATIO;
                float vertY = floatValue(dict->objectForKey("y")) / PTM_RATIO;
                
                verts.push_back(b2Vec2(vertX, -vertY));
            }
            
            b2ChainShape shape;
            shape.CreateChain(&verts[0], points->count());
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            
            String *type = (String *)objectProperties->objectForKey("type");
            if (type) {
                
                GameObject *gameObject = NULL;
                if(type->compare("Nerve") == 0) {
                    gameObject = new Nerve();
                    ((Nerve *)gameObject)->init(_world, objectProperties, _player);
                }
                
                if (gameObject) {
                    fixtureDef.userData = gameObject;
                    _staticObjects.push_back(gameObject);
                }
                
            }
            
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.position.Set(x, y);
            b2Body *body = _world->CreateBody(&bodyDef);
            
            body->CreateFixture(&fixtureDef);
        }
        
    }
    
    _movingHorizontalStates.push_back(MovingStateHorizontalStopped);
    _movingVerticalStates.push_back(MovingStateVerticalStopped);
    
	_hudPowerBar = new HudPowerBar();
	_hudApneaBar = new HudApneaBar();

	_hudPowerBar->init();
	_hudPowerBar->autorelease();
	
	_hudApneaBar->init();
	_hudApneaBar->autorelease();

	this->addChild(_hudPowerBar);
	this->addChild(_hudApneaBar);
    
    _timeLabel = LabelBMFont::create("0", "MainFont.fnt", _winSize.width / 5, Label::HAlignment::RIGHT);
    
	_timeLabel->setAnchorPoint(Point(1, 1));
	_timeLabel->setPosition(Point(_winSize.width - 10, _winSize.height - 10));
    
	this->addChild(_timeLabel);

	_pauseLayer = LayerColor::create(Color4B(0, 0, 0, 130));
	_pauseLayer->retain();
	_pauseLayer->setPosition(Point(0,0));
	_pauseLayer->setContentSize(Size(1024, 768));
    
	LabelBMFont* pauseLabel = LabelBMFont::create("Paused!", "MainFont.fnt", 300, Label::HAlignment::CENTER);
	pauseLabel->setAnchorPoint(Point(0.5f, 0.5f));
	pauseLabel->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
	_pauseLayer->addChild(pauseLabel);

	_isPaused = false;

    this->scheduleUpdate();
    
	_debugLayer = NULL;
    //_debugLayer = B2DebugDrawLayer::create(_world, PTM_RATIO);
    
    if (_debugLayer)
        this->addChild(_debugLayer, 9999);
    
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("main_bgm.wav");

    return true;
}

void GamePlay::update(float dt) {
    
	if(!_isPaused)
		_world->Step(dt, 8, 3);
    
    if (_isTouching) {
        Point touchLocation = Director::getInstance()->convertToGL(_touchLocation);
        touchLocation = _tiledMap->convertToNodeSpace(touchLocation);
        _player->followPoint(touchLocation);
    }
    
    _gameTime += dt;
    
    _timeLabel->setString(String::createWithFormat("%.1f", _gameTime)->getCString());
    if (_gameTime > 10)
        _timeLabel->setColor(Color3B(255, 100, 100));
    
    _player->setMovingHorizontalState(_movingHorizontalStates[_movingHorizontalStates.size() - 1]);
    _player->setMovingVerticalState(_movingVerticalStates[_movingVerticalStates.size() - 1]);
    _player->update(dt);
    
    Point playerPosition = _player->getNode()->getPosition();
    
    int x = MAX(playerPosition.x, _winSize.width/2);
    int y = MAX(playerPosition.y, _winSize.height/2);
    x = MIN(x, (_tiledMap->getMapSize().width * _tiledMap->getTileSize().width) - _winSize.width / 2);
    y = MIN(y, (_tiledMap->getMapSize().height * _tiledMap->getTileSize().height) - _winSize.height/2);
    Point actualPosition = Point(x, y);
    
    Point centerOfView = Point(_winSize.width/2, _winSize.height/2);
    Point viewPoint = centerOfView - actualPosition;
    _mainLayer->setPosition(viewPoint);
    
    if (_debugLayer)
        _debugLayer->setPosition(viewPoint);
    
    for(std::vector<GameObject *>::size_type i = 0; i < _gameObjects.size(); i++) {
        _gameObjects[i]->update(dt);
    }
    
    for(std::vector<GameObject *>::size_type i = 0; i < _staticObjects.size(); i++) {
        _staticObjects[i]->update(dt);
    }
    
    for(std::vector<EnemySpawnPoint *>::size_type i = 0; i < _enemySpawnPoints.size(); i++) {
        _enemySpawnPoints[i]->update(dt);
    }
    
    for(std::vector<CollectableSpawnPoint *>::size_type i = 0; i < _collectableSpawnPoints.size(); i++) {
        _collectableSpawnPoints[i]->update(dt);
    }
    
	//Add the dead objects to be removed in a separated array
	for(std::vector<GameObject *>::size_type i = 0; i < _gameObjects.size(); i++) {
        GameObject *gameObj = _gameObjects[i];
        
		if (gameObj->getState() == GameObjectStateDying && !gameObj->getIsDestroyed())
			gameObj->destroyBody(_world);
        
		if (gameObj->getState() == GameObjectStateDead) {
        /*
			switch(gameObj->getType()) {
                case GameObjectTypeCoffee:
                    SimpleAudioEngine::getInstance()->playEffect("drinked_coffee.wav");
					break;

					break;
				default:
                    break;
			}
         */

            this->removeObject(gameObj);
		}
            
    }
    
	_hudPowerBar->setLevel(_player->getPower());
	_hudApneaBar->setLevel(_player->getApneaLevel());
    
    if (_player->getApneaLevel() <= 0) {
        
        GameState::getInstance()->addActTime(_gameTime);
        
        if (GameState::getInstance()->getActLevel() > 10) {
            SimpleAudioEngine::getInstance()->playEffect("wilhem.wav");
            Director::getInstance()->replaceScene(GameoverScene::scene());
        } else {
            Director::getInstance()->replaceScene(LevelTransition::scene());
        }
        
		return;
    }

}

GameObject* GamePlay::createGameObject(GameObjectType type, Dictionary *properties) {
    
    switch (type) {
            
            /*
             * Collectables
             */
            /*
        case GameObjectTypeCoffee:
        {
            Coffee* newPowerup = new Coffee();
            newPowerup->init(_world, properties);
            _mainBatchNode->addChild(newPowerup->getNode());
            _gameObjects.push_back(newPowerup);

            return newPowerup;
        }
            break;
             */
            
            /*
             * Enemies
             */
        case GameObjectTypeBadNeuron:
        {
            auto newEnemy = new BadNeuron();
            newEnemy->init(_world, properties, _player);
            _mainBatchNode->addChild(newEnemy->getNode());
            _gameObjects.push_back(newEnemy);

            return newEnemy;
        }
            break;
            
        default:
            break;
    }
    
    return NULL;
}

void GamePlay::removeObject(GameObject* deadObject) {
    
	std::vector<GameObject *>::iterator pos;
	pos = std::find(_gameObjects.begin(), _gameObjects.end(), deadObject);
	_gameObjects.erase(pos);
    
	Node* deadNode = deadObject->getNode();


	if (!deadObject->getIsDestroyed())
		deadObject->destroyBody(_world);
    
    deadNode->stopAllActions();
	deadNode->removeFromParentAndCleanup(true);
	deadObject->release();
    deadObject = NULL;
    
}

void GamePlay::registerWithTouchDispatcher()
{
    Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GamePlay::ccTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    _isTouching = true;
    
    this->ccTouchMoved(pTouch, pEvent);
    
    return true;
}

void GamePlay::ccTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    
    _touchLocation = pTouch->getLocationInView();
    
    Point touchLocation = Director::getInstance()->convertToGL(_touchLocation);
    touchLocation = _tiledMap->convertToNodeSpace(touchLocation);
    
    _player->followPoint(touchLocation);
    
}

void GamePlay::ccTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (_isPaused)
        this->buttonB(false);
    _isTouching = false;
    _player->stopFollowingPoint();
}

void GamePlay::ccTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    _isTouching = false;
    _player->stopFollowingPoint();
}

void GamePlay::buttonLeft(bool pressed) {
    
    if (pressed) {
        
        if (_buttonLeftPressed)
            return;
        _buttonLeftPressed = true;
        
        _movingHorizontalStates.push_back(MovingStateLeft);
        _player->changeDirection(kDirectionLeft);
        
    } else {
        
        _buttonLeftPressed = false;
        
        std::vector<MovingHorizontalState>::iterator pos;
        pos = std::find(_movingHorizontalStates.begin(), _movingHorizontalStates.end(), MovingStateLeft);
        if (pos != _movingHorizontalStates.end()) {
            _movingHorizontalStates.erase(pos);
        }
        
    }
    
}

void GamePlay::buttonRight(bool pressed) {
    
    if (pressed) {
        
        if (_buttonRightPressed)
            return;
        _buttonRightPressed = true;
        
        _movingHorizontalStates.push_back(MovingStateRight);
        _player->changeDirection(kDirectionRight);
        
    } else {
        
        _buttonRightPressed = false;
        
        std::vector<MovingHorizontalState>::iterator pos;
        pos = std::find(_movingHorizontalStates.begin(), _movingHorizontalStates.end(), MovingStateRight);
        if (pos != _movingHorizontalStates.end()) {
            _movingHorizontalStates.erase(pos);
        }
        
    }
    
}

void GamePlay::buttonUp(bool pressed) {
    
    if (pressed) {
        
        if (_buttonUpPressed)
            return;
        _buttonUpPressed = true;
        
        _movingVerticalStates.push_back(MovingStateUp);
        _player->changeDirection(kDirectionUp);
        
    } else {
        
        _buttonUpPressed = false;
        
        std::vector<MovingVerticalState>::iterator pos;
        pos = std::find(_movingVerticalStates.begin(), _movingVerticalStates.end(), MovingStateUp);
        if (pos != _movingVerticalStates.end()) {
            _movingVerticalStates.erase(pos);
        }
        
    }
    
}

void GamePlay::buttonDown(bool pressed) {
    
    if (pressed) {
        
        if (_buttonDownPressed)
            return;
        _buttonDownPressed = true;
        
        _movingVerticalStates.push_back(MovingStateDown);
        _player->changeDirection(kDirectionDown);
        
    } else {
        
        _buttonDownPressed = false;
        
        std::vector<MovingVerticalState>::iterator pos;
        pos = std::find(_movingVerticalStates.begin(), _movingVerticalStates.end(), MovingStateDown);
        if (pos != _movingVerticalStates.end()) {
            _movingVerticalStates.erase(pos);
        }
        
    }
    
}

void GamePlay::buttonA(bool pressed) {
	if(!pressed)
		return;
    
    _player->action();
    
}

void GamePlay::buttonB(bool pressed) {
	if(pressed)
		return;

	if(_isPaused)
	{
		_isPaused = false;
		this->removeChild(_pauseLayer);
		this->recursivelyResumeAllChildren(this);
	} else 
	{
		_isPaused = true;
		this->addChild(_pauseLayer);
		this->recursivelyPauseAllChildren(this);
	}
}

void GamePlay::onEnter() {
	BaseLayer::onEnter();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("main_bgm.wav", true);
}


void GamePlay::showScore(Point positionToShow, int scoreAmount) {
	LabelBMFont *deathScore = LabelBMFont::create(
					String::createWithFormat("%d", scoreAmount)->getCString(), 
					"MainFont.fnt", 50, Label::HAlignment::CENTER);

	//deathScore->setPosition(Point(_node->getPosition().x + (_node->getContentSize().width / 2),
	//	_node->getPosition().y + (_node->getContentSize().height + 10)));

	deathScore->setPosition(Point(positionToShow.x, positionToShow.y));

	_mainLayer->addChild(deathScore);

	int randomNumber = (15 + (rand() % 31)) * (rand() % 2 == 0 ? 1 : -1);
	int randomHeig = (15 + (rand() % 16));

	JumpTo* scoreJump = JumpTo::create(1.0f, Point(deathScore->getPosition().x + randomNumber,
		deathScore->getPosition().y), randomHeig, 1);
    
	FadeOut* fadingOut = FadeOut::create(0.7f);
	Sequence* timerFade = Sequence::create(DelayTime::create(0.4f),
                                           fadingOut,
                                           CallFunc::create([deathScore](){
        deathScore->removeFromParentAndCleanup(true);
    }),
                                           NULL);

	Spawn* wrap = Spawn::create(timerFade, scoreJump, NULL);

	deathScore->runAction(wrap);
}

void GamePlay::shakeScreen() {
    
    int multiplier = rand() % 2 == 0 ? 1 : -1;
    
    Action *shakeAction = Sequence::create(
                                           MoveTo::create(0.06f, Point( 6,  6 * multiplier)),
                                           MoveTo::create(0.06f, Point(-6, -6 * multiplier)),
                                           MoveTo::create(0.05f, Point( 4,  4 * multiplier)),
                                           MoveTo::create(0.05f, Point(-4, -4 * multiplier)),
                                           MoveTo::create(0.04f, Point( 2,  2 * multiplier)),
                                           MoveTo::create(0.04f, Point(-2, -2 * multiplier)),
                                           MoveTo::create(0.03f, Point( 0,  0)),
                                           NULL);
    
    this->stopAllActions();
    this->runAction(shakeAction);
    
}

void GamePlay::addPulse(float angle, float power) {
    
    auto pulse = new Pulse();
    pulse->init(_world, NULL, _player, angle, power);
    
    _mainBatchNode->addChild(pulse->getNode());
    _gameObjects.push_back(pulse);
    
}