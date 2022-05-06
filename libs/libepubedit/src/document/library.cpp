#include "document/bookpointers.h"

//== BookData
//=====================================================================================
BookData::BookData()
{
  d = new SharedBookData;
}

BookData::BookData(quint64 uid,
                   const QString& filename,
                   const QString& title,
                   QStringList authors)
{
  d = new SharedBookData;
  setUId(uid);
  setFilename(filename);
  setTitle(title);
  setCreators(authors);
  setModified(true);
}

BookData::BookData(quint64 uid,
                   const QString& filename,
                   const QString& title,
                   QStringList authors,
                   const QString& series,
                   quint16 index)
{
  d = new SharedBookData;
  setUId(uid);
  setFilename(filename);
  setTitle(title);
  setCreators(authors);
  setSeries(series);
  setSeriesIndex(index);
  setModified(true);
}

BookData::BookData(const BookData& other)
  : d(other.d)
{}

quint64
BookData::uid()
{
  return d->m_uid;
}

void
BookData::setUId(quint64 uid)
{
  d->m_uid = uid;
  setModified(true);
}

QString
BookData::filename()
{
  return d->m_filename;
}

void
BookData::setFilename(const QString& filename)
{
  d->m_filename = filename;
  setModified(true);
}

QString
BookData::title()
{
  return d->m_title;
}

void
BookData::setTitle(const QString& title)
{
  d->m_title = title;
  setModified(true);
}

QStringList
BookData::creators()
{
  return d->m_creators;
}

void
BookData::setCreators(QStringList creators)
{
  d->m_creators = creators;
}

QString
BookData::series()
{
  return d->m_series;
}

void
BookData::setSeries(const QString& series)
{
  d->m_series = series;
  setModified(true);
}

quint16
BookData::seriesIndex()
{
  return d->m_seriesIndex;
}

//void
//BookData::setSeriesIndex(QString index)
//{
//  d->m_seriesIndex = index;
//  setModified(true);
//}

void
BookData::setSeriesIndex(quint16 index)
{

  d->m_seriesIndex = index;
  setModified(true);
}

QString
BookData::currentSpineId() const
{
  return d->m_currentSpineId;
}

void
BookData::setCurrentSpineId(QString id)
{
  d->m_currentSpineId = id;
  setModified(true);
}

quint16
BookData::currentSpineLineNo()
{
  return d->m_currentSpineLineNo;
}

void
BookData::setCurrentSpineLineNo(quint8 lineNo)
{
  d->m_currentSpineLineNo = lineNo;
  setModified(true);
}

bool
BookData::modified()
{
  return d->m_modified;
}

void
BookData::setModified(bool modified)
{
  d->m_modified = modified;
}

QDateTime
BookData::dateLastRead()
{
  return d->m_dateLastRead;
}

void
BookData::setDateLastRead(QDateTime dateLastRead)
{
  d->m_dateLastRead = dateLastRead;
  setModified(true);
}

quint64
BookData::readPosition()
{
  return d->m_readPosition;
}

void
BookData::setReadPosition(quint64 readPosition)
{
  d->m_readPosition = readPosition;
  setModified(true);
}

bool
BookData::showContentsList()
{
  return d->m_showContentsList;
}

void
BookData::setShowContentsList(bool showContentsList)
{
  d->m_showContentsList = showContentsList;
  setModified(true);
}

QString BookData::page() { return d->page; }

void BookData::setPage(const QString &page) { d->page = page; }

bool
BookData::isValid()
{
  return !(d->m_uid < 0);
}

quint64
BookData::nextUid()
{
  return ++highestUid;
}

//== EBookLibraryDB
//==============================================================================
quint64 BookData::highestUid = 0;

// YAML config/library name constants.
const QString EBookLibraryDB::FILENAME = "filename";
const QString EBookLibraryDB::TITLE = "title";
const QString EBookLibraryDB::CREATORS = "creators";
const QString EBookLibraryDB::SERIES_ID = "series uid";
const QString EBookLibraryDB::SERIES_INDEX = "series index";
const QString EBookLibraryDB::CURRENT_SPINE_ID = "spine id";
const QString EBookLibraryDB::SPINE_LINENO = "spine lineno";
const QString EBookLibraryDB::LAST_READ = "last read";
const QString EBookLibraryDB::READ_POSITION = "read position";
const QString EBookLibraryDB::SHOW_CONTENTS = "show contents";

EBookLibraryDB::EBookLibraryDB(POptions options, QObject* parent)
  : QObject(parent)
  , m_options(options)
  , m_modified(false)
{}

EBookLibraryDB::~EBookLibraryDB() {}

bool
EBookLibraryDB::save(const QString& filename)
{
  return saveLibrary(filename);
}

bool
EBookLibraryDB::load(const QString& filename)
{
  return loadLibrary(filename);
}

