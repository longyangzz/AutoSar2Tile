#include "CesiumTileByEntwine.h"

#include "ogr_spatialref.h"
#include "gdal.h"
#include <gdal_priv.h>
#include <cpl_conv.h>

#include "QFileInfo"
#include "QDirIterator"
#include "PointCloud.h"
#include "QCoreApplication"
#include "Logger.h"

#include "QProcess"
#include "QTextStream"

#include "XMLSettingValueManager.h"

CesiumTileByEntwine::CesiumTileByEntwine(QString desFolder)
{
	isDataTransformSucess = false;
	m_desDirectory = desFolder;
}

CesiumTileByEntwine::~CesiumTileByEntwine()
{

}

//! ��������������װ��������򣬽��м������
void CesiumTileByEntwine::UpdateTileChanged(const QString& fileNmaeNew) 
{
	//! �жϴ�����ļ���չ�� text�Ļ���ʹ��ָ���ֶ�תΪ��ɫֵ��תΪlas temp�ļ�������������Ƶ��ļ����ƣ����ݵ�bat��ִ��
	QString fileName = fileNmaeNew;
	QString extension = QFileInfo(fileName).suffix();

	//txtתlas
	if (extension == "txt")
	{
		//! ִ��ת��Ϊlas��д�������ݸ�
		m_inputFilenameForTile = TransformTxt2las(fileName);
	}
	else {
		m_inputFilenameForTile = fileName;
	}

	if (!QFile(m_inputFilenameForTile).exists()){
		Logger::Error(QStringLiteral("��������Ч��las�ļ�%1").arg(m_inputFilenameForTile));
		return;
	}

	//!las����entwine��Ƭ
	QString baseDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210");
	QString proPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210\\entwine.exe");
	QString runbatFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210\\run.bat");
	isDirExist(m_desDirectory);
	clearTempFiles(m_desDirectory);
	if (!isDataTransformSucess)
	{
		Logger::Error(QStringLiteral("��Ƭʧ�ܣ���������ת�����ɹ�"));
		return;
	}
	//! ��ǰ��������Ŀ¼��д����run.bat�ļ�����ִ�С�
	//! process->startDetached����bat���ᵯ���ڿ�
	//@ system����Ե����ڿ����bat
	QFile file(runbatFile);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		return;
	}

	//�� ����PROJ_LIB������������
	QString gdalProjlibdir = QCoreApplication::applicationDirPath() + "\\Third\\Entwine210\\share\\proj";
	bool setProjlibdir = qputenv(qPrintable("PROJ_LIB"), qPrintable(gdalProjlibdir));


	QTextStream stream(&file);
	QString line = "cd " + baseDir;
	stream << line << "\r\n";

	line.clear();
	line.append(QString("%1 %2 -i %3 -o %4 -f -r EPSG:4978").arg(proPath).arg("build").arg(m_inputFilenameForTile).arg(m_desDirectory));
	stream << line << "\r\n";

	//! �Ƿ�ִ��ת��Ϊcesium��ʽ
	if (true)
	{
		line.clear();
		line.append(QString("%1 %2 -i %3 -o %4").arg(proPath).arg("convert").arg(m_desDirectory).arg(m_desDirectory + "\\cesium"));
		stream << line << "\r\n";
	}

	file.close();
	QProcess* process = new QProcess(nullptr);
	process->setWorkingDirectory(baseDir);

	QStringList arg;
	arg.append(runbatFile);
	bool status = process->startDetached(qPrintable(runbatFile));

	if (status)
	{
		Logger::Message(QStringLiteral("������Ƭ���óɹ�"));
	}
	else {
		Logger::Message(process->errorString());
	}
	
	int a = 6;
}


void clearFiles(const QString &folderFullPath)
{
	QDir dir(folderFullPath);
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	for (int i = 0; i < fileCount; i++)
		dir.remove(dir[i]);
}


