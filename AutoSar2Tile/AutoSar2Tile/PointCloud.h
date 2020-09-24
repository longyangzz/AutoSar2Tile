#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QObject>

class PointCloud : public QObject
{
	Q_OBJECT

public:
	PointCloud(QObject *parent);
	~PointCloud();

private:
	
};

#endif // POINTCLOUD_H