quint64
EBookLibraryDB::insertOrUpdateBook(PBookData bookData)
{
  if (bookData->uid() == 0) { // should already be set.
    bookData->setUId(++BookData::highestUid);
  }
  if (!m_bookData.contains(bookData->uid()) && bookData->modified()) {
    m_bookData.insert(bookData->uid(), bookData);
    m_bookByTitle.insert(bookData->title().toLower(), bookData);
    m_bookByFile.insert(bookData->filename(), bookData);
    m_modified = false;
  }
  return bookData->uid();
}

bool
EBookLibraryDB::removeBook(quint64 index)
{
  if (m_bookData.contains(index)) {
    auto book = m_bookData.value(index);
    m_bookData.remove(index);
    m_bookByTitle.remove(book->title().toLower());
    m_bookByFile.remove(book->filename());
    m_modified = true;
    return true;
  }
  return false;
}

PBookData
EBookLibraryDB::bookByUid(quint64 uid)
{
  return m_bookData.value(uid);
}

BookList
EBookLibraryDB::bookByTitle(QString title)
{
  return m_bookByTitle.values(title.toLower());
}

PBookData
EBookLibraryDB::bookByFile(QString filename)
{
  return m_bookByFile.value(filename);
}

QString
EBookLibraryDB::currentBookId(QString filename)
{
  return m_bookByFile.value(filename)->currentSpineId();
}

bool
EBookLibraryDB::isModified()
{
  return m_modified;
}

void
EBookLibraryDB::setModified(bool modified)
{
  m_modified = modified;
}

bool
EBookLibraryDB::loadLibrary(const QString& filename)
{
  QFile file(filename);
  if (file.exists()) {
    auto libraryNode = YAML::LoadFile(file);
    if (libraryNode.IsMap()) {
      for (YAML::const_iterator it = libraryNode.begin();
           it != libraryNode.end();
           ++it) {
        auto book = PBookData(new BookData());
        book->setUId(it->first.as<quint64>());

        auto bookNode = it->second;
        book->setFilename(bookNode[FILENAME].as<QString>());
        book->setTitle(bookNode[TITLE].as<QString>());
        book->setCreators(bookNode[CREATORS].as<QList<QString>>());
        book->setSeries(bookNode[SERIES_ID].as<QString>());
        book->setSeriesIndex(bookNode[SERIES_INDEX].as<quint16>());
        book->setCurrentSpineId(bookNode[CURRENT_SPINE_ID].as<QString>());
        book->setCurrentSpineLineNo(bookNode[SPINE_LINENO].as<int>());
        book->setDateLastRead(QDateTime::fromString(
          bookNode[LAST_READ].as<QString>(), Qt::ISODate));
        book->setReadPosition(bookNode[READ_POSITION].as<quint64>());
        book->setShowContentsList(bookNode[SHOW_CONTENTS].as<bool>());

        BookData::highestUid =
          (book->uid() > BookData::highestUid ? book->uid()
                                              : BookData::highestUid);

        m_bookData.insert(book->uid(), book);
        m_bookByTitle.insert(book->title().toLower(), book);
        m_bookByFile.insert(book->filename(), book);
      }
    }
    m_modified = false;
    return true;
  }

  return false;
}

bool
EBookLibraryDB::saveLibrary(const QString& filename)
{
  QFile file(filename);
  //  if (m_modified || overrideModified) {
  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    emitter << YAML::Comment(
      tr("A YAML File is supposed to be user readable/editable but\n"
         "you need to be careful when manually editing.\n"
         "Remember that the uid numbers stand for unique identifier\n"
         "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
         "you repeat one the second will overwrite the first."));

    emitter << YAML::BeginMap; // books map
    {
      for (auto& bookData : m_bookData) {
        emitter << YAML::Key << bookData->uid();
        emitter << YAML::Value;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << FILENAME;
        emitter << YAML::Value << bookData->filename();
        emitter << YAML::Key << TITLE;
        emitter << YAML::Value << bookData->title();
        emitter << YAML::Key << CREATORS;
        emitter << YAML::Value << bookData->creators();
        emitter << YAML::Key << SERIES_ID;
        emitter << YAML::Value << bookData->series();
        emitter << YAML::Key << SERIES_INDEX;
        emitter << YAML::Value << bookData->seriesIndex();
        emitter << YAML::Key << CURRENT_SPINE_ID;
        emitter << YAML::Value << bookData->currentSpineId();
        emitter << YAML::Key << SPINE_LINENO;
        emitter << YAML::Value << bookData->currentSpineLineNo();
        emitter << YAML::Key << LAST_READ;
        emitter << YAML::Value
                << bookData->dateLastRead().toString(Qt::ISODate);
        emitter << YAML::Key << READ_POSITION;
        emitter << YAML::Value << bookData->readPosition();
        emitter << YAML::Key << SHOW_CONTENTS;
        emitter << YAML::Value << bookData->showContentsList();
        emitter << YAML::EndMap; // individual book map
      }
    }
    emitter << YAML::EndMap; // end books map

    QTextStream out(&file);
    out << emitter.c_str();
  }

  return true;
  //  }
  //  return false;

} // library changed
