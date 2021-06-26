#include "epubdocument.h"

const QString EPubDocument::MIMETYPE_FILE = "mimetype";
const QByteArray EPubDocument::MIMETYPE = "application/epub+zip";
const QString EPubDocument::CONTAINER_FILE = "META-INF/container.xml";
// const QString EPubDocument::TOC_TITLE = "<h2>%1</h2>";
// const QString EPubDocument::LIST_START = "<html><body><ul>";
// const QString EPubDocument::LIST_END = "</ul></body></html>";
// const QString EPubDocument::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
// const QString EPubDocument::LIST_BUILD_ITEM = "<li><a
// href=\"%1#%2\">%3</li>"; const QString EPubDocument::LIST_FILEPOS =
// "position%1"; const QString EPubDocument::HTML_DOCTYPE =
//  "<!DOCTYPE html PUBLIC "
//  "\"-//W3C//DTD XHTML 1.1//EN\" "
//  "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
// const QString EPubDocument::XML_HEADER =
//  "<?xml version='1.0' encoding='utf-8'?>";
// const QString EPubDocument::HTML_XMLNS = "http://www.w3.org/1999/xhtml";

EPubDocument::EPubDocument(QObject* parent)
  : QObject(parent)
{
  m_container = Container(new EPubContainer(this));
}

bool
EPubDocument::loadDocument(const QString& filename)
{
  // epub file are basically a zipped set of files, primarily
  // html files.
  // open the zip file
  m_filename.clear();
  auto archive = new QuaZip(filename);
  if (!archive->open(QuaZip::mdUnzip)) {
    qDebug() << tr("Failed to open %1").arg(filename);
    return false;
  }

  // read the internal file list
  auto files = archive->getFileNameList();
  if (files.isEmpty()) {
    qDebug() << tr("Empty EPUB file").arg(filename);
    return false;
  }

  // Get and check that the mimetype is correct.
  // According to the standard this must be unencrypted.
  if (!parseMimetype(files, archive)) {
    return false;
  }

  if (!parseContainer(files, archive)) {
    return false;
  }

  emit updateMetadata();

  m_filename = filename;
  return true;
}

QString
EPubDocument::mimetype()
{
  return m_mimetype;
}

bool
EPubDocument::isMimetypeValid()
{
  return (m_mimetype == MIMETYPE);
}

QString
EPubDocument::filename() const
{
  return m_filename;
}

bool
EPubDocument::parseMimetype(QStringList files, QuaZip* archive)
{
  if (files.contains(MIMETYPE_FILE)) {
    archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      int error = archive->getZipError();
      qDebug() << tr("Unable to open mimetype file : error %1").arg(error);
      return false;
    }

    auto mimetype = mimetypeFile.readAll();
    if (mimetype != MIMETYPE) {
      qDebug() << tr("Unexpected mimetype %1").arg(QString(mimetype));
      m_mimetype = QString();
    } else {
      m_mimetype = QString(mimetype);
    }
  } else {
    qDebug() << tr("Unable to find mimetype in file");
    return false;
  }
  return true;
}

bool
EPubDocument::parseContainer(QStringList files, QuaZip* archive)
{
  if (files.contains(CONTAINER_FILE)) {
    archive->setCurrentFile(CONTAINER_FILE);
    QuaZipFile containerFile(archive);
    containerFile.setZip(archive);

    if (!containerFile.open(QIODevice::ReadOnly)) {
      int error = archive->getZipError();
      qDebug() << tr("Unable to open container file error %1").arg(error);
      return false;
    }

    QString container(containerFile.readAll());
    QDomDocument containerDocument;
    containerDocument.setContent(container);
    QDomElement rootElem = containerDocument.documentElement();
    QDomNamedNodeMap attMap = rootElem.attributes();
    auto containerVersion = rootElem.attribute("version");
    auto containerXmlns = rootElem.attribute("xmlns");
    m_container->setData(containerVersion, containerXmlns);

    QDomNodeList rootFiles = rootElem.elementsByTagName("rootfiles");
    if (rootFiles.size() > 0) {
      QDomElement rootfilesElem = rootFiles.at(0).toElement();
      QDomNodeList rootNodes = rootElem.elementsByTagName("rootfile");
      for (int i = 0; i < rootNodes.count(); i++) {
        QDomElement rootElement = rootNodes.at(i).toElement();
        auto containerFullpath = rootElement.attribute("full-path");
        auto containerMediatype = rootElement.attribute("media-type");
        if (containerFullpath.isEmpty()) {
          qWarning() << tr("Invalid root file entry");
          continue;
        }
        auto package =
          m_container->addPackage(containerFullpath, containerMediatype);
        package->parse(archive);
      }
    }

  } else {
    qDebug() << tr("Unable to find container information");
    return false;
  }

  //  // Limitations:
  //  //  - We only read one rootfile
  //  //  - We don't read the following from META-INF/
  //  //     - manifest.xml (unknown contents, just reserved)
  //  //     - metadata.xml (unused according to spec, just reserved)
  //  //     - rights.xml (reserved for DRM, not standardized)
  //  //     - signatures.xml (signatures for files, standardized)
  //  // Actually these are rarely included in an epub file anyway.

  //  qDebug() << tr("Unable to find and use any content files");
  return false;
}
