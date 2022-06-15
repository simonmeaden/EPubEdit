#ifndef TIDYDOC_H
#define TIDYDOC_H

#include <QObject>

#include "tidy.h"
#include "tidybuffio.h"

class TidyDocData
{
public:
  TidyDocData() {}
  virtual ~TidyDocData() = default;
};

class TidyDocBoolData : public TidyDocData
{
public:
  TidyDocBoolData(bool value) { data = value; }
  bool data;
};

class TidyDocIntData : public TidyDocData
{
public:
  TidyDocIntData(int value) { data = value; }
  int data;
};

class TidyDocStrData : public TidyDocData
{
public:
  TidyDocStrData(QString value) { data = value; }
  QString data;
};

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

class QTidyDoc : public QObject
{
  Q_OBJECT

public:
  enum Diagnostics
  {
    Good,
    Warning,
    Error,
  };

  QTidyDoc(QObject* parent);
  ~QTidyDoc();

  QString toCleanHtml(const QString& text,
                      QList<TidyDocOption*> options = QList<TidyDocOption*>());
  QString toCleanXHtml(const QString& text,
                       QList<TidyDocOption*> options = QList<TidyDocOption*>());

  QString errors();

  // This is used to convert an object back to the C form
  // so it can be used in the normal C tidy functions.
  operator TidyDoc() const { return m_doc; }

  //  // Need to do this to satisfy rule of 5
  //  // Don't worry about this section yet.
//    QTidyDoc(QTidyDoc const&) = delete;
//    QTidyDoc(QTidyDoc&&) = delete;
//    QTidyDoc& operator=(QTidyDoc const&) = delete;
//    QTidyDoc& operator=(QTidyDoc&&) = delete;

private:
  TidyBuffer m_output;
  TidyBuffer m_errbuf;
  TidyDoc m_doc;
  Diagnostics m_diagnostics;

  bool handleTidyOptions(QList<TidyDocOption *> &options);
};

#endif // TIDYDOC_H
