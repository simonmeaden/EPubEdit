#ifndef SIMPLEHELPDIALOG_H
#define SIMPLEHELPDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QTabWidget>

/*!
  \class SimpleHelpDialog helpdialogs.h
  \brief An help window that displays a simple help dialog.

  The dialog simply displays the help string and a 'Close' button.
  The 'Close' button returns no value is returned so use the show()
  method rather that exec() method to invoke the dialog.

  \code
  auto dlg = new SimpleDialog(this);
  dlg->setHelptext(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  );
  dlg->show();
  \endcode
 */
class SimpleHelpDialog : public QDialog
{
  Q_OBJECT
public:
  SimpleHelpDialog(const QString& title = QString(), QWidget* parent = nullptr);

  void setHelpText(const QString& text);

private:
  QLabel* m_primaryLbl;
};

/*!
  \class ExtendedHelpDialog helpdialogs.h
  \brief An help window that displays a slightly extended help dialog.

  The dialog displays the help string and holds a 'Close' button, however
  a 'More' button is also displayed and this will cause a hidden area,
  in which more help can be stored, to appear. The 'More' button will
  now read 'Less'.

  The 'Close' button returns no value is returned so use the show()
  method rather that exec() method to invoke the dialog.

  \code
  auto dlg = new ExtendedHelpDialog(this);
  dlg->setHelptext(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->setPrimaryHelpText(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->setAdditionalHelpText(
    tr("More help text can be added here in the hidden area.\n"
       "This will not appear until the 'More' button is clicked.\n"
       "Clicking the button, now displaying 'Less' will cause the\n"
       "hidden area to disappear again, returning the button text to 'More'\n"
       "Multiline can bedisplayed simply be adding newline characters."
    );
  );
  dlg->show();
  \endcode
 */
class ExtendedHelpDialog : public QDialog
{
  Q_OBJECT
public:
  ExtendedHelpDialog(const QString& title = QString(), QWidget* parent = nullptr);

  //! Sets the text in the primary help window
  void setPrimaryHelpText(const QString& text);
  //! Sets the text in the additional help window
  void setAdditionalHelpText(const QString& text);

private:
  QWidget* m_extension;
  QPushButton* m_moreBtn;
  QLabel *m_primaryLbl, *m_extendedLbl;

  void more();
};

/*!
  \class ExtendedHelpWithTabsDialog helpdialogs.h
  \brief An help window that displays a more extended help dialog.

  The dialog displays the help string and holds a 'Close' button, however
  a 'More' button is also displayed and this will cause a hidden tabbed area,
  in which more help pages can be stored, to appear. The 'More' button will
  now read 'Less'.

  The 'Close' button returns no value is returned so use the show()
  method rather that exec() method to invoke the dialog.

  \code
  auto dlg = new ExtendedHelpDialog(this);
  dlg->setHelptext(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->setPrimaryHelpText(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->setAdditionalHelpText(
    tr("More help text can be added here in the hidden area.\n"
       "This will not appear until the 'More' button is clicked.\n"
       "Clicking the button, now displaying 'Less' will cause the\n"
       "hidden area to disappear again, returning the button text to 'More'\n"
       "Multiline can bedisplayed simply be adding newline characters."
    );
  );
  dlg->show();
  \endcode
 */
class ExtendedHelpWithTabsDialog : public QDialog
{
  Q_OBJECT
public:
  ExtendedHelpWithTabsDialog(const QString& title = QString(), QWidget* parent = nullptr);

  //! Sets the text in the primary help window
  void setPrimaryHelpText(const QString& text);
  //! Sets the text in the additional help window
  void addAdditionalHelpPage(const QString &title, const QString& text);

private:
  QTabWidget* m_tabs;
  QPushButton* m_moreBtn;
  QLabel *m_primaryLbl, *m_extendedLbl;

  void more();
};

#endif // SIMPLEHELPDIALOG_H
