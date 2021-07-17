#ifndef EPUBEDITOR_H
#define EPUBEDITOR_H

#include <QWidget>

class EPubDocument;

class EPubEditor : public QWidget
{
  Q_OBJECT
public:
  explicit EPubEditor(QWidget* parent = nullptr);
  ~EPubEditor();

  void setDocument(QSharedPointer<EPubDocument> document);

signals:

private:
  QSharedPointer<EPubDocument> m_document;

  void updateDocument();
};

#endif // EPUBEDITOR_H
