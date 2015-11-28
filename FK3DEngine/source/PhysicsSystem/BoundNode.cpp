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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/PhysiceSystem/PhysicsSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	Quaternion BoundNode::mTmpQuat;	//计算临时四元数
	//--------------------------------------------------------------------------------------------------------------
	BoundNode::BoundNode()
		: mpParentNode			(NULL)
		, mLocalMatrix			(Matrix4::Identity)
		, mWorldMatrix			(Matrix4::Identity)
		, mInvWorldMatrix		(Matrix4::Identity)
		, mbUpdateLocalMatrix	(false)
		, mbUpdateWorldMatrix	(false)
		, mbUpdateChildMatrix	(false)
		, mPosition				(Vector3::Zero)
		, mScale				(Vector3::Identity)
		, mOrientation			(Quaternion::Identity)
	{
		mChildNodeList.Initialize( 20, 20 );
		mBoundVolumeList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	BoundNode::~BoundNode()
	{
		DeleteAllChild();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建子碰撞节点
	BoundNode* BoundNode::CreateChild()
	{
		BoundNode* pChild = new BoundNode;
		*mChildNodeList.Push() = pChild;
		pChild->mpParentNode = this;
		return pChild;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定子节点
	void BoundNode::DeleteChild( BoundNode* pChild )
	{
		if( pChild == NULL )
			return;

		BoundNodeList::Iterator it = mChildNodeList.Begin();
		BoundNodeList::Iterator end = mChildNodeList.End();
		for(; it!=end; ++it )
		{
			if( *it == pChild )
			{
				SafeDelete( pChild );
				mChildNodeList.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有子节点
	void BoundNode::DeleteAllChild()
	{
		BoundNodeList::Iterator it = mChildNodeList.Begin();
		BoundNodeList::Iterator end = mChildNodeList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mChildNodeList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//挂接碰撞体
	void BoundNode::AttachBoundVolume( BoundVolume* pBoundVolume )
	{
		*mBoundVolumeList.Push() = pBoundVolume;
		pBoundVolume->mpBoundNode = this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开碰撞体
	void BoundNode::DetachBoundVolume( BoundVolume* pBoundVolume )
	{
		BoundVolumeList::Iterator it = mBoundVolumeList.Begin();
		BoundVolumeList::Iterator end = mBoundVolumeList.End();
		for(; it!=end; ++it )
		{
			if( *it == pBoundVolume )
			{
				mBoundVolumeList.Erase( it );
				pBoundVolume->mpBoundNode = NULL;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//位移节点（相对）
	void BoundNode::TranslateNode( const Vector3& vTrans )
	{
		mPosition += vTrans;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::TranslateNode( const float x, const float y, const float z )
	{
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//位移节点
	void BoundNode::SetPosition( const Vector3& vTrans )
	{
		mPosition = vTrans;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::SetPosition( const float x, const float y, const float z )
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//旋转节点（相对）
	void BoundNode::RotateNode( const Vector3& vAxis, const float fDegree )
	{
		mTmpQuat.SetRotationAxis( fDegree, vAxis );
		mOrientation *= mTmpQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::RotateNode( const Quaternion& rQuat )
	{
		mOrientation *= rQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//旋转节点
	void BoundNode::SetRotate( const Vector3& vAxis, const float fDegree )
	{
		mOrientation.SetRotationAxis( fDegree, vAxis );

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::SetRotate( const Quaternion& rQuat )
	{
		mOrientation = rQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::SetRotate( const float fPitch, const float fYaw, const float fRoll )
	{
		mOrientation.SetRotationPitchYawRoll( fPitch, fYaw, fRoll );

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//绕 X 轴旋转（相对）
	void BoundNode::Pitch( const float fDegree )
	{
		mTmpQuat.SetRotationAxis( fDegree, Vector3::UnitX );
		mOrientation *= mTmpQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//绕 Y 轴旋转（相对）
	void BoundNode::Yaw( const float fDegree )
	{
		mTmpQuat.SetRotationAxis( fDegree, Vector3::UnitY );
		mOrientation *= mTmpQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//绕 Z 轴旋转（相对）
	void BoundNode::Roll( const float fDegree )
	{
		mTmpQuat.SetRotationAxis( fDegree, Vector3::UnitZ );
		mOrientation *= mTmpQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//缩放节点（相对）
	void BoundNode::ScaleNode( const Vector3& vScl )
	{
		mScale *= vScl;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::ScaleNode( const float x, const float y, const float z )
	{
		static Vector3 scalVec;

		scalVec.x = x;	scalVec.y = y;	scalVec.z = z;
		mScale *= scalVec;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//缩放节点
	void BoundNode::SetScale( const Vector3& vScl )
	{
		mScale = vScl;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void BoundNode::SetScale( const float x, const float y, const float z )
	{
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//通知父节点需要更新矩阵
	void FKFastcall BoundNode::_NotifyParentUpdateMatrix()
	{
		//如果存在父节点则通知父节点它存在需要更新的子节点
		if( mpParentNode != NULL && !mpParentNode->mbUpdateChildMatrix )
		{
			mpParentNode->mbUpdateChildMatrix = true;
			mpParentNode->_NotifyParentUpdateMatrix();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新节点矩阵
	void FKFastcall BoundNode::_UpdateMatrix()
	{
		//从根节点递归更新所有需要更新的节点
		BoundNode* pNode = NULL;

		//如果该节点局部坐标矩阵需要更新
		if( mbUpdateLocalMatrix )
		{
			//设置旋转矩阵
			static Matrix4 matRota;
			mOrientation.GetRotationMatrix4( &matRota );

			//设置缩放矩阵
			static Matrix4 matScl;
			matScl.SetScaling( mScale.x, mScale.y, mScale.z );

			//生成当前节点局部坐标矩阵
			Mat4TransMat4( &mLocalMatrix, &matScl, &matRota );

			//设置局部坐标矩阵平移
			mLocalMatrix._41 = mPosition.x;	mLocalMatrix._42 = mPosition.y;	mLocalMatrix._43 = mPosition.z;

			mbUpdateLocalMatrix = false;

			//更新该节点的世界矩阵
			goto UpdateWorldMatrix;
		}
		//或者该节点的子节点需要更新
		else if( mbUpdateChildMatrix )
		{
			//更新所有子节点的矩阵
			BoundNodeList::Iterator it = mChildNodeList.Begin();
			BoundNodeList::Iterator itend = mChildNodeList.End();
			for(; it!=itend; ++it )
				(*it)->_UpdateMatrix();

			mbUpdateChildMatrix = false;
		}

		//如果该节点的世界坐标矩阵需要更新
		if( mbUpdateWorldMatrix )
		{
UpdateWorldMatrix:
			//与父级矩阵级联生成该节点世界坐标矩阵
			if( mpParentNode != NULL )
				Mat4TransMat4( &mWorldMatrix, &mLocalMatrix, &mpParentNode->mWorldMatrix );
			else
				mWorldMatrix = mLocalMatrix;

			Mat4Inverse( &mInvWorldMatrix, &mWorldMatrix );

			//更新所有子节点的世界坐标矩阵（如需要则会同时更新其局部坐标矩阵）
			BoundNodeList::Iterator it = mChildNodeList.Begin();
			BoundNodeList::Iterator itend = mChildNodeList.End();
			for(; it!=itend; ++it )
			{
				pNode = *it;
				pNode->mbUpdateWorldMatrix = true;
				pNode->_UpdateMatrix();
			}

			mbUpdateWorldMatrix = false;

			//更新所有碰撞体世界坐标包围盒
			_UpdateBoundVolumeBox();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新碰撞体世界坐标包围盒
	void FKFastcall BoundNode::_UpdateBoundVolumeBox()
	{
		BoundVolumeList::Iterator it = mBoundVolumeList.Begin();
		BoundVolumeList::Iterator end = mBoundVolumeList.End();
		for(; it!=end; ++it )
		{
			BoundVolume* pBoundVolume = *it;
			pBoundVolume->mLocalBoundBox.GetTransform( &pBoundVolume->mWorldBoundBox, mWorldMatrix );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取与指定包围盒相交的碰撞体
	void FKFastcall BoundNode::_GetIntersectBoundVolume( BoundVolumeList* pBoundVolumeList, const AxisBox3& refBoundBox )
	{
		//遍历该节点下所有碰撞体
		BoundVolumeList::Iterator vit = mBoundVolumeList.Begin();
		BoundVolumeList::Iterator vend = mBoundVolumeList.End();
		for(; vit!=vend; ++vit )
		{
			BoundVolume* pBoundVolume = *vit;

			//如果指定包围盒与该包围盒相交
			if( pBoundVolume->mWorldBoundBox.IsIntersect( refBoundBox ) )
				*pBoundVolumeList->Push() = pBoundVolume;
		}

		//递归获取子节点的相交碰撞体
		BoundNodeList::Iterator nit = mChildNodeList.Begin();
		BoundNodeList::Iterator nend = mChildNodeList.End();
		for(; nit!=nend; ++nit )
			(*nit)->_GetIntersectBoundVolume( pBoundVolumeList, refBoundBox );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
