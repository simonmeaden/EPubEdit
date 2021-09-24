#ifndef SIMPLEHELPDIALOG_H
#define SIMPLEHELPDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QWidget>

// clang-format off
/*!
 * \file helpdialogs.h
 *
 * This module holds a few simple help dialogs for when you only need a
 * simple dialog to show a small amount of help and don not require the
 * power of an integrated help system.
 *
 * All help text is displayed in a QTextEdit so anythig text that can be
 * added to a QTextEdit can be used, for instance html tags or Markdown.
 * By default the dialogs expect text to be in Markdown format. If you need
 * to use HTML then you will need to set the Typw flags to HTML rather than
 * MARKDOWN the default.
 *
 * [The supported subset of HTML can be found](https://doc.qt.io/qt-5/richtext-html-subset.html)
 * [The supported set of Markdown can be found](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
 *
 * - SimpleHelpDialog
 *   Shows a single page of text.
 * - ExtendedHelpDialog
 *   Shows a single page of text, with a hidden page of extra text displayed
 *   via a 'More/Less' button.
 * - ExtendedHelpWithTabsDialog
 *   Shows a single page of text, with one or more hidden page of extra text
 *   displayed via a 'More/Less' button. Pages are store in a QTabWidget so
 * pages can be titled. This is suitable if you only have a few extra pages.
 * - ExtendedHelpWithComboDialog
 *   Shows a single page of text, with one or more hidden page of extra text
 *   displayed via a 'More/Less' button. Pages are store in a stack with a
 *   controlling combo box which shows a list of available page descriptions.
 *   This is suitable if you only have more than a few extra pages.
 *
 * - Primary help is set using the setHelpText(QString) method.
 * - Secondary page(s) are set using the setAdditionalHelpText(QString, Type) for
 *   ExtendedHelpDialog and addAdditionalHelpPage(QString,QString, Type) for the
 *   multi-page versions.
 *
 *   \note The dialogs are non-resizable, however they attempt to resize appropriately
 *         for the supplied text length.
 */
// clang-format on

//! \brief Base Help Dialog class.
class BaseHelpDialog : public QDialog
{
public:
  //! \brief Defines whether HTML or Markdown fromatting is used.
  //! The default is MARKDOWN.
  enum Type
  {
    PLAIN,    //!< Text is plain text.
    HTML,     //!< HTML tags are used.
    MARKDOWN, //!< Markdown is used.
  };
  BaseHelpDialog(QWidget* parent)
    : QDialog(parent)
  {}

  //! Sets the text in the primary help window
  void setHelpText(const QString& text, Type type = MARKDOWN);

//  QSize sizeHint() const override {return QSize(1000, 500);}

protected:
  QTextEdit* m_primaryLbl;

  int calculateMinimumWidth(const QString&text);
};

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
class SimpleHelpDialog : public BaseHelpDialog
{
  Q_OBJECT
public:
  SimpleHelpDialog(const QString& title = QString(), QWidget* parent = nullptr);

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
  dlg->setHelpText(
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
class ExtendedHelpDialog : public BaseHelpDialog
{
  Q_OBJECT
public:
  ExtendedHelpDialog(const QString& title = QString(),
                     QWidget* parent = nullptr);

  //! Sets the text in the additional help window
  void setAdditionalHelpText(const QString& text, Type type = MARKDOWN);

private:
  QPushButton* m_moreBtn;
  QTextEdit* m_extended;

  void more();
};

/*!
  \class ExtendedHelpWithTabsDialog helpdialogs.h
  \brief An help window that displays a more extended help dialog, with a
         tabbed extension page.

  The dialog displays the help string and holds a 'Close' button, however
  a 'More' button is also displayed and this will cause a hidden tabbed area,
  in which more help pages can be stored, to appear. The 'More' button will
  now read 'Less'.

  The 'Close' button returns no value is returned so use the show()
  method rather that exec() method to invoke the dialog.

  \code
  auto dlg = new ExtendedHelpWithTabsDialog(this);
  dlg->setPrimaryHelpText(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->setHelpText(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->addAdditionalHelpPage(
    tr("More help text can be added here in the hidden area.\n"
       "This will not appear until the 'More' button is clicked.\n"
       "Clicking the button, now displaying 'Less' will cause the\n"
       "hidden area to disappear again, returning the button text to 'More'\n"
       "Multiline can bedisplayed simply be adding newline characters."
    );
    // More pages can be added here.
  );
  dlg->show();
  \endcode

  If you require a large number of extra pages you can use the
  ExtendedHelpWithComboDialog dialog in stead.
 */
class ExtendedHelpWithTabsDialog : public BaseHelpDialog
{
  Q_OBJECT
public:
  ExtendedHelpWithTabsDialog(const QString& title = QString(),
                             QWidget* parent = nullptr);

  //! Sets the text in the additional help window
  void addAdditionalHelpPage(const QString& title,
                             const QString& text,
                             Type type = MARKDOWN);

private:
  QTabWidget* m_tabs;
  QPushButton* m_moreBtn;
  void more();
};

/*!
  \class ExtendedHelpWithComboDialog helpdialogs.h
  \brief An help window that displays a more extended help dialog, with a
         combo box extension page.

  The dialog displays the help string and holds a 'Close' button, however
  a 'More' button is also displayed and this will cause a hidden area,
  in which more help pages can be stored, to appear. The 'More' button will
  now read 'Less'. Pages are chosen via the combo box.

  The 'Close' button returns no value is returned so use the show()
  method rather that exec() method to invoke the dialog.

  \code
  auto dlg = new ExtendedHelpWithTabsDialog(this);
  dlg->setPrimaryHelpText(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->setPrimaryHelpText(
    tr("This text can be used to display a simple\n"
       "text help message. Multiline can bedisplayed\n"
       "simply be adding newline characters."
    );
  dlg->addAdditionalHelpPage(
    tr("More help text can be added here in the hidden area.\n"
       "This will not appear until the 'More' button is clicked.\n"
       "Clicking the button, now displaying 'Less' will cause the\n"
       "hidden area to disappear again, returning the button text to 'More'\n"
       "Multiline can bedisplayed simply be adding newline characters."
    );
    // More pages can be added here.
  );
  dlg->show();
  \endcode

  If you only require a few pages the ExtendedHelpWithTabsDialog dialog
  might be preferable.
 */
class ExtendedHelpWithComboDialog : public BaseHelpDialog
{
  Q_OBJECT
public:
  ExtendedHelpWithComboDialog(const QString& title = QString(),
                              QWidget* parent = nullptr);

  //! Sets the text in the additional help window
  void addAdditionalHelpPage(const QString& title,
                             const QString& text,
                             Type type = MARKDOWN);

private:
  QComboBox* m_box;
  QFrame* m_extension;
  QStackedLayout* m_stack;
  QPushButton* m_moreBtn;

  void more();
  void changePage(int page);
};

#endif // SIMPLEHELPDIALOG_H
