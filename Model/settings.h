#pragma once
#include <QObject>

class QString;
class QStringListModel;
class QJsonParseError;
class QJsonObject;
class QFile;
class QDir;

namespace CANMonitor{
    typedef std::pair<QJsonObject, QJsonParseError> JsonObjErrPair;
    class settings : public QObject
    {
        Q_OBJECT
    public:
        explicit settings(QObject *parent, QString filename );
        QString getBaudRate();
        int getSamplingPoint();
        QStringListModel &getCommandsAsModel();
        void ParseJsonData();
    signals:
        void NotifyStatusMessage(QString message);
    private:
        QString mFilename;
        QString mApplicationName;
        QString mBaudRate;
        int mSamplingPoint;
        int mHighSpeed;
        int mMediumSpeed;
        int mLowSpeed;
        int mBusClock;
        QStringListModel& mModelMessages;

        QString ReadJsonFile();
        QString ReadFromInternalResource();

        explicit settings(const settings& rhs) = delete;
        settings& operator= (const settings& rhs) = delete;

        void SendErrorMessage(const QString& msg);
        void ShowJsonParseError(QJsonParseError jsonErr);
        JsonObjErrPair GetJsonObject(const QString &rawJson);
        void SetupCommands(QJsonObject jsonObj);
        QDir OpenConfigurationDirectory();
        void WriteDefaultsToStdConfigFile(QFile &stdConfigFile, const QString &settingsStr);
    };
}



