#ifndef UNSTATISTICAL_H
#define UNSTATISTICAL_H

#include <QMap>
#include <QString>
#include <QStringLiteral>

/*!
  \brief The UNStatisticalCodes class contains the Enlish language
UN Statistical Code list.

  The list of countries or areas contains the names of countries or areas in
alphabetical order, their three-digit numerical codes used for statistical
processing purposes by the Statistics Division of the United Nations
Secretariat, and their three-digit alphabetical codes assigned by the
International Organization for Standardization (ISO).1 In general, this list of
countries or areas includes those countries or areas for which statistical data
are compiled by the Statistics Division of the United Nations Secretariat.

The names of countries or areas refer to their short form used in day-to-day
operations of the United Nations and not necessarily to their official name as
used in formal documents. These names are based on the United Nations
Terminology Database (UNTERM).2 The designations employed and the presentation
of material at this site do not imply the expression of any opinion whatsoever
on the part of the Secretariat of the United Nations concerning the legal status
of any country, territory, city or area or of its authorities, or concerning the
delimitation of its frontiers or boundaries.
 */

class UNStatisticalCodes
{
  class UNStatisticalCodesBase
  {
  public:
    //! \brief Returns a QStringList containing all of the UN statistical named
    //! areas.
    QStringList names();

    //! \brief returns the numerical code for the UN area as an integer value.
    //!
    //! Normally these codes are required as a three character code. For this
    //! use the static numericalCodeString(const QString&) method.
    int m49(const QString& name);

    //! \brief Returns the numerical code for the UN area an a three character
    //! numerical code ("049" rather than an int).
    //!
    //! If you need the code as a numerical int value use numericalCode(const
    //! QString&).
    QString m49AsString(const QString& name);

    //! \brief Returns the three character alpha code for the UN area as a
    //! QString.
    QString alphaCode(const QString& name);

    //! Checks whether the M49 (as an int) is a valid code. Returns true if it
    //! is, otherwise returns false.
    bool isM49Valid(int value);

    //! Checks whether the M49 (as a 3 digit QString) is a valid code. Returns
    //! true if it is, otherwise returns false.
    bool isM49Valid(const QString& value);

    //! Checks whether the alpha 3 value is a valid code. Returns
    //! true if it is, otherwise returns false.
    bool isAlpha3Valid(const QString& value);

    //    static QList<int> M49CODES;
    //    static QStringList ALPHA3CODES;

    virtual QStringList getNames() = 0;
    virtual int regionIndex(const QString& name) = 0;
  }; // end UNStatisticalCodesBase

  class UNStatisticalCodesEnglish : public UNStatisticalCodesBase
  {
  public:
    static QStringList REGIONS;
    static void addCode(QString region);

    int regionIndex(const QString& name) { return REGIONS.indexOf(name); }
    QStringList getNames() { return REGIONS; }
  }; // end UNStatisticalCodesEn

  class UNStatisticalCodesRussian : public UNStatisticalCodesBase
  {
  public:
    static QStringList REGIONS;
    static void addCode(QString region);

    int regionIndex(const QString& name) { return REGIONS.indexOf(name); }
    QStringList getNames() { return REGIONS; }
  }; // end UNStatisticalCodesRus

  class UNStatisticalCodesFrench : public UNStatisticalCodesBase
  {
  public:
    static QStringList REGIONS;
    static void addCode(QString region);

    int regionIndex(const QString& name) { return REGIONS.indexOf(name); }
    QStringList getNames() { return REGIONS; }
  }; // end UNStatisticalCodesRus

  class UNStatisticalCodesSpanish : public UNStatisticalCodesBase
  {
  public:
    static QStringList REGIONS;
    static void addCode(QString region);

    int regionIndex(const QString& name) { return REGIONS.indexOf(name); }
    QStringList getNames() { return REGIONS; }
  }; // end UNStatisticalCodesRus

  class UNStatisticalCodesChinese : public UNStatisticalCodesBase
  {
  public:
    static QStringList REGIONS;
    static void addCode(QString region);

    int regionIndex(const QString& name) { return REGIONS.indexOf(name); }
    QStringList getNames() { return REGIONS; }
  }; // end UNStatisticalCodesRus

  class UNStatisticalCodesArabic : public UNStatisticalCodesBase
  {
  public:
    static QStringList REGIONS;
    static void addCode(QString region);

    int regionIndex(const QString& name) { return REGIONS.indexOf(name); }
    QStringList getNames() { return REGIONS; }
  }; // end UNStatisticalCodesRus

public:
  enum Language
  {
    English,
    Russian,
    Chinese,
    French,
    Spanish,
    Arabic,
  };
  //! \brief Returns a QStringList containing all of the UN statistical named
  //! areas.
  QStringList names(Language lang);

  //! \brief returns the numerical code for the UN area as an integer value.
  //!
  //! Normally these codes are required as a three character code. For this
  //! use the static m49(const QString&) method.
  int m49AsInt(Language lang, const QString& name);

  //! \brief Returns the numerical code for the UN area an a three character
  //! numerical code ("049" rather than an int).
  //!
  //! If you need the code as a numerical int value use m49AsInt(const
  //! QString&).
  QString m49(Language lang, const QString& name);

  //! \brief Returns the three character alpha code for the UN area as a
  //! QString.
  QString alphaCode(Language lang, const QString& name);

  //! Checks whether the M49 (as an int) is a valid code. Returns true if it is,
  //! otherwise returns false.
  bool isM49Valid(Language lang, int value);

  //! Checks whether the M49 (as a 3 digit QString) is a valid code. Returns
  //! true if it is, otherwise returns false.
  bool isM49Valid(Language lang, const QString& value);

  //! Checks whether the alpha 3 value is a valid code. Returns
  //! true if it is, otherwise returns false.
  bool isAlpha3Valid(Language lang, const QString& value);

protected:
private:
  static QList<int> M49CODES;
  static QStringList ALPHA3CODES;

  static UNStatisticalCodesEnglish m_english;
  static UNStatisticalCodesRussian m_russian;
  static UNStatisticalCodesFrench m_french;
  static UNStatisticalCodesSpanish m_spanish;
  static UNStatisticalCodesChinese m_chinese;
  static UNStatisticalCodesArabic m_arabic;

  static void addCodes();
  static void addCode(int m49, const QString& alpha3);
};

#endif // UNSTATISTICAL_H
