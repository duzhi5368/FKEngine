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

#include "Player.h"

//--------------------------------------------------------------------------------------------------------------
Player::Player()
	: mPlayerPos		(Vector3::Zero)
	, mViewPitch		(0.0f)
	, mViewYaw			(0.0f)
	, mViewRoll			(0.0f)
	, mMoveDir			(Vector3::Zero)
	, mVelocity			(Vector3::Zero)
	, mMovement			(Vector3::Zero)
	, mGroundNormal		(Vector3::Zero)
	, mfGravity			(120.0f)
	, mbIsLand			(false)
	, mLandMoveFactor	(160.0f)
	, mAirMoveFactor	(20.0f)
	, mJumpFactor		(50.0f)
	, mRotaFactor		(30.0f)
	, mRollFactor		(10.0f)
	, mMaxRollValue		(2.0f)
	, mWalkLimit		(40.0f)
	, mWalkFriction		(80.0f)
	, mLookLimit		(30.0f)
	, mLookFriction		(120.0f)
	, mpPlayerRigid		(NULL)
{
	mpPlayerRigid = (RigidCylinder*)PhysicsSystem::mpSingleton->CreateRigidBody( RigidCylinder::Creator );
	mpPlayerRigid->SetRigidCylinder( 1.5f, 5.5f );
}
//--------------------------------------------------------------------------------------------------------------
Player::~Player()
{
}
//--------------------------------------------------------------------------------------------------------------
//载入载入玩家模型及动画
void Player::LoadPlayerModel( LPCSTR szPlayerFileName )
{
	//ObjectFileLoader::mpSingleton->LoadObjectFile( "ElfMesh.scene" );
	//mpMesh = (TriMesh*)*ObjectFileLoader::mpSingleton->mLoadedGeometryList.Begin();

	//ObjectFileLoader::mpSingleton->LoadObjectFile( "ElfStand.scene" );
	//mpAnim1 = ObjectFileLoader::mpSingleton->mpLoadedSkeleton;

	//ObjectFileLoader::mpSingleton->LoadObjectFile( "ElfBeginRun.scene" );
	//mpAnim2 = ObjectFileLoader::mpSingleton->mpLoadedSkeleton;

	//ObjectFileLoader::mpSingleton->LoadObjectFile( "ElfRun.scene" );
	//mpAnim3 = ObjectFileLoader::mpSingleton->mpLoadedSkeleton;

	//mpSceneNode = (SceneNode*)g_pSceneMgr->GetSceneRootNode()->CreateChild();

	//mpMesh->InitializeInstanceList( 1, 1 );
	//mpMesh->SetSkeleton( mpAnim3 );
	//mpRenderInst = mpMesh->CreateInstance();

	//mpAnimControl = AnimControlManager::mpSingleton->CreateAnimControl();
	//mpAnimControl->SetAnimationInfo( mpAnim3->mNumKey, mpAnim3->mIntervalTime );
	//mpRenderInst->mpAnimControl = mpAnimControl;

	//mpSceneNode->AttachRenderInstance( mpRenderInst );
}
//--------------------------------------------------------------------------------------------------------------
//设置玩家位置
void Player::SetPlayerPos( const Vector3& vPos )
{
	mPlayerPos = vPos;
	mpPlayerRigid->SetMassCenter( vPos );
}
//--------------------------------------------------------------------------------------------------------------
//更新玩家状态
void Player::UpdatePlayer( const Input& rInput, const Status eStatus )
{
	//----------------------------------------------------------
	// 首先根据鼠标输入计算摄像机显示角度
	//
	// 鼠标向左移动时视角向左偏，mViewYaw 为负数，并逐渐减小。
	// 鼠标向右移动时视角向右偏，mViewYaw 为正数，并逐渐增大。
	// 鼠标向上移动时视角向下偏，mViewPitch 为负数，并逐渐减小。
	// 鼠标向下移动时视角向上偏，mViewPitch 为正数，并逐渐增大。
	//----------------------------------------------------------

	float fRotaAddYaw	= rInput.fViewYaw * mRotaFactor * rInput.fFrameTime;
	float fRotaAddPitch	= rInput.fViewPitch * mRotaFactor * rInput.fFrameTime;

	mViewPitch -= fRotaAddPitch;

	//计算摄像机显示角度，要根据显示是否倒转确定如何改变横向视角。
	if( mViewPitch > 90.0f || mViewPitch < -90.0f )
		mViewYaw += fRotaAddYaw;
	else
		mViewYaw -= fRotaAddYaw;

	if( mViewYaw	< -180.0f )		mViewYaw	+= 360.0f;
	if( mViewPitch	< -180.0f )		mViewPitch	+= 360.0f;
	if( mViewYaw	> 180.0f )		mViewYaw	-= 360.0f;
	if( mViewPitch	> 180.0f )		mViewPitch	-= 360.0f;


	//根据键盘输入计算控制移动方向
	mMoveDir.x = 0.0f;
	mMoveDir.y = 0.0f;
	mMoveDir.z = 0.0f;

	if( rInput.bForward )
		mMoveDir.z += 1.0f;
	if( rInput.bBackward )
		mMoveDir.z -= 1.0f;
	if( rInput.bShiftLeft )
		mMoveDir.x -= 1.0f;
	if( rInput.bShiftRight )
		mMoveDir.x += 1.0f;

	//无侧移时逐渐恢复倾斜角
	const float fRollFactor = mRollFactor * rInput.fFrameTime;
	if( mMoveDir.x == 0.0f )
	{
		if( fRollFactor > Abs( mViewRoll ) )
		{
			mViewRoll = 0.0f;
		}
		else
		{
			if( mViewRoll > 0.0f )
				mViewRoll -= fRollFactor;
			else if( mViewRoll < 0.0f )
				mViewRoll += fRollFactor;
		}
	}
	//当侧移时改变倾斜角
	else if( mMoveDir.x > 0.0f )
	{
		mViewRoll += fRollFactor;
		if( mViewRoll > mMaxRollValue )
			mViewRoll = mMaxRollValue;
	}
	else
	{
		mViewRoll -= fRollFactor;
		if( mViewRoll < -mMaxRollValue )
			mViewRoll = -mMaxRollValue;
	}

	//单位化移动方向
	mMoveDir.Normalize();


	//计算旋转矩阵

	//绕 Y 轴旋转 * 绕 X 轴旋转
	static Matrix4 MatA;
	static Matrix4 MatB;

	//绕 Y 轴旋转
	MatA.SetRotationY( mViewYaw );

	//获取水平旋转矩阵
	mMatRotaYaw = MatA.GetInverse();

	//级联绕 X 轴旋转
	MatB.SetRotationX( mViewPitch );
	Mat4TransMat4( &mMatRotaFull, &MatA, &MatB );

	//生成全方向旋转矩阵（观察空间到世界空间转换矩阵）
	mMatRotaFull.SetInverse();


	//计算玩家移动矢量
	static Vector3 vMoveVec;

	//判断玩家移动状态
	switch (eStatus)
	{
		//普通行走状态
	case Walk:
		{
			//用水平旋转矩阵转换移动方向
			mMoveDir *= mMatRotaYaw;

			//计算玩家下落速度
			mVelocity.y -= mfGravity * rInput.fFrameTime;	// V （t 时间的速度） = a （加速度） * t （时间）

			//如果玩家已落地
			if( mbIsLand )
			{
				//计算玩家控制移动速度
				vMoveVec = mMoveDir * mLandMoveFactor * rInput.fFrameTime;

				//根据地面法线修正移动方向
				vMoveVec -= mGroundNormal * mGroundNormal.GetDot( vMoveVec );

				//计算摩擦力
				const float fFriction = mWalkFriction * rInput.fFrameTime * mGroundNormal.y;

				//修正移动速度
				_ClipVelocity( vMoveVec, fFriction, mWalkLimit );

				//如果玩家进行跳跃则加上向上的速度
				if( rInput.bJump )
					mVelocity.y += mJumpFactor;
			}
			//如果玩家在空中
			else
			{
				//允许玩家在空中控制以很小的速度控制移动方向（便于攀跳物体）
				mVelocity += mMoveDir * mAirMoveFactor * rInput.fFrameTime;
			}

			//计算玩家移动距离
			mMovement = mVelocity * rInput.fFrameTime * 0.5f;	// S （t 时间匀变速运动距离） = V （t 时间的速度） * 0.5

			//进行玩家碰撞检测
			mbIsLand = false;

			_DoCollisionDetect();
			break;
		}
		//自由观看状态
	case FreeLook:
		{
			//用旋转矩阵转换移动方向
			mMoveDir *= mMatRotaFull;

			//计算玩家控制移动速度
			vMoveVec = mMoveDir * mLandMoveFactor * rInput.fFrameTime;

			//计算摩擦力
			const float fFriction = mLookFriction * rInput.fFrameTime;

			//修正移动速度
			_ClipVelocity( vMoveVec, fFriction, mLookLimit );

			//计算玩家移动距离
			mMovement = mVelocity * rInput.fFrameTime * 0.5f;

			//设置玩家新位置
			mPlayerPos += mMovement;
			mpPlayerRigid->SetMassCenter( mPlayerPos );
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
//更新玩家摄像机
void Player::UpdateCamera( Camera* pCamera )
{
	pCamera->SetView( mPlayerPos, mViewPitch, mViewYaw, mViewRoll );
	pCamera->mViewPos.y += 2.0f;
}
//--------------------------------------------------------------------------------------------------------------
//修正移动速度
void Player::_ClipVelocity( const Vector3& vMoveVec, const float fFriction, const float fSpeedLimit )
{
	//加上移动速度
	mVelocity += vMoveVec;

	//计算摩擦力
	float fSpeed = mVelocity.GetLength();

	if( fSpeed > fFriction )
	{
		const float fOldSpeed = fSpeed;
		fSpeed -= fFriction;

		mVelocity *= fSpeed / fOldSpeed;
	}
	else
	{
		mVelocity = Vector3::Zero;
	}

	//限制最大速度
	float fSpeedFactor = fSpeedLimit;
	if( fSpeed > fSpeedFactor )
	{
		fSpeedFactor /= fSpeed;
		mVelocity *= fSpeedFactor;
	}
}
//--------------------------------------------------------------------------------------------------------------
//进行玩家碰撞检测
void Player::_DoCollisionDetect()
{
	//设置玩家刚体线性移动量
	mpPlayerRigid->SetMassCenter( mPlayerPos );
	mpPlayerRigid->SetLinearMove( mMovement );

	static CollisionInfo sCollisionInfo;
	PhysicsSystem::mpSingleton->DoCollisionDetect( mpPlayerRigid, &sCollisionInfo );

	//如果未发生碰撞
	if( !sCollisionInfo.bCollision )
	{
		mPlayerPos += mMovement;
		mpPlayerRigid->SetMassCenter( mPlayerPos );
		return;
	}

	//计算移动目的坐标
	Vector3 vTargetPos = mPlayerPos + mMovement;
	mPlayerPos = sCollisionInfo.vCollisionPos;

	//判断碰撞体类型
	switch (sCollisionInfo.pCollisionVolume->mBoundType)
	{
		//普通碰撞类型
	case BoundInfo::General:
		{
			//计算滑动坐标
			Vector3 vSlidePos = vTargetPos + sCollisionInfo.vFaceNormal *
				( sCollisionInfo.vCollisionPos - vTargetPos ).GetDot( sCollisionInfo.vFaceNormal );

			//计算滑动矢量
			mMovement = vSlidePos - sCollisionInfo.vCollisionPos;

			//修正速度方向
			mVelocity -= sCollisionInfo.vFaceNormal * mVelocity.GetDot( sCollisionInfo.vFaceNormal );

			//检测是否已落地（碰撞面陡峭程度）
			if( sCollisionInfo.vFaceNormal.y >= 0.6f )
			{
				mbIsLand = true;
				mGroundNormal = sCollisionInfo.vFaceNormal;
			}

			//检测滑动碰撞
			_DoCollisionDetect();
			break;
		}
		//楼梯碰撞类型
	case BoundInfo::Stair:
		{
			//根据移动速度上提玩家坐标
			mPlayerPos.y += ( vTargetPos - sCollisionInfo.vCollisionPos ).GetLength();
			break;
		}
		//阶梯碰撞类型
	case BoundInfo::Ladder:
		{
			break;
		}
	}
}