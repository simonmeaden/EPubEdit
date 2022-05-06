#include "document/authors.h"

// starts at 1 - 0 == null value
quint64 EBookAuthorsDB::m_highestUid = 1;

EBookAuthorsDB::EBookAuthorsDB(QObject* parent)
  : QObject(parent)
  , m_author_changed(false)
{}

EBookAuthorsDB::~EBookAuthorsDB() {}

bool
EBookAuthorsDB::save(const QString& filename)
{
  return saveAuthors(filename);
}

bool
EBookAuthorsDB::load(const QString& filename)
{
  return loadAuthors(filename);
}

quint64
EBookAuthorsDB::insertAuthor(AuthorData author_data)
{
  if (!author_data)
    return 0;

  if (author_data->uid() == 0) {
    author_data->setUid(nextUid());
  }
  m_authorData.insert(author_data->uid(), author_data);
  if (author_data->displayName().isEmpty()) {
    if (!author_data->isEmpty()) {
      // TODO handle asian type surname first format.
      QString display_name = author_data->forename();
      display_name += (display_name.isEmpty() ? "" : " " + display_name);
      display_name += " " + author_data->surname();
      author_data->setDisplayName(display_name);
    }
  } else {
    if (author_data->isEmpty()) {
      QStringList splits = author_data->displayName().split(" ");
      if (splits.size() == 1) {
        author_data->setSurname(splits.at(0));
      } else if (splits.size() == 2) {
        author_data->setForename(splits.at(0));
        author_data->setSurname(splits.at(1));
      } else {
        QString middle_names;
        author_data->setForename(splits.first());
        author_data->setSurname(splits.last());
        for (int i = 1; i < splits.size() - 1; i++) {
          if (i > 1)
            middle_names += " ";
          middle_names += splits.at(i);
        }
        author_data->setMiddlenames(middle_names);
      }
    }
  }

  if (author_data->fileAs().isEmpty() && !author_data->isEmpty()) {
    if (!author_data->surname().isEmpty() &&
        !author_data->forename().isEmpty()) {
      QString file_as = author_data->surname() + ", " +
                        author_data->forename() +
                        (author_data->middlenames().isEmpty()
                           ? ""
                           : " " + author_data->middlenames());
      author_data->setFile_as(file_as);
    }
  }

  addAuthor(author_data);
  return author_data->uid();
}

QStringList
EBookAuthorsDB::compareAndDiscard(QStringList names)
{
  QStringList cleaned;
  for (auto& value : names) {
    QString lower = value.toLower();
    // check if the surname list has a match.
    QList<AuthorData> list = m_authorBySurname.values(value);
    for (auto& data : list) {
      if (lower == data->forename().toLower() ||
          lower == data->surname().toLower()) {
        if (!cleaned.contains(value))
          cleaned += value;
      }
    }
    // check if the forename list has a match.
    list = m_authorByForename.values(value);
    for (auto& data : list) {
      if (lower == data->forename().toLower() ||
          lower == data->surname().toLower()) {
        if (!cleaned.contains(value))
          cleaned += value;
      }
    }
  }
  return cleaned;
}

bool
EBookAuthorsDB::removeBook(quint64 index)
{
  if (m_authorData.contains(index)) {
    AuthorData author = m_authorData.value(index);
    m_authorData.remove(index);
    m_authorByDisplayname.remove(author->displayName(), author);
    m_authorByFileas.remove(author->fileAs(), author);
    m_author_changed = true;
    return true;
  }
  return false;
}

/*!
 * \brief gets the author data by display name if it exists otherwise attempts
 * to use surname only.
 *
 * \param name - display name.
 * \return the relevant AuthorData object.
 */
AuthorData
EBookAuthorsDB::author(QString name)
{
  AuthorData data;
  data = m_authorByDisplayname.value(name);
  if (!data.isNull()) {
    return data;
  }

  /* The display name is normally in the form 'FORENAME SURNAME' or
   * vicky-verky  in the case of asian names.
   */
  QStringList splits = name.split(" ");
  if (splits.size() > 1) {
    if (splits.size() == 2) { // normal case unless middle names are supplied.
      if (m_authorBySurname.contains(splits.last())) {
        data = m_authorBySurname.value(splits.last());
        if (data->forename() == splits.first()) {
          return data;
        } else {
          // TODO different surname?
        }
      }
    } else {
    }
  } else {
  }
  return data;
}

AuthorData
EBookAuthorsDB::authorByFileAs(QString file_as)
{
  return m_authorByFileas.value(file_as);
}

AuthorData
EBookAuthorsDB::author(quint64 uid)
{
  return m_authorData.value(uid);
}

QList<AuthorData>
EBookAuthorsDB::authors()
{
  return m_authorData.values();
}

QList<AuthorData>
EBookAuthorsDB::authorsBySurname(QString surname)
{
  return m_authorBySurname.values(surname);
}

QList<AuthorData>
EBookAuthorsDB::authorsByForename(QString surname)
{
  return m_authorByForename.values(surname);
}

