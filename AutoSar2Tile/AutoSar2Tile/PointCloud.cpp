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

//��ȡָ����
//������ڵ�ʱ�ſɵ���
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


//! ���ı��ļ��м��ص������꣬xyz��4�еĻ������ݵ�����������ɫ
bool PointCloud::ReadFromFile(QString fileName) {

	Sleep(100);
	//�� ���ļ�����xyzֵ�洢����
	//fileName = "D:/data/fsdsar/test/3mDEM1.txt";
	QFile file(fileName);

	//�ļ�������
	if (!file.exists())
	{
		Logger::Error(QObject::tr("File [%1] doesn\'t exist.").arg(fileName));
		return false;
	}

	int fileSize = file.size();
	//�ļ�Ϊ��
	if (fileSize == 0)
	{
		Logger::Message(QObject::tr("File [%1] is empty.").arg(fileName));
		return false;
	}

	//���ļ�;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Logger::Message(QStringLiteral("�ļ���ʧ��%1").arg(fileName));
		qDebug() << file.error();
		qDebug() << file.errorString();
		return false;
	}
		
	//�Ƿ�ʶ���ѧ������
	bool scientificNotation = true;

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString currentLine = in.readLine();

		//��ȡ�ļ���ÿ������;
		QStringList lineList = QString(currentLine).split(QRegExp("[, ;|]+"));

		if (lineList.size() < 3) {
			continue;
		}
		DCVector3D pt(lineList[0].toDouble(&scientificNotation), lineList[1].toDouble(&scientificNotation), lineList[2].toDouble(&scientificNotation));
		
		//�� ���=4����ݱ���������ɫ
		if (lineList.size() == 4) {
			double curScale = lineList[3].toDouble(&scientificNotation);
			if (!m_curScaleField)
			{
				m_curScaleField = new ScalField("sar");
			}

			m_curScaleField->AddData(curScale);
		}else if (lineList.size() >= 6) {
			//!ֱ��rgbcolor����
			RGBColor color = { lineList[3].toInt(&scientificNotation), lineList[4].toInt(&scientificNotation), lineList[5].toInt(&scientificNotation) };
			m_rgbColors.push_back(color);
		}

		//�����ݶ�ȡ����;
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
//! ��csv�ļ��м��ص�������EFG�������꣬J����λ�ƣ����ݵ�J��������ɫ
bool PointCloud::ReadFromCSVFile(QString fileName) {

	Sleep(100);

	//ָ���ļ�·��
	std::string aa = fileName.toStdString();
	const char* path = aa.c_str();
																	//��ʼ��CSVreader�����
	CSVreader reader(path);
	// ����3��
	int ignoreLine = 4;
	for (int i = 0; i < ignoreLine; ++i)
	{
		reader.readline();
	}

	//��������
	while (!reader.readline())
	{
		//�õ�ԭʼ���ݣ��洢��������
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
