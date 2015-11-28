/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BoundNode
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	碰撞节点类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class BoundNode;
	typedef Stack< BoundNode* >		BoundNodeList;
	//--------------------------------------------------------------------------------------------------------------
	//碰撞节点类
	class FK_DLL BoundNode
	{
	public:
		BoundNodeList		mChildNodeList;		//子碰撞节点列表
		BoundVolumeList		mBoundVolumeList;	//挂接碰撞体列表

		BoundNode*			mpParentNode;		//父级碰撞节点

		Matrix4				mLocalMatrix;		//碰撞节点局部坐标矩阵
		Matrix4				mWorldMatrix;		//碰撞节点世界坐标矩阵
		Matrix4				mInvWorldMatrix;	//碰撞节点世界坐标反矩阵

		bool				mbUpdateLocalMatrix;//是否需要更新局部坐标矩阵
		bool				mbUpdateWorldMatrix;//是否需要更新世界坐标矩阵
		bool				mbUpdateChildMatrix;//它的某些子节点是否需要更新矩阵

		Vector3				mPosition;			//位移矢量
		Quaternion			mOrientation;		//旋转四元数
		Vector3				mScale;				//缩放矢量

		static Quaternion	mTmpQuat;			//计算临时四元数

	public:
		BoundNode();
		~BoundNode();

		//创建子碰撞节点
		BoundNode* CreateChild();

		//删除指定子节点
		void DeleteChild( BoundNode* pChild );

		//删除所有子节点
		void DeleteAllChild();

		//挂接碰撞体
		void AttachBoundVolume( BoundVolume* pBoundVolume );

		//断开碰撞体
		void DetachBoundVolume( BoundVolume* pBoundVolume );

		//位移节点（相对）
		void TranslateNode( const Vector3& vTrans );
		void TranslateNode( const float x, const float y, const float z );
		//位移节点
		void SetPosition( const Vector3& vTrans );
		void SetPosition( const float x, const float y, const float z );

		//旋转节点（相对）
		void RotateNode( const Vector3& vAxis, const float fDegree );
		void RotateNode( const Quaternion& rQuat );
		//旋转节点
		void SetRotate( const Vector3& vAxis, const float fDegree );
		void SetRotate( const Quaternion& rQuat );
		void SetRotate( const float fPitch, const float fYaw, const float fRoll );

		//绕 X 轴旋转（相对）
		void Pitch( const float fDegree );
		//绕 Y 轴旋转（相对）
		void Yaw( const float fDegree );
		//绕 Z 轴旋转（相对）
		void Roll( const float fDegree );

		//缩放节点（相对）
		void ScaleNode( const Vector3& vScl );
		void ScaleNode( const float x, const float y, const float z );
		//缩放节点
		void SetScale( const Vector3& vScl );
		void SetScale( const float x, const float y, const float z );

		//通知父节点需要更新矩阵
		void FKFastcall _NotifyParentUpdateMatrix();

		//更新节点矩阵
		void FKFastcall _UpdateMatrix();

		//更新碰撞体世界坐标包围盒
		void FKFastcall _UpdateBoundVolumeBox();

		//获取与指定包围盒相交的碰撞体
		void FKFastcall _GetIntersectBoundVolume( BoundVolumeList* pBoundVolumeList, const AxisBox3& refBoundBox );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
