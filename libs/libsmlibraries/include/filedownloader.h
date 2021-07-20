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
  explicit FileDownloader(QUrl url, QObject* parent = nullptr);
  virtual ~FileDownloader();
  QByteArray downloadedData() const;

signals:
  void downloadComplete();

private:
  QNetworkAccessManager m_controller;
  QByteArray m_downloadedData;

  void fileDownloaded(QNetworkReply* pReply);
};

#endif // FILEDOWNLOADER_H
