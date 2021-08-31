#ifndef CONFIGURATIONEDITOR_H
#define CONFIGURATIONEDITOR_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>

class Config;

class ConfigurationEditor : public QDialog
{
  Q_OBJECT
public:
  explicit ConfigurationEditor(Config* config, QWidget* parent = nullptr);

  Config* config() const;

signals:

private:
  Config* m_config;

  void initGui();
  QWidget* initPage1();
};

#endif // CONFIGURATIONEDITOR_H
