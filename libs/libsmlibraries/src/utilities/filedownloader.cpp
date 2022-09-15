#include "utilities/filedownloader.h"

//====================================================================
//=== FileDownloader
//====================================================================
FileDownloader::FileDownloader(QObject* parent)
  : QObject(parent)
{
  m_controller = new QNetworkAccessManager(this);
  connect(m_controller,
          &QNetworkAccessManager::finished,
          this,
          &FileDownloader::fileDownloaded);
}

FileDownloader::~FileDownloader() {}

void
FileDownloader::fileDownloaded(QNetworkReply* reply)
{
  qDebug() << "Reply received";
  if (reply->error()) {
    qDebug() << "ERROR!";
    qDebug() << reply->errorString();
  } else {
    emit dataDownloaded(reply->readAll());
    reply->deleteLater();
    emit finished();
  }
}

void
FileDownloader::setDownloadUrl(const QString& urlStr)
{
  m_url = QUrl(urlStr);
}

void
FileDownloader::setDownloadUrl(QUrl url)
{
  m_url = url;
}

void
FileDownloader::download()
{
  if (!m_url.isEmpty()) {
    QNetworkRequest request(m_url);
    m_controller->get(request);
  } else {
    emit error(tr("Error: Url has not been set!"));
  }
}
