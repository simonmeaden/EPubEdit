#ifndef EPUBEDITOR_H
#define EPUBEDITOR_H

#include <QWidget>

class EPubDocument;
class Config;

class EPubEditor : public QWidget
{
  Q_OBJECT
public:
  explicit EPubEditor(Config* config, QWidget* parent = nullptr);
  ~EPubEditor();

  void setDocument(QSharedPointer<EPubDocument> document);

signals:

private:
  Config* m_config;
  QSharedPointer<EPubDocument> m_document;

  void updateDocument();
};

#endif // EPUBEDITOR_H
