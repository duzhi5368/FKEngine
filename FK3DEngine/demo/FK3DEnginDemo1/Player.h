/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Player
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

#include "../../include/CommonBase/CommonBaseInsideHead.h"
#include "../../include/Math/MathInsideHead.h"
#include "../../include/Control/ControlInsideHeader.h"
#include "../../include/PhysiceSystem/PhysicsSystemInsideHeader.h"
#include "../../include/RenderSystem/RenderSystemInsideHeader.h"
#include "../../include/SoundSystem/SoundSystemInsideHeader.h"
#include "../../include/UISystem/UISystemInsideHeader.h"
#include "../../include/ParticleSystem/ParticleSystemInsideHeader.h"
#include "../../include/Scene/SceneInsideHeader.h"
#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"
using namespace FK3DEngine;

//--------------------------------------------------------------------------------------------------------------
//玩家类
class Player
{
public:
	//控制输入结构
	struct Input
	{
		bool	bForward;		//前进
		bool	bBackward;		//后腿
		bool	bShiftLeft;		//左移
		bool	bShiftRight;	//右移

		bool	bJump;			//跳跃

		float	fViewPitch;		//观察俯仰角
		float	fViewYaw;		//观察偏转角

		float	fFrameTime;		//帧延时
	};

	//玩家状态
	enum Status
	{
		Walk,					//普通行走
		FreeLook,				//自由观看
	};

public:
	Vector3			mPlayerPos;		//玩家坐标
	float			mViewPitch;		//观察俯仰角
	float			mViewYaw;		//观察偏转角
	float			mViewRoll;		//观察倾斜角

	Matrix4			mMatRotaFull;	//全方向旋转矩阵
	Matrix4			mMatRotaYaw;	//水平旋转矩阵
	Vector3			mMoveDir;		//移动方向
	Vector3			mVelocity;		//移动速度
	Vector3			mMovement;		//移动矢量
	Vector3			mGroundNormal;	//地面法线

	float			mfGravity;		//重力
	bool			mbIsLand;		//是否已落地

	float			mLandMoveFactor;//移动量参数
	float			mAirMoveFactor;	//空中移动量参数
	float			mJumpFactor;	//跳跃量参数
	float			mRotaFactor;	//旋转量参数
	float			mRollFactor;	//倾斜量参数
	float			mMaxRollValue;	//最大倾斜值
	float			mWalkLimit;		//步行最大速度值
	float			mWalkFriction;	//步行摩擦力
	float			mLookLimit;		//观看最大速度值
	float			mLookFriction;	//观看摩擦力

	RigidCylinder*	mpPlayerRigid;	//玩家碰撞刚体

	TriMesh*		mpMesh;			//玩家网格模型

	Skeleton*		mpAnim1;
	Skeleton*		mpAnim2;
	Skeleton*		mpAnim3;

	SceneNode*		mpSceneNode;	//玩家场景节点
	RenderInstance*	mpRenderInst;	//玩家渲染实例
	AnimControl*	mpAnimControl;	//玩家动画控制器

public:
	Player();
	~Player();

	//载入载入玩家模型及动画
	void LoadPlayerModel( LPCSTR szPlayerFileName );

	//设置玩家位置
	void SetPlayerPos( const Vector3& vPos );

	//更新玩家状态
	void UpdatePlayer( const Input& rInput, const Status eStatus = Walk );

	//更新玩家摄像机
	void UpdateCamera( Camera* pCamera );

protected:
	//修正移动速度
	void _ClipVelocity( const Vector3& vMoveVec, const float fFriction, const float fSpeedLimit );

	//进行玩家碰撞检测
	void _DoCollisionDetect();
};