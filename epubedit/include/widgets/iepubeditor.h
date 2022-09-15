#ifndef IEPUBEDITOR_H
#define IEPUBEDITOR_H

#include <QAbstractScrollArea>
#include <QTextCursor>
#include <QTextDocument>

/*!
 * \class IEPubEditor iepubeditor.h "includes/forms/iepubeditor.h"
 * \brief Defines an interface for methods that all EPUB editors will require.
 *
 * This allows the EPubEditor class to handle a few different file types. By
 * default all the getter methods return either null pointers or invalid empty
 * values so only implement those you require. For example the ImageEditor
 * does not handle the setText(const QString&), currentCursor() or document()
 * methods.
 *
 * \sa EPubEdit - EPub text editor.
 * \sa CodeEdit - EPub html code editor
 * \sa ImageEdit - Simple image editor.
 */
class IEPubEditor
{
public:
  enum Type
  {
    Text,
    Code,
    Image,
  };
  explicit IEPubEditor() {}

  virtual QTextDocument* document() { return nullptr; };

  virtual QTextCursor currentCursor() { return QTextCursor(); };
  virtual void setCurrentCursor(const QTextCursor& /*cursor*/){};

  virtual const QString href() const { return QString(); };
  virtual void loadHref(const QString& href) = 0;

  virtual void setText(const QString& /*text*/){};

  virtual void focusInEvent(QFocusEvent* event) = 0;
  virtual void focusOutEvent(QFocusEvent* event) = 0;
  virtual void mousePressEvent(QMouseEvent* event) = 0;

  Type type() const;
  void setType(Type type);

protected:
  Type m_type;

private:
};

inline IEPubEditor::Type
IEPubEditor::type() const
{
  return m_type;
}

inline void
IEPubEditor::setType(IEPubEditor::Type type)
{
  m_type = type;
}

#endif // IEPUBEDITOR_H
