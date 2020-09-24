#ifndef DCGP_BASIC_TYPES_H
#define DCGP_BASIC_TYPES_H

//C++��׼��
#include <array>

using std::array;

//Common
#include "BasicTypes.h"

//��������
typedef unsigned short NormalType;

//��ɫ����(R, G, B)
typedef unsigned char ColorType;

const ColorType MAX_COLOR_COMP = 255;

//! Scan index type
typedef unsigned char ScanIndexType;

//������
typedef DCVector3D PointCoord;			//3D
typedef array<PointCoordinateType, 2> PointCoord2D;			//2D


typedef array<ColorType, 3> PointColor;
//RGB��ɫ
typedef array<ColorType, 3> RGBColor;
//RGBA��ɫ
typedef array<ColorType, 4> RGBAColor;
//������ֵ
typedef DCVector3D Normal;

namespace DcGp
{
	enum ColorMode
	{
		//α��ɫ
		eFalseColor			=	0	,
		eFalseRangeColor			,
		eFalseHeightColor			,
		eFalseScalarColor			,

		//���ɫ
		eTrueColor					,
		eTrueLinearColor			,

		//����
		eOther						,
		eSingleColor				,

		//��Ԫɫ
		eUnit						,
		eUnitmaterial				,

		//����
		eAmplitude					,
		eAmplitudeLinearColor		,
		eTrueLogarithmicColor		,
		eDeviationColor
	};

	namespace Color
	{
		//Ԥ������ɫֵ
		static const ColorType WHITE[3]			=	{MAX_COLOR_COMP,MAX_COLOR_COMP,MAX_COLOR_COMP};
		static const ColorType LIGHTGREY[3]		=	{200,200,200};
		static const ColorType DARKGREY[3]		=	{MAX_COLOR_COMP/2,MAX_COLOR_COMP/2,MAX_COLOR_COMP/2};
		static const ColorType RED[3]			=	{MAX_COLOR_COMP,0,0};
		static const ColorType GREEN[3]			=	{0,MAX_COLOR_COMP,0};
		static const ColorType BLUE[3]			=	{0,0,MAX_COLOR_COMP};
		static const ColorType DARKBLUE[3]		=	{0,0,MAX_COLOR_COMP/2};
		static const ColorType MAGENTA[3]		=	{MAX_COLOR_COMP,0,MAX_COLOR_COMP};
		static const ColorType CYAN[3]		    =	{0,MAX_COLOR_COMP,MAX_COLOR_COMP};
		static const ColorType ORANGE[3]		=	{MAX_COLOR_COMP,MAX_COLOR_COMP/2,0};
		static const ColorType BLACK[3]			=	{0,0,0};
		static const ColorType YELLOW[3]		=	{MAX_COLOR_COMP,MAX_COLOR_COMP,0};

		//Ԥ�������ɫ
		static const float BRIGHT[4] =	{1.0, 1.0, 1.0, 1.0};
		static const float LIGHTER[4] = {0.83f,0.83f,0.83f,1.0f};
		static const float LIGHT[4] =	{0.66f,0.66f,0.66f,1.0f};
		static const float MIDDLE[4] =	{0.5f,0.5f,0.5f,1.0f};
		static const float DARK[4] = {0.34f,0.34f,0.34f,1.0f};
		static const float DARKER[4] =	{0.17f,0.17f,0.17f,1.0f};
		static const float DARKEST[4] = {0.08f,0.08f,0.08f,1.0f};
		static const float NIGHT[4] =	{0.0F,0.0F,0.0F,1.0F};
		static const float DEFAULTMESHFRONTDIFFUSE[4] = {0.0f,1.0f,0.32f,1.0f};
		static const float DEFAULTMESHBACKDIFFUSE[4] = {0.32f,1.0f,1.0f,1.0f};
		//static const float DEFAULTMESHBACKDIFFUSE[4] = {0.f,0.0f,0.0f,1.0f};

		//Ԥ���屳��ɫ
		static const ColorType DEFAULTCOLOR[3]			=   {MAX_COLOR_COMP,MAX_COLOR_COMP,MAX_COLOR_COMP};
		static const ColorType DEFAULTBACKGROUNDCOLOR[3]		=   {138,138,138};
		static const ColorType DEFAULTHISTBACKGROUNDCOLOR[3]	=   {51,0,51};
		static const ColorType DEFAULTLABELCOLOR[3]		=   {255,255,0};
	};

}
#endif
