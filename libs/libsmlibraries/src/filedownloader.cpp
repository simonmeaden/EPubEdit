#include "filedownloader.h"

//====================================================================
//=== FileDownloader
//====================================================================
FileDownloader::FileDownloader(QUrl url, QObject* parent)
  : QObject(parent)
{
  connect(&m_controller,
          &QNetworkAccessManager::finished,
          this,
          &FileDownloader::fileDownloaded);

  QNetworkRequest request(url);
  m_controller.get(request);
}

FileDownloader::~FileDownloader() {}

void
FileDownloader::fileDownloaded(QNetworkReply* reply)
{
  m_downloadedData = reply->readAll();
  reply->deleteLater();
  emit downloadComplete();
}

QByteArray
FileDownloader::downloadedData() const
{
  return m_downloadedData;
}
