#ifndef LNSETTINGSDIALOG_H
#define LNSETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QGridLayout>


/*!
   \file lnsettingsdialog.h "widgets/lnsettingsdialog.h"
   \class LNSettingsDialog lnsettingsdialog.h
   \brief Creates a settings dialog

   \since 5.7.0
   \license{The MIT License}
   \copyright © 2022 Simon Meaden. All rights reserved.
   \see LNPlainTextEdit
*/
class LNSettingsDialog : public QDialog
{
  Q_OBJECT
public:
  LNSettingsDialog(QWidget* parent = nullptr);

  int addTab(QWidget* page, const QString& label);
  int addTab(QWidget* page, const QIcon& icon, const QString& label);
  int insertTab(int index, QWidget* page, const QString& label);
  int insertTab(int index,
                QWidget* page,
                const QIcon& icon,
                const QString& label);
  bool 	isTabEnabled(int index) const;
  bool 	isTabVisible(int index) const;
  void 	removeTab(int index);

protected:
  QTabWidget* m_tabs;

private:
  QWidget *initMainSettings();
};

#endif // LNSETTINGSDIALOG_H
