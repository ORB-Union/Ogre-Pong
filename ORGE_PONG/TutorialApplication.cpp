/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
___                 __    __ _ _    _
/___\__ _ _ __ ___  / / /\ \ (_) | _(_)
//  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
|___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"
using namespace Ogre;

#define Up_Moving 15;
#define Down_Moving -15;

Ball *ballptr = NULL;

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication()
	: mMove(120),
	eMove(100),
	mPlayerNode(0),
	mEnemyNode(0),
	mBallNode(0),
	mDirection(Ogre::Vector3::ZERO),
	eDirection(Ogre::Vector3::ZERO),
	mInfoLabel(0),
	rInfoLabel(0),
	InfoLabel(0),
	pause(false)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}



void TutorialApplication::createFrameListener()
{
	BaseApplication::createFrameListener();
	mInfoLabel = mTrayMgr->createLabel(
		OgreBites::TL_TOPLEFT, // Label location
		"PlayerScore", // Label name
		"", // Label caption
		150); // Label width

	
	rInfoLabel = mTrayMgr->createLabel(
		OgreBites::TL_TOPRIGHT, // Label location
		"EnemyScore", // Label name
		"", // Label caption
		150); // Label width

	InfoLabel = mTrayMgr->createLabel(
		OgreBites::TL_TOP, // Label location
		"Title", // Label name
		"", // Label caption
		400); // Label width
		
}


//ball class
Ball::Ball(SceneManager *ball)
{
	sceneManagerBall = ball;
	speed = 250;
	BallDirection = Vector3(1,0,-1);
	playerscore = 0;
	enemyscore = 0;

	mparticleNode = 0;

}


Ball::~Ball()
{

}

Vector3 Ball::getPosition()
{
	return BallNode->getPosition();
}


void Ball::addToScene()
{
	Entity* sphereEnt = sceneManagerBall->createEntity("MySphere", SceneManager::PT_SPHERE);
	sphereEnt->setMaterialName("Examples/droplete");
	BallNode = sceneManagerBall->getRootSceneNode()->createChildSceneNode();
	BallNode->setScale(0.1f, 0.1f, 0.1f);
	BallNode->attachObject(sphereEnt);
	BallNode->setPosition(0.0f,0.0f,0.0f);

	ParticleSystem* sunParticle = sceneManagerBall->createParticleSystem("Sun", "Space/Sun");
	SceneNode* particleNode = sceneManagerBall->getRootSceneNode()->createChildSceneNode("Particle");
	particleNode->setPosition(0.0f,0.0f,0.0f);
	particleNode->attachObject(sunParticle);

	mparticleNode = particleNode;
}


void Ball::moveball(Real time)
{
	BallNode->translate((BallDirection * (speed * time)));
	Vector3 position = BallNode->getPosition();

	mparticleNode->translate((BallDirection * (speed * time)));

	if((position.x - 10) <= -200)
	{
		enemyscore++;
		BallNode->setPosition(0,0,0);
		mparticleNode->setPosition(0,0,0);
	}

	else if((position.x + 10) >= 200)
	{
		playerscore++;
		BallNode->setPosition(0,0,0);
		mparticleNode->setPosition(0,0,0);
	}

	else if((position.z -10) <= -100 && BallDirection.z < 0)
	{
		BallNode->setPosition(position.x, position.y , (-100 + 10));
		//mparticleNode->setPosition(position.x, position.y , (-100 + 10));
		changeVerticalDirection(); // 공의 방향전환
	}

	else if((position.z + 10) >= 100 &&BallDirection.z > 0)
	{
		BallNode->setPosition(position.x, position.y, (100 - 10));
		//mparticleNode->setPosition(position.x, position.y , (100 + -10));
		changeVerticalDirection(); // 공의 방향전환
	}
}
//공 방향 변화
void Ball::changeHorizontalDirection()   
{   
    BallDirection *= Vector3( -1, 1, 1 );
}
//공 방향 변화
void Ball::changeVerticalDirection()   
{   
    BallDirection *= Vector3( 1, 1, -1 ); // reverse the vertical direction   
}

