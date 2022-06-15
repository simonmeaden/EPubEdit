#include "document/options.h"
#include "document/authors.h"
#include "document/library.h"
#include "document/series.h"

#include "qyamlcpp.h"

QString EBookOptions::POSITION = "window";
QString EBookOptions::DIALOG = "options dialog";
QString EBookOptions::PREF_CURRENT_INDEX = "current book";
QString EBookOptions::PREF_CURRENT_ITEMS = "current book item";
QString EBookOptions::PREF_CURRENT_LINE_NOS = "current book line nos";
QString EBookOptions::PREF_COUNT = "count";
QString EBookOptions::PREF_BOOKLIST = "book list";
QString EBookOptions::PREF_LIBRARY = "library list";
QString EBookOptions::CODE_OPTIONS = "code editor";
QString EBookOptions::CODE_FONT = "font";
QString EBookOptions::CODE_NORMAL = "normal";
QString EBookOptions::CODE_ATTRIBUTE = "attribute";
QString EBookOptions::CODE_TAG = "tag";
QString EBookOptions::CODE_ERROR = "error";
QString EBookOptions::CODE_STRING = "string";
QString EBookOptions::CODE_STYLE = "style";
QString EBookOptions::CODE_SCRIPT = "script";
QString EBookOptions::CODE_COLOR = "color";
QString EBookOptions::CODE_BACK = "background";
QString EBookOptions::CODE_WEIGHT = "weight";
QString EBookOptions::CODE_ITALIC = "italic";
QString EBookOptions::COPY_BOOKS_TO_STORE = "copy books to store";
QString EBookOptions::DELETE_OLD_BOOK = "delete old book";
QString EBookOptions::SHOW_TOC = "show toc";
QString EBookOptions::TOC_POSITION = "toc position";
QString EBookOptions::VIEW_STATE = "view state";

EBookOptions::EBookOptions(QObject* parent)
  : QObject(parent)
  , m_code_font(QFont("Courier", 10))
  , m_normal_color(Qt::black)
  , m_normal_back(Qt::white)
  , m_normal_italic(false)
  , m_normal_weight(QFont::Normal)
  , m_attribute_color(Qt::cyan)
  , m_attribute_back(Qt::white)
  , m_attribute_italic(false)
  , m_attribute_weight(QFont::Normal)
  , m_tag_color(Qt::blue)
  , m_tag_back(Qt::white)
  , m_tag_italic(false)
  , m_tag_weight(QFont::Normal)
  , m_string_color(Qt::green)
  , m_string_back(Qt::white)
  , m_string_italic(false)
  , m_string_weight(QFont::Normal)
  , m_error_color(Qt::yellow)
  , m_error_back(Qt::red)
  , m_error_italic(false)
  , m_error_weight(QFont::Normal)
  , m_script_color("mediumorchid")
  , m_script_back(Qt::white)
  , m_script_italic(false)
  , m_script_weight(QFont::Normal)
  , m_style_color("silver")
  , m_style_back(Qt::white)
  , m_style_italic(false)
  , m_style_weight(QFont::Normal)
{
  lib_key = QPixmapCache::insert(QPixmap(":/icons/library"));
  up_key = QPixmapCache::insert(QPixmap(":/icons/go-up"));
  down_key = QPixmapCache::insert(QPixmap(":/icons/go-down"));
  previous_key = QPixmapCache::insert(QPixmap(":/icons/go-previous"));
  next_key = QPixmapCache::insert(QPixmap(":/icons/go-next"));
  top_key = QPixmapCache::insert(QPixmap(":/icons/go-top"));
  bottom_key = QPixmapCache::insert(QPixmap(":/icons/go-bottom"));
  plus_key = QPixmapCache::insert(QPixmap(":/icons/add"));
  minus_key = QPixmapCache::insert(QPixmap(":/icons/remove"));
  new_key = QPixmapCache::insert(QPixmap(":/icons/new"));
  open_key = QPixmapCache::insert(QPixmap(":/icons/open"));
  save_key = QPixmapCache::insert(QPixmap(":/icons/save"));
  editor_key = QPixmapCache::insert(QPixmap(":/icons/editor"));
  code_key = QPixmapCache::insert(QPixmap(":/icons/code"));
  meta_key = QPixmapCache::insert(QPixmap(":/icons/metadata"));
  bookshelf_key = QPixmapCache::insert(QPixmap(":/icons/bookshelf"));
  tree_key = QPixmapCache::insert(QPixmap(":/icons/tree"));
  dialog_error_key = QPixmapCache::insert(QPixmap(":/icons/dialog-error"));
  view_refresh_key = QPixmapCache::insert(QPixmap(":/icons/view-refresh"));
  text_html_key = QPixmapCache::insert(QPixmap(":/icons/text-html"));

  QFile file;
  file.setFileName(":/javascript/jquery");
  file.open(QIODevice::ReadOnly);
  jquery = file.readAll();
  jquery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
  file.close();

  file.setFileName(":qtwebchannel/webchannel");
  file.open(QIODevice::ReadOnly);
  jwebchannel = file.readAll();
  file.close();

  file.setFileName(":javascript/webpage");
  file.open(QIODevice::ReadOnly);
  jwebpage = file.readAll();
  file.close();
}

