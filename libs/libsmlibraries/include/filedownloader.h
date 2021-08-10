#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class FileDownloader : public QObject
{
  Q_OBJECT
public:
  explicit FileDownloader(QObject* parent = nullptr);
  virtual ~FileDownloader();

  void setDownloadUrl(const QString& urlStr);
  void setDownloadUrl(QUrl url);
  void download();

signals:
  void finished();
  void dataDownloaded(const QByteArray& data);
  void error(const QString& errorStr);

private:
  QNetworkAccessManager* m_controller;
  QByteArray m_downloadedData;
  QUrl m_url;

  void fileDownloaded(QNetworkReply* pReply);
};

#endif // FILEDOWNLOADER_H
