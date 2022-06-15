#include "document/series.h"
#include "document/options.h"
#include "qyamlcpp.h"

quint64 EBookSeriesData::m_highestUid = 0;
//quint64 EBookSeriesDB::m_highestUid = 0;

EBookSeriesData::EBookSeriesData()
  : m_uid(0)
{}

EBookSeriesData::~EBookSeriesData() {}

quint64
EBookSeriesData::uid() const
{
  return m_uid;
}

void
EBookSeriesData::setUid(const quint64& uid)
{
  m_uid = uid;
}

QString
EBookSeriesData::name() const
{
  return m_name;
}

void
EBookSeriesData::setName(const QString& name)
{
  m_name = name;
}

QStringList
EBookSeriesData::wordList() const
{
  return m_seriesWords;
}

void
EBookSeriesData::setSeriesWords(const QStringList& series_words)
{
  m_seriesWords = series_words;
}

EBookSeriesDB::EBookSeriesDB(POptions options, QObject* parent)
  : QObject(parent)
  , m_options(options)
  , m_seriesChanged(false)
{}

EBookSeriesDB::EBookSeriesDB(const EBookSeriesDB& other)
{
  m_seriesMap = other.m_seriesMap;
  m_seriesByName = other.m_seriesByName;
  m_seriesList = other.m_seriesList;
}

EBookSeriesDB::~EBookSeriesDB() {}

bool
EBookSeriesDB::save(const QString& filename)
{
  return saveSeries(filename);
}

bool
EBookSeriesDB::load(const QString& filename)
{
  return loadSeries(filename);
}

SeriesList
EBookSeriesDB::seriesList()
{
  return m_seriesList;
}

PSeriesData
EBookSeriesDB::series(quint64 uid)
{
  return m_seriesMap.value(uid);
}

PSeriesData
EBookSeriesDB::seriesByName(QString name)
{
  return m_seriesByName.value(name.toLower());
}

void
EBookSeriesData::setMaxUid(quint64 seriesUid)
{
  EBookSeriesData::m_highestUid = std::max(seriesUid, EBookSeriesData::m_highestUid);
}

//quint64
//EBookSeriesDB::nextUid()
//{
//  return ++EBookSeriesDB::m_highestUid;
//}

bool
EBookSeriesDB::loadSeries(const QString& filename)
{
  QFile file(filename);
  if (file.exists()) {
    auto library_node = YAML::LoadFile(file);
    if (library_node.IsMap()) {
      for (auto it1 = library_node.begin(); it1 != library_node.end(); ++it1) {
        auto series = PSeriesData(new EBookSeriesData());
        series->setUid(it1->first.as<quint64>());
        auto series_node = it1->second;
        if (!series_node.IsNull() && series_node.IsMap()) {
          series->setName(series_node["name"].as<QString>());
          QStringList words;
          auto word_list = series_node["word list"];
          if (!word_list.IsNull() && word_list.IsSequence()) {
            for (auto it2 = word_list.begin(); it2 != word_list.end(); ++it2) {
              words << it2->second.as<QString>();
            }
          }
        }

        EBookSeriesData::setMaxUid(series->uid());
        insertSeries(series);
      }
    }
    m_seriesChanged = false;
  }

  return true;
}

bool
EBookSeriesDB::saveSeries(const QString& filename)
{
  QFile file(filename);
  //  if (m_series_changed) {
  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    emitter << YAML::Comment(
      tr("A YAML File is supposed to be user readable/editable but\n"
         "you need to be careful when manually editing.\n"
         "Remember that the uid numbers stand for unique identifier\n"
         "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
         "you repeat one the second will overwrite the first."));

    emitter << YAML::BeginMap; // series map
    {
      auto seriesKeys = m_seriesMap.keys();
      for (auto key : seriesKeys) {
        auto series_data = m_seriesMap.value(key);
        auto name = series_data->name();
        auto words = series_data->wordList();
        emitter << YAML::Key << series_data->uid(); // map key
        emitter << YAML::Value;
        emitter << YAML::BeginMap; // individual series data
        emitter << YAML::Key << "name";
        emitter << YAML::Value << name;
        emitter << YAML::Key << "word list";
        emitter << YAML::BeginSeq;
        for (auto& word : words) {
          emitter << word;
        }
        emitter << YAML::EndSeq;
        emitter << YAML::EndMap; // end individual series data
      }
    }
    emitter << YAML::EndMap; // end series map

    QTextStream out(&file);
    out << emitter.c_str();
  }

  return true;
}

quint64
EBookSeriesDB::insertOrGetSeries(QString series)
{
  if (!m_seriesByName.contains(series.toLower())) {
    auto seriesData = PSeriesData(new EBookSeriesData());
    auto uid = seriesData->nextUid();
    seriesData->setUid(uid);
    seriesData->setName(series);
    insertSeries(seriesData);
    return uid;
  }
  return m_seriesByName.value(series.toLower())->uid();
}

void
EBookSeriesDB::insertSeries(PSeriesData series_data)
{
  m_seriesMap.insert(series_data->uid(), series_data);
  m_seriesByName.insert(series_data->name().toLower(), series_data);
  m_seriesList.append(series_data->name());
}

bool
EBookSeriesDB::removeSeries(quint64 index)
{
  if (m_seriesMap.contains(index)) {
    auto data = m_seriesMap.value(index);
    m_seriesMap.remove(index);
    m_seriesList.removeOne(data->name());
    return true;
  }
  return false;
}