void Ball::HitBar()
{
	Vector3 BottomBarPosition = sceneManagerBall->getSceneNode("playernode")->getPosition();

	Vector3 TopBarPosition = sceneManagerBall->getSceneNode("enemynode")->getPosition();

	Vector3 ballposition = getPosition();

	if((ballposition.x - 10) < (BottomBarPosition.x +1))
	{
		if((ballposition.z - 10) < (BottomBarPosition.z + 15) &&
			(ballposition.z +10) > (BottomBarPosition.z -15))
		{
			changeHorizontalDirection();
			BallNode->setPosition((BottomBarPosition.x + 11), ballposition.y, ballposition.z);
		}
	}

	else if((ballposition.x + 10) > (TopBarPosition.x -1))
	{
		if((ballposition.z -10) < (TopBarPosition.z + 15) && (ballposition.z + 10) > (TopBarPosition.z - 15))
		{
			changeHorizontalDirection();
			BallNode->setPosition((TopBarPosition.x - 11), ballposition.y, ballposition.z);
		}
	}
}

void Ball::changeSpeed(int change)
{
	if(speed <= 200 && speed >= 10)
	{
		speed += change;
	}

	if(speed < 10)
	{
		speed = 10;
	}

	else if(speed > 200)
	{
		speed = 200;
	}
}

void TutorialApplication::gamereset(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	if(pause)
	{
		mEnemyNode->translate(eDirection *evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		if(mEnemyNode->getPosition().z > 75)
		{
			mEnemyNode->setPosition(160, eDirection.y, 75);
		}

		else if(mEnemyNode->getPosition().z < -75)
		{
			mEnemyNode->setPosition(160, eDirection.y, -75);
		}

		ballptr->moveball(evt.timeSinceLastFrame);
		ballptr->HitBar();

		//플레이어
		mPlayerNode->translate(mDirection *evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		if(mPlayerNode->getPosition().z > 75)
		{
			mPlayerNode->setPosition(-160, mDirection.y, 75);
		}

		else if(mPlayerNode->getPosition().z < -75)
		{
			mPlayerNode->setPosition(-160, mDirection.y, -75);
		}
	}


	
	if(ballptr->playerscore >= 3 || ballptr->enemyscore >= 3)
	{
		ballptr->playerscore = 0;
		ballptr->enemyscore = 0;
		pause = false;

		mPlayerNode->setPosition(-160,0,0);
		mEnemyNode->setPosition(160,0,0);

	}



}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);
	char playerscoretext[30];
	char enemyscoretext[30];
	char Titletext[30];

	//player
	sprintf(playerscoretext, "Player : %i" , ballptr->playerscore);
	mInfoLabel->show();
	mInfoLabel->setCaption(playerscoretext);

	//enemy
	rInfoLabel->show();
	sprintf(enemyscoretext, "Enemy : %i" , ballptr->enemyscore);
	rInfoLabel->setCaption(enemyscoretext);

	InfoLabel->show();
	sprintf(Titletext, "PONG-GAME");
	InfoLabel->setCaption(Titletext);

	gamereset(evt);

	if(ballptr->playerscore >= 3 || ballptr->enemyscore >= 3)
	{
		gamereset(evt);
	}

	return ret;
}