EBookOptions::~EBookOptions() {}

void
EBookOptions::save(const QString& filename)
{
  QFile file(filename);

//  if (m_pref_changed) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << POSITION;
        emitter << YAML::Value << m_rect;
        emitter << YAML::Key << DIALOG;
        emitter << YAML::Value << m_options_dlg_size;
        emitter << YAML::Key << PREF_CURRENT_INDEX;
        emitter << YAML::Value << m_currentindex;
        emitter << YAML::Key << SHOW_TOC;
        emitter << YAML::Value << m_toc_visible;
        emitter << YAML::Key << TOC_POSITION;
        emitter << YAML::Value
                << (m_toc_position == EBookOptions::LEFT ? "LEFT" : "RIGHT");
        emitter << YAML::Key << PREF_BOOKLIST;
        {
          // Start of PREF_BOOKLIST
          emitter << YAML::BeginSeq;
          for (auto& book : m_current_files) {
            emitter << book;
          }
          emitter << YAML::EndSeq;
        } // End of PREF_BOOKLIST
        emitter << YAML::Key << CODE_OPTIONS;
        {
          // Start of CODE_OPTIONS
          emitter << YAML::BeginMap;
          emitter << YAML::Key << CODE_FONT;
          emitter << YAML::Value << m_code_font;
          emitter << YAML::Key << CODE_NORMAL;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_normal_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_normal_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_normal_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_normal_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_ATTRIBUTE;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_attribute_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_attribute_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_attribute_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_attribute_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_TAG;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_tag_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_tag_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_tag_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_tag_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_STRING;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_string_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_string_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_string_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_string_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_ERROR;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_error_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_error_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_error_weight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_error_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_STYLE;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_style_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_style_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_style_weight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_style_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_SCRIPT;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_script_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_script_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_script_weight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_script_italic;
            emitter << YAML::EndMap;
          }
        } // End of CODE_OPTIONS
        emitter << YAML::EndMap;
      }
      QTextStream out(&file);
      out << emitter.c_str();
      file.close();
    }
//  }
}

