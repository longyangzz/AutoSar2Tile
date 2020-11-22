#include "OGCImageTileBygdal2tilesDotpy.h"

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
#include "QTime"

#include "XMLSettingValueManager.h"

OGCImageTileBygdal2tilesDotpy::OGCImageTileBygdal2tilesDotpy(QString desFolder)
{
	m_desDirectory = desFolder;
}

OGCImageTileBygdal2tilesDotpy::~OGCImageTileBygdal2tilesDotpy()
{
}

char* findImageTypeGDAL( char *pDstImgFileName)
{
	char *dstExtension = strlwr(strrchr(pDstImgFileName, '.') + 1);
	char *Gtype = NULL;
	if (0 == strcmp(dstExtension, "bmp")) Gtype = "BMP";
	else if (0 == strcmp(dstExtension, "jpg")) Gtype = "JPEG";
	else if (0 == strcmp(dstExtension, "png")) Gtype = "PNG";
	else if (0 == strcmp(dstExtension, "tif")) Gtype = "GTiff";
	else if (0 == strcmp(dstExtension, "gif")) Gtype = "GIF";
	else Gtype = NULL;

	return Gtype;
}

bool ReadImageData(unsigned char **pImageData, int &nWidth, int &nHeight, int &nChannels, const QString& strFilePath)
{
	GDALAllRegister();
	GDALDataset *poDataset = NULL;

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	poDataset = (GDALDataset*)GDALOpen(strFilePath.toLocal8Bit(), GA_ReadOnly);
	if (poDataset == NULL)
	{
		//qDebug()<<QString::fromLocal8Bit("%1 GDAL��ʧ��").arg(strFilePath);
		GDALClose(poDataset);
		return false;
	}

	nWidth = poDataset->GetRasterXSize();
	nHeight = poDataset->GetRasterYSize();
	nChannels = poDataset->GetRasterCount();
	double adfGeoTransform[6] = { 0, //top left x
		1, //w-e pixel resolution (can be negative)
		0, //0
		0, //top left y
		0, //0
		1  //n-s pixel resolution (can be negative)
	};

	if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
	{
		Logger::Message(QString("TopLeft = (%1,%2)").arg(adfGeoTransform[0], 0, 'f', 6).arg(adfGeoTransform[3], 0, 'f', 6));
		Logger::Message(QString("Pixel Resolution = (%1,%2)").arg(adfGeoTransform[1], 0, 'f', 6).arg(adfGeoTransform[5], 0, 'f', 6));
	}

	unsigned char *pImageDataIn = new unsigned char[nChannels * nWidth * nHeight];
	*pImageData = pImageDataIn;
	//ZeroMemory(*pImageData, nChannels * nWidth * nHeight);

	for (int i = 1; i <= nChannels; ++i)
	{
		unsigned char *pImageOffset = *pImageData + i - 1;
		GDALRasterBand* pBand = poDataset->GetRasterBand(i);

		pBand->RasterIO(
			GF_Read,
			0, 0,
			nWidth, nHeight,
			pImageOffset,
			nWidth, nHeight,
			GDT_Byte,
			nChannels,
			0);

		//!�洢����pImageOffset
		std::vector< std::vector<unsigned char > > srcData;
		for (int row = 0; row < nHeight; ++row)
		{
			std::vector<unsigned char > temp;
			for (int col = 0; col < nWidth; ++col)
			{
				unsigned char value = pImageDataIn[ (row * nWidth  + col) * 3 + (i - 1)];

				temp.push_back(value);
			}
			srcData.push_back(temp);
		}

		//!����pImageOffset������y��˳��
		for (int row = 0; row < nHeight; ++row)
		{
			for (int col = 0; col < nWidth; ++col)
			{
				pImageDataIn[(row * nWidth + col) * 3 + (i - 1)] = srcData[nHeight - 1 - row][col];

			}
		}
	}

	GDALClose(poDataset);
	return true;
}
bool WriteImageData( char* strDestFilePath, unsigned char* pImageData, int nWidth, int nHeight, int nChannels, int nNewChannels)
{
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	GDALAllRegister();

	//QString strType;
	//GetDriverType(strDestFilePath, strType);

	char *GType = NULL;

	GType = findImageTypeGDAL(strDestFilePath);
	if (GType == NULL) { return false; }

	GDALDriver *pMemDriver = NULL;
	pMemDriver = GetGDALDriverManager()->GetDriverByName("MEM");
	if (pMemDriver == NULL) { return false; }

	GDALDataset * pMemDataSet = pMemDriver->Create("", nWidth, nHeight, nNewChannels, GDT_Byte, NULL);
	GDALRasterBand *pBand = NULL;

	for (int i = 1; i <= nNewChannels; i++)
	{
		if (i == nNewChannels && nNewChannels > nChannels)
		{
			unsigned char *pTempImageData = new unsigned char[nNewChannels*nWidth*nHeight];
			memset(pTempImageData, 0xFE, nNewChannels*nWidth*nHeight);

			pBand = pMemDataSet->GetRasterBand(i);
			pBand->RasterIO(GF_Write,
				0,
				0,
				nWidth,
				nHeight,
				pTempImageData,
				nWidth,
				nHeight,
				GDT_Byte,
				nNewChannels,
				0);
		}
		else
		{
			pBand = pMemDataSet->GetRasterBand(i);
			pBand->RasterIO(GF_Write,
				0,
				0,
				nWidth,
				nHeight,
				pImageData + i - 1,
				nWidth,
				nHeight,
				GDT_Byte,
				nChannels,
				0);
		}
	}

	GDALDriver *pDstDriver = NULL;
	pDstDriver = (GDALDriver *)GDALGetDriverByName(GType);
	if (pDstDriver == NULL)
	{
		GDALClose(pMemDataSet);

		return false;
	}

	GDALDataset * pDataSet = pDstDriver->CreateCopy(strDestFilePath, pMemDataSet, FALSE, NULL, NULL, NULL);
	if (pDataSet == NULL)
	{
		GDALClose(pMemDataSet);

		return false;
	}

	GDALClose(pDataSet);
	GDALClose(pMemDataSet);

	return true;
}

