/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
using namespace Ogre;
//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);
protected:
	
    virtual void createScene(void);
	void Collision(void);
	virtual void createFrameListener();

	//움직이기 시전
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual void gamereset(const Ogre::FrameEvent& evt);
	
	/*
	virtual bool playerkeyPressed(const Ogre::FrameEvent& evt);
	virtual bool enemyKeyPressed(const Ogre::FrameEvent& evt);
	*/
	

//	virtual bool MovingBall(const Ogre::FrameEvent& evt);

	Real mMove;
	Real eMove;
	SceneNode* mPlayerNode;
	SceneNode* mEnemyNode;
	SceneNode* mBallNode;
	Vector3 mDirection; // 플레이어
	Vector3 eDirection; // 적
	bool pause;


private :
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);
	OgreBites::Label* mInfoLabel;
	OgreBites::Label* rInfoLabel;
	OgreBites::Label* InfoLabel;

	/*
	virtual bool enemykeyPressed(const OIS::KeyEvent& eke);
	virtual bool enemykeyReleased(const OIS::KeyEvent& eke);
	*/

};


class Ball
{
public :
	int playerscore;
	int enemyscore;
	Ball(SceneManager* sceneManagerBall);
	~Ball();
	void addToScene();
	void moveball(Real time);
	void HitBar();
	void changeSpeed(int change);
	Vector3 getPosition();

	SceneNode* mparticleNode;


private :
	SceneNode* BallNode;
	SceneManager* sceneManagerBall;

	int speed;
	Vector3 BallDirection;

	void changeHorizontalDirection(); // change horizontal direction 
	void changeVerticalDirection(); // change vertical direction 

};
//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
