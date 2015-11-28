/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiBase
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/UISystem/UISystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	GuiClient::GuiClient()
		: mType		(NoClient)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiClient::GuiClient( const GuiClient& refClient )
	{
		//判断客户区类型
		switch (refClient.mType)
		{
		case Rect:
			{
				mType = Rect;
				memcpy( &mRect, &refClient.mRect, sizeof(GuiRect) );
				break;
			}
		case MaskMap:
			{
				mType = MaskMap;
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的用户界面对象客户区范围类型。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiClient::GuiClient( int nLeft, int nTop, int nRight, int nBottom )
		: mType			(Rect)
	{
		mRect.Left		= nLeft;
		mRect.Top		= nTop;
		mRect.Right		= nRight;
		mRect.Bottom	= nBottom;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置矩形客户区范围
	void FKFastcall GuiClient::SetClient( int nLeft, int nTop, int nRight, int nBottom )
	{
		mType = Rect;

		mRect.Left		= nLeft;
		mRect.Top		= nTop;
		mRect.Right		= nRight;
		mRect.Bottom	= nBottom;
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测指定坐标是否在客户区中
	bool FKFastcall GuiClient::IsOnClient( int x, int y )
	{
		//判断客户区类型
		switch (mType)
		{
		case Rect:
			{
				if( x > mRect.Left && x < mRect.Right &&
					y > mRect.Top && y < mRect.Bottom )
					return true;
				break;
			}
		case MaskMap:
			{
				if( x < 0 || x > mMaskMap.nWidth || y < 0 || y > mMaskMap.nHeight )
				{
					//计算该坐标对应的遮罩图位数
					UINT nBit = y *  mMaskMap.nWidth + x;
					UINT nByte = nBit >> 3;
					UINT nBitInByte = nBit % 8;

					//判断该字节的对应位是否为真
					if( ( mMaskMap.pBitMask[ nByte ] << nBitInByte ) >> (8-nBitInByte) == 1 )
						return true;
				}
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的用户界面对象客户区范围类型。" );
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiBase::GuiBase( GuiLoca* pLoca, GuiClient* pClient, GuiDialog* pDialog )
		: mbEnable		(true)
		, mbVisable		(true)
		, mbFocus		(false)
		, mpDialog		(pDialog)
	{
		//存储控件位置
		if( pLoca != NULL )
			memcpy( &mLoca, pLoca, sizeof(GuiLoca) );
		else
			memset( &mLoca, 0, sizeof(GuiLoca) );

		//存储客户区范围
		if( pClient != NULL )
			memcpy( &mClient, pClient, sizeof(GuiClient) );
		else
			memset( &mClient, 0, sizeof(GuiClient) );
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiBase::~GuiBase()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测指定坐标是否在客户区中
	bool GuiBase::IsOnClient( int x, int y )
	{
		//根据界面位置修正点击坐标
		int nRelX = x - mLoca.X;
		int nRelY = y - mLoca.Y;

		//检测指定坐标是否在控件用户区内
		return mClient.IsOnClient( nRelX, nRelY );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

