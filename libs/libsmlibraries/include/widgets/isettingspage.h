#ifndef ISETTINGSPAGE_H
#define ISETTINGSPAGE_H

#include <QDialog>
#include <QGridLayout>
#include <QTabWidget>
#include <QWidget>

class SettingsWidget;

class ISettings
{
public:
  virtual bool isModified() const = 0;
};

class ISettingsWidget
{
public:
  virtual bool isModified() const = 0;
  virtual SettingsWidget* settingsPage() = 0;
  virtual void setSettingsPage(SettingsWidget* widget) = 0;
};

//!
class SettingsWidget
  : public QWidget
  , public ISettings
{
  Q_OBJECT
public:
  SettingsWidget(QWidget* parent);

protected:
  QGridLayout* m_layout;
};

//class ISettingsDialog
//  : public QDialog
//  , public ISettingsWidget
//{
//public:
//  ISettingsDialog(QWidget* parent)
//    : QDialog(parent)
//  {
//  }
//};

//! An extension of QTabWidget that is used to create setting/options
//! pages.
class SettingsTabWidget : public QTabWidget
{
  Q_OBJECT
public:
  //! Constructor for SettingsTabWidget
  SettingsTabWidget(QWidget* parent);

  //! Returns the SettingsWidget at index.
  SettingsWidget* widget(int index) const;
};

#endif // ISETTINGSPAGE_H
