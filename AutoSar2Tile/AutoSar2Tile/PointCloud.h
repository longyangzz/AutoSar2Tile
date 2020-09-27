#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QObject>

#include "GpBasicTypes.h"

#include "ScalField.h"

class PointCloud : public QObject
{
	Q_OBJECT

public:
	PointCloud(QObject *parent);
	~PointCloud();

	unsigned long Size();

	std::vector<RGBColor> GetColors() const;

	//获取指定点
	DCVector3D GetPoint(unsigned index);

	bool ReadFromFile(QString fileName);
private:
	//点数据
	std::vector<DCVector3D> m_points;

	//各顶点颜色
	std::vector<PointColor> m_rgbColors;

	//！ 颜色
	std::vector<PointColor> scalarColors;

	//！标量值容器
	ScalField* m_curScaleField;
};

#endif // POINTCLOUD_H