QString CesiumTileByEntwine::TransformTxt2las(QString fileName)
{
	QString outLasFilename;
	
	//! ʹ��io�����ȡtxt
	bool isGenrgb = true;
	QString wktSrccoord = "EPSG:4542";
	QString wktDescoord = "EPSG:32647";
	QString extension = QFileInfo(fileName).suffix();

	//! ����ָ���������ɱ���ɫ
	PointCloud* cloud = new PointCloud(nullptr);
	bool isReadSuc = false;
	if (extension == "txt") {
		isReadSuc = cloud->ReadFromFile(fileName);
	}
	
	int ptNum = cloud->Size();
	if (ptNum == 0) {
		Logger::Message(QStringLiteral("���ļ� [%1] ��ȡ����xyz�������Ϊ0���޷����к���ת��!").arg(fileName));
		return outLasFilename;
	}

	//��ʾ��Ϣ
	if (isReadSuc)
	{
		isDataTransformSucess = true;
		Logger::Message(QStringLiteral("���ļ� [%1] ��ȡ����xyz����ɹ�").arg(fileName));
	}
	else
	{
		Logger::Message(QStringLiteral("���ļ� [%1] ��ȡ����xyz����ʧ��!").arg(fileName));
		return outLasFilename;
	}

	//! д��λ��ʱlas�ļ�
	//�����tempĿ¼
	isDirExist(QFileInfo(fileName).absolutePath() + "//" + "temp//");
	clearTempFiles(QFileInfo(fileName).absolutePath() + "//" + "temp//");
	QString  outtxtFilename = QFileInfo(fileName).absolutePath() + "//" + "temp//" + QFileInfo(fileName).baseName() + "-temp.txt";
	outLasFilename = QFileInfo(fileName).absolutePath() + "//" + "temp//" + QFileInfo(fileName).baseName() + "-temp.las";
	bool outstate = SaveToFile(cloud, outtxtFilename);
	//��ʾ��Ϣ
	if (outstate && QFile(outtxtFilename).exists())
	{
		isDataTransformSucess = true;
		Logger::Message(QStringLiteral("�����ļ� [%1] �ɹ�").arg(outtxtFilename));
	}
	else
	{
		Logger::Message(QStringLiteral("�����ļ� [%1] ʧ��!").arg(outtxtFilename));
		return outLasFilename;
	}

	//! ���������txtrgbת��Ϊlas  txt2las -i 3mDEM-temp.txt -o 3mDEM-temp.las -parse xyzRGB
	//! 1������txt2las.exe�������ļ�����
	QString baseDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210");
	QString proPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210\\txt2las.exe");
	QString prolas2lasPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210\\las2las.exe");

	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(baseDir);

	QStringList arg;
	arg.append(QString("-i"));
	arg.append(QString("%1").arg(outtxtFilename));
	arg.append(QString("-o"));
	arg.append(QString("%1").arg(outLasFilename));
	arg.append(QString("-parse"));
	arg.append(QString("xyzRGB"));
	process->start(proPath, arg);
	if (process->waitForFinished())
	{
		Logger::Message(QStringLiteral("txt2lasִ�гɹ�"));

		//las2las��������ϵsrc��Ϣlas2las -i 3mDEM-temp.las -o out-3mDEM-temp.las -utm 47T  -meter -elevation_meter
		QString outLaswithSrcFilename = QFileInfo(fileName).absolutePath() + "//temp//" + QFileInfo(fileName).baseName() + "-srs" + "-temp.las";
		arg.clear();
		arg.append(QString("-i"));
		arg.append(QString("%1").arg(outLasFilename));
		arg.append(QString("-o"));
		arg.append(QString("%1").arg(outLaswithSrcFilename));
		arg.append(QString("-epsg"));
		arg.append(QString::number(QString(wktDescoord).remove("EPSG:").toInt()));
		//arg.append(QString("-utm"));
		//arg.append(QString("47T"));
		arg.append(QString("-meter"));
		arg.append(QString("-elevation_meter"));
		process->start(prolas2lasPath, arg);
		if (process->waitForFinished()) {
			Logger::Message(QStringLiteral("las2lasִ�гɹ�"));
		}
		else {
			Logger::Message(process->errorString());
		}
		return outLaswithSrcFilename;
	}
	else {
		Logger::Message(process->errorString());
		return outLasFilename;
	}

	return outLasFilename;
}

bool CesiumTileByEntwine::SaveToFile(PointCloud* entity, QString filename)
{
	QString wktSrccoord = "EPSG:4542";
	QString wktDescoord = "EPSG:32647";

	//!ʹ��liblas�� srs����㣬ͶӰΪout����
	QString gdalDatadir = QCoreApplication::applicationDirPath() + "\\gdal-data";
	CPLSetConfigOption("GDAL_DATA", qPrintable(gdalDatadir));

	OGRSpatialReference oSRS;
	int srcepsg = QString(wktSrccoord).remove("EPSG:").toInt();
	oSRS.importFromEPSG(srcepsg);




	OGRSpatialReference ooutSRS;
	int outepsg = QString(wktDescoord).remove("EPSG:").toInt();
	ooutSRS.importFromEPSG(outepsg);
	OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(&oSRS, &ooutSRS);

	if (!poCT)
	{
		Logger::Message(QStringLiteral("gdal����ת���� [%1] ����ʧ��!").arg("OGRCoordinateTransformation"));
		return false;
	}

	//д������
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		return false;
	}
	QTextStream stream(&file);
	//���ø�ʽ����
	stream.setRealNumberNotation(QTextStream::FixedNotation);
	stream.setRealNumberPrecision(3);

	PointCloud* pointCloud = entity;
	if (pointCloud == nullptr)
	{
		return false;
	}


	//��ȡ��������ɫ����������Ϣ
	unsigned long number = pointCloud->Size();
	bool writeField = true;

	std::vector<PointColor> scalarColors = pointCloud->GetColors();

	//д������
	for (unsigned long i = 0; i != number; ++i)
	{
		//����һ����ǰ�б���
		QString line;

		//д�뵱ǰ����
		Point_3D point = Point_3D::FromArray(pointCloud->GetPoint(i).u);


		/*libpt.SetCoordinates(point.x - shift.x, point.y - shift.y, point.z - shift.z);
		pro->transform(libpt);*/
		double temX = point.x;
		double temY = point.y;
		double temZ = point.z;
		poCT->Transform(1, &temX, &temY, &temZ);

		line.append(QString("%1").arg(temX, 0, 'f', 3));
		line.append(" "); //��ӷָ���
		line.append(QString("%1").arg(temY, 0, 'f', 3));
		line.append(" "); //��ӷָ���
		line.append(QString("%1").arg(temZ, 0, 'f', 3));
		line.append(" "); //��ӷָ���

		//����һ����ǰ�б����洢��ɫֵ
		QString color;
		if (writeField)
		{
			//���rgb��ɫ
			PointColor rgbColor{255, 0, 0};
			if (scalarColors.size() > 0 && i < scalarColors.size()) {
				rgbColor = scalarColors[i];
			}
			color.append(" ");
			color.append(QString::number(rgbColor[0]));
			color.append(" ");
			color.append(QString::number(rgbColor[1]));
			color.append(" ");
			color.append(QString::number(rgbColor[2]));

			line.append(color);
		}

		stream << line << "\n";
	}

	file.close();
	return true;
}
