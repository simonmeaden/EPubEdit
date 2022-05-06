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

#include "document/bookpointers.h"

class ConfigurationEditor : public QDialog
{
  Q_OBJECT
public:
  explicit ConfigurationEditor(PConfig config, QWidget* parent = nullptr);

  PConfig config() const;

signals:

private:
  PConfig m_config;

  void initGui();
  QWidget* initPage1();
};

#endif // CONFIGURATIONEDITOR_H
