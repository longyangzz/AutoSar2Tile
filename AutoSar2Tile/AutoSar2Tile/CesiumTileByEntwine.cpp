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

//! 根据输入数据组装，计算程序，进行计算输出
void CesiumTileByEntwine::UpdateTileChanged(const QString& fileNmaeNew) 
{
	//! 判断传入的文件扩展名 text的话，使用指定字段转为颜色值，转为las temp文件，更新输入点云的文件名称，传递到bat中执行
	QString fileName = fileNmaeNew;
	QString extension = QFileInfo(fileName).suffix();

	//txt转las
	if (extension == "txt")
	{
		//! 执行转换为las，写出并传递给
		m_inputFilenameForTile = TransformTxt2las(fileName);
	}
	else {
		m_inputFilenameForTile = fileName;
	}

	if (!QFile(m_inputFilenameForTile).exists()){
		Logger::Error(QStringLiteral("不存在有效的las文件%1").arg(m_inputFilenameForTile));
		return;
	}

	//!las调用entwine切片
	QString baseDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210");
	QString proPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210\\entwine.exe");
	QString runbatFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\Third\\Entwine210\\run.bat");
	isDirExist(m_desDirectory);
	clearTempFiles(m_desDirectory);
	if (!isDataTransformSucess)
	{
		Logger::Error(QStringLiteral("切片失败，输入数据转换不成功"));
		return;
	}
	//! 当前第三方库目录下写出个run.bat文件，并执行。
	//! process->startDetached调用bat不会弹出黑框
	//@ system则可以弹出黑框调用bat
	QFile file(runbatFile);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		return;
	}

	//！ 设置PROJ_LIB到环境变量中
	QString gdalProjlibdir = QCoreApplication::applicationDirPath() + "\\Third\\Entwine210\\share\\proj";
	bool setProjlibdir = qputenv(qPrintable("PROJ_LIB"), qPrintable(gdalProjlibdir));


	QTextStream stream(&file);
	QString line = "cd " + baseDir;
	stream << line << "\r\n";

	line.clear();
	line.append(QString("%1 %2 -i %3 -o %4 -f -r EPSG:4978").arg(proPath).arg("build").arg(m_inputFilenameForTile).arg(m_desDirectory));
	stream << line << "\r\n";

	//! 是否执行转换为cesium格式
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
		Logger::Message(QStringLiteral("点云切片调用成功"));
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
	
	//! 使用io插件读取txt
	bool isGenrgb = true;
	QString wktSrccoord = "EPSG:4542";
	QString wktDescoord = "EPSG:32647";
	QString extension = QFileInfo(fileName).suffix();

	//! 根据指定的列生成标量色
	PointCloud* cloud = new PointCloud(nullptr);
	bool isReadSuc = false;
	if (extension == "txt") {
		isReadSuc = cloud->ReadFromFile(fileName);
	}
	
	int ptNum = cloud->Size();
	if (ptNum == 0) {
		Logger::Message(QStringLiteral("从文件 [%1] 读取点云xyz坐标个数为0，无法进行后续转换!").arg(fileName));
		return outLasFilename;
	}

	//提示信息
	if (isReadSuc)
	{
		isDataTransformSucess = true;
		Logger::Message(QStringLiteral("从文件 [%1] 读取点云xyz坐标成功").arg(fileName));
	}
	else
	{
		Logger::Message(QStringLiteral("从文件 [%1] 读取点云xyz坐标失败!").arg(fileName));
		return outLasFilename;
	}

	//! 写出位临时las文件
	//！清空temp目录
	isDirExist(QFileInfo(fileName).absolutePath() + "//" + "temp//");
	clearTempFiles(QFileInfo(fileName).absolutePath() + "//" + "temp//");
	QString  outtxtFilename = QFileInfo(fileName).absolutePath() + "//" + "temp//" + QFileInfo(fileName).baseName() + "-temp.txt";
	outLasFilename = QFileInfo(fileName).absolutePath() + "//" + "temp//" + QFileInfo(fileName).baseName() + "-temp.las";
	bool outstate = SaveToFile(cloud, outtxtFilename);
	//提示信息
	if (outstate && QFile(outtxtFilename).exists())
	{
		isDataTransformSucess = true;
		Logger::Message(QStringLiteral("导出文件 [%1] 成功").arg(outtxtFilename));
	}
	else
	{
		Logger::Message(QStringLiteral("导出文件 [%1] 失败!").arg(outtxtFilename));
		return outLasFilename;
	}

	//! 保存出来的txtrgb转换为las  txt2las -i 3mDEM-temp.txt -o 3mDEM-temp.las -parse xyzRGB
	//! 1、启动txt2las.exe，传入文件参数
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
		Logger::Message(QStringLiteral("txt2las执行成功"));

		//las2las赋予坐标系src信息las2las -i 3mDEM-temp.las -o out-3mDEM-temp.las -utm 47T  -meter -elevation_meter
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
			Logger::Message(QStringLiteral("las2las执行成功"));
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

	//!使用liblas对 srs坐标点，投影为out类型
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
		Logger::Message(QStringLiteral("gdal坐标转换类 [%1] 创建失败!").arg("OGRCoordinateTransformation"));
		return false;
	}

	//写出数据
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		return false;
	}
	QTextStream stream(&file);
	//设置格式控制
	stream.setRealNumberNotation(QTextStream::FixedNotation);
	stream.setRealNumberPrecision(3);

	PointCloud* pointCloud = entity;
	if (pointCloud == nullptr)
	{
		return false;
	}


	//获取个数及颜色法向量等信息
	unsigned long number = pointCloud->Size();
	bool writeField = true;

	std::vector<PointColor> scalarColors = pointCloud->GetColors();

	//写出数据
	for (unsigned long i = 0; i != number; ++i)
	{
		//定义一个当前行变量
		QString line;

		//写入当前坐标
		Point_3D point = Point_3D::FromArray(pointCloud->GetPoint(i).u);


		/*libpt.SetCoordinates(point.x - shift.x, point.y - shift.y, point.z - shift.z);
		pro->transform(libpt);*/
		double temX = point.x;
		double temY = point.y;
		double temZ = point.z;
		poCT->Transform(1, &temX, &temY, &temZ);

		line.append(QString("%1").arg(temX, 0, 'f', 3));
		line.append(" "); //添加分隔符
		line.append(QString("%1").arg(temY, 0, 'f', 3));
		line.append(" "); //添加分隔符
		line.append(QString("%1").arg(temZ, 0, 'f', 3));
		line.append(" "); //添加分隔符

		//定义一个当前行变量存储颜色值
		QString color;
		if (writeField)
		{
			//添加rgb颜色
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