bool TutorialApplication::keyPressed(const OIS::KeyEvent& ke)
{
		switch (ke.key)
		{
		case OIS::KC_ESCAPE:
			mShutDown = true;
			break;
		case OIS::KC_A:
			mDirection.z = -mMove;
			break;
		case OIS::KC_D:
			mDirection.z = mMove;
			break;

		case OIS::KC_J:
			eDirection.z = -eMove;
			break;

		case OIS::KC_L:
			eDirection.z = +eMove;
			break;
		case OIS::KC_SPACE:
			pause = true;
			break;
		default:
			break;
		}
	return true;
}
bool TutorialApplication::keyReleased(const OIS::KeyEvent& ke)
{

	switch (ke.key)
	{
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;
	case OIS::KC_A:
		mDirection.z = 0;
		break;
	case OIS::KC_D:
		mDirection.z = 0;
		break;

	case OIS::KC_J:
		eDirection.z = 0;
		break;

	case OIS::KC_L:
		eDirection.z = 0;
		break;
	default:
		break;
	}
	return true;
}



//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	ballptr = new Ball(mSceneMgr);
	ballptr->addToScene();

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	//배경
	mSceneMgr->setSkyBox(true,"Examples/SpaceSkyBox");
	// Create a light
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
	// Change a camera's position
	mCamera->setPosition(-300, 100, 0);
	mCamera->yaw(Ogre::Degree(-90));
	mCamera->pitch(Ogre::Degree(-20));


	//create wall(바닥)
	// Create a plane
	Ogre::Entity* planeEnt = mSceneMgr->createEntity("MyPlane", Ogre::SceneManager::PT_PLANE);
	planeEnt->setMaterialName("Examples/checker");
	Ogre::SceneNode *planeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	planeNode->pitch(Ogre::Degree(-90));
	planeNode->setPosition(0, -5, 0);
	planeNode->setScale(2.0f,1.0f,2.0f);
	planeNode->attachObject(planeEnt);


	//"Examples/checker.png"
	

	//create right wall
	Ogre::Entity* rightwallcubeEnt = mSceneMgr->createEntity("rightwall", Ogre::SceneManager::PT_CUBE);
	rightwallcubeEnt->setMaterialName("Examples/checkerwall");
	Ogre::SceneNode* rightwallcubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightwallnode");
	rightwallcubeNode->setPosition(0, 7.5, 100);
	rightwallcubeNode->setScale(4.0f, 0.25f, 0.1f);
	rightwallcubeNode->attachObject(rightwallcubeEnt);

	//create left wall
	Ogre::Entity* leftwallcubeEnt = mSceneMgr->createEntity("leftwall", Ogre::SceneManager::PT_CUBE);
	leftwallcubeEnt->setMaterialName("Examples/checkerwall");
	Ogre::SceneNode* leftwallcubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftwallnode");
	leftwallcubeNode->setPosition(0, 7.5, -100);
	leftwallcubeNode->setScale(4.0f, 0.25f, 0.1f);
	leftwallcubeNode->attachObject(leftwallcubeEnt);



	//Player Pannel
	//create cube
	// Create a cube
	Ogre::Entity* playercubeEnt = mSceneMgr->createEntity("Player", Ogre::SceneManager::PT_CUBE);
	playercubeEnt->setMaterialName("Examples/floorspecularPlayer");
	Ogre::SceneNode* playercubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("playernode");
	playercubeNode->setPosition(-160, 0, 0);
	playercubeNode->setScale(0.05f, 0.1f, 0.4f);
	playercubeNode->attachObject(playercubeEnt);

	//75 -75 좌표 

	mPlayerNode = playercubeNode;


	//Enemy Pannel
	//create cube
	// Create a cube
	Ogre::Entity* enemycubeEnt = mSceneMgr->createEntity("Enemy", Ogre::SceneManager::PT_CUBE);
	enemycubeEnt->setMaterialName("Examples/floorspecularEnemy");
	Ogre::SceneNode* enemycubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("enemynode");
	enemycubeNode->setPosition(160, 0, 0);
	enemycubeNode->setScale(0.05f, 0.1f, 0.4f);
	enemycubeNode->attachObject(enemycubeEnt);

	mEnemyNode = enemycubeNode;	
	//Examples/floorspecularDome

}


//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		TutorialApplication app;

		try {
			app.go();
		} catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			//MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;

#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif
//---------------------------------------------------------------------------