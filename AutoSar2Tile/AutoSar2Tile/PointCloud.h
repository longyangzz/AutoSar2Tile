#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QObject>

#include "GpBasicTypes.h"

class PointCloud : public QObject
{
	Q_OBJECT

public:
	PointCloud(QObject *parent);
	~PointCloud();

	unsigned long Size();

	std::vector<RGBColor> GetColors() const;

	//��ȡָ����
	DCVector3D GetPoint(unsigned index);

	bool ReadFromFile(QString fileName);
private:
	//������
	std::vector<DCVector3D> m_points;

	//��������ɫ
	std::vector<PointColor> m_rgbColors;

	//�� ��ɫ
	std::vector<PointColor> scalarColors;
};

#endif // POINTCLOUD_H
