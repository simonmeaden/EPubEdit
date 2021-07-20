#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFormLayout>
#include <QImageReader>
#include <QLineEdit>
#include <QSharedPointer>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTextEdit>
#include <QUndoView>
#include <QWidget>

#include "forms/configurationeditor.h"
#include "forms/metadataform.h"

class EPubEditor;
class EPubDocument;

class EPubEdit : public QWidget
{
  Q_OBJECT

public:
  explicit EPubEdit(Config* config, QWidget* parent = nullptr);
  ~EPubEdit();

  void setConfig(Config* config);
  void saveConfig(const QString& filename = QString());

  bool loadDocument(const QString& filename);
  bool newDocument();

  bool isLoaded() const;

  QUndoView* getUndoView();
  QAction* getUndoAction();
  QAction* getRedoAction();

signals:
  void sendStatusMessage(const QString& message, int timeout = 20);

private:
  QUndoStack* m_undoStack;
  QUndoView* m_undoView;
  Config* m_config;
  QTabWidget* m_tabs;
  MetadataForm* m_metadataForm;
  EPubEditor* m_editor;
  QSharedPointer<EPubDocument> m_document;
  bool m_loaded;

  void initGui();

  void loadConfig(const QString& filename = QString());

  void updateMetadataForm();
  void metadataHasChanged(MetadataForm::Modifications modifications);

  static const QString STATUS_TIMEOUT;
};

#endif // EPUBEDIT_H