void OGCImageTileBygdal2tilesDotpy::UpdateTileChanged(const QString& fileNmaeNew)
{
	Sleep(1000 * 10);

	//! �����Ƿ�����pngת����������Ƭ����pngת��
	QString validPng = getGlobleSettingFieldValue("pngtransform", "valid", "false");
	QString imgname = getGlobleSettingFieldValue("pngtransform", "imgname", "guopupng");
	
	if (validPng == "true")
	{
		//!out Ŀ¼��д��ͬ��png�ļ�
		unsigned char *pImageData = NULL;
		int nWidth;
		int nHeight;
		int nChannels;
		//QString strFilePath = "C:\\Users\\Administrator\\Desktop\\Geotiff_Deformation_Image_20201020125810.tiff";
		QString fileNmaeNewtemp = "D:\\data\\fsdsar\\Geotiff_Deformation_Image_20201020125810.tiff";
		bool readState = ReadImageData(&pImageData, nWidth, nHeight, nChannels, fileNmaeNewtemp);
		//char* strDestFilePath = "C:\\Users\\Administrator\\Desktop\\png2.png";
		int nNewChannels = 3;

		QString outpath = m_desDirectory + "\\" + imgname + ".png";
		std::string aa = outpath.toStdString();
		char* strDestFilePath = (char* )aa.c_str();
		bool saveState = WriteImageData(strDestFilePath, pImageData, nWidth, nHeight, nChannels, nNewChannels);

		if (saveState)
		{
			Logger::Message(QStringLiteral("tiffתдΪpng����ͼƬ���óɹ�"));
			Logger::Message(QStringLiteral("tiffתдΪpng����ͼƬ���óɹ�,���Ŀ¼Ϊ%1").arg(outpath));
		}
	}

	QString validImgTile = getGlobleSettingFieldValue("image", "valid", "false");
	if (validImgTile == "true")
	{
		//! 1������gdal2tiles.py�������ļ�����
		QString baseDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts");
		QString proPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts\\gdal2tiles.py");
		QString runbatFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts\\run.bat");

		QString pythonexepath = getGlobleSettingFieldValue("image", "python", "");
		QString zoom = getGlobleSettingFieldValue("image", "zoom", "2-10");

		if (!QFile(pythonexepath).exists())
		{
			Logger::Error(QStringLiteral("%1��python������").arg(pythonexepath));
			return;
		}

		isDirExist(m_desDirectory);
		clearTempFiles(m_desDirectory);
		//! ��ǰ��������Ŀ¼��д����run.bat�ļ�����ִ��
		QFile file(runbatFile);
		if (!file.open(QFile::WriteOnly | QFile::Truncate))
		{
			Logger::Error(QStringLiteral("%1�ļ���ʧ��").arg(runbatFile));
			return;
		}
		QTextStream stream(&file);

		QString line = "cd " + baseDir;
		stream << line << "\r\n";

		line.clear();
		line.append(QString("%1 %2 %3 %4 --zoom=%5 --xyz").arg("start python").arg(proPath).arg(fileNmaeNew).arg(m_desDirectory).arg(zoom));
		stream << line << "\r\n";
		file.close();
		//3-���ո�ʹ�ô���ģʽ���ܹ�����
		QProcess* process = new QProcess(this);

		QStringList arg;
		arg.append(QString("%1").arg(proPath));
		arg.append(QString("%1").arg(fileNmaeNew));
		arg.append(QString("%1").arg(m_desDirectory));
		arg.append(QString("--zoom"));
		arg.append(QString("%1").arg(zoom));
		arg.append(QString("%1").arg("--xyz"));

		bool status = process->startDetached(pythonexepath, arg);
		if (status)
		{
			Logger::Message(QStringLiteral("Ӱ����Ƭ���óɹ�"));
			Logger::Message(QStringLiteral("���Ŀ¼Ϊ%1").arg(m_desDirectory));
		}
		else {
			Logger::Error(QStringLiteral("��ȷ���Ƿ�������python����"));
			Logger::Error(process->errorString());
		}
	}

	
}