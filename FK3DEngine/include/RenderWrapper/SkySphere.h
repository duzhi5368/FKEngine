/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkySphere
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	天空球
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//天空球
	class FK_DLL SkySphere : public SkyEntity
	{
	protected:
		UINT				mRing;				//环数
		UINT				mSegment;			//段数
		UINT				mNumVertex;			//顶点数量
		UINT				mNumIndex;			//索引数量
		UINT				mNumFace;			//三角面数量

		Matrix4				mSkyWorldMatrix;	//天空球渲染世界矩阵

		Texture*			mpTexture;			//天空球纹理

		VertexDeclaration*	mpVD;				//顶点声明
		VertexBuffer*		mpPositionVB;		//顶点坐标缓存
		VertexBuffer*		mpTexcoordVB;		//纹理坐标缓存
		IndexBuffer*		mpIB;				//索引缓存

		float				mRotaValue;			//滚动速度
		float				mRotaDegree;		//滚动值
		Vector3				mRotaAxis;			//滚动轴

	public:
		SkySphere( UINT nNumRing, UINT nNumSegment );
		virtual ~SkySphere();

		//设置天空球纹理
		void SetSphereTexture( Texture* pTexture );

		//设置表层旋转
		void SetSurfaceRotation( float fRotaVale, const Vector3& vAxis );

		//更新天空体
		void _UpdateSky( float fTimeSinceLastFrame );

		//渲染天空体
		void _RenderSky();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
