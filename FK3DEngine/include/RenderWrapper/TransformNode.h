/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TransformNode
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	变换节点类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class TransformNode;
	typedef Stack< TransformNode* >		TransformNodeList;
	//--------------------------------------------------------------------------------------------------------------
	//变换节点类
	class FK_DLL TransformNode
	{
	public:
		//变换空间
		enum TransformSpace
		{
			TS_Local,		//相对于局部空间的变换
			TS_Parent,		//相对于父级空间的变换
			TS_World		//相对于世界空间的变换
		};

	public:
		Matrix4				mLocalMatrix;		//变换节点局部坐标矩阵
		Matrix4				mWorldMatrix;		//变换节点世界坐标矩阵

		TransformNode*		mpParentNode;		//父级变换节点

		Vector3				mPosition;			//位移矢量
		Quaternion			mOrientation;		//旋转四元数
		Vector3				mScale;				//缩放矢量

		Vector3				mDerivedPosition;	//继承位移矢量
		Quaternion			mDerivedOrientation;//继承旋转四元数
		Vector3				mDerivedScale;		//继承缩放矢量

	protected:
		TransformNodeList	mNodeChildren;		//子节点列表

		bool				mbUpdateLocalMatrix;//是否需要更新局部坐标矩阵
		bool				mbUpdateWorldMatrix;//是否需要更新世界坐标矩阵
		bool				mbUpdateChildMatrix;//它的某些子节点是否需要更新矩阵

		bool				mbUpdateNodeBox;	//是否需要更新节点包围盒

		AxisBox3			mNodeBoundBox;		//节点局部坐标包围盒
		AxisBox3			mWorldBoundBox;		//节点世界坐标包围盒
		Vector3				mBoundBoxVer[8];	//节点世界坐标包围盒顶点

	public:
		TransformNode();
		virtual ~TransformNode();

		//创建子节点
		TransformNode* CreateChild();

		//删除子节点
		void DeleteChild( TransformNode* pNode );

		//删除所有子节点
		void DeleteAllChild();

		//改变子节点到另一父节点下
		void SetParent( TransformNode* pNewParent );

		//位移节点（相对）
		void Translate( const Vector3& vTrans, TransformSpace eRelativeTo = TS_Parent );
		void Translate( const float x, const float y, const float z, TransformSpace eRelativeTo = TS_Parent );

		//位移节点
		void SetPosition( const Vector3& vTrans );
		void SetPosition( const float x, const float y, const float z );

		//旋转节点（相对）
		void Rotate( const Quaternion& rQuat, TransformSpace eRelativeTo = TS_Local );
		void Rotate( const Vector3& vAxis, const float fDegree, TransformSpace eRelativeTo = TS_Local );

		//旋转节点
		void SetRotate( const Vector3& vAxis, const float fDegree );
		void SetRotate( const Quaternion& rQuat );
		void SetRotate( const float fPitch, const float fYaw, const float fRoll );

		//绕 X 轴旋转（相对）
		void Pitch( const float fDegree, TransformSpace eRelativeTo = TS_Local );
		//绕 Y 轴旋转（相对）
		void Yaw( const float fDegree, TransformSpace eRelativeTo = TS_Local );
		//绕 Z 轴旋转（相对）
		void Roll( const float fDegree, TransformSpace eRelativeTo = TS_Local );

		//缩放节点（相对）
		void Scale( const Vector3& vScl );
		void Scale( const float x, const float y, const float z );

		//缩放节点
		void SetScale( const Vector3& vScl );
		void SetScale( const float x, const float y, const float z );

	protected:
		//节点创建函数
		virtual TransformNode* FKFastcall _NodeCreator() = 0;

		//获取节点挂接物体包围盒
		virtual const AxisBox3& FKFastcall _GetLocalObjectBox() = 0;

		//更新继承变换数据
		void FKFastcall _UpdateDerivedTransform();

		//通知父节点需要更新矩阵
		void FKFastcall _NotifyParentUpdateMatrix();

		//通知父节点需要更新包围盒
		void FKFastcall _NotifyParentUpdateBox();

	public:
		//更新节点矩阵
		void FKFastcall _UpdateMatrix();

		//更新节点世界坐标包围盒
		void FKFastcall _UpdateBox();

		//通知包围盒已更新
		virtual void FKFastcall _NotifyBoxUpdated() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
