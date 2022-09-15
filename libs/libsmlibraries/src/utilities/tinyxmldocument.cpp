#include "utilities/tinyxmldocument.h"

//====================================================================
//=== TinyXmlDocument
//====================================================================
TinyXmlDocument::TinyXmlDocument(QObject* parent)
  : QObject{ parent }
{
  m_document = new tinyxml2::XMLDocument();
}

TinyXmlDocument::~TinyXmlDocument()
{
  delete m_document;
}

tinyxml2::XMLError
TinyXmlDocument::parse(const QString& xml, int nBytes)
{
  return m_document->Parse(xml.toLatin1().data(), nBytes);
}

tinyxml2::XMLError
TinyXmlDocument::loadFile(const QString& filename)
{
  return m_document->LoadFile(filename.toLatin1().data());
}

tinyxml2::XMLError
TinyXmlDocument::loadFile(const QFile& file)
{
  auto f = fopen(file.fileName().toLatin1().data(), "rb");
  return m_document->LoadFile(f);
}

tinyxml2::XMLError
TinyXmlDocument::loadFile(QString filename)
{
  return m_document->LoadFile(filename.toLatin1().data());
}

tinyxml2::XMLError
TinyXmlDocument::saveFile(const QString& filename, bool compact)
{
  return m_document->SaveFile(filename.toLatin1().data(), compact);
}

tinyxml2::XMLError
TinyXmlDocument::saveFile(const QFile& file, bool compact)
{
  auto f = fopen(file.fileName().toLatin1().data(), "rb");
  return m_document->SaveFile(f, compact);
}

bool
TinyXmlDocument::processEntities() const
{
  return m_document->ProcessEntities();
}

tinyxml2::Whitespace
TinyXmlDocument::whitespaceMode() const
{
  return m_document->WhitespaceMode();
}

bool
TinyXmlDocument::hasBOM() const
{
  return m_document->HasBOM();
}

void
TinyXmlDocument::setBOM(bool useBOM)
{
  m_document->SetBOM(useBOM);
}

tinyxml2::XMLElement*
TinyXmlDocument::rootElement()
{
  return m_document->RootElement();
}

const tinyxml2::XMLElement*
TinyXmlDocument::rootElement() const
{
  return m_document->RootElement();
}

void
TinyXmlDocument::print(tinyxml2::XMLPrinter* streamer) const
{
  // TODO
  m_document->Print(streamer);
}

bool
TinyXmlDocument::accept(tinyxml2::XMLVisitor* visitor) const
{
  // TODO
  return m_document->Accept(visitor);
}

tinyxml2::XMLElement*
TinyXmlDocument::newElement(const QString& name)
{
  return m_document->NewElement(name.toLatin1().data());
}

tinyxml2::XMLComment*
TinyXmlDocument::newComment(const QString& comment)
{
  return m_document->NewComment(comment.toLatin1().data());
}

tinyxml2::XMLText*
TinyXmlDocument::newText(const QString& text)
{
  return m_document->NewText(text.toLatin1().data());
}

tinyxml2::XMLDeclaration*
TinyXmlDocument::newDeclaration(const QString& text)
{
  return m_document->NewDeclaration(text.toLatin1().data());
}

tinyxml2::XMLUnknown*
TinyXmlDocument::newUnknown(const QString& text)
{
  return m_document->NewUnknown(text.toLatin1().data());
}

void
TinyXmlDocument::deleteNode(tinyxml2::XMLNode* node)
{
  m_document->DeleteNode(node);
}

void
TinyXmlDocument::clearError()
{
  m_document->ClearError();
}

bool
TinyXmlDocument::error() const
{
  return m_document->Error();
}

tinyxml2::XMLError
TinyXmlDocument::errorID() const
{
  return m_document->ErrorID();
}

const QString
TinyXmlDocument::errorName() const
{
  return QString(m_document->ErrorName());
}

const char*
TinyXmlDocument::errorIDToName(tinyxml2::XMLError errorID)
{
  return tinyxml2::XMLDocument::ErrorIDToName(tinyxml2::XMLError(errorID));
}

const QString
TinyXmlDocument::errorStr() const
{
  return QString(m_document->ErrorStr());
}

void
TinyXmlDocument::printError() const
{
  m_document->PrintError();
}

int
TinyXmlDocument::errorLineNum() const
{
  return m_document->ErrorLineNum();
}

void
TinyXmlDocument::clear()
{
  return m_document->Clear();
}
