#ifndef CONFIGURATIONEDITOR_H
#define CONFIGURATIONEDITOR_H

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

#include "forms/config.h"

class ConfigurationEditor : public QDialog
{
  Q_OBJECT
public:
  explicit ConfigurationEditor(Config config, QWidget* parent = nullptr);

  Config config() const;
  bool modified() const;

signals:

private:
  Config m_config;
  bool m_modified;

  void initGui();
  QWidget* initPage1();

  void statusTimeoutChanged(int value);
};

#endif // CONFIGURATIONEDITOR_H