void
EBookOptions::load(const QString &filename)
{
  QFile file(filename);
  if (file.exists() && file.open(QFile::ReadOnly)) {
    YAML::Node preferences = YAML::LoadFile(file);
    // Last window position.
    if (preferences[POSITION]) {
      m_rect = preferences[POSITION].as<QRect>();
    } else {
      m_rect = QRect(0, 0, DEF_WIDTH, DEF_HEIGHT);
    }
    if (preferences[DIALOG]) {
      m_options_dlg_size = preferences[DIALOG].as<QSize>();
    } else {
      m_options_dlg_size = QSize(DEF_DLG_WIDTH, DEF_DLG_HEIGHT);
    }
    // Current book being read/edited.
    if (preferences[PREF_CURRENT_INDEX]) {
      m_currentindex = preferences[PREF_CURRENT_INDEX].as<int>();
    } else {
      m_currentindex = 0;
    }
    if (preferences[SHOW_TOC]) {
      m_toc_visible = preferences[SHOW_TOC].as<bool>();
    } else {
      m_toc_visible = true;
    }
    if (preferences[TOC_POSITION]) {
      QString pos = preferences[TOC_POSITION].as<QString>();
      m_toc_position =
        (pos == "LEFT" ? EBookOptions::LEFT : EBookOptions::RIGHT);
    } else {
      m_toc_position = EBookOptions::LEFT;
    }
    // Last books loaded in library.
    YAML::Node books = preferences[PREF_BOOKLIST];
    if (books && books.IsSequence()) {
      m_current_files.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_current_files.append(books[i].as<QString>());
      }
    }

    YAML::Node codeoptions = preferences[CODE_OPTIONS];
    if (codeoptions && codeoptions.IsMap()) {
      YAML::Node codefont = codeoptions[CODE_FONT];
      if (codefont) {
        m_code_font = codefont.as<QFont>();
      }
      YAML::Node codenormal = codeoptions[CODE_NORMAL];
      if (codenormal && codenormal.IsMap()) {
        m_normal_color = codenormal[CODE_COLOR].as<QColor>();
        m_normal_back = codenormal[CODE_BACK].as<QColor>();
        m_normal_weight = QFont::Weight(codenormal[CODE_WEIGHT].as<int>());
        m_normal_italic = codenormal[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeatt = codeoptions[CODE_ATTRIBUTE];
      if (codeatt && codeatt.IsMap()) {
        m_attribute_color = codeatt[CODE_COLOR].as<QColor>();
        m_attribute_back = codeatt[CODE_BACK].as<QColor>();
        m_attribute_weight = QFont::Weight(codeatt[CODE_WEIGHT].as<int>());
        m_attribute_italic = codeatt[CODE_ITALIC].as<bool>();
      }
      YAML::Node codetag = codeoptions[CODE_TAG];
      if (codetag && codetag.IsMap()) {
        m_tag_color = codetag[CODE_COLOR].as<QColor>();
        m_tag_back = codetag[CODE_BACK].as<QColor>();
        m_tag_weight = QFont::Weight(codetag[CODE_WEIGHT].as<int>());
        m_tag_italic = codeatt[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeattstr = codeoptions[CODE_STRING];
      if (codeattstr && codeattstr.IsMap()) {
        m_string_color = codeattstr[CODE_COLOR].as<QColor>();
        m_string_back = codeattstr[CODE_BACK].as<QColor>();
        m_string_weight = QFont::Weight(codeattstr[CODE_WEIGHT].as<int>());
        m_string_italic = codeattstr[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeerr = codeoptions[CODE_ERROR];
      if (codeerr && codeerr.IsMap()) {
        m_error_color = codeerr[CODE_COLOR].as<QColor>();
        m_error_back = codeerr[CODE_BACK].as<QColor>();
        m_error_weight = QFont::Weight(codeerr[CODE_WEIGHT].as<int>());
        m_error_italic = codeerr[CODE_ITALIC].as<bool>();
      }
      YAML::Node codestyle = codeoptions[CODE_STYLE];
      if (codestyle && codestyle.IsMap()) {
        m_style_color = codestyle[CODE_COLOR].as<QColor>();
        m_style_back = codestyle[CODE_BACK].as<QColor>();
        m_style_weight = QFont::Weight(codestyle[CODE_WEIGHT].as<int>());
        m_style_italic = codestyle[CODE_ITALIC].as<bool>();
      }
      YAML::Node codescript = codeoptions[CODE_SCRIPT];
      if (codescript && codescript.IsMap()) {
        m_script_color = codescript[CODE_COLOR].as<QColor>();
        m_script_back = codescript[CODE_BACK].as<QColor>();
        m_script_weight = QFont::Weight(codescript[CODE_WEIGHT].as<int>());
        m_script_italic = codescript[CODE_ITALIC].as<bool>();
      }
    }
  } else {
    m_rect = QRect(DEF_X, DEF_Y, DEF_WIDTH, DEF_HEIGHT);
    m_currentindex = -1;
    m_current_files = QStringList();
    m_pref_changed = true;
  }
}

EBookOptions::TocPosition
EBookOptions::tocPosition() const
{
  return m_toc_position;
}

void
EBookOptions::setTocPosition(const TocPosition position)
{
  m_toc_position = position;
  m_pref_changed = true;
}

bool
EBookOptions::tocVisible() const
{
  return m_toc_visible;
}

void
EBookOptions::setTocVisible(const bool visible)
{
  m_toc_visible = visible;
  m_pref_changed = true;
}

QString
EBookOptions::codeOptionToString(const CodeOptions options)
{
  switch (options) {
    case NORMAL:
      return tr("Normal");
    case TAG:
      return tr("Tag");
    case STRING:
      return tr("String");
    case ATTRIBUTE:
      return tr("Attribute");
    case ERROR:
      return tr("Error");
    case STYLE:
      return tr("Style");
    case SCRIPT:
      return tr("Script");
  }
  return QString();
}

QString
EBookOptions::weightToString(const QFont::Weight weight)
{
  switch (weight) {
    case QFont::Thin:
      return "Thin";
    case QFont::ExtraLight:
      return "ExtraLight";
    case QFont::Light:
      return "Light";
    case QFont::Normal:
      return "Normal";
    case QFont::Medium:
      return "Medium";
    case QFont::DemiBold:
      return "DemiBold";
    case QFont::Bold:
      return "Bold";
    case QFont::ExtraBold:
      return "ExtraBold";
    case QFont::Black:
      return "Black";
  }
  return QString();
}

QColor
EBookOptions::normalColor() const
{
  return m_normal_color;
}

void
EBookOptions::setNormalColor(const QColor& normal_color)
{
  m_normal_color = normal_color;
  m_pref_changed = true;
}

QColor
EBookOptions::normalBack() const
{
  return m_normal_back;
}

void
EBookOptions::setNormalBack(const QColor& normal_back)
{
  m_normal_back = normal_back;
  m_pref_changed = true;
}

bool
EBookOptions::normalItalic() const
{
  return m_normal_italic;
}

void
EBookOptions::setNormalItalic(bool normal_italic)
{
  m_normal_italic = normal_italic;
  m_pref_changed = true;
}

QFont
EBookOptions::codeFont() const
{
  return m_code_font;
}

void
EBookOptions::setCodeFont(const QFont& code_font)
{
  m_code_font = code_font;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::normalWeight() const
{
  return m_normal_weight;
}

void
EBookOptions::setNormalWeight(const QFont::Weight& normal_weight)
{
  m_normal_weight = normal_weight;
  m_pref_changed = true;
}

QColor
EBookOptions::attributeColor() const
{
  return m_attribute_color;
}

void
EBookOptions::setAttributeColor(const QColor& attribute_color)
{
  m_attribute_color = attribute_color;
  m_pref_changed = true;
}

QColor
EBookOptions::attributeBack() const
{
  return m_attribute_back;
}

void
EBookOptions::setAttributeBack(const QColor& attribute_back)
{
  m_attribute_back = attribute_back;
  m_pref_changed = true;
}

bool
EBookOptions::attributeItalic() const
{
  return m_attribute_italic;
}

void
EBookOptions::setAttribute_italic(bool attribute_italic)
{
  m_attribute_italic = attribute_italic;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::attributeWeight() const
{
  return m_attribute_weight;
}

void
EBookOptions::setAttributeWeight(const QFont::Weight& attribute_weight)
{
  m_attribute_weight = attribute_weight;
  m_pref_changed = true;
}

QColor
EBookOptions::tagColor() const
{
  return m_tag_color;
}

void
EBookOptions::setTagColor(const QColor& tag_color)
{
  m_tag_color = tag_color;
  m_pref_changed = true;
}

QColor
EBookOptions::tagBack() const
{
  return m_tag_back;
}

void
EBookOptions::setTagBack(const QColor& tag_back)
{
  m_tag_back = tag_back;
  m_pref_changed = true;
}

bool
EBookOptions::tagItalic() const
{
  return m_tag_italic;
}

void
EBookOptions::setTagItalic(bool tag_italic)
{
  m_tag_italic = tag_italic;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::tagWeight() const
{
  return m_tag_weight;
}

void
EBookOptions::setTagWeight(const QFont::Weight& tag_weight)
{
  m_tag_weight = tag_weight;
  m_pref_changed = true;
}

QColor
EBookOptions::stringColor() const
{
  return m_string_color;
}

void
EBookOptions::setStringColor(const QColor& string_color)
{
  m_string_color = string_color;
  m_pref_changed = true;
}

QColor
EBookOptions::stringBack() const
{
  return m_string_back;
}

void
EBookOptions::setStringBack(const QColor& string_back)
{
  m_string_back = string_back;
  m_pref_changed = true;
}

bool
EBookOptions::stringItalic() const
{
  return m_string_italic;
}

void
EBookOptions::setStringItalic(bool string_italic)
{
  m_string_italic = string_italic;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::stringWeight() const
{
  return m_string_weight;
}

void
EBookOptions::setStringWeight(const QFont::Weight& string_weight)
{
  m_string_weight = string_weight;
  m_pref_changed = true;
}

QColor
EBookOptions::errorColor() const
{
  return m_error_color;
}

void
EBookOptions::setErrorColor(const QColor& error_color)
{
  m_error_color = error_color;
  m_pref_changed = true;
}

QColor
EBookOptions::errorBack() const
{
  return m_error_back;
}

void
EBookOptions::setErrorBack(const QColor& error_back)
{
  m_error_back = error_back;
  m_pref_changed = true;
}

bool
EBookOptions::errorItalic() const
{
  return m_error_italic;
}

void
EBookOptions::setErrorItalic(bool error_italic)
{
  m_error_italic = error_italic;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::errorWeight() const
{
  return m_error_weight;
}

void
EBookOptions::setErrorWeight(const QFont::Weight& error_weight)
{
  m_error_weight = error_weight;
  m_pref_changed = true;
}

QColor
EBookOptions::scriptColor() const
{
  return m_script_color;
}

void
EBookOptions::setScriptColor(const QColor& script_color)
{
  m_script_color = script_color;
  m_pref_changed = true;
}

QColor
EBookOptions::scriptBack() const
{
  return m_script_back;
}

void
EBookOptions::setScriptBack(const QColor& script_back)
{
  m_script_back = script_back;
  m_pref_changed = true;
}

bool
EBookOptions::scriptItalic() const
{
  return m_script_italic;
}

void
EBookOptions::setScriptItalic(bool script_italic)
{
  m_script_italic = script_italic;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::scriptWeight() const
{
  return m_script_weight;
}

void
EBookOptions::setScriptWeight(const QFont::Weight& script_weight)
{
  m_script_weight = script_weight;
  m_pref_changed = true;
}

QColor
EBookOptions::styleColor() const
{
  return m_style_color;
}

void
EBookOptions::setStyleColor(const QColor& style_color)
{
  m_style_color = style_color;
  m_pref_changed = true;
}

QColor
EBookOptions::styleBack() const
{
  return m_style_back;
}

void
EBookOptions::setStyleBack(const QColor& style_back)
{
  m_style_back = style_back;
  m_pref_changed = true;
}

bool
EBookOptions::styleItalic() const
{
  return m_style_italic;
}

void
EBookOptions::setStyleItalic(bool style_italic)
{
  m_style_italic = style_italic;
  m_pref_changed = true;
}

QFont::Weight
EBookOptions::styleWeight() const
{
  return m_style_weight;
}

void
EBookOptions::setStyleWeight(const QFont::Weight& style_weight)
{
  m_style_weight = style_weight;
  m_pref_changed = true;
}

//QString
//EBookOptions::homeDir() const
//{
//  return m_home_directory;
//}

//void
//EBookOptions::setHomeDir(const QString& home_directiory)
//{
//  m_home_directory = home_directiory;
//}

//QString
//EBookOptions::libraryDir() const
//{
//  return m_libraryDirectory;
//}

//void
//EBookOptions::setLibraryDir(const QString& libraryDirectory)
//{
//  m_libraryDirectory = libraryDirectory;
//}

//QString
//EBookOptions::configDir() const
//{
//  return m_config_directory;
//}

//void
//EBookOptions::setConfigDir(const QString& config_directory)
//{
//  m_config_directory = config_directory;
//}

//QString
//EBookOptions::configFile() const
//{
//  return m_config_file;
//}

//void
//EBookOptions::setConfigFile(const QString& config_file)
//{
//  m_config_file = config_file;
//}

//QString
//EBookOptions::libraryFile() const
//{
//  return m_lib_file;
//}

//void
//EBookOptions::setLibraryFile(const QString& lib_file)
//{
//  m_lib_file = lib_file;
//}

//QString
//EBookOptions::authorsFile() const
//{
//  return m_authors_file;
//}

//void
//EBookOptions::setAuthorsFile(const QString& authors_file)
//{
//  m_authors_file = authors_file;
//}

//QString
//EBookOptions::dicDir() const
//{
//  return m_dic_directory;
//}

//void
//EBookOptions::setDicDir(const QString& dic_dir)
//{
//  m_dic_directory = dic_dir;
//}

//QString
//EBookOptions::bdicDir() const
//{
//  return m_bdic_directory;
//}

//void
//EBookOptions::setBdicDir(const QString& dic_dir)
//{
//  m_bdic_directory = dic_dir;
//}

//QString
//EBookOptions::seriesFile() const
//{
//  return m_series_file;
//}

//void
//EBookOptions::setSeriesFile(const QString& seriesFile)
//{
//  m_series_file = seriesFile;
//}

QRect
EBookOptions::rect() const
{
  return m_rect;
}

void
EBookOptions::setRect(const QRect& rect)
{
  m_rect = rect;
  m_pref_changed = true;
}

QSize
EBookOptions::optionsDlgSize() const
{
  return m_options_dlg_size;
}

void
EBookOptions::setOptionsDlgSize(const QSize& options_dlg)
{
  m_options_dlg_size = options_dlg;
  m_pref_changed = true;
}

int
EBookOptions::currentIndex() const
{
  return m_currentindex;
}

void
EBookOptions::setCurrentIndex(const int index)
{
  m_currentindex = index;
  m_pref_changed = true;
}

bool
EBookOptions::currentFilesContains(const QString filename)
{
  return m_current_files.contains(filename);
}

void
EBookOptions::appendCurrentFile(const QString filename)
{
  if (!m_current_files.contains(filename)) {
    m_current_files.append(filename);
    m_pref_changed = true;
  }
}

void
EBookOptions::replaceCurrentFile(const QString filename)
{
  m_current_files.replace(m_currentindex, filename);
  m_pref_changed = true;
}

QStringList
EBookOptions::currentfiles() const
{
  return m_current_files;
}

int
EBookOptions::bookCount() const
{
  return m_current_files.size();
}

EBookOptions::ViewState
EBookOptions::viewState() const
{
  return m_view_state;
}

void
EBookOptions::setViewState(const EBookOptions::ViewState& view_state)
{
  m_view_state = view_state;
  m_pref_changed = true;
}

QColor
EBookOptions::color(const CodeOptions options) const
{
  switch (options) {
    case EBookOptions::NORMAL:
      return m_normal_color;
    case EBookOptions::TAG:
      return m_tag_color;
    case EBookOptions::ATTRIBUTE:
      return m_attribute_color;
    case EBookOptions::ERROR:
      return m_error_color;
    case EBookOptions::STRING:
      return m_string_color;
    case EBookOptions::SCRIPT:
      return m_script_color;
    case EBookOptions::STYLE:
      return m_style_color;
  }
  return QColor();
}

void
EBookOptions::setColor(const CodeOptions options, const QColor color)
{
  switch (options) {
    case EBookOptions::NORMAL:
      m_normal_color = color;
      m_pref_changed = true;
      break;
    case EBookOptions::TAG:
      m_tag_color = color;
      m_pref_changed = true;
      break;
    case EBookOptions::ATTRIBUTE:
      m_attribute_color = color;
      m_pref_changed = true;
      break;
    case EBookOptions::ERROR:
      m_error_color = color;
      m_pref_changed = true;
      break;
    case EBookOptions::STRING:
      m_string_color = color;
      m_pref_changed = true;
      break;
    case EBookOptions::SCRIPT:
      m_script_color = color;
      m_pref_changed = true;
      break;
    case EBookOptions::STYLE:
      m_style_color = color;
      m_pref_changed = true;
      break;
  }
}

QColor
EBookOptions::background(const CodeOptions options) const
{
  switch (options) {
    case EBookOptions::NORMAL:
      return m_normal_back;
    case EBookOptions::TAG:
      return m_tag_back;
    case EBookOptions::ATTRIBUTE:
      return m_attribute_back;
    case EBookOptions::ERROR:
      return m_error_back;
    case EBookOptions::STRING:
      return m_string_back;
    case EBookOptions::SCRIPT:
      return m_script_back;
    case EBookOptions::STYLE:
      return m_style_back;
  }
  return QColor();
}

void
EBookOptions::setBackground(const CodeOptions options, const QColor color)
{
  switch (options) {
    case EBookOptions::NORMAL:
      m_normal_back = color;
      m_pref_changed = true;
      break;
    case EBookOptions::TAG:
      m_tag_back = color;
      m_pref_changed = true;
      break;
    case EBookOptions::ATTRIBUTE:
      m_attribute_back = color;
      m_pref_changed = true;
      break;
    case EBookOptions::ERROR:
      m_error_back = color;
      m_pref_changed = true;
      break;
    case EBookOptions::STRING:
      m_string_back = color;
      m_pref_changed = true;
      break;
    case EBookOptions::SCRIPT:
      m_script_back = color;
      m_pref_changed = true;
      break;
    case EBookOptions::STYLE:
      m_style_back = color;
      m_pref_changed = true;
      break;
  }
}

bool
EBookOptions::italic(const CodeOptions options) const
{
  switch (options) {
    case EBookOptions::NORMAL:
      return m_normal_italic;
    case EBookOptions::TAG:
      return m_tag_italic;
    case EBookOptions::ATTRIBUTE:
      return m_attribute_italic;
    case EBookOptions::ERROR:
      return m_error_italic;
    case EBookOptions::STRING:
      return m_string_italic;
    case EBookOptions::SCRIPT:
      return m_script_italic;
    case EBookOptions::STYLE:
      return m_style_italic;
  }
  return false;
}

void
EBookOptions::setItalic(const CodeOptions options, const bool italic)
{
  switch (options) {
    case EBookOptions::NORMAL:
      m_normal_italic = italic;
      m_pref_changed = true;
      break;
    case EBookOptions::TAG:
      m_tag_italic = italic;
      m_pref_changed = true;
      break;
    case EBookOptions::ATTRIBUTE:
      m_attribute_italic = italic;
      m_pref_changed = true;
      break;
    case EBookOptions::ERROR:
      m_error_italic = italic;
      m_pref_changed = true;
      break;
    case EBookOptions::STRING:
      m_string_italic = italic;
      m_pref_changed = true;
      break;
    case EBookOptions::SCRIPT:
      m_script_italic = italic;
      m_pref_changed = true;
      break;
    case EBookOptions::STYLE:
      m_style_italic = italic;
      m_pref_changed = true;
      break;
  }
}

QFont::Weight
EBookOptions::weight(const CodeOptions options) const
{
  switch (options) {
    case EBookOptions::NORMAL:
      return m_normal_weight;
    case EBookOptions::TAG:
      return m_tag_weight;
    case EBookOptions::ATTRIBUTE:
      return m_attribute_weight;
    case EBookOptions::ERROR:
      return m_error_weight;
    case EBookOptions::STRING:
      return m_string_weight;
    case EBookOptions::SCRIPT:
      return m_script_weight;
    case EBookOptions::STYLE:
      return m_style_weight;
  }
  return QFont::Normal;
}

void
EBookOptions::setWeight(const CodeOptions options, const QFont::Weight weight)
{
  switch (options) {
    case EBookOptions::NORMAL:
      m_normal_weight = weight;
      m_pref_changed = true;
      break;
    case EBookOptions::TAG:
      m_tag_weight = weight;
      m_pref_changed = true;
      break;
    case EBookOptions::ATTRIBUTE:
      m_pref_changed = true;
      m_attribute_weight = weight;
      break;
    case EBookOptions::ERROR:
      m_error_weight = weight;
      m_pref_changed = true;
      break;
    case EBookOptions::STRING:
      m_string_weight = weight;
      m_pref_changed = true;
      break;
    case EBookOptions::SCRIPT:
      m_script_weight = weight;
      m_pref_changed = true;
      break;
    case EBookOptions::STYLE:
      m_style_weight = weight;
      m_pref_changed = true;
      break;
  }
}

QColor
EBookOptions::contrastingColor(const QColor color)
{
  int v = (color.red() + color.green() + color.blue()) / 3 > 127 ? 0 : 255;
  return QColor(v, v, v);
}