bool
EBookAuthorsDB::loadAuthors(const QString& filename)
{
  QFile file(filename);
  if (file.exists()) {
    YAML::Node authors_map = YAML::LoadFile(filename.toStdString());
    if (authors_map && authors_map.IsMap()) {
      for (YAML::const_iterator it1 = authors_map.begin();
           it1 != authors_map.end();
           ++it1) {
        YAML::Node author_node = it1->second;
        AuthorData author = AuthorData(new EBookAuthorData());

        quint64 uid =
          it1->first.as<quint64>(); // author_node["uid"].as<quint64>();
        author->setUid(uid);
        author->setSurname(
          (author_node["surname"] ? author_node["surname"].as<QString>() : ""));
        author->setForename((author_node["forenames"]
                               ? author_node["forenames"].as<QString>()
                               : ""));
        author->setMiddlenames((author_node["middlenames"]
                                  ? author_node["middlenames"].as<QString>()
                                  : ""));
        author->setDisplayName((author_node["display name"]
                                  ? author_node["display name"].as<QString>()
                                  : ""));
        author->setFile_as(
          (author_node["file as"] ? author_node["file as"].as<QString>() : ""));
        author->setWebsite(
          (author_node["website"] ? author_node["website"].as<QString>() : ""));
        author->setWikipedia((author_node["wikipedia"]
                                ? author_node["wikipedia"].as<QString>()
                                : ""));
        author->setSurnameLast((author_node["surname last"]
                                  ? author_node["surname last"].as<bool>()
                                  : true));
        if (author_node["image"]) {
          QPixmap pixmap = author_node["image"].as<QPixmap>();
          if (!pixmap.isNull())
            author->setPixmap(pixmap);
        }
        QStringList words;
        YAML::Node word_list = author_node["word list"];
        if (!word_list.IsNull() && word_list.IsSequence()) {
          for (YAML::const_iterator it2 = word_list.begin();
               it2 != word_list.end();
               ++it2) {
            words << it2->second.as<QString>();
          }
        }
        if (author->uid() > m_highestUid) {
          m_highestUid = author->uid();
        }

        addAuthor(author);
      }
    }
    m_author_changed = false;
    return true;
  }

  return false;
}

bool
EBookAuthorsDB::saveAuthors(const QString& filename)
{
  QFile file(filename);
  //  if (m_author_changed) {
  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    emitter << YAML::Comment(
      tr("A YAML File is supposed to be user readable/editable but\n"
         "you need to be careful when manually editing.\n"
         "Remember that the uid numbers stand for unique identifier\n"
         "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
         "you repeat one the second will overwrite the first."));

    emitter << YAML::BeginMap;
    for (auto& author_data : m_authorData) {
      // for some reason emitter wont take the result directly from the
      // method. might need some work on the YAML files.
      quint64 uid = author_data->uid();
      QString surname = author_data->surname();
      QString forename = author_data->forename();
      QString middlenames = author_data->middlenames();
      QString display_name = author_data->displayName();
      QString file_as = author_data->fileAs();
      QString website = author_data->website();
      QString wikipedia = author_data->wikipedia();
      QPixmap pixmap = author_data->pixmap();
      QStringList words = author_data->wordList();

      emitter << YAML::Key << uid;
      emitter << YAML::Value;
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "surname";
      emitter << YAML::Value << surname;
      emitter << YAML::Key << "forenames";
      emitter << YAML::Value << forename;
      emitter << YAML::Key << "middlenames";
      emitter << YAML::Value << middlenames;
      emitter << YAML::Key << "display name";
      emitter << YAML::Value << display_name;
      emitter << YAML::Key << "file as";
      emitter << YAML::Value << file_as;
      emitter << YAML::Key << "surname last";
      emitter << YAML::Value << author_data->surnameLast();
      emitter << YAML::Key << "website";
      emitter << YAML::Value << website;
      emitter << YAML::Key << "wikipedia";
      emitter << YAML::Value << wikipedia;
      if (!author_data->pixmap().isNull()) {
        emitter << YAML::Key << "image";
        emitter << YAML::Value << pixmap;
      }
      emitter << YAML::Key << "word list";
      emitter << YAML::BeginSeq;
      for (int i = 0; i < words.size(); i++) {
        QString word = words.at(i);
        emitter << word;
      }
      emitter << YAML::EndSeq;

      emitter << YAML::EndMap;
    }
    emitter << YAML::EndMap;
    QTextStream out(&file);
    out << emitter.c_str();
    return true;
  }
  //  } // authors changed
  return false;
}

AuthorData
EBookAuthorsDB::addAuthor(QString display_name)
{
  // TODO maybe this is not used.
  /* The display name is normally in the form 'FORENAME SURNAME' or
   * vicky-verky  in the case of asian names.
   */
  AuthorData data;
  QStringList splits = display_name.split(" ");
  if (splits.size() > 1) {
    if (splits.size() == 2) { // normal case unless middle names are supplied.
      if (m_authorBySurname.contains(splits.last())) {
        data = m_authorBySurname.value(splits.last());
        if (data->forename() == splits.first()) {
          return AuthorData(nullptr);
        }
      }
    }
  }
  return data;
}

