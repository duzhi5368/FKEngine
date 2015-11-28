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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	TransformNode::TransformNode()
		: mpParentNode			(NULL)
		, mPosition				(Vector3::Zero)
		, mScale				(Vector3::Identity)
		, mOrientation			(Quaternion::Identity)
		, mDerivedPosition		(Vector3::Zero)
		, mDerivedScale			(Vector3::Identity)
		, mDerivedOrientation	(Quaternion::Identity)
		, mLocalMatrix			(Matrix4::Identity)
		, mWorldMatrix			(Matrix4::Identity)
		, mbUpdateLocalMatrix	(false)
		, mbUpdateWorldMatrix	(false)
		, mbUpdateChildMatrix	(false)
		, mbUpdateNodeBox		(false)
	{
		mNodeChildren.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	TransformNode::~TransformNode()
	{
		DeleteAllChild();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建子节点
	TransformNode* TransformNode::CreateChild()
	{
		TransformNode* pNewNode = _NodeCreator();

		pNewNode->SetParent( this );

		return pNewNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除子节点
	void TransformNode::DeleteChild( TransformNode* pNode )
	{
		if( pNode == NULL )
			return;

		TransformNodeList::Iterator it = mNodeChildren.Begin();
		TransformNodeList::Iterator end = mNodeChildren.End();
		for(; it!=end; ++it )
		{
			if( *it == pNode )
			{
				SafeDelete( pNode );
				mNodeChildren.Erase( it );
				return;
			}
		}

		//通知父节点需要更新包围盒
		_NotifyParentUpdateBox();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有子节点
	void TransformNode::DeleteAllChild()
	{
		TransformNodeList::Iterator it = mNodeChildren.Begin();
		TransformNodeList::Iterator end = mNodeChildren.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mNodeChildren.Clear();

		//通知父节点需要更新包围盒
		_NotifyParentUpdateBox();
	}
	//--------------------------------------------------------------------------------------------------------------
	//改变本节点到另一父节点下
	void TransformNode::SetParent( TransformNode* pNewParent )
	{
		if( pNewParent == NULL )
			return;

		//将该节点从它的父节点的子节点列表中删除
		if( mpParentNode != NULL )
		{
			TransformNodeList::Iterator it = mpParentNode->mNodeChildren.Begin();
			TransformNodeList::Iterator end = mpParentNode->mNodeChildren.End();
			for(; it!=end; ++it )
			{
				if( *it == this )
				{
					mpParentNode->mNodeChildren.Erase(it);
					break;
				}
			}

			//通知原父节点需要更新包围盒
			mpParentNode->_NotifyParentUpdateBox();
		}

		//通知新父节点需要更新包围盒
		pNewParent->_NotifyParentUpdateBox();

		//然后将其加入到新的父节点下
		*pNewParent->mNodeChildren.Push() = this;
		mpParentNode = pNewParent;

		//更新继承变换数据
		_UpdateDerivedTransform();
	}
	//--------------------------------------------------------------------------------------------------------------
	//位移节点（相对）
	void TransformNode::Translate( const Vector3& vTrans, TransformSpace eRelativeTo )
	{
		switch (eRelativeTo)
		{
		case TS_Local:
			{
				mPosition += mOrientation * vTrans;
				break;
			}
		case TS_World:
			{
				if( mpParentNode != NULL )
					mPosition += mpParentNode->mDerivedOrientation * vTrans;
				else
					mPosition += vTrans;
				break;
			}
		case TS_Parent:
			{
				mPosition += vTrans;
				break;
			}
		}

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::Translate( const float x, const float y, const float z, TransformSpace eRelativeTo )
	{
		Translate( Vector3( x, y, z ), eRelativeTo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//位移节点
	void TransformNode::SetPosition( const Vector3& vTrans )
	{
		mPosition = vTrans;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::SetPosition( const float x, const float y, const float z )
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//旋转节点（相对）
	void TransformNode::Rotate( const Quaternion& rQuat, TransformSpace eRelativeTo )
	{
		switch (eRelativeTo)
		{
		case TS_Parent:
			{
				mOrientation = rQuat * mOrientation;
				break;
			}
		case TS_World:
			{
				mOrientation = mOrientation * mDerivedOrientation.GetInverse() * rQuat * mDerivedOrientation;
				break;
			}
		case TS_Local:
			{
				mOrientation = mOrientation * rQuat;
				break;
			}
		}

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::Rotate( const Vector3& vAxis, const float fDegree, TransformSpace eRelativeTo )
	{
		Rotate( Quaternion( fDegree, vAxis ), eRelativeTo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//旋转节点
	void TransformNode::SetRotate( const Vector3& vAxis, const float fDegree )
	{
		mOrientation.SetRotationAxis( fDegree, vAxis );

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::SetRotate( const Quaternion& rQuat )
	{
		mOrientation = rQuat;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::SetRotate( const float fPitch, const float fYaw, const float fRoll )
	{
		mOrientation.SetRotationPitchYawRoll( fPitch, fYaw, fRoll );

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//绕 X 轴旋转（相对）
	void TransformNode::Pitch( const float fDegree, TransformSpace eRelativeTo )
	{
		Rotate( Quaternion( fDegree, Vector3::UnitX ), eRelativeTo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//绕 Y 轴旋转（相对）
	void TransformNode::Yaw( const float fDegree, TransformSpace eRelativeTo )
	{
		Rotate( Quaternion( fDegree, Vector3::UnitY ), eRelativeTo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//绕 Z 轴旋转（相对）
	void TransformNode::Roll( const float fDegree, TransformSpace eRelativeTo )
	{
		Rotate( Quaternion( fDegree, Vector3::UnitZ ), eRelativeTo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//缩放节点（相对）
	void TransformNode::Scale( const Vector3& vScl )
	{
		mScale *= vScl;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::Scale( const float x, const float y, const float z )
	{
		static Vector3 scalVec;

		scalVec.x = x;	scalVec.y = y;	scalVec.z = z;
		mScale *= scalVec;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//缩放节点
	void TransformNode::SetScale( const Vector3& vScl )
	{
		mScale = vScl;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	void TransformNode::SetScale( const float x, const float y, const float z )
	{
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;

		mbUpdateLocalMatrix = true;
		_NotifyParentUpdateMatrix();
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新继承变换数据
	void FKFastcall TransformNode::_UpdateDerivedTransform()
	{
		if( mpParentNode != NULL )
		{
			mDerivedOrientation = mpParentNode->mDerivedOrientation * mOrientation;
			mDerivedPosition = mpParentNode->mDerivedOrientation * ( mPosition * mpParentNode->mDerivedScale );
			mDerivedScale = mScale * mpParentNode->mDerivedScale;

			Mat4TransMat4( &mWorldMatrix, &mLocalMatrix, &mpParentNode->mWorldMatrix );
		}
		else
		{
			mDerivedOrientation = mOrientation;
			mDerivedPosition = mPosition;
			mDerivedScale = mScale;

			mWorldMatrix = mLocalMatrix;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//通知父节点需要更新矩阵
	void FKFastcall TransformNode::_NotifyParentUpdateMatrix()
	{
		//如果存在父节点则通知父节点它存在需要更新的子节点
		if( mpParentNode != NULL && !mpParentNode->mbUpdateChildMatrix )
		{
			mpParentNode->mbUpdateChildMatrix = true;
			mpParentNode->_NotifyParentUpdateMatrix();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//通知父节点需要更新包围盒
	void FKFastcall TransformNode::_NotifyParentUpdateBox()
	{
		mbUpdateNodeBox = true;

		//如果存在父节点则通知父节点它存在需要更新的子节点
		if( mpParentNode != NULL && !mpParentNode->mbUpdateNodeBox )
			mpParentNode->_NotifyParentUpdateBox();
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新节点矩阵
	void FKFastcall TransformNode::_UpdateMatrix()
	{
		//从根节点递归更新所有需要更新的节点
		TransformNode* pNode = NULL;

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
			TransformNodeList::Iterator it = mNodeChildren.Begin();
			TransformNodeList::Iterator itend = mNodeChildren.End();
			for(; it!=itend; ++it )
				(*it)->_UpdateMatrix();

			mbUpdateChildMatrix = false;
		}

		//如果该节点的世界坐标矩阵需要更新
		if( mbUpdateWorldMatrix )
		{
UpdateWorldMatrix:
			//更新该节点继承变换数据
			_UpdateDerivedTransform();

			//更新所有子节点的世界坐标矩阵（如需要则会同时更新其局部坐标矩阵）
			TransformNodeList::Iterator it = mNodeChildren.Begin();
			TransformNodeList::Iterator itend = mNodeChildren.End();
			for(; it!=itend; ++it )
			{
				pNode = *it;
				pNode->mbUpdateWorldMatrix = true;
				pNode->_UpdateMatrix();
			}

			mbUpdateWorldMatrix = false;
			mbUpdateChildMatrix = false;

			//通知父级更新节点包围盒
			_NotifyParentUpdateBox();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新节点世界坐标包围盒
	void FKFastcall TransformNode::_UpdateBox()
	{
		//递归调用所有需要更新包围盒的子节点（首先更新末端子节点的包围盒）
		TransformNode* pNode = NULL;

		//如果该节点需要更新包围盒
		if( mbUpdateNodeBox )
		{
			//更新所有子节点的包围盒
			TransformNodeList::Iterator itbeg = mNodeChildren.Begin();
			TransformNodeList::Iterator itend = mNodeChildren.End();
			for( TransformNodeList::Iterator it=itbeg; it!=itend; ++it )
				(*it)->_UpdateBox();

			//更新该节点的局部坐标包围盒

			//获取节点挂接物体包围盒
			mNodeBoundBox = _GetLocalObjectBox();

			//合并该节点下所有子节点的局部坐标包围盒
			for( TransformNodeList::Iterator it=itbeg; it!=itend; ++it )
				mNodeBoundBox.Combine( (*it)->mNodeBoundBox );

			if( mNodeBoundBox.mbEmpty )
				return;

			//获取节点世界坐标包围盒及转换顶点
			mNodeBoundBox.GetTransform( &mWorldBoundBox, mWorldMatrix, mBoundBoxVer );

			//将包围盒转换到该节点局部坐标
			mNodeBoundBox.Transform( mLocalMatrix );

			//通知包围盒已更新
			_NotifyBoxUpdated();

			mbUpdateNodeBox = false;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
