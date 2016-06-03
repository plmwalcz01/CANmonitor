#include "settings.h"
#include <QDir>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringListModel>
#include <QMessageBox>
#include <QStandardPaths>
#include <utility>
#include <iostream>
#include <QDebug>
#include "utils.h"

namespace CANMonitor
{
    static auto RESOURCE_PREFIX = QStringLiteral(":/json");
    settings::settings(QObject *parent, QString filename) :
        QObject(parent),
        mFilename(filename),
        mModelMessages(*new QStringListModel(this))
    {
    }

    QString settings::getBaudRate()
    {
        return mBaudRate;
    }

    int settings::getSamplingPoint()
    {
        return mSamplingPoint;
    }

    QStringListModel& settings::getCommandsAsModel()
    {
        return mModelMessages;
    }

    void settings::ParseJsonData()
    {
        QString rawJson = ReadJsonFile();
        if(rawJson.size() == 0)
        {
            return;
        }
        auto jsonResult = GetJsonObject(rawJson);
        auto jsonErr = jsonResult.second;
        if(jsonErr.error != QJsonParseError::NoError)
        {
            ShowJsonParseError(jsonErr);
        }
        auto jsonObj = jsonResult.first;
        mBaudRate = jsonObj["baudRate"].toString();
        mSamplingPoint = jsonObj["samplingpoint"].toInt();
        mApplicationName = jsonObj["applicationTitle"].toString();
        mHighSpeed = jsonObj["highSpeed"].toInt();
        mMediumSpeed = jsonObj["mediumSpeed"].toInt();
        mLowSpeed = jsonObj["lowSpeed"].toInt();
        mBusClock = jsonObj["busClock"].toInt();
        SetupCommands(jsonObj);
    }

    QString settings::ReadJsonFile()
    {
        auto defaultSettings = ReadFromInternalResource();
        QDir configDir = OpenConfigurationDirectory();
        auto path = configDir.filePath(mFilename);
        QFile stdFile(path);
        if(stdFile.exists())
        {
            if(!stdFile.open(QFile::ReadOnly|QFile::Text))
            {
                SendErrorMessage("Could not open "+ path);
                return defaultSettings;
            }
            QString settingsStr = stdFile.readAll();
            stdFile.close();
            return settingsStr;
        }
        else
        {
            WriteDefaultsToStdConfigFile(stdFile, defaultSettings);
            return defaultSettings;
        }
    }
    void settings::WriteDefaultsToStdConfigFile(QFile& stdConfigFile,
                                                const QString& settingsStr)
    {
        int lenght  = settingsStr.length();
        if(!stdConfigFile.open(QFile::WriteOnly|QFile::Text))
        {
            SendErrorMessage("'Could not open file to write" + stdConfigFile.fileName());
        }
        auto bytesWritten = stdConfigFile.write(qPrintable(settingsStr), lenght);
        if(bytesWritten != lenght)
        {
            SendErrorMessage("'Could not write settings to file" + stdConfigFile.fileName());
            if(!stdConfigFile.remove())
            {
                SendErrorMessage("Couldn't remove configuration file. Please delete manualy"+
                                 stdConfigFile.fileName() );
            }
        }
        stdConfigFile.close();
    }
    QDir settings::OpenConfigurationDirectory()
    {
        QDir configDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
        if(configDir.exists())
        {
            QDir dir;
            dir.mkpath(configDir.path());
        }
        else
        {
            QDir dir("config");
            configDir = dir;
            dir.mkpath(configDir.path());
        }
        return configDir;
    }

    void settings::SetupCommands(QJsonObject jsonObj)
    {
        QJsonArray cmdArray = jsonObj["canMessages"].toArray();
        QStringList cmdList;
        for (auto item: cmdArray)
        {
            cmdList.append(item.toString());
        }
        mModelMessages.setStringList(cmdList);
    }
    void settings::ShowJsonParseError(QJsonParseError jsonErr)
    {
        QString msg = tr("Error parsing Json settings file. \n");
        msg.append(jsonErr.errorString());
        QMessageBox::critical(0,tr("CANMonitor"), msg);
    }
    JsonObjErrPair settings::GetJsonObject(const QString& rawJson)
    {
        QJsonParseError jsonParseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(rawJson.toUtf8(), &jsonParseError);
        QJsonObject jsonObj = jsonDoc.object();
        return std::make_pair(jsonObj, jsonParseError);
    }

    QString settings::ReadFromInternalResource()
    {
        QDir res_dir (RESOURCE_PREFIX);
        if (!res_dir.exists())
        {
            SendErrorMessage("Internal resource path missing" + res_dir.canonicalPath());
            return "";
        }
        auto path = res_dir.filePath(mFilename);
        QFile res_file(path);
        if (!res_file.open(QFile::ReadOnly| QFile::Text))
        {
            SendErrorMessage("Could not open internal resource " + path);
            return "";
        }
        QString setingsStr = res_file.readAll();
        return setingsStr;
    }

    void settings::SendErrorMessage(const QString& msg)
    {
        emit NotifyStatusMessage(msg);
    }
}
