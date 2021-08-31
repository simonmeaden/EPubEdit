#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFormLayout>
#include <QImageReader>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTextEdit>
#include <QUndoView>
#include <QWidget>

#include "forms/metadataform.h"
#include "config.h"

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
  bool saveDocument(const QString& filename = QString());
  bool newDocument();

  bool isLoaded() const;

  QUndoView* undoView();
  QAction* undoAction();
  QAction* redoAction();

signals:
  void sendStatusMessage(const QString& message,
                         int timeout = Config::StatusTimeout);
  void sendLogMessage(const QString& message);

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
};

#endif // EPUBEDIT_H
