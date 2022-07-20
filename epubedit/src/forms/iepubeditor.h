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
  explicit IEPubEditor(/*QAbstractScrollArea* editor*/) {}

  virtual QTextDocument* document() { return nullptr; };
  virtual QTextCursor currentCursor() { return QTextCursor(); };

  virtual const QString& href() const { return QString(); };
  virtual void loadHref(const QString& href){};

  virtual void setText(const QString& text) {}

private:
};

#endif // IEPUBEDITOR_H