void
EBookAuthorsDB::addAuthor(AuthorData author_data)
{
  if (author_data->isValid()) {
    if (!m_authorData.contains(author_data->uid())) {
      m_authorData.insert(author_data->uid(), author_data);
      if (!author_data->surname().isEmpty())
        m_authorBySurname.insert(author_data->surname().toLower(),
                                   author_data);
      if (!author_data->forename().isEmpty())
        m_authorByForename.insert(author_data->forename().toLower(),
                                    author_data);
      if (!author_data->displayName().isEmpty())
        m_authorByDisplayname.insert(author_data->displayName(), author_data);
      if (!author_data->fileAs().isEmpty())
        m_authorByFileas.insert(author_data->fileAs().toLower(), author_data);
    }
  }
}

QString
EBookAuthorData::forename() const
{
  return d->m_forename;
}

void
EBookAuthorData::setForename(const QString& value)
{
  d->m_modified = true;
  d->m_forename = value;
}

QString
EBookAuthorData::middlenames() const
{
  return d->m_middlenames;
}

void
EBookAuthorData::setMiddlenames(const QString& value)
{
  d->m_modified = true;
  d->m_middlenames = value;
}

QString
EBookAuthorData::surname() const
{
  return d->m_surname;
}

void
EBookAuthorData::setSurname(const QString& surname)
{
  d->m_modified = true;
  d->m_surname = surname;
}

QString
EBookAuthorData::fileAs() const
{
  return d->m_file_as;
}

void
EBookAuthorData::setFile_as(const QString& file_as)
{
  d->m_modified = true;
  d->m_file_as = file_as;
}

quint64
EBookAuthorData::uid() const
{
  return d->m_uid;
}

void
EBookAuthorData::setUid(const quint64& uid)
{
  d->m_uid = uid;
}

QString
EBookAuthorData::displayName() const
{
  return d->m_display_name;
}

void
EBookAuthorData::setDisplayName(const QString& display_name)
{
  d->m_modified = true;
  d->m_display_name = display_name;
}

QPixmap
EBookAuthorData::pixmap() const
{
  return d->m_pixmap;
}

void
EBookAuthorData::setPixmap(const QPixmap& pixmap)
{
  d->m_modified = true;
  d->m_pixmap = pixmap;
}

bool
EBookAuthorData::surnameLast() const
{
  return d->m_surname_last;
}

void
EBookAuthorData::setSurnameLast(bool surname_last)
{
  d->m_modified = true;
  d->m_surname_last = surname_last;
}

QStringList
EBookAuthorData::wordList() const
{
  return d->m_word_list;
}

void
EBookAuthorData::setWordList(const QStringList& word_list)
{
  d->m_word_list = word_list;
}

QList<quint64>
EBookAuthorData::books() const
{
  return d->m_books;
}

void
EBookAuthorData::setBooks(const QList<quint64>& books)
{
  d->m_modified = true;
  d->m_books = books;
}

QString
EBookAuthorData::wikipedia() const
{
  return d->m_wikipedia;
}

void
EBookAuthorData::setWikipedia(const QString& wikipedia)
{
  d->m_modified = true;
  d->m_wikipedia = wikipedia;
}

QString
EBookAuthorData::website() const
{
  return d->m_website;
}

void
EBookAuthorData::setWebsite(const QString& website)
{
  d->m_modified = true;
  d->m_website = website;
}

EBookAuthorData::EBookAuthorData()
{
  d = new SharedAuthorData;
  d->m_uid = 0;
  d->m_surname_last = true;
}

EBookAuthorData::EBookAuthorData(const EBookAuthorData& other)
{
  d->m_modified = other.d->m_modified;
  d->m_uid = other.d->m_uid;
  d->m_display_name = other.d->m_display_name;
  d->m_surname = other.d->m_surname;
  d->m_forename = other.d->m_forename;
  d->m_middlenames = other.d->m_middlenames;
  d->m_surname_last = other.d->m_surname_last;
  d->m_file_as = other.d->m_file_as;
  d->m_website = other.d->m_website;
  d->m_wikipedia = other.d->m_wikipedia;
  d->m_pixmap = other.d->m_pixmap;
}

EBookAuthorData::~EBookAuthorData() {}

bool
EBookAuthorData::isValid()
{
  return (d->m_uid > 0);
}

bool
EBookAuthorData::isEmpty()
{
  return (d->m_surname.isEmpty() && d->m_forename.isEmpty() &&
          d->m_middlenames.isEmpty());
}

bool
EBookAuthorData::isModified()
{
  return d->m_modified;
}

EBookAuthorData::Comparison
EBookAuthorData::compare(QString forename, QString middlenames, QString surname)
{
  Comparison match = NO_MATCH;
  if (surname.toLower() == surname)
    match = SURNAME_MATCH;

  if (forename.toLower() == forename) {
    if (match == SURNAME_MATCH)
      match = FORE_AND_SURNAME_MATCH;
    else
      match = FORENAME_MATCH;
  }

  if (middlenames.toLower() == middlenames) {
    if (match == FORE_AND_SURNAME_MATCH)
      match = ALL_MATCH;
    else
      match = PARTIAL_MATCH;
  }

  return match;
}

