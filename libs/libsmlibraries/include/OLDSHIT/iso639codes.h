#ifndef ISO639CODES_H
#define ISO639CODES_H

#include <QString>
#include <QStringList>
/// \cond DO_NOT_DOCUMENT
class ISO639Codes
{
  class RegionalNames
  {
  public:
    QStringList getNames() { return NAMES; }
    int regionIndex(const QString& name) { return NAMES.indexOf(name); }
    void addCode(const QString& name);

  protected:
    QStringList NAMES;
  };

public:
  //! Supported languages. These are the only languages shown
  //! on the US local gov website.
  enum Language
  {
    English, //!< English language names
             //    Russian, //!< Russian language names
             //    Chinese, //!< Chinese language names
    French,  //!< French language names
             //    Spanish, //!< Spanish language names
             //    Arabic,  //!< Arabic language names
    German,  //!< German language names
  };

  ISO639Codes();

private:
  QStringList ISO639_1_CODES;
  QStringList ISO639_2_CODES;

  RegionalNames m_english;
  RegionalNames m_french;
  RegionalNames m_german;

  void addCodes() {
      addCode("aar", "aa", "Afar", "afar", "Danakil-Sprache");
      addCode("abk", "ab", "Abkhazian", "abkhaze", "Abchasisch");
      addCode("ace", "", "Achinese", "aceh", "Aceh-Sprache");
      addCode("ach", "", "Acoli", "acoli", "Acholi-Sprache");
      addCode("ada", "", "Adangme", "adangme", "Adangme-Sprache");
      addCode("ady", "", "Adyghe", "adyghé", "Adygisch");
      addCode("ady", "", "Adygei", "adyghé", "Adygisch"); // alternate english spelling
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
      addCode("THREE", "TWO", "ENGLISH", "FRENCH", "GERMAN");
   }
  void addCode(const QString& three,
               const QString& two,
               const QString& eng,
               const QString& fre,
               const QString& ger)
  {
      ISO639_2_CODES.append(three);
      ISO639_1_CODES.append(two);
      m_english.addCode(eng);
      m_french.addCode(fre);
      m_german.addCode(ger);
  }
};
/// \endcond DO_NOT_DOCUMENT
#endif // ISO639CODES_H
