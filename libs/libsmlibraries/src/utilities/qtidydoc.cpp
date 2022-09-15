#include "utilities/qtidydoc.h"

QTidyDoc::QTidyDoc(QObject* parent)
  : QObject(parent)
{
  m_output = { 0 };
  m_errbuf = { 0 };
  m_doc = tidyCreate(); // Initialize "document"
}

QTidyDoc::~QTidyDoc()
{
  tidyBufFree(&m_output);
  tidyBufFree(&m_errbuf);
  tidyRelease(m_doc);
}

bool
QTidyDoc::handleTidyOptions(QList<TidyDocOption*> &options)
{
  bool ok = false;

  for (auto option : options) {
    auto d = dynamic_cast<TidyDocBoolData*>(option->data);
    if (d) {
      if (d->data)
        ok = tidyOptSetBool(m_doc, TidyOptionId(option->id), yes);
      else
        ok = tidyOptSetBool(m_doc, TidyOptionId(option->id), no);
      continue;
    }
    auto i = dynamic_cast<TidyDocIntData*>(option->data);
    if (i) {
      ok = tidyOptSetInt(m_doc, TidyOptionId(option->id), i->data);
      continue;
    }
    auto c = dynamic_cast<TidyDocStrData*>(option->data);
    if (c) {
      ok = tidyOptSetValue(m_doc,
                           TidyOptionId(option->id),
                           QString(c->data).toLatin1().data());
      continue;
    }
  }
  return ok;
}

QString
QTidyDoc::toCleanHtml(const QString& text, QList<TidyDocOption*> options)
{
  auto rc = -1;
  // convert QString to c string
  auto str = text.toStdString();
  auto c_str = str.c_str();
  options.prepend(new TidyDocOption(TidyXhtmlOut, new TidyDocBoolData(true)));

  auto ok = handleTidyOptions(options);

  if (ok)
    rc = tidySetErrorBuffer(m_doc, &m_errbuf); // Capture diagnostics

  if (rc >= 0)
    rc = tidyParseString(m_doc, c_str); // Parse the input

  if (rc >= 0)
    rc = tidyCleanAndRepair(m_doc); // Tidy it up!

  if (rc >= 0)
    rc = tidyRunDiagnostics(m_doc); // handle problems

  if (rc > 1) // If error, force output.
    rc = (tidyOptSetBool(m_doc, TidyForceOutput, yes) ? rc : -1);

  if (rc >= 0)
    rc = tidySaveBuffer(m_doc, &m_output); // Pretty Print

  if (rc == 1) {
    m_diagnostics = Warning;
  } else if (rc == 2) {
    m_diagnostics = Error;
  }

  // convert result back to QString.
  auto outstr{ (char*)(m_output.bp) };
  return QString(outstr);
}

QString
QTidyDoc::toCleanXHtml(const QString& text, QList<TidyDocOption *> options)
{
  auto rc = -1;
  // convert QString to c string
  auto str = text.toStdString();
  auto c_str = str.c_str();
  options.prepend(new TidyDocOption(TidyHtmlOut, new TidyDocBoolData(true)));
  options.prepend(new TidyDocOption(TidyWrapLen, new TidyDocIntData(0)));

  auto ok = handleTidyOptions(options);

  if (ok)
    rc = tidySetErrorBuffer(m_doc, &m_errbuf); // Capture diagnostics

  if (rc >= 0)
    rc = tidyParseString(m_doc, c_str); // Parse the input

  if (rc >= 0)
    rc = tidyCleanAndRepair(m_doc); // Tidy it up!

  if (rc >= 0)
    rc = tidyRunDiagnostics(m_doc); // handle problems

  if (rc > 1) // If error, force output.
    rc = (tidyOptSetBool(m_doc, TidyForceOutput, yes) ? rc : -1);

  if (rc >= 0)
    rc = tidySaveBuffer(m_doc, &m_output); // Pretty Print

  if (rc == 1) {
    m_diagnostics = Warning;
  } else if (rc == 2) {
    m_diagnostics = Error;
  }

  // convert result back to QString.
  auto outstr{ (char*)(m_output.bp) };
  auto output = QString(outstr);
  return output;
}

QString
QTidyDoc::errors()
{
  auto errStr{ (char*)(m_errbuf.bp) };
  auto errOut = QString(errStr);
  auto output = QString("%1 : %2")
                  .arg(m_diagnostics == Warning ? tr("Warning")
                       : m_diagnostics == Error ? tr("Error")
                                                : tr("Good"))
                  .arg(errOut);
  return output;
}
