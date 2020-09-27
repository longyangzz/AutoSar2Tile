#include "PointCloud.h"

#include "QFile"
#include "Logger.h"
#include "QTextStream"
#include "QDir"
#include "QDebug"
#include "QTime"
#include "QCoreApplication"
PointCloud::PointCloud(QObject *parent)
	: QObject(parent)
	, m_curScaleField(nullptr)
{

}

PointCloud::~PointCloud()
{

}

unsigned long PointCloud::Size()
{
	return m_points.size();
}

std::vector<RGBColor> PointCloud::GetColors() const {
	return m_rgbColors;
}

//获取指定点
//必须存在点时才可调用
DCVector3D PointCloud::GetPoint(unsigned index)
{
	if (index >= Size())
	{
		index = Size() - 1;
	}

	if (index < 0)
	{
		index = 0;
	}

	return m_points[index];
}

void Sleep(unsigned int msec)
{
	QTime reachTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < reachTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}

}

//! 从文本文件中加载点云坐标，xyz，4列的话，根据第四列生成颜色
bool PointCloud::ReadFromFile(QString fileName) {

	Sleep(100);
	//！ 打开文件读，xyz值存储起来
	//fileName = "D:/data/fsdsar/test/3mDEM1.txt";
	QFile file(fileName);

	//文件不存在
	if (!file.exists())
	{
		Logger::Error(QObject::tr("File [%1] doesn\'t exist.").arg(fileName));
		return false;
	}

	int fileSize = file.size();
	//文件为空
	if (fileSize == 0)
	{
		Logger::Message(QObject::tr("File [%1] is empty.").arg(fileName));
		return false;
	}

	//打开文件;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Logger::Message(QStringLiteral("文件打开失败%1").arg(fileName));
		qDebug() << file.error();
		qDebug() << file.errorString();
		return false;
	}
		
	//是否识别科学计数法
	bool scientificNotation = true;

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString currentLine = in.readLine();

		//读取文件的每行内容;
		QStringList lineList = QString(currentLine).split(QRegExp("[, ;|]+"));

		if (lineList.size() < 3) {
			continue;
		}
		DCVector3D pt(lineList[0].toDouble(&scientificNotation), lineList[1].toDouble(&scientificNotation), lineList[2].toDouble(&scientificNotation));
		
		//！ 如果=4则根据标量解析颜色
		if (lineList.size() == 4) {
			double curScale = lineList[3].toDouble(&scientificNotation);
			if (!m_curScaleField)
			{
				m_curScaleField = new ScalField("sar");
			}

			m_curScaleField->AddData(curScale);
		}

		if (lineList.size() >= 6) {
			//!直接rgbcolor读入
		}

		//将数据读取出来;
		m_points.push_back(pt);
	}

	file.close();

	if (m_curScaleField)
	{
		m_curScaleField->Prepare();
		m_rgbColors = m_curScaleField->GetColors();
	}

	return true;
}