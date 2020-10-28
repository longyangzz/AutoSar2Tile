#include "PointCloud.h"

#include "math.h"

#include "QFile"
#include "Logger.h"
#include "QTextStream"
#include "QDir"
#include "QDebug"
#include "QTime"
#include "QCoreApplication"
#include "XMLSettingValueManager.h"

#include "ReadCsvData.h"

PointCloud::PointCloud(QObject *parent)
	: QObject(parent)
	, m_curScaleField(nullptr)
{

}

PointCloud::~PointCloud()
{
	if (m_curScaleField)
	{
		delete m_curScaleField;
		m_curScaleField = nullptr;
	}
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
		}else if (lineList.size() >= 6) {
			//!直接rgbcolor读入
			RGBColor color = { lineList[3].toInt(&scientificNotation), lineList[4].toInt(&scientificNotation), lineList[5].toInt(&scientificNotation) };
			m_rgbColors.push_back(color);
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
//! 从csv文件中加载点云坐标EFG列是坐标，J列是位移，根据第J列生成颜色
bool PointCloud::ReadFromCSVFile(QString fileName) {

	Sleep(100);

	//指定文件路径
	std::string aa = fileName.toStdString();
	const char* path = aa.c_str();
																	//初始化CSVreader类对象
	CSVreader reader(path);
	// 跳过3行
	int ignoreLine = 4;
	for (int i = 0; i < ignoreLine; ++i)
	{
		reader.readline();
	}

	//解析数据
	while (!reader.readline())
	{
		//得到原始数据，存储给点云类
		if (isnan(reader.data[4]) || isnan(reader.data[5]) || isnan(reader.data[6]) || isnan(reader.data[9]))
		{
			continue;
		}
		
		DCVector3D pt(reader.data[4], reader.data[5], reader.data[6]);

		double curScale = reader.data[9];
		if (!m_curScaleField)
		{
			m_curScaleField = new ScalField("sar");
		}

		m_curScaleField->AddData(curScale);
		m_points.push_back(pt);
	}

	if (m_curScaleField)
	{
		m_curScaleField->Prepare();
		m_rgbColors = m_curScaleField->GetColors();
	}
	return true;
}
