#ifndef DCCORE_BASICTYPES_H
#define DCCORE_BASICTYPES_H

//C++
#include <cmath>
#include <climits>
#include <vector>

//Self
#include "Vector.h"


namespace DCCore
{
	//Բ����PI
	static const double M_pi = 3.1415926;
	static const double M_pi_2 = 6.2831852;

	//��
	static const float Zero = 1.0e-7f;

	//��ά��
	typedef Vector2DF Point_2;
	//��ά����
	typedef Vector2DF Vector_2;
	//��ά���
	typedef std::vector<Point_2> PointSet_2;

	//��ά��
	typedef Vector3DF Point_3;
	//��ά����
	typedef Vector3DF Vector_3;
	//��ά���
	typedef std::vector<Point_3> PointSet_3;

	//��ά������
	typedef Vector_3 Normal_3;
	//��ά��������
	typedef std::vector<Normal_3> NormalSet_3;

	//��ά������
	struct Triangulation_2
	{
		Point_2 VertexA;
		Point_2 VertexB;
		Point_2 VertexC;
	};

	//��ά������
	struct Triangulation_3
	{
		Point_3 VertexA;
		Point_3 VertexB;
		Point_3 VertexC;
	};

	typedef double real;
}

//��������ݿ���---------------------------------------------------------
typedef DCCore::Vector3DF DCVector3D;
typedef Vector3D<double> Point_3D;

//Ĭ�϶�ά��������
typedef DCCore::Vector2DF DCVector2D;

//��������
typedef float PointCoordinateType;

//�޷����ַ���
typedef unsigned char uchar;

//��������
typedef float ScalarType;
//��������ݿ���---------------------------------------------------------

#endif