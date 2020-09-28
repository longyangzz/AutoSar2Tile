#pragma once

#include <QtCore/QCoreApplication>

#include "QFile"
#include "QFileInfo"
#include "QDomDocument"
#include "QDir"
#include "FileMoniter.h"
#include "TileFactory.h"
#include "Logger.h"
#include "QDebug"

QString getGlobleSettingFieldValue(QString xmlkey, QString attrskey, QString attrsValueDefault);
void Sleep(unsigned int msec);
bool isDirExist(QString fullPath);
void clearTempFiles(const QString& temp_path);
