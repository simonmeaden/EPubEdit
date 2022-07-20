#ifndef TIDYDOC_H
#define TIDYDOC_H

#include <QObject>

#include "tidy.h"
#include "tidybuffio.h"

/*!
 * \class TidyDocData qtidydoc.h "includes/qtidydoc.h"
 * \brief The TidyDocData class is the base class for TidyDoc data wrapper classes.
 */
class TidyDocData
{
public:
  TidyDocData() {}
  virtual ~TidyDocData() = default;
};

/*!
 * \class TidyDocBoolData qtidydoc.h "includes/qtidydoc.h"
 * \brief The TidyDocBoolData class is a simple wrapper class around TidyDoc boolean data.
 */
class TidyDocBoolData : public TidyDocData
{
public:
  TidyDocBoolData(bool value) { data = value; }
  bool data;
};

/*!
 * \class TidyDocIntData qtidydoc.h "includes/qtidydoc.h"
 * \brief The TidyDocIntData class is a simple wrapper class around TidyDoc boolean data.
 */
class TidyDocIntData : public TidyDocData
{
public:
  TidyDocIntData(int value) { data = value; }
  int data;
};

/*!
 * \class TidyDocStrData qtidydoc.h "includes/qtidydoc.h"
 * \brief The TidyDocStrData class is a simple wrapper class around TidyDoc string data.
 */
class TidyDocStrData : public TidyDocData
{
public:
  TidyDocStrData(QString value) { data = value; }
  QString data;
};

/*!
 * \class TidyDocOption qtidydoc.h "includes/qtidydoc.h"
 * \brief The TidyDocOption class is a wrapper class allowing users to pass a list
 * of TidyDoc TidyOptionId and data to the TidyDoc clean functions.
 */
struct TidyDocOption
{
  TidyDocOption(TidyOptionId id, TidyDocData* data)
  {
    this->id = id;
    this->data = data;
  }
  TidyOptionId id;
  TidyDocData* data;
};

/*!
 * \class QTidyDoc qtidydoc.h "includes/qtidydoc.h"
 * \brief The QTidyDoc class is a simple wrapper class around TidyDoc.
 *
 * \code
 *    // Create a QTidyDoc
 *    auto doc = QTidyDoc(this);
 *    // Create the option list
 *    QList<TidyDocOption*> options;
 *    // Add a TidyDocOption with a TidyOptionId and data, in this case a string
 *    options.prepend(new TidyDocOption(TidyDoctype, new TidyDocStrData("omit")));
 *    // Clean the html/xhtml to xhtml
 *    auto cleaned = doc.toCleanXHtml(input, options);
 *
 * \endcode
 *
 * <a href="https://api.html-tidy.org/tidy/quickref_5.8.0.html#DocumentIOHeader">
 * TidyDoc 5.8.0 Documentation.
 * </a>
 */
class QTidyDoc : public QObject
{
  Q_OBJECT

  enum Diagnostics
  {
    Good,
    Warning,
    Error,
  };

public:

  /*!
   * \brief Constructs a QTidyDoc which is a child of parent.
   */
  QTidyDoc(QObject* parent);
  //!< Destroys the QTidyDoc, deleting all C internal objects.
  ~QTidyDoc();

  /*!
   * \brief toCleanHtml uses TidyDoc to clean the input text and return it as an html 5 QString.
   */
  QString toCleanHtml(const QString& text,
                      QList<TidyDocOption*> options = QList<TidyDocOption*>());
  /*!
   * \brief toCleanXHtml uses TidyDoc to clean the input text and return it as a xhtml QString.
   */
  QString toCleanXHtml(const QString& text,
                       QList<TidyDocOption*> options = QList<TidyDocOption*>());

  //!< returns the TidyError as a QString
  QString errors();

  /*!
   *  This is used to convert an object back to the C form so it can be used
   *  in the normal C tidy functions.
   */
  operator TidyDoc() const { return m_doc; }

private:
  TidyBuffer m_output;
  TidyBuffer m_errbuf;
  TidyDoc m_doc;
  Diagnostics m_diagnostics;

  bool handleTidyOptions(QList<TidyDocOption*>& options);
};

#endif // TIDYDOC_H
