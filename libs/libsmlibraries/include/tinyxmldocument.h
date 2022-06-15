#ifndef QTINYXML2H
#define QTINYXML2H

#include "tinyxml2.h"
#include <QFile>
#include <QObject>

class XMLDocument;
class XMLElement;
class XMLAttribute;
class XMLComment;
class XMLText;
class XMLDeclaration;
class XMLUnknown;
class XMLPrinter;

class TinyXmlDocument;

/*!
 * \class TinyXmlDocument tinyxmldocument.cpp tinyxmldocument.h
 * \brief The TinyXmlDocument class is a wrapper around the TinyXml
 * XMLDocument class.
 */
class TinyXmlDocument : public QObject
{
  Q_OBJECT
public:

  explicit TinyXmlDocument(QObject* parent = nullptr);
  ~TinyXmlDocument();

  /*!
    \brief Parse an XML file from a QString.

    \return XML_SUCCESS (0) on success, or an errorID.

    You may optionally pass in the 'nBytes', which is
    the number of bytes which will be parsed. If not
    specified, TinyXML-2 will assume 'xml' points to a
    null terminated string.
  */
  tinyxml2::XMLError parse(const QString& xml, int nBytes = -1);

  /**
    Load an XML file from disk.
    Returns XML_SUCCESS (0) on success, or
    an errorID.
  */
  tinyxml2::XMLError loadFile(const QString& filename);

  /**
    Load an XML file from disk with a QFile.

    Returns XML_SUCCESS (0) on success, or an errorID.
  */
  tinyxml2::XMLError loadFile(const QFile& file);

  /**
    Load an XML file from disk. You are responsible
    for providing and closing the FILE*.

      NOTE: The file should be opened as binary ("rb")
      not text in order for TinyXML-2 to correctly
      do newline normalization.

    Returns XML_SUCCESS (0) on success, or an errorID.
  */
  tinyxml2::XMLError loadFile(QString filename);

  /**
    Save the XML file to disk.

    Returns XML_SUCCESS (0) on success, or an errorID.
  */
  tinyxml2::XMLError saveFile(const QString& filename, bool compact = false);

  /**
    Save the XML file to disk.

    Returns XML_SUCCESS (0) on success, or an errorID.
  */
  tinyxml2::XMLError saveFile(const QFile& file, bool compact = false);

  bool processEntities() const;
  tinyxml2::Whitespace whitespaceMode() const;

  /**
    Returns true if this document has a leading Byte Order Mark of UTF8.
  */
  bool hasBOM() const;
  /** Sets whether to write the BOM when writing the file.
   */
  void setBOM(bool useBOM);

  /** Return the root element of DOM. Equivalent to FirstChildElement().
      To get the first node, use FirstChild().
  */
  tinyxml2::XMLElement* rootElement();
  const tinyxml2::XMLElement* rootElement() const;

  /** Print the Document. If the Printer is not provided, it will
      print to stdout. If you provide Printer, this can print to a file:
    @verbatim
    XMLPrinter printer( fp );
    doc.Print( &printer );
    @endverbatim

    Or you can use a printer to print to memory:
    @verbatim
    XMLPrinter printer;
    doc.Print( &printer );
    // printer.CStr() has a const char* to the XML
    @endverbatim
  */
  void print(tinyxml2::XMLPrinter* streamer = 0) const;

  virtual bool accept(tinyxml2::XMLVisitor* visitor) const;

  /**
    Create a new Element associated with
    this Document. The memory for the Element
    is managed by the Document.
  */
  tinyxml2::XMLElement* newElement(const QString& name);

  /**
    Create a new Comment associated with
    this Document. The memory for the Comment
    is managed by the Document.
  */
  tinyxml2::XMLComment* newComment(const QString& comment);

  /**
    Create a new Text associated with
    this Document. The memory for the Text
    is managed by the Document.
  */
  tinyxml2::XMLText* newText(const QString& text);

  /**
    Create a new Declaration associated with
    this Document. The memory for the object
    is managed by the Document.

    If the 'text' param is null, the standard
    declaration is used.:
    @verbatim
      <?xml version="1.0" encoding="UTF-8"?>
    @endverbatim
  */
  tinyxml2::XMLDeclaration* newDeclaration(const QString& text);

  /**
    Create a new Unknown associated with
    this Document. The memory for the object
    is managed by the Document.
  */
  tinyxml2::XMLUnknown* newUnknown(const QString& text);

  /**
    Delete a node associated with this document.
    It will be unlinked from the DOM.
  */
  void deleteNode(tinyxml2::XMLNode* node);

  /// Clears the error flags.
  void clearError();

  /// Return true if there was an error parsing the document.
  bool error() const;

  /// Return the errorID.
  tinyxml2::XMLError errorID() const;

  const QString errorName() const;

  static const char* errorIDToName(tinyxml2::XMLError errorID);

  /** Returns a "long form" error description. A hopefully helpful
      diagnostic with location, line number, and/or additional info.
  */
  const QString errorStr() const;

  /// A (trivial) utility function that prints the ErrorStr() to stdout.
  void printError() const;

  /// Return the line where the error occurred, or zero if unknown.
  int errorLineNum() const;

  /// Clear the document, resetting it to the initial state.
  void clear();

private:
  tinyxml2::XMLDocument* m_document;
};

#endif // QTINYXML2H
