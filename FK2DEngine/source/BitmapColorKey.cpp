/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BitmapColorKey
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Bitmap.h"
#include <vector>

//--------------------------------------------------------------------
void FK2DEngine::ApplyColorKey( CBitmap& p_Bitmap, CColor p_Key )
{
	std::vector< CColor >	vecSurroundingColors;
	vecSurroundingColors.reserve( 4 );

	for( unsigned int unY = 0; unY < p_Bitmap.Height(); ++unY )
	{
		for( unsigned int unX = 0; unX < p_Bitmap.Width(); ++unX )
		{
			if( p_Bitmap.GetPixel( unX, unY ) == p_Key )
			{
				vecSurroundingColors.clear();

				if( unX > 0 && p_Bitmap.GetPixel( unX - 1, unY ) != p_Key )
				{
					vecSurroundingColors.push_back( p_Bitmap.GetPixel( unX - 1, unY ) );
				}
				if( unX < p_Bitmap.Width() - 1 && p_Bitmap.GetPixel( unX + 1, unY ) != p_Key )
				{
					vecSurroundingColors.push_back( p_Bitmap.GetPixel( unX + 1, unY ) );
				}
				if( unY > 0 && p_Bitmap.GetPixel( unX, unY - 1 ) != p_Key )
				{
					vecSurroundingColors.push_back( p_Bitmap.GetPixel( unX, unY - 1 ) );
				}
				if( unY < p_Bitmap.Height() - 1 && p_Bitmap.GetPixel( unX, unY + 1 ) != p_Key )
				{
					vecSurroundingColors.push_back( p_Bitmap.GetPixel( unX, unY + 1 ) );
				}


				if( vecSurroundingColors.empty() )
				{
					p_Bitmap.SetPixel( unX, unY, CColor::NONE );
					continue;
				}

				unsigned int unRed = 0;
				unsigned int unGreen = 0;
				unsigned int unBlue = 0;

				for( unsigned int i = 0; i < vecSurroundingColors.size(); ++i )
				{
					unRed		+= vecSurroundingColors[i].Red();
					unGreen		+= vecSurroundingColors[i].Green();
					unBlue		+= vecSurroundingColors[i].Blue();
				}

				p_Bitmap.SetPixel( unX, unY, CColor( 0, unRed / vecSurroundingColors.size(),
					unGreen / vecSurroundingColors.size(), unBlue / vecSurroundingColors.size() ) );
			}
		}
	}
}
//--------------------------------------------------------------------
void FK2DEngine::UnApplyColorKey( CBitmap& p_Bitmap, CColor p_BackGround )
{
	CColor* p = p_Bitmap.Data();
	for( unsigned int i = p_Bitmap.Width() * p_Bitmap.Height(); i > 0; --i, ++p )
	{
		if( p->Alpha() == 0 )
		{
			*p = p_BackGround;
		}
		else
		{
			p->SetAlpha( 255 );
		}
	}
}