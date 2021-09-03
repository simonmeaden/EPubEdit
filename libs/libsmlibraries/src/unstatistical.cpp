#include "unstatistical.h"
#include <map>

QStringList
UNStatisticalCodes::names(UNStatisticalCodes::Language lang)
{
  switch (lang) {
    case English:
      return m_english.names();
    case Russian:
      return m_russian.names();
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return QStringList();
  }
  return QStringList();
}

int
UNStatisticalCodes::m49AsInt(UNStatisticalCodes::Language lang,
                             const QString& name)
{
  switch (lang) {
    case English:
      return m_english.m49(name);
    case Russian:
      return m_russian.m49(name);
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return -1;
  }
  return -1;
}

QString
UNStatisticalCodes::m49(UNStatisticalCodes::Language lang, const QString& name)
{
  switch (lang) {
    case English:
      return m_english.m49AsString(name);
    case Russian:
      return m_russian.m49AsString(name);
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return QString();
  }
  return QString();
}

QString
UNStatisticalCodes::alphaCode(UNStatisticalCodes::Language lang,
                              const QString& name)
{
  switch (lang) {
    case English:
      return m_english.alphaCode(name);
    case Russian:
      return m_russian.alphaCode(name);
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return QString();
  }
  return QString();
}

bool
UNStatisticalCodes::isM49Valid(UNStatisticalCodes::Language lang, int value)
{
  switch (lang) {
    case English:
      return m_english.isM49Valid(value);
    case Russian:
      return m_russian.isM49Valid(value);
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return false;
  }
  return false;
}

bool
UNStatisticalCodes::isM49Valid(UNStatisticalCodes::Language lang,
                               const QString& value)
{
  switch (lang) {
    case English:
      return m_english.isM49Valid(value);
    case Russian:
      return m_russian.isM49Valid(value);
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return false;
  }
  return false;
}

bool
UNStatisticalCodes::isAlpha3Valid(UNStatisticalCodes::Language lang,
                                  const QString& value)
{
  switch (lang) {
    case English:
      return m_english.isAlpha3Valid(value);
    case Russian:
      return m_russian.isAlpha3Valid(value);
    case Chinese:
    case French:
    case Spanish:
    case Arabic:
      return false;
  }
  return false;
}

void
UNStatisticalCodes::addCodes()
{
  addCode(4, QStringLiteral("AFG"));
  m_english.addCode(QStringLiteral("Afghanistan"));
  m_russian.addCode(QStringLiteral("Афганистан"));
  m_spanish.addCode(QStringLiteral("Afganistán"));
  m_french.addCode(QStringLiteral("Afghanistan"));
  m_chinese.addCode(QStringLiteral("阿富汗"));
  m_arabic.addCode(QStringLiteral("أفغانستان"));
  addCode(248, QStringLiteral("ALA"));
  m_english.addCode(QStringLiteral("Åland Islands"));
  m_russian.addCode(QStringLiteral("Аландских островов"));
  m_spanish.addCode(QStringLiteral("Islas Åland"));
  m_french.addCode(QStringLiteral("Îles d’Åland"));
  m_chinese.addCode(QStringLiteral("奥兰群岛"));
  m_arabic.addCode(QStringLiteral("جزر ألاند"));
  addCode(8, QStringLiteral("ALB"));
  m_english.addCode(QStringLiteral("Albania"));
  m_russian.addCode(QStringLiteral("Албания"));
  m_spanish.addCode(QStringLiteral("Albania"));
  m_french.addCode(QStringLiteral("Albanie"));
  m_chinese.addCode(QStringLiteral("阿尔巴尼亚"));
  m_arabic.addCode(QStringLiteral("ألبانيا"));
  addCode(12, QStringLiteral("DZA"));
  m_english.addCode(QStringLiteral("Algeria"));
  m_russian.addCode(QStringLiteral("Алжир"));
  m_spanish.addCode(QStringLiteral("Argelia"));
  m_french.addCode(QStringLiteral("Algérie"));
  m_chinese.addCode(QStringLiteral("阿尔及利亚"));
  m_arabic.addCode(QStringLiteral("الجزائر"));
  addCode(16, QStringLiteral("ASM"));
  m_english.addCode(QStringLiteral("American Samoa"));
  m_russian.addCode(QStringLiteral("Американское Самоа"));
  m_spanish.addCode(QStringLiteral("Samoa Americana"));
  m_french.addCode(QStringLiteral("Samoa américaines"));
  m_chinese.addCode(QStringLiteral("美属萨摩亚"));
  m_arabic.addCode(QStringLiteral("ساموا الأمريكية"));
  addCode(20, QStringLiteral("AND"));
  m_english.addCode(QStringLiteral("Andorra"));
  m_russian.addCode(QStringLiteral("Андорра"));
  m_spanish.addCode(QStringLiteral("Andorra"));
  m_french.addCode(QStringLiteral("Andorre"));
  m_chinese.addCode(QStringLiteral("安道尔"));
  m_arabic.addCode(QStringLiteral("أندورا"));
  addCode(24, QStringLiteral("AGO"));
  m_english.addCode(QStringLiteral("Angola"));
  m_russian.addCode(QStringLiteral("Ангола"));
  m_spanish.addCode(QStringLiteral("Angola"));
  m_french.addCode(QStringLiteral("Angola"));
  m_chinese.addCode(QStringLiteral("安哥拉"));
  m_arabic.addCode(QStringLiteral("أنغولا"));
  addCode(660, QStringLiteral("AIA"));
  m_english.addCode(QStringLiteral("Anguilla"));
  m_russian.addCode(QStringLiteral("Ангилья"));
  m_spanish.addCode(QStringLiteral("Anguila"));
  m_french.addCode(QStringLiteral("Anguilla"));
  m_chinese.addCode(QStringLiteral("安圭拉"));
  m_arabic.addCode(QStringLiteral("أنغويلا"));
  addCode(10, QStringLiteral("ATA"));
  m_english.addCode(QStringLiteral("Antarctica"));
  m_russian.addCode(QStringLiteral("Антарктике"));
  m_spanish.addCode(QStringLiteral("Antártida"));
  m_french.addCode(QStringLiteral("Antarctique"));
  m_chinese.addCode(QStringLiteral("南极洲"));
  m_arabic.addCode(QStringLiteral("أنتاركتيكا"));
  addCode(28, QStringLiteral("ATG"));
  m_english.addCode(QStringLiteral("Antigua and Barbuda"));
  m_russian.addCode(QStringLiteral("Антигуа и Барбуда"));
  m_spanish.addCode(QStringLiteral("Antigua y Barbuda"));
  m_french.addCode(QStringLiteral("Antigua-et-Barbuda"));
  m_chinese.addCode(QStringLiteral("安提瓜和巴布达"));
  m_arabic.addCode(QStringLiteral("أنتيغوا وبربودا"));
  addCode(32, QStringLiteral("ARG"));
  m_english.addCode(QStringLiteral("Argentina"));
  m_russian.addCode(QStringLiteral("Аргентина"));
  m_spanish.addCode(QStringLiteral("Argentina"));
  m_french.addCode(QStringLiteral("Argentine"));
  m_chinese.addCode(QStringLiteral("阿根廷"));
  m_arabic.addCode(QStringLiteral("الأرجنتين"));
  addCode(51, QStringLiteral("ARM"));
  m_english.addCode(QStringLiteral("Armenia"));
  m_russian.addCode(QStringLiteral("Армения"));
  m_spanish.addCode(QStringLiteral("Armenia"));
  m_french.addCode(QStringLiteral("Arménie"));
  m_chinese.addCode(QStringLiteral("亚美尼亚"));
  m_arabic.addCode(QStringLiteral("أرمينيا"));
  addCode(533, QStringLiteral("ABW"));
  m_english.addCode(QStringLiteral("Aruba"));
  m_russian.addCode(QStringLiteral("Аруба"));
  m_spanish.addCode(QStringLiteral("Aruba"));
  m_french.addCode(QStringLiteral("Aruba"));
  m_chinese.addCode(QStringLiteral("阿鲁巴"));
  m_arabic.addCode(QStringLiteral("أروبا"));
  addCode(36, QStringLiteral("AUS"));
  m_english.addCode(QStringLiteral("Australia"));
  m_russian.addCode(QStringLiteral("Австралия"));
  m_spanish.addCode(QStringLiteral("Australia"));
  m_french.addCode(QStringLiteral("Australie"));
  m_chinese.addCode(QStringLiteral("澳大利亚"));
  m_arabic.addCode(QStringLiteral("أستراليا"));
  addCode(40, QStringLiteral("AUT"));
  m_english.addCode(QStringLiteral("Austria"));
  m_russian.addCode(QStringLiteral("Австрия"));
  m_spanish.addCode(QStringLiteral("Austria"));
  m_french.addCode(QStringLiteral("Autriche"));
  m_chinese.addCode(QStringLiteral("奥地利"));
  m_arabic.addCode(QStringLiteral("النمسا"));
  addCode(31, QStringLiteral("ALPHA"));
  m_english.addCode(QStringLiteral("Azerbaijan"));
  m_russian.addCode(QStringLiteral("Азербайджан"));
  m_spanish.addCode(QStringLiteral("Azerbaiyán"));
  m_french.addCode(QStringLiteral("Azerbaïdjan"));
  m_chinese.addCode(QStringLiteral("阿塞拜疆"));
  m_arabic.addCode(QStringLiteral("أذربيجان"));
  addCode(44, QStringLiteral("BHS"));
  m_english.addCode(QStringLiteral("Bahamas"));
  m_russian.addCode(QStringLiteral("Багамские Острова"));
  m_spanish.addCode(QStringLiteral("Bahamas"));
  m_french.addCode(QStringLiteral("Bahamas"));
  m_chinese.addCode(QStringLiteral("巴哈马"));
  m_arabic.addCode(QStringLiteral("جزر البهاما"));
  addCode(48, QStringLiteral("BHR"));
  m_english.addCode(QStringLiteral("Bahrain"));
  m_russian.addCode(QStringLiteral("Бахрейн"));
  m_spanish.addCode(QStringLiteral("Bahrein"));
  m_french.addCode(QStringLiteral("Bahreïn"));
  m_chinese.addCode(QStringLiteral("巴林"));
  m_arabic.addCode(QStringLiteral("البحرين"));
  addCode(50, QStringLiteral("BGD"));
  m_english.addCode(QStringLiteral("Bangladesh"));
  m_russian.addCode(QStringLiteral("Бангладеш"));
  m_spanish.addCode(QStringLiteral("Bangladesh"));
  m_french.addCode(QStringLiteral("Bangladesh"));
  m_chinese.addCode(QStringLiteral("孟加拉国"));
  m_arabic.addCode(QStringLiteral("بنغلاديش"));
  addCode(52, QStringLiteral("BRB"));
  m_english.addCode(QStringLiteral("Barbados"));
  m_russian.addCode(QStringLiteral("Барбадос"));
  m_spanish.addCode(QStringLiteral("Barbados"));
  m_french.addCode(QStringLiteral("Barbade"));
  m_chinese.addCode(QStringLiteral("巴巴多斯"));
  m_arabic.addCode(QStringLiteral("بربادوس"));
  addCode(112, QStringLiteral("BLR"));
  m_english.addCode(QStringLiteral("Belarus"));
  m_russian.addCode(QStringLiteral("Беларусь"));
  m_spanish.addCode(QStringLiteral("Belarús"));
  m_french.addCode(QStringLiteral("Bélarus"));
  m_chinese.addCode(QStringLiteral("白俄罗斯"));
  m_arabic.addCode(QStringLiteral("بيلاروس"));
  addCode(56, QStringLiteral("BEL"));
  m_english.addCode(QStringLiteral("Belgium"));
  m_russian.addCode(QStringLiteral("Бельгия"));
  m_spanish.addCode(QStringLiteral("Bélgica"));
  m_french.addCode(QStringLiteral("Belgique"));
  m_chinese.addCode(QStringLiteral("比利时"));
  m_arabic.addCode(QStringLiteral("بلجيكا"));
  addCode(84, QStringLiteral("BLZ"));
  m_english.addCode(QStringLiteral("Belize"));
  m_russian.addCode(QStringLiteral("Белиз"));
  m_spanish.addCode(QStringLiteral("Belice"));
  m_french.addCode(QStringLiteral("Belize"));
  m_chinese.addCode(QStringLiteral("伯利兹"));
  m_arabic.addCode(QStringLiteral("بليز"));
  addCode(204, QStringLiteral("BEN"));
  m_english.addCode(QStringLiteral("Benin"));
  m_russian.addCode(QStringLiteral("Бенин"));
  m_spanish.addCode(QStringLiteral("Benin"));
  m_french.addCode(QStringLiteral("Bénin"));
  m_chinese.addCode(QStringLiteral("贝宁"));
  m_arabic.addCode(QStringLiteral("بنن"));
  addCode(60, QStringLiteral("BMU"));
  m_english.addCode(QStringLiteral("Bermuda"));
  m_russian.addCode(QStringLiteral("Бермудские острова"));
  m_spanish.addCode(QStringLiteral("Bermuda"));
  m_french.addCode(QStringLiteral("Bermudes"));
  m_chinese.addCode(QStringLiteral("百慕大"));
  m_arabic.addCode(QStringLiteral("برمودا"));
  addCode(64, QStringLiteral("BTN"));
  m_english.addCode(QStringLiteral("Bhutan"));
  m_russian.addCode(QStringLiteral("Бутан"));
  m_spanish.addCode(QStringLiteral("Bhután"));
  m_french.addCode(QStringLiteral("Bhoutan"));
  m_chinese.addCode(QStringLiteral("不丹"));
  m_arabic.addCode(QStringLiteral("بوتان"));
  addCode(68, QStringLiteral("BOL"));
  m_english.addCode(QStringLiteral("Bolivia (Plurinational State of)"));
  m_russian.addCode(QStringLiteral("Боливия (Многонациональное Государство)"));
  m_spanish.addCode(QStringLiteral("Bolivia (Estado Plurinacional de)"));
  m_french.addCode(QStringLiteral("Bolivie (État plurinational de)"));
  m_chinese.addCode(QStringLiteral("多民族玻利维亚国"));
  m_arabic.addCode(QStringLiteral("بوليفيا (دولة - المتعددة القوميات)"));
  addCode(535, QStringLiteral("BES"));
  m_english.addCode(QStringLiteral("Bonaire, Sint Eustatius and Saba"));
  m_russian.addCode(QStringLiteral("Бонайре, Синт-Эстатиус и Саба"));
  m_spanish.addCode(QStringLiteral("Bonaire, San Eustaquio y Saba"));
  m_french.addCode(QStringLiteral("Bonaire, Saint-Eustache et Saba"));
  m_chinese.addCode(QStringLiteral("博纳尔，圣俄斯塔休斯和萨巴"));
  m_arabic.addCode(QStringLiteral("بونير وسانت يوستاشيوس وسابا"));
  addCode(70, QStringLiteral("BIH"));
  m_english.addCode(QStringLiteral("Bosnia and Herzegovina"));
  m_russian.addCode(QStringLiteral("Босния и Герцеговина"));
  m_spanish.addCode(QStringLiteral("Bosnia y Herzegovina"));
  m_french.addCode(QStringLiteral("Bosnie-Herzégovine"));
  m_chinese.addCode(QStringLiteral("波斯尼亚和黑塞哥维那"));
  m_arabic.addCode(QStringLiteral("البوسنة والهرسك"));
  addCode(72, QStringLiteral("BWA"));
  m_english.addCode(QStringLiteral("Botswana"));
  m_russian.addCode(QStringLiteral("Ботсвана"));
  m_spanish.addCode(QStringLiteral("Botswana"));
  m_french.addCode(QStringLiteral("Botswana"));
  m_chinese.addCode(QStringLiteral("博茨瓦纳"));
  m_arabic.addCode(QStringLiteral("بوتسوانا"));
  addCode(74, QStringLiteral("BVT"));
  m_english.addCode(QStringLiteral("Bouvet Island"));
  m_russian.addCode(QStringLiteral("Остров Буве"));
  m_spanish.addCode(QStringLiteral("Isla Bouvet"));
  m_french.addCode(QStringLiteral("Île Bouvet"));
  m_chinese.addCode(QStringLiteral("布维岛"));
  m_arabic.addCode(QStringLiteral("جزيرة بوفيت"));
  addCode(76, QStringLiteral("BRA"));
  m_english.addCode(QStringLiteral("Brazil"));
  m_russian.addCode(QStringLiteral("Бразилия"));
  m_spanish.addCode(QStringLiteral("Brasil"));
  m_french.addCode(QStringLiteral("Brésil"));
  m_chinese.addCode(QStringLiteral("巴西"));
  m_arabic.addCode(QStringLiteral("البرازيل"));
  addCode(86, QStringLiteral("IOT"));
  m_english.addCode(QStringLiteral("British Indian Ocean Territory"));
  m_russian.addCode(QStringLiteral("Британская территория в Индийском океане"));
  m_spanish.addCode(QStringLiteral("Territorio Británico del Océano Índico"));
  m_french.addCode(QStringLiteral("Territoire britannique de l'océan Indien"));
  m_chinese.addCode(QStringLiteral("英属印度洋领土"));
  m_arabic.addCode(QStringLiteral("المحيط الهندي الإقليم البريطاني في"));
  addCode(92, QStringLiteral("VGB"));
  m_english.addCode(QStringLiteral("British Virgin Islands"));
  m_russian.addCode(QStringLiteral("Британские Виргинские острова"));
  m_spanish.addCode(QStringLiteral("Islas Vírgenes Británicas"));
  m_french.addCode(QStringLiteral("Îles Vierges britanniques"));
  m_chinese.addCode(QStringLiteral("英属维尔京群岛"));
  m_arabic.addCode(QStringLiteral("جزر فرجن البريطانية"));
  addCode(96, QStringLiteral("BRN"));
  m_english.addCode(QStringLiteral("Brunei Darussalam"));
  m_russian.addCode(QStringLiteral("Бруней-Даруссалам"));
  m_spanish.addCode(QStringLiteral("Brunei Darussalam"));
  m_french.addCode(QStringLiteral("Brunéi Darussalam"));
  m_chinese.addCode(QStringLiteral("文莱达鲁萨兰国"));
  m_arabic.addCode(QStringLiteral("بروني دار السلام"));
  addCode(100, QStringLiteral("BGR"));
  m_english.addCode(QStringLiteral("Bulgaria"));
  m_russian.addCode(QStringLiteral("Болгария"));
  m_spanish.addCode(QStringLiteral("Bulgaria"));
  m_french.addCode(QStringLiteral("Bulgarie"));
  m_chinese.addCode(QStringLiteral("保加利亚"));
  m_arabic.addCode(QStringLiteral("بلغاريا"));
  addCode(854, QStringLiteral("BFA"));
  m_english.addCode(QStringLiteral("Burkina Faso"));
  m_russian.addCode(QStringLiteral("Буркина-Фасо"));
  m_spanish.addCode(QStringLiteral("Burkina Faso"));
  m_french.addCode(QStringLiteral("Burkina Faso"));
  m_chinese.addCode(QStringLiteral("布基纳法索"));
  m_arabic.addCode(QStringLiteral("بوركينا فاسو"));
  addCode(108, QStringLiteral("BDI"));
  m_english.addCode(QStringLiteral("Burundi"));
  m_russian.addCode(QStringLiteral("Бурунди"));
  m_spanish.addCode(QStringLiteral("Burundi"));
  m_french.addCode(QStringLiteral("Burundi"));
  m_chinese.addCode(QStringLiteral("布隆迪"));
  m_arabic.addCode(QStringLiteral("بوروندي"));
  addCode(132, QStringLiteral("CPV"));
  m_english.addCode(QStringLiteral("Cabo Verde"));
  m_russian.addCode(QStringLiteral("Кабо-Верде"));
  m_spanish.addCode(QStringLiteral("Cabo Verde"));
  m_french.addCode(QStringLiteral("Cabo Verde"));
  m_chinese.addCode(QStringLiteral("佛得角"));
  m_arabic.addCode(QStringLiteral("كابو فيردي"));
  addCode(116, QStringLiteral("KHM"));
  m_english.addCode(QStringLiteral("Cambodia"));
  m_russian.addCode(QStringLiteral("Камбоджа"));
  m_spanish.addCode(QStringLiteral("Camboya"));
  m_french.addCode(QStringLiteral("Cambodge"));
  m_chinese.addCode(QStringLiteral("柬埔寨"));
  m_arabic.addCode(QStringLiteral("كمبوديا"));
  addCode(120, QStringLiteral("CMR"));
  m_english.addCode(QStringLiteral("Cameroon"));
  m_russian.addCode(QStringLiteral("Камерун"));
  m_spanish.addCode(QStringLiteral("Camerún"));
  m_french.addCode(QStringLiteral("Cameroun"));
  m_chinese.addCode(QStringLiteral("喀麦隆"));
  m_arabic.addCode(QStringLiteral("الكاميرون"));
  addCode(124, QStringLiteral("CAN"));
  m_english.addCode(QStringLiteral("Canada"));
  m_russian.addCode(QStringLiteral("Канада"));
  m_spanish.addCode(QStringLiteral("Canadá"));
  m_french.addCode(QStringLiteral("Canada"));
  m_chinese.addCode(QStringLiteral("加拿大"));
  m_arabic.addCode(QStringLiteral("كندا"));
  addCode(136, QStringLiteral("CYM"));
  m_english.addCode(QStringLiteral("Cayman Islands"));
  m_russian.addCode(QStringLiteral("Кайман острова"));
  m_spanish.addCode(QStringLiteral("Islas Caimán"));
  m_french.addCode(QStringLiteral("Îles Caïmanes"));
  m_chinese.addCode(QStringLiteral("开曼群岛"));
  m_arabic.addCode(QStringLiteral("جزر كايمان"));
  addCode(140, QStringLiteral("CAF"));
  m_english.addCode(QStringLiteral("Central African Republic"));
  m_russian.addCode(QStringLiteral("Центральноафриканская Республика"));
  m_spanish.addCode(QStringLiteral("República Centroafricana"));
  m_french.addCode(QStringLiteral("République centrafricaine"));
  m_chinese.addCode(QStringLiteral("中非共和国"));
  m_arabic.addCode(QStringLiteral("جمهورية أفريقيا الوسطى"));
  addCode(148, QStringLiteral("TCD"));
  m_english.addCode(QStringLiteral("Chad"));
  m_russian.addCode(QStringLiteral("Чад"));
  m_spanish.addCode(QStringLiteral("Chad"));
  m_french.addCode(QStringLiteral("Tchad"));
  m_chinese.addCode(QStringLiteral("乍得"));
  m_arabic.addCode(QStringLiteral("تشاد"));
  addCode(152, QStringLiteral("CHL"));
  m_english.addCode(QStringLiteral("Chile"));
  m_russian.addCode(QStringLiteral("Чили"));
  m_spanish.addCode(QStringLiteral("Chile"));
  m_french.addCode(QStringLiteral("Chili"));
  m_chinese.addCode(QStringLiteral("智利"));
  m_arabic.addCode(QStringLiteral("شيلي"));
  addCode(156, QStringLiteral("CHN"));
  m_english.addCode(QStringLiteral("China"));
  m_russian.addCode(QStringLiteral("Китай"));
  m_spanish.addCode(QStringLiteral("China"));
  m_french.addCode(QStringLiteral("Chine"));
  m_chinese.addCode(QStringLiteral("中国"));
  m_arabic.addCode(QStringLiteral("الصين"));
  addCode(344, QStringLiteral("HKG"));
  m_english.addCode(
    QStringLiteral("China, Hong Kong Special Administrative Region"));
  m_russian.addCode(
    QStringLiteral("Китай, Специальный административный район Гонконг"));
  m_spanish.addCode(
    QStringLiteral("China, región administrativa especial de Hong Kong"));
  m_french.addCode(
    QStringLiteral("Chine, région administrative spéciale de Hong Kong"));
  m_chinese.addCode(QStringLiteral("中国香港特别行政区"));
  m_arabic.addCode(QStringLiteral("الصين، منطقة هونغ كونغ الإدارية الخاصة"));
  addCode(446, QStringLiteral("MAC"));
  m_english.addCode(
    QStringLiteral("China, Macao Special Administrative Region"));
  m_russian.addCode(
    QStringLiteral("Китай, Специальный административный район Макао"));
  m_spanish.addCode(
    QStringLiteral("China, región administrativa especial de Macao"));
  m_french.addCode(
    QStringLiteral("Chine, région administrative spéciale de Macao"));
  m_chinese.addCode(QStringLiteral("中国澳门特别行政区"));
  m_arabic.addCode(QStringLiteral("الصين، منطقة ماكاو الإدارية الخاصة"));
  addCode(162, QStringLiteral("CXR"));
  m_english.addCode(QStringLiteral("Christmas Island"));
  m_russian.addCode(QStringLiteral("остров Рождества"));
  m_spanish.addCode(QStringLiteral("Isla Christmas"));
  m_french.addCode(QStringLiteral("Île Christmas"));
  m_chinese.addCode(QStringLiteral("圣诞岛"));
  m_arabic.addCode(QStringLiteral("جزيرة عيد الميلاد"));
  addCode(166, QStringLiteral("CCK"));
  m_english.addCode(QStringLiteral("Cocos (Keeling) Islands"));
  m_russian.addCode(QStringLiteral("Кокосовых (Килинг) островов"));
  m_spanish.addCode(QStringLiteral("Islas Cocos (Keeling)"));
  m_french.addCode(QStringLiteral("Îles des Cocos (Keeling)"));
  m_chinese.addCode(QStringLiteral("科科斯（基林）群岛"));
  m_arabic.addCode(QStringLiteral("جزر كوكس (كيلينغ)"));
  addCode(170, QStringLiteral("COL"));
  m_english.addCode(QStringLiteral("Colombia"));
  m_russian.addCode(QStringLiteral("Колумбия"));
  m_spanish.addCode(QStringLiteral("Colombia"));
  m_french.addCode(QStringLiteral("Colombie"));
  m_chinese.addCode(QStringLiteral("哥伦比亚"));
  m_arabic.addCode(QStringLiteral("كولومبيا"));
  addCode(174, QStringLiteral("COM"));
  m_english.addCode(QStringLiteral("Comoros"));
  m_russian.addCode(QStringLiteral("Коморские Острова"));
  m_spanish.addCode(QStringLiteral("Comoras"));
  m_french.addCode(QStringLiteral("Comores"));
  m_chinese.addCode(QStringLiteral("科摩罗"));
  m_arabic.addCode(QStringLiteral("جزر القمر"));
  addCode(178, QStringLiteral("COG"));
  m_english.addCode(QStringLiteral("Congo"));
  m_russian.addCode(QStringLiteral("Конго"));
  m_spanish.addCode(QStringLiteral("Congo"));
  m_french.addCode(QStringLiteral("Congo"));
  m_chinese.addCode(QStringLiteral("刚果"));
  m_arabic.addCode(QStringLiteral("الكونغو"));
  addCode(184, QStringLiteral("COK"));
  m_english.addCode(QStringLiteral("Cook Islands"));
  m_russian.addCode(QStringLiteral("Острова Кука"));
  m_spanish.addCode(QStringLiteral("Islas Cook"));
  m_french.addCode(QStringLiteral("Îles Cook"));
  m_chinese.addCode(QStringLiteral("库克群岛"));
  m_arabic.addCode(QStringLiteral("جزر كوك"));
  addCode(188, QStringLiteral("CRI"));
  m_english.addCode(QStringLiteral("Costa Rica"));
  m_russian.addCode(QStringLiteral("Коста-Рика"));
  m_spanish.addCode(QStringLiteral("Costa Rica"));
  m_french.addCode(QStringLiteral("Costa Rica"));
  m_chinese.addCode(QStringLiteral("哥斯达黎加"));
  m_arabic.addCode(QStringLiteral("كوستاريكا"));
  addCode(384, QStringLiteral("CIV"));
  m_english.addCode(QStringLiteral("Côte d’Ivoire"));
  m_russian.addCode(QStringLiteral("Кот-д'Ивуар"));
  m_spanish.addCode(QStringLiteral("Côte d’Ivoire"));
  m_french.addCode(QStringLiteral("Côte d’Ivoire"));
  m_chinese.addCode(QStringLiteral("科特迪瓦"));
  m_arabic.addCode(QStringLiteral("كوت ديفوار"));
  addCode(191, QStringLiteral("HRV"));
  m_english.addCode(QStringLiteral("Croatia"));
  m_russian.addCode(QStringLiteral("Хорватия"));
  m_spanish.addCode(QStringLiteral("Croacia"));
  m_french.addCode(QStringLiteral("Croatie"));
  m_chinese.addCode(QStringLiteral("克罗地亚"));
  m_arabic.addCode(QStringLiteral("كرواتيا"));
  addCode(192, QStringLiteral("CUB"));
  m_english.addCode(QStringLiteral("Cuba"));
  m_russian.addCode(QStringLiteral("Куба"));
  m_spanish.addCode(QStringLiteral("Cuba"));
  m_french.addCode(QStringLiteral("Cuba"));
  m_chinese.addCode(QStringLiteral("古巴"));
  m_arabic.addCode(QStringLiteral("كوبا"));
  addCode(531, QStringLiteral("CUW"));
  m_english.addCode(QStringLiteral("Curaçao"));
  m_russian.addCode(QStringLiteral("Кюрасао"));
  m_spanish.addCode(QStringLiteral("Curazao"));
  m_french.addCode(QStringLiteral("Curaçao"));
  m_chinese.addCode(QStringLiteral("库拉索"));
  m_arabic.addCode(QStringLiteral("كوراساو"));
  addCode(196, QStringLiteral("CYP"));
  m_english.addCode(QStringLiteral("Cyprus"));
  m_russian.addCode(QStringLiteral("Кипр"));
  m_spanish.addCode(QStringLiteral("Chipre"));
  m_french.addCode(QStringLiteral("Chypre"));
  m_chinese.addCode(QStringLiteral("塞浦路斯"));
  m_arabic.addCode(QStringLiteral("قبرص"));
  addCode(203, QStringLiteral("CZE"));
  m_english.addCode(QStringLiteral("Czechia"));
  m_russian.addCode(QStringLiteral("Чехия"));
  m_spanish.addCode(QStringLiteral("Chequia"));
  m_french.addCode(QStringLiteral("Tchéquie"));
  m_chinese.addCode(QStringLiteral("捷克"));
  m_arabic.addCode(QStringLiteral("تشيكيا"));
  addCode(408, QStringLiteral("PRK"));
  m_english.addCode(QStringLiteral("Democratic People's Republic of Korea"));
  m_russian.addCode(
    QStringLiteral("Корейская Народно-Демократическая Республика"));
  m_spanish.addCode(QStringLiteral("República Popular Democrática de Corea"));
  m_french.addCode(
    QStringLiteral("République populaire démocratique de Corée"));
  m_chinese.addCode(QStringLiteral("朝鲜民主主义人民共和国"));
  m_arabic.addCode(QStringLiteral("جمهورية كوريا الشعبية الديمقراطية"));
  addCode(180, QStringLiteral("COD"));
  m_english.addCode(QStringLiteral("Democratic Republic of the Congo"));
  m_russian.addCode(QStringLiteral("Демократическая Республика Конго"));
  m_spanish.addCode(QStringLiteral("República Democrática del Congo"));
  m_french.addCode(QStringLiteral("République démocratique du Congo"));
  m_chinese.addCode(QStringLiteral("刚果民主共和国"));
  m_arabic.addCode(QStringLiteral("جمهورية الكونغو الديمقراطية"));
  addCode(208, QStringLiteral("DNK"));
  m_english.addCode(QStringLiteral("Denmark"));
  m_russian.addCode(QStringLiteral("Дания"));
  m_spanish.addCode(QStringLiteral("Dinamarca"));
  m_french.addCode(QStringLiteral("Danemark"));
  m_chinese.addCode(QStringLiteral("丹麦"));
  m_arabic.addCode(QStringLiteral("الدانمرك"));
  addCode(262, QStringLiteral("DJI"));
  m_english.addCode(QStringLiteral("Djibouti"));
  m_russian.addCode(QStringLiteral("Джибути"));
  m_spanish.addCode(QStringLiteral("Djibouti"));
  m_french.addCode(QStringLiteral("Djibouti"));
  m_chinese.addCode(QStringLiteral("吉布提"));
  m_arabic.addCode(QStringLiteral("جيبوتي"));
  addCode(212, QStringLiteral("DMA"));
  m_english.addCode(QStringLiteral("Dominica"));
  m_russian.addCode(QStringLiteral("Доминика"));
  m_spanish.addCode(QStringLiteral("Dominica"));
  m_french.addCode(QStringLiteral("Dominique"));
  m_chinese.addCode(QStringLiteral("多米尼克"));
  m_arabic.addCode(QStringLiteral("دومينيكا"));
  addCode(214, QStringLiteral("DOM"));
  m_english.addCode(QStringLiteral("Dominican Republic"));
  m_russian.addCode(QStringLiteral("Доминиканская Республика"));
  m_spanish.addCode(QStringLiteral("República Dominicana"));
  m_french.addCode(QStringLiteral("République dominicaine"));
  m_chinese.addCode(QStringLiteral("多米尼加"));
  m_arabic.addCode(QStringLiteral("الجمهورية الدومينيكية"));
  addCode(218, QStringLiteral("ECU"));
  m_english.addCode(QStringLiteral("Ecuador"));
  m_russian.addCode(QStringLiteral("Эквадор"));
  m_spanish.addCode(QStringLiteral("Ecuador"));
  m_french.addCode(QStringLiteral("Équateur"));
  m_chinese.addCode(QStringLiteral("厄瓜多尔"));
  m_arabic.addCode(QStringLiteral("إكوادور"));
  addCode(818, QStringLiteral("EGY"));
  m_english.addCode(QStringLiteral("Egypt"));
  m_russian.addCode(QStringLiteral("Египет"));
  m_spanish.addCode(QStringLiteral("Egipto"));
  m_french.addCode(QStringLiteral("Égypte"));
  m_chinese.addCode(QStringLiteral("埃及"));
  m_arabic.addCode(QStringLiteral("مصر"));
  addCode(222, QStringLiteral("SLV"));
  m_english.addCode(QStringLiteral("El Salvador"));
  m_russian.addCode(QStringLiteral("Сальвадор"));
  m_spanish.addCode(QStringLiteral("El Salvador"));
  m_french.addCode(QStringLiteral("El Salvador"));
  m_chinese.addCode(QStringLiteral("萨尔瓦多"));
  m_arabic.addCode(QStringLiteral("السلفادور"));
  addCode(226, QStringLiteral("GNQ"));
  m_english.addCode(QStringLiteral("Equatorial Guinea"));
  m_russian.addCode(QStringLiteral("Экваториальная Гвинея"));
  m_spanish.addCode(QStringLiteral("Guinea Ecuatorial"));
  m_french.addCode(QStringLiteral("Guinée équatoriale"));
  m_chinese.addCode(QStringLiteral("赤道几内亚"));
  m_arabic.addCode(QStringLiteral("غينيا الاستوائية"));
  addCode(232, QStringLiteral("ERI"));
  m_english.addCode(QStringLiteral("Eritrea"));
  m_russian.addCode(QStringLiteral("Эритрея"));
  m_spanish.addCode(QStringLiteral("Eritrea"));
  m_french.addCode(QStringLiteral("Érythrée"));
  m_chinese.addCode(QStringLiteral("厄立特里亚"));
  m_arabic.addCode(QStringLiteral("إريتريا"));
  addCode(233, QStringLiteral("EST"));
  m_english.addCode(QStringLiteral("Estonia"));
  m_russian.addCode(QStringLiteral("Эстония"));
  m_spanish.addCode(QStringLiteral("Estonia"));
  m_french.addCode(QStringLiteral("Estonie"));
  m_chinese.addCode(QStringLiteral("爱沙尼亚"));
  m_arabic.addCode(QStringLiteral("إستونيا"));
  addCode(748, QStringLiteral("SWZ"));
  m_english.addCode(QStringLiteral("Eswatini"));
  m_russian.addCode(QStringLiteral("Эсватини"));
  m_spanish.addCode(QStringLiteral("Eswatini"));
  m_french.addCode(QStringLiteral("Eswatini"));
  m_chinese.addCode(QStringLiteral("斯威士兰"));
  m_arabic.addCode(QStringLiteral("إسواتيني"));
  addCode(231, QStringLiteral("ETH"));
  m_english.addCode(QStringLiteral("Ethiopia"));
  m_russian.addCode(QStringLiteral("Эфиопия"));
  m_spanish.addCode(QStringLiteral("Etiopía"));
  m_french.addCode(QStringLiteral("Éthiopie"));
  m_chinese.addCode(QStringLiteral("埃塞俄比亚"));
  m_arabic.addCode(QStringLiteral("إثيوبيا"));
  addCode(238, QStringLiteral("FLK"));
  m_english.addCode(QStringLiteral("Falkland Islands (Malvinas)"));
  m_russian.addCode(QStringLiteral("Фолклендские (Мальвинские) острова"));
  m_spanish.addCode(QStringLiteral("Islas Malvinas (Falkland)"));
  m_french.addCode(QStringLiteral("Îles Falkland (Malvinas)"));
  m_chinese.addCode(QStringLiteral("福克兰群岛（马尔维纳斯）"));
  m_arabic.addCode(QStringLiteral("جزر فوكلاند (مالفيناس)"));
  addCode(234, QStringLiteral("FRO"));
  m_english.addCode(QStringLiteral("Faroe Islands"));
  m_russian.addCode(QStringLiteral("Фарерские острова"));
  m_spanish.addCode(QStringLiteral("Islas Feroe"));
  m_french.addCode(QStringLiteral("Îles Féroé"));
  m_chinese.addCode(QStringLiteral("法罗群岛"));
  m_arabic.addCode(QStringLiteral("جزر فايرو"));
  addCode(242, QStringLiteral("FJI"));
  m_english.addCode(QStringLiteral("Fiji"));
  m_russian.addCode(QStringLiteral("Фиджи"));
  m_spanish.addCode(QStringLiteral("Fiji"));
  m_french.addCode(QStringLiteral("Fidji"));
  m_chinese.addCode(QStringLiteral("斐济"));
  m_arabic.addCode(QStringLiteral("فيجي"));
  addCode(246, QStringLiteral("FIN"));
  m_english.addCode(QStringLiteral("Finland"));
  m_russian.addCode(QStringLiteral("Финляндия"));
  m_spanish.addCode(QStringLiteral("Finlandia"));
  m_french.addCode(QStringLiteral("Finlande"));
  m_chinese.addCode(QStringLiteral("芬兰"));
  m_arabic.addCode(QStringLiteral("فنلندا"));
  addCode(250, QStringLiteral("FRA"));
  m_english.addCode(QStringLiteral("France"));
  m_russian.addCode(QStringLiteral("Франция"));
  m_spanish.addCode(QStringLiteral("Francia"));
  m_french.addCode(QStringLiteral("France"));
  m_chinese.addCode(QStringLiteral("法国"));
  m_arabic.addCode(QStringLiteral("فرنسا"));
  addCode(254, QStringLiteral("GUF"));
  m_english.addCode(QStringLiteral("French Guiana"));
  m_russian.addCode(QStringLiteral("Французская Гвиана"));
  m_spanish.addCode(QStringLiteral("Guayana Francesa"));
  m_french.addCode(QStringLiteral("Guyane française"));
  m_chinese.addCode(QStringLiteral("法属圭亚那"));
  m_arabic.addCode(QStringLiteral("غيانا الفرنسية"));
  addCode(258, QStringLiteral("PYF"));
  m_english.addCode(QStringLiteral("French Polynesia"));
  m_russian.addCode(QStringLiteral("Французская Полинезия"));
  m_spanish.addCode(QStringLiteral("Polinesia Francesa"));
  m_french.addCode(QStringLiteral("Polynésie française"));
  m_chinese.addCode(QStringLiteral("法属波利尼西亚"));
  m_arabic.addCode(QStringLiteral("بولينيزيا الفرنسية"));
  addCode(260, QStringLiteral("ATF"));
  m_english.addCode(QStringLiteral("French Southern Territories"));
  m_russian.addCode(
    QStringLiteral("Южные земли (французская заморская территория)"));
  m_spanish.addCode(
    QStringLiteral("Territorio de las Tierras Australes Francesas"));
  m_french.addCode(QStringLiteral("Terres australes françaises"));
  m_chinese.addCode(QStringLiteral("法属南方领地"));
  m_arabic.addCode(QStringLiteral("الأراضي الفرنسية الجنوبية الجنوبية"));
  addCode(266, QStringLiteral("GAB"));
  m_english.addCode(QStringLiteral("Gabon"));
  m_russian.addCode(QStringLiteral("Габон"));
  m_spanish.addCode(QStringLiteral("Gabón"));
  m_french.addCode(QStringLiteral("Gabon"));
  m_chinese.addCode(QStringLiteral("加蓬"));
  m_arabic.addCode(QStringLiteral("غابون"));
  addCode(270, QStringLiteral("GMB"));
  m_english.addCode(QStringLiteral("Gambia"));
  m_russian.addCode(QStringLiteral("Гамбия"));
  m_spanish.addCode(QStringLiteral("Gambia"));
  m_french.addCode(QStringLiteral("Gambie"));
  m_chinese.addCode(QStringLiteral("冈比亚"));
  m_arabic.addCode(QStringLiteral("غامبيا"));
  addCode(268, QStringLiteral("GEO"));
  m_english.addCode(QStringLiteral("Georgia"));
  m_russian.addCode(QStringLiteral("Грузия"));
  m_spanish.addCode(QStringLiteral("Georgia"));
  m_french.addCode(QStringLiteral("Géorgie"));
  m_chinese.addCode(QStringLiteral("格鲁吉亚"));
  m_arabic.addCode(QStringLiteral("جورجيا"));
  addCode(276, QStringLiteral("DEU"));
  m_english.addCode(QStringLiteral("Germany"));
  m_russian.addCode(QStringLiteral("Германия"));
  m_spanish.addCode(QStringLiteral("Alemania"));
  m_french.addCode(QStringLiteral("Allemagne"));
  m_chinese.addCode(QStringLiteral("德国"));
  m_arabic.addCode(QStringLiteral("ألمانيا"));
  addCode(288, QStringLiteral("GHA"));
  m_english.addCode(QStringLiteral("Ghana"));
  m_russian.addCode(QStringLiteral("Гана"));
  m_spanish.addCode(QStringLiteral("Ghana"));
  m_french.addCode(QStringLiteral("Ghana"));
  m_chinese.addCode(QStringLiteral("加纳"));
  m_arabic.addCode(QStringLiteral("غانا"));
  addCode(292, QStringLiteral("GIB"));
  m_english.addCode(QStringLiteral("Gibraltar"));
  m_russian.addCode(QStringLiteral("Гибралтар"));
  m_spanish.addCode(QStringLiteral("Gibraltar"));
  m_french.addCode(QStringLiteral("Gibraltar"));
  m_chinese.addCode(QStringLiteral("直布罗陀"));
  m_arabic.addCode(QStringLiteral("جبل طارق"));
  addCode(300, QStringLiteral("GRC"));
  m_english.addCode(QStringLiteral("Greece"));
  m_russian.addCode(QStringLiteral("Греция"));
  m_spanish.addCode(QStringLiteral("Grecia"));
  m_french.addCode(QStringLiteral("Grèce"));
  m_chinese.addCode(QStringLiteral("希腊"));
  m_arabic.addCode(QStringLiteral("اليونان"));
  addCode(304, QStringLiteral("GRL"));
  m_english.addCode(QStringLiteral("Greenland"));
  m_russian.addCode(QStringLiteral("Гренландия"));
  m_spanish.addCode(QStringLiteral("Groenlandia"));
  m_french.addCode(QStringLiteral("Groenland"));
  m_chinese.addCode(QStringLiteral("格陵兰"));
  m_arabic.addCode(QStringLiteral("غرينلند"));
  addCode(308, QStringLiteral("GRD"));
  m_english.addCode(QStringLiteral("Grenada"));
  m_russian.addCode(QStringLiteral("Гренада"));
  m_spanish.addCode(QStringLiteral("Granada"));
  m_french.addCode(QStringLiteral("Grenade"));
  m_chinese.addCode(QStringLiteral("格林纳达"));
  m_arabic.addCode(QStringLiteral("غرينادا"));
  addCode(312, QStringLiteral("GLP"));
  m_english.addCode(QStringLiteral("Guadeloupe"));
  m_russian.addCode(QStringLiteral("Гваделупа"));
  m_spanish.addCode(QStringLiteral("Guadalupe"));
  m_french.addCode(QStringLiteral("Guadeloupe"));
  m_chinese.addCode(QStringLiteral("瓜德罗普"));
  m_arabic.addCode(QStringLiteral("غوادلوب"));
  addCode(316, QStringLiteral("GUM"));
  m_english.addCode(QStringLiteral("Guam"));
  m_russian.addCode(QStringLiteral("Гуам"));
  m_spanish.addCode(QStringLiteral("Guam"));
  m_french.addCode(QStringLiteral("Guam"));
  m_chinese.addCode(QStringLiteral("关岛"));
  m_arabic.addCode(QStringLiteral("غوام"));
  addCode(320, QStringLiteral("GTM"));
  m_english.addCode(QStringLiteral("Guatemala"));
  m_russian.addCode(QStringLiteral("Гватемала"));
  m_spanish.addCode(QStringLiteral("Guatemala"));
  m_french.addCode(QStringLiteral("Guatemala"));
  m_chinese.addCode(QStringLiteral("危地马拉"));
  m_arabic.addCode(QStringLiteral("غواتيمالا"));
  addCode(831, QStringLiteral("GGY"));
  m_english.addCode(QStringLiteral("Guernsey"));
  m_russian.addCode(QStringLiteral("Гернси"));
  m_spanish.addCode(QStringLiteral("Guernsey"));
  m_french.addCode(QStringLiteral("Guernesey"));
  m_chinese.addCode(QStringLiteral("格恩西"));
  m_arabic.addCode(QStringLiteral("غيرنسي"));
  addCode(324, QStringLiteral("GIN"));
  m_english.addCode(QStringLiteral("Guinea"));
  m_russian.addCode(QStringLiteral("Гвинея"));
  m_spanish.addCode(QStringLiteral("Guinea"));
  m_french.addCode(QStringLiteral("Guinée"));
  m_chinese.addCode(QStringLiteral("几内亚"));
  m_arabic.addCode(QStringLiteral("غينيا"));
  addCode(624, QStringLiteral("GNB"));
  m_english.addCode(QStringLiteral("Guinea-Bissau"));
  m_russian.addCode(QStringLiteral("Гвинея-Бисау"));
  m_spanish.addCode(QStringLiteral("Guinea-Bissau"));
  m_french.addCode(QStringLiteral("Guinée-Bissau"));
  m_chinese.addCode(QStringLiteral("几内亚比绍"));
  m_arabic.addCode(QStringLiteral("غينيا - بيساو"));
  addCode(328, QStringLiteral("GUY"));
  m_english.addCode(QStringLiteral("Guyana"));
  m_russian.addCode(QStringLiteral("Гайана"));
  m_spanish.addCode(QStringLiteral("Guyana"));
  m_french.addCode(QStringLiteral("Guyana"));
  m_chinese.addCode(QStringLiteral("圭亚那"));
  m_arabic.addCode(QStringLiteral("غيانا"));
  addCode(332, QStringLiteral("HTI"));
  m_english.addCode(QStringLiteral("Haiti"));
  m_russian.addCode(QStringLiteral("Гаити"));
  m_spanish.addCode(QStringLiteral("Haití"));
  m_french.addCode(QStringLiteral("Haïti"));
  m_chinese.addCode(QStringLiteral("海地"));
  m_arabic.addCode(QStringLiteral("هايتي"));
  addCode(334, QStringLiteral("HMD"));
  m_english.addCode(QStringLiteral("Heard Island and McDonald Islands"));
  m_russian.addCode(QStringLiteral("Остров Херд и острова Макдональд"));
  m_spanish.addCode(QStringLiteral("Islas Heard y McDonald"));
  m_french.addCode(QStringLiteral("Île Heard-et-Îles MacDonald"));
  m_chinese.addCode(QStringLiteral("赫德岛和麦克唐纳岛"));
  m_arabic.addCode(QStringLiteral("جزيرة هيرد وجزر ماكدونالد"));
  addCode(336, QStringLiteral("VAT"));
  m_english.addCode(QStringLiteral("Holy See"));
  m_russian.addCode(QStringLiteral("Святой Престол"));
  m_spanish.addCode(QStringLiteral("Santa Sede"));
  m_french.addCode(QStringLiteral("Saint-Siège"));
  m_chinese.addCode(QStringLiteral("教廷"));
  m_arabic.addCode(QStringLiteral("الكرسي الرسولي"));
  addCode(340, QStringLiteral("HND"));
  m_english.addCode(QStringLiteral("Honduras"));
  m_russian.addCode(QStringLiteral("Гондурас"));
  m_spanish.addCode(QStringLiteral("Honduras"));
  m_french.addCode(QStringLiteral("Honduras"));
  m_chinese.addCode(QStringLiteral("洪都拉斯"));
  m_arabic.addCode(QStringLiteral("هندوراس"));
  addCode(348, QStringLiteral("HUN"));
  m_english.addCode(QStringLiteral("Hungary"));
  m_russian.addCode(QStringLiteral("Венгрия"));
  m_spanish.addCode(QStringLiteral("Hungría"));
  m_french.addCode(QStringLiteral("Hongrie"));
  m_chinese.addCode(QStringLiteral("匈牙利"));
  m_arabic.addCode(QStringLiteral("هنغاريا"));
  addCode(352, QStringLiteral("ISL"));
  m_english.addCode(QStringLiteral("Iceland"));
  m_russian.addCode(QStringLiteral("Исландия"));
  m_spanish.addCode(QStringLiteral("Islandia"));
  m_french.addCode(QStringLiteral("Islande"));
  m_chinese.addCode(QStringLiteral("冰岛"));
  m_arabic.addCode(QStringLiteral("آيسلندا"));
  addCode(356, QStringLiteral("IND"));
  m_english.addCode(QStringLiteral("India"));
  m_russian.addCode(QStringLiteral("Индия"));
  m_spanish.addCode(QStringLiteral("India"));
  m_french.addCode(QStringLiteral("Inde"));
  m_chinese.addCode(QStringLiteral("印度"));
  m_arabic.addCode(QStringLiteral("الهند"));
  addCode(360, QStringLiteral("IDN"));
  m_english.addCode(QStringLiteral("Indonesia"));
  m_russian.addCode(QStringLiteral("Индонезия"));
  m_spanish.addCode(QStringLiteral("Indonesia"));
  m_french.addCode(QStringLiteral("Indonésie"));
  m_chinese.addCode(QStringLiteral("印度尼西亚"));
  m_arabic.addCode(QStringLiteral("إندونيسيا"));
  addCode(364, QStringLiteral("IRN"));
  m_english.addCode(QStringLiteral("Iran (Islamic Republic of)"));
  m_russian.addCode(QStringLiteral("Иран (Исламская Республика)"));
  m_spanish.addCode(QStringLiteral("Irán (República Islámica del)"));
  m_french.addCode(QStringLiteral("Iran (République islamique d’)"));
  m_chinese.addCode(QStringLiteral("伊朗伊斯兰共和国"));
  m_arabic.addCode(QStringLiteral("إيران (جمهورية - الإسلامية)"));
  addCode(368, QStringLiteral("IRQ"));
  m_english.addCode(QStringLiteral("Iraq"));
  m_russian.addCode(QStringLiteral("Ирак"));
  m_spanish.addCode(QStringLiteral("Iraq"));
  m_french.addCode(QStringLiteral("Iraq"));
  m_chinese.addCode(QStringLiteral("伊拉克"));
  m_arabic.addCode(QStringLiteral("العراق"));
  addCode(372, QStringLiteral("IRL"));
  m_english.addCode(QStringLiteral("Ireland"));
  m_russian.addCode(QStringLiteral("Ирландия"));
  m_spanish.addCode(QStringLiteral("Irlanda"));
  m_french.addCode(QStringLiteral("Irlande"));
  m_chinese.addCode(QStringLiteral("爱尔兰"));
  m_arabic.addCode(QStringLiteral("آيرلندا"));
  addCode(833, QStringLiteral("IMN"));
  m_english.addCode(QStringLiteral("Isle of Man"));
  m_russian.addCode(QStringLiteral("Остров Мэн"));
  m_spanish.addCode(QStringLiteral("Isla de Man"));
  m_french.addCode(QStringLiteral("Île de Man"));
  m_chinese.addCode(QStringLiteral("马恩岛"));
  m_arabic.addCode(QStringLiteral("جزيرة مان"));
  addCode(376, QStringLiteral("ISR"));
  m_english.addCode(QStringLiteral("Israel"));
  m_russian.addCode(QStringLiteral("Израиль"));
  m_spanish.addCode(QStringLiteral("Israel"));
  m_french.addCode(QStringLiteral("Israël"));
  m_chinese.addCode(QStringLiteral("以色列"));
  m_arabic.addCode(QStringLiteral("إسرائيل"));
  addCode(380, QStringLiteral("ITA"));
  m_english.addCode(QStringLiteral("Italy"));
  m_russian.addCode(QStringLiteral("Италия"));
  m_spanish.addCode(QStringLiteral("Italia"));
  m_french.addCode(QStringLiteral("Italie"));
  m_chinese.addCode(QStringLiteral("意大利"));
  m_arabic.addCode(QStringLiteral("إيطاليا"));
  addCode(388, QStringLiteral("JAM"));
  m_english.addCode(QStringLiteral("Jamaica"));
  m_russian.addCode(QStringLiteral("Ямайка"));
  m_spanish.addCode(QStringLiteral("Jamaica"));
  m_french.addCode(QStringLiteral("Jamaïque"));
  m_chinese.addCode(QStringLiteral("牙买加"));
  m_arabic.addCode(QStringLiteral("جامايكا"));
  addCode(392, QStringLiteral("JPN"));
  m_english.addCode(QStringLiteral("Japan"));
  m_russian.addCode(QStringLiteral("Япония"));
  m_spanish.addCode(QStringLiteral("Japón"));
  m_french.addCode(QStringLiteral("Japon"));
  m_chinese.addCode(QStringLiteral("日本"));
  m_arabic.addCode(QStringLiteral("اليابان"));
  addCode(832, QStringLiteral("JEY"));
  m_english.addCode(QStringLiteral("Jersey"));
  m_russian.addCode(QStringLiteral("Джерси"));
  m_spanish.addCode(QStringLiteral("Jersey"));
  m_french.addCode(QStringLiteral("Jersey"));
  m_chinese.addCode(QStringLiteral("泽西"));
  m_arabic.addCode(QStringLiteral("جيرسي"));
  addCode(400, QStringLiteral("JOR"));
  m_english.addCode(QStringLiteral("Jordan"));
  m_russian.addCode(QStringLiteral("Иордания"));
  m_spanish.addCode(QStringLiteral("Jordania"));
  m_french.addCode(QStringLiteral("Jordanie"));
  m_chinese.addCode(QStringLiteral("约旦"));
  m_arabic.addCode(QStringLiteral("الأردن"));
  addCode(398, QStringLiteral("KAZ"));
  m_english.addCode(QStringLiteral("Kazakhstan"));
  m_russian.addCode(QStringLiteral("Казахстан"));
  m_spanish.addCode(QStringLiteral("Kazajstán"));
  m_french.addCode(QStringLiteral("Kazakhstan"));
  m_chinese.addCode(QStringLiteral("哈萨克斯坦"));
  m_arabic.addCode(QStringLiteral("كازاخستان"));
  addCode(404, QStringLiteral("KEN"));
  m_english.addCode(QStringLiteral("Kenya"));
  m_russian.addCode(QStringLiteral("Кения"));
  m_spanish.addCode(QStringLiteral("Kenya"));
  m_french.addCode(QStringLiteral("Kenya"));
  m_chinese.addCode(QStringLiteral("肯尼亚"));
  m_arabic.addCode(QStringLiteral("كينيا"));
  addCode(296, QStringLiteral("KIR"));
  m_english.addCode(QStringLiteral("Kiribati"));
  m_russian.addCode(QStringLiteral("Кирибати"));
  m_spanish.addCode(QStringLiteral("Kiribati"));
  m_french.addCode(QStringLiteral("Kiribati"));
  m_chinese.addCode(QStringLiteral("基里巴斯"));
  m_arabic.addCode(QStringLiteral("كيريباس"));
  addCode(414, QStringLiteral("KWT"));
  m_english.addCode(QStringLiteral("Kuwait"));
  m_russian.addCode(QStringLiteral("Кувейт"));
  m_spanish.addCode(QStringLiteral("Kuwait"));
  m_french.addCode(QStringLiteral("Koweït"));
  m_chinese.addCode(QStringLiteral("科威特"));
  m_arabic.addCode(QStringLiteral("الكويت"));
  addCode(417, QStringLiteral("KGZ"));
  m_english.addCode(QStringLiteral("Kyrgyzstan"));
  m_russian.addCode(QStringLiteral("Кыргызстан"));
  m_spanish.addCode(QStringLiteral("Kirguistán"));
  m_french.addCode(QStringLiteral("Kirghizistan"));
  m_chinese.addCode(QStringLiteral("吉尔吉斯斯坦"));
  m_arabic.addCode(QStringLiteral("قيرغيزستان"));
  addCode(418, QStringLiteral("LAO"));
  m_english.addCode(QStringLiteral("Lao People's Democratic Republic"));
  m_russian.addCode(
    QStringLiteral("Лаосская Народно-Демократическая Республика"));
  m_spanish.addCode(QStringLiteral("República Democrática Popular Lao"));
  m_french.addCode(QStringLiteral("Palaos"));
  m_chinese.addCode(QStringLiteral("老挝人民民主共和国"));
  m_arabic.addCode(QStringLiteral("جمهورية لاو الديمقراطية الشعبية"));
  addCode(428, QStringLiteral("LVA"));
  m_english.addCode(QStringLiteral("Latvia"));
  m_russian.addCode(QStringLiteral("Латвия"));
  m_spanish.addCode(QStringLiteral("Letonia"));
  m_french.addCode(QStringLiteral("Lettonie"));
  m_chinese.addCode(QStringLiteral("拉脱维亚"));
  m_arabic.addCode(QStringLiteral("لاتفيا"));
  addCode(422, QStringLiteral("LBN"));
  m_english.addCode(QStringLiteral("Lebanon"));
  m_russian.addCode(QStringLiteral("Ливан"));
  m_spanish.addCode(QStringLiteral("Líbano"));
  m_french.addCode(QStringLiteral("Liban"));
  m_chinese.addCode(QStringLiteral("黎巴嫩"));
  m_arabic.addCode(QStringLiteral("لبنان"));
  addCode(430, QStringLiteral("LBR"));
  m_english.addCode(QStringLiteral("Liberia"));
  m_russian.addCode(QStringLiteral("Либерия"));
  m_spanish.addCode(QStringLiteral("Liberia"));
  m_french.addCode(QStringLiteral("Libéria"));
  m_chinese.addCode(QStringLiteral("利比里亚"));
  m_arabic.addCode(QStringLiteral("ARABIC"));
  addCode(426, QStringLiteral("LSO"));
  m_english.addCode(QStringLiteral("Lesotho"));
  m_russian.addCode(QStringLiteral("Лесото"));
  m_spanish.addCode(QStringLiteral("Lesotho"));
  m_french.addCode(QStringLiteral("Lesotho"));
  m_chinese.addCode(QStringLiteral("莱索托"));
  m_arabic.addCode(QStringLiteral("ليسوتو"));
  addCode(434, QStringLiteral("LBY"));
  m_english.addCode(QStringLiteral("Libya"));
  m_russian.addCode(QStringLiteral("Ливия"));
  m_spanish.addCode(QStringLiteral("Libia"));
  m_french.addCode(QStringLiteral("Libye"));
  m_chinese.addCode(QStringLiteral("利比亚"));
  m_arabic.addCode(QStringLiteral("ليبيا"));
  addCode(438, QStringLiteral("LIE"));
  m_english.addCode(QStringLiteral("Liechtenstein"));
  m_russian.addCode(QStringLiteral("Лихтенштейн"));
  m_spanish.addCode(QStringLiteral("Liechtenstein"));
  m_french.addCode(QStringLiteral("Liechtenstein"));
  m_chinese.addCode(QStringLiteral("列支敦士登"));
  m_arabic.addCode(QStringLiteral("ليختنشتاين"));
  addCode(440, QStringLiteral("LTU"));
  m_english.addCode(QStringLiteral("Lithuania"));
  m_russian.addCode(QStringLiteral("Литва"));
  m_spanish.addCode(QStringLiteral("Lituania"));
  m_french.addCode(QStringLiteral("Lituanie"));
  m_chinese.addCode(QStringLiteral("立陶宛"));
  m_arabic.addCode(QStringLiteral("ليتوانيا"));
  addCode(442, QStringLiteral("LUX"));
  m_english.addCode(QStringLiteral("Luxembourg"));
  m_russian.addCode(QStringLiteral("Люксембург"));
  m_spanish.addCode(QStringLiteral("Luxemburgo"));
  m_french.addCode(QStringLiteral("Luxembourg"));
  m_chinese.addCode(QStringLiteral("卢森堡"));
  m_arabic.addCode(QStringLiteral("لكسمبرغ"));
  addCode(450, QStringLiteral("MDG"));
  m_english.addCode(QStringLiteral("Madagascar"));
  m_russian.addCode(QStringLiteral("Мадагаскар"));
  m_spanish.addCode(QStringLiteral("Madagascar"));
  m_french.addCode(QStringLiteral("Madagascar"));
  m_chinese.addCode(QStringLiteral("马达加斯加"));
  m_arabic.addCode(QStringLiteral("مدغشقر"));
  addCode(454, QStringLiteral("MWI"));
  m_english.addCode(QStringLiteral("Malawi"));
  m_russian.addCode(QStringLiteral("Малави"));
  m_spanish.addCode(QStringLiteral("Malawi"));
  m_french.addCode(QStringLiteral("Malawi"));
  m_chinese.addCode(QStringLiteral("马拉维"));
  m_arabic.addCode(QStringLiteral("ملاوي"));
  addCode(458, QStringLiteral("MYS"));
  m_english.addCode(QStringLiteral("Malaysia"));
  m_russian.addCode(QStringLiteral("Малайзия"));
  m_spanish.addCode(QStringLiteral("Malasia"));
  m_french.addCode(QStringLiteral("Malaisie"));
  m_chinese.addCode(QStringLiteral("马来西亚"));
  m_arabic.addCode(QStringLiteral("ماليزيا"));
  addCode(462, QStringLiteral("MDV"));
  m_english.addCode(QStringLiteral("Maldives"));
  m_russian.addCode(QStringLiteral("Мальдивские Острова"));
  m_spanish.addCode(QStringLiteral("Maldivas"));
  m_french.addCode(QStringLiteral("Maldives"));
  m_chinese.addCode(QStringLiteral("马尔代夫"));
  m_arabic.addCode(QStringLiteral("ملديف"));
  addCode(466, QStringLiteral("MLI"));
  m_english.addCode(QStringLiteral("Mali"));
  m_russian.addCode(QStringLiteral("Мали"));
  m_spanish.addCode(QStringLiteral("Malí"));
  m_french.addCode(QStringLiteral("Mali"));
  m_chinese.addCode(QStringLiteral("马里"));
  m_arabic.addCode(QStringLiteral("مالي"));
  addCode(470, QStringLiteral("MLT"));
  m_english.addCode(QStringLiteral("Malta"));
  m_russian.addCode(QStringLiteral("Мальта"));
  m_spanish.addCode(QStringLiteral("Malta"));
  m_french.addCode(QStringLiteral("Malte"));
  m_chinese.addCode(QStringLiteral("马耳他"));
  m_arabic.addCode(QStringLiteral("مالطة"));
  addCode(584, QStringLiteral("ALPHA"));
  m_english.addCode(QStringLiteral("Marshall Islands"));
  m_arabic.addCode(QStringLiteral("جزر مارشال"));
  m_chinese.addCode(QStringLiteral("马绍尔群岛"));
  m_french.addCode(QStringLiteral("Îles Marshall"));
  m_russian.addCode(QStringLiteral("Маршалловы Острова"));
  m_spanish.addCode(QStringLiteral("Islas Marshall"));
  addCode(474, QStringLiteral("MTQ"));
  m_english.addCode(QStringLiteral("Martinique"));
  m_arabic.addCode(QStringLiteral("مارتينيك"));
  m_chinese.addCode(QStringLiteral("马提尼克"));
  m_french.addCode(QStringLiteral("Martinique"));
  m_russian.addCode(QStringLiteral("Мартиника"));
  m_spanish.addCode(QStringLiteral("Martinica"));
  addCode(478, QStringLiteral("MRT"));
  m_english.addCode(QStringLiteral("Mauritania"));
  m_arabic.addCode(QStringLiteral("موريتانيا"));
  m_chinese.addCode(QStringLiteral("毛里塔尼亚"));
  m_french.addCode(QStringLiteral("Mauritanie"));
  m_russian.addCode(QStringLiteral("Мавритания"));
  m_spanish.addCode(QStringLiteral("Mauritania"));
  addCode(480, QStringLiteral("MUS"));
  m_english.addCode(QStringLiteral("Mauritius"));
  m_arabic.addCode(QStringLiteral("موريشيوس"));
  m_chinese.addCode(QStringLiteral("毛里求斯"));
  m_french.addCode(QStringLiteral("Maurice"));
  m_russian.addCode(QStringLiteral("Маврикий"));
  m_spanish.addCode(QStringLiteral("Mauricio"));
  addCode(175, QStringLiteral("MYT"));
  m_english.addCode(QStringLiteral("Mayotte"));
  m_arabic.addCode(QStringLiteral("مايوت"));
  m_chinese.addCode(QStringLiteral("马约特"));
  m_french.addCode(QStringLiteral("Mayotte"));
  m_russian.addCode(QStringLiteral("Остров Майотта"));
  m_spanish.addCode(QStringLiteral("Mayotte"));
  addCode(484, QStringLiteral("MEX"));
  m_english.addCode(QStringLiteral("Mexico"));
  m_arabic.addCode(QStringLiteral("المكسيك"));
  m_chinese.addCode(QStringLiteral("墨西哥"));
  m_french.addCode(QStringLiteral("Mexique"));
  m_russian.addCode(QStringLiteral("Мексика"));
  m_spanish.addCode(QStringLiteral("México"));
  addCode(583, QStringLiteral("FSM"));
  m_english.addCode(QStringLiteral("Micronesia (Federated States of)"));
  m_arabic.addCode(QStringLiteral("ميكرونيزيا (ولايات - الموحدة)"));
  m_chinese.addCode(QStringLiteral("密克罗尼西亚联邦"));
  m_french.addCode(QStringLiteral("Micronésie (États fédérés de)"));
  m_russian.addCode(QStringLiteral("Микронезия (Федеративные Штаты)"));
  m_spanish.addCode(QStringLiteral("Micronesia (Estados Federados de)"));
  addCode(492, QStringLiteral("MCO"));
  m_english.addCode(QStringLiteral("Monaco"));
  m_arabic.addCode(QStringLiteral("موناكو"));
  m_chinese.addCode(QStringLiteral("摩纳哥"));
  m_french.addCode(QStringLiteral("Monaco"));
  m_russian.addCode(QStringLiteral("Монако"));
  m_spanish.addCode(QStringLiteral("Mónaco"));
  addCode(496, QStringLiteral("MNG"));
  m_english.addCode(QStringLiteral("Mongolia"));
  m_arabic.addCode(QStringLiteral("منغوليا"));
  m_chinese.addCode(QStringLiteral("蒙古"));
  m_french.addCode(QStringLiteral("Mongolie"));
  m_russian.addCode(QStringLiteral("Монголия"));
  m_spanish.addCode(QStringLiteral("Mongolia"));
  addCode(499, QStringLiteral("MNE"));
  m_english.addCode(QStringLiteral("Montenegro"));
  m_arabic.addCode(QStringLiteral("الجبل الأسود"));
  m_chinese.addCode(QStringLiteral("黑山"));
  m_french.addCode(QStringLiteral("Monténégro"));
  m_russian.addCode(QStringLiteral("Черногория"));
  m_spanish.addCode(QStringLiteral("Montenegro"));
  addCode(500, QStringLiteral("MSR"));
  m_english.addCode(QStringLiteral("Montserrat"));
  m_arabic.addCode(QStringLiteral("مونتسيرات"));
  m_chinese.addCode(QStringLiteral("蒙特塞拉特"));
  m_french.addCode(QStringLiteral("Montserrat"));
  m_russian.addCode(QStringLiteral("Монтсеррат"));
  m_spanish.addCode(QStringLiteral("Montserrat"));
  addCode(504, QStringLiteral("MAR"));
  m_english.addCode(QStringLiteral("Morocco"));
  m_arabic.addCode(QStringLiteral("المغرب"));
  m_chinese.addCode(QStringLiteral("摩洛哥"));
  m_french.addCode(QStringLiteral("Maroc"));
  m_russian.addCode(QStringLiteral("Марокко"));
  m_spanish.addCode(QStringLiteral("Marruecos"));
  addCode(508, QStringLiteral("MOZ"));
  m_english.addCode(QStringLiteral("Mozambique"));
  m_arabic.addCode(QStringLiteral("موزامبيق"));
  m_chinese.addCode(QStringLiteral("莫桑比克"));
  m_french.addCode(QStringLiteral("Mozambique"));
  m_russian.addCode(QStringLiteral("Мозамбик"));
  m_spanish.addCode(QStringLiteral("Mozambique"));
  addCode(104, QStringLiteral("MMR"));
  m_english.addCode(QStringLiteral("Myanmar"));
  m_arabic.addCode(QStringLiteral("ميانمار"));
  m_chinese.addCode(QStringLiteral("缅甸"));
  m_french.addCode(QStringLiteral("Myanmar"));
  m_russian.addCode(QStringLiteral("Мьянма"));
  m_spanish.addCode(QStringLiteral("Myanmar"));
  addCode(516, QStringLiteral("NAM"));
  m_english.addCode(QStringLiteral("Namibia"));
  m_arabic.addCode(QStringLiteral("ناميبيا"));
  m_chinese.addCode(QStringLiteral("纳米比亚"));
  m_french.addCode(QStringLiteral("Namibie"));
  m_russian.addCode(QStringLiteral("Намибия"));
  m_spanish.addCode(QStringLiteral("Namibia"));
  addCode(520, QStringLiteral("NRU"));
  m_english.addCode(QStringLiteral("Nauru"));
  m_arabic.addCode(QStringLiteral("ناورو"));
  m_chinese.addCode(QStringLiteral("瑙鲁"));
  m_french.addCode(QStringLiteral("Nauru"));
  m_russian.addCode(QStringLiteral("Науру"));
  m_spanish.addCode(QStringLiteral("Nauru"));
  addCode(524, QStringLiteral("NPL"));
  m_english.addCode(QStringLiteral("Nepal"));
  m_arabic.addCode(QStringLiteral("نيبال"));
  m_chinese.addCode(QStringLiteral("尼泊尔"));
  m_french.addCode(QStringLiteral("Népal"));
  m_russian.addCode(QStringLiteral("Непал"));
  m_spanish.addCode(QStringLiteral("Nepal"));
  addCode(528, QStringLiteral("NLD"));
  m_english.addCode(QStringLiteral("Netherlands"));
  m_arabic.addCode(QStringLiteral("هولندا"));
  m_chinese.addCode(QStringLiteral("荷兰"));
  m_french.addCode(QStringLiteral("Pays-Bas"));
  m_russian.addCode(QStringLiteral("Нидерланды"));
  m_spanish.addCode(QStringLiteral("Países Bajos"));
  addCode(540, QStringLiteral("NCL"));
  m_english.addCode(QStringLiteral("New Caledonia"));
  m_arabic.addCode(QStringLiteral("كاليدونيا الجديدة"));
  m_chinese.addCode(QStringLiteral("新喀里多尼亚"));
  m_french.addCode(QStringLiteral("Nouvelle-Calédonie"));
  m_russian.addCode(QStringLiteral("Новая Каледония"));
  m_spanish.addCode(QStringLiteral("Nueva Caledonia"));
  addCode(554, QStringLiteral("NZL"));
  m_english.addCode(QStringLiteral("New Zealand"));
  m_arabic.addCode(QStringLiteral("نيوزيلندا"));
  m_chinese.addCode(QStringLiteral("新西兰"));
  m_french.addCode(QStringLiteral("Nouvelle-Zélande"));
  m_russian.addCode(QStringLiteral("Новая Зеландия"));
  m_spanish.addCode(QStringLiteral("Nueva Zelandia"));
  addCode(558, QStringLiteral("NIC"));
  m_english.addCode(QStringLiteral("Nicaragua"));
  m_arabic.addCode(QStringLiteral("نيكاراغوا"));
  m_chinese.addCode(QStringLiteral("尼加拉瓜"));
  m_french.addCode(QStringLiteral("Nicaragua"));
  m_russian.addCode(QStringLiteral("Никарагуа"));
  m_spanish.addCode(QStringLiteral("Nicaragua"));
  addCode(562, QStringLiteral("NER"));
  m_english.addCode(QStringLiteral("Niger"));
  m_arabic.addCode(QStringLiteral("النيجر"));
  m_chinese.addCode(QStringLiteral("尼日尔"));
  m_french.addCode(QStringLiteral("Niger"));
  m_russian.addCode(QStringLiteral("Нигер"));
  m_spanish.addCode(QStringLiteral("Níger"));
  addCode(566, QStringLiteral("NGA"));
  m_english.addCode(QStringLiteral("Nigeria"));
  m_arabic.addCode(QStringLiteral("نيجيريا"));
  m_chinese.addCode(QStringLiteral("尼日利亚"));
  m_french.addCode(QStringLiteral("Nigéria"));
  m_russian.addCode(QStringLiteral("Нигерия"));
  m_spanish.addCode(QStringLiteral("Nigeria"));
  addCode(570, QStringLiteral("NIU"));
  m_english.addCode(QStringLiteral("Niue"));
  m_arabic.addCode(QStringLiteral("نيوي"));
  m_chinese.addCode(QStringLiteral("纽埃"));
  m_french.addCode(QStringLiteral("Nioué"));
  m_russian.addCode(QStringLiteral("Ниуэ"));
  m_spanish.addCode(QStringLiteral("Niue"));
  addCode(574, QStringLiteral("NFK"));
  m_english.addCode(QStringLiteral("Norfolk Island"));
  m_arabic.addCode(QStringLiteral("جزيرة نورفولك"));
  m_chinese.addCode(QStringLiteral("诺福克岛"));
  m_french.addCode(QStringLiteral("Île Norfolk"));
  m_russian.addCode(QStringLiteral("Остров Норфолк"));
  m_spanish.addCode(QStringLiteral("Isla Norfolk"));
  addCode(807, QStringLiteral("MKD"));
  m_english.addCode(QStringLiteral("North Macedonia"));
  m_arabic.addCode(QStringLiteral("مقدونيا الشمالية"));
  m_chinese.addCode(QStringLiteral("北马其顿"));
  m_french.addCode(QStringLiteral("Macédoine du Nord"));
  m_russian.addCode(QStringLiteral("Северная Македония"));
  m_spanish.addCode(QStringLiteral("Macedonia del Norte"));
  addCode(580, QStringLiteral("MNP"));
  m_english.addCode(QStringLiteral("Northern Mariana Islands"));
  m_arabic.addCode(QStringLiteral("جزر ماريانا الشمالية"));
  m_chinese.addCode(QStringLiteral("北马里亚纳群岛"));
  m_french.addCode(QStringLiteral("Îles Mariannes du Nord"));
  m_russian.addCode(QStringLiteral("Северные Марианские острова"));
  m_spanish.addCode(QStringLiteral("Islas Marianas Septentrionales"));
  addCode(578, QStringLiteral("NOR"));
  m_english.addCode(QStringLiteral("Norway"));
  m_arabic.addCode(QStringLiteral("النرويج"));
  m_chinese.addCode(QStringLiteral("挪威"));
  m_french.addCode(QStringLiteral("Norvège"));
  m_russian.addCode(QStringLiteral("Норвегия"));
  m_spanish.addCode(QStringLiteral("Noruega"));
  addCode(512, QStringLiteral("OMN"));
  m_english.addCode(QStringLiteral("Oman"));
  m_arabic.addCode(QStringLiteral("عمان"));
  m_chinese.addCode(QStringLiteral("阿曼"));
  m_french.addCode(QStringLiteral("Oman"));
  m_russian.addCode(QStringLiteral("Оман"));
  m_spanish.addCode(QStringLiteral("Omán"));
  addCode(586, QStringLiteral("PAK"));
  m_english.addCode(QStringLiteral("Pakistan"));
  m_arabic.addCode(QStringLiteral("باكستان"));
  m_chinese.addCode(QStringLiteral("巴基斯坦"));
  m_french.addCode(QStringLiteral("Pakistan"));
  m_russian.addCode(QStringLiteral("Пакистан"));
  m_spanish.addCode(QStringLiteral("Pakistán"));
  addCode(585, QStringLiteral("PLW"));
  m_english.addCode(QStringLiteral("Palau"));
  m_arabic.addCode(QStringLiteral("بالاو"));
  m_chinese.addCode(QStringLiteral("帕劳"));
  m_french.addCode(QStringLiteral("Palaos"));
  m_russian.addCode(QStringLiteral("Палау"));
  m_spanish.addCode(QStringLiteral("Palau"));
  addCode(591, QStringLiteral("PAN"));
  m_english.addCode(QStringLiteral("Panama"));
  m_arabic.addCode(QStringLiteral("بنما"));
  m_chinese.addCode(QStringLiteral("巴拿马"));
  m_french.addCode(QStringLiteral("Panama"));
  m_russian.addCode(QStringLiteral("Панама"));
  m_spanish.addCode(QStringLiteral("Panamá"));
  addCode(598, QStringLiteral("PNG"));
  m_english.addCode(QStringLiteral("Papua New Guinea"));
  m_arabic.addCode(QStringLiteral("بابوا غينيا الجديدة"));
  m_chinese.addCode(QStringLiteral("巴布亚新几内亚"));
  m_french.addCode(QStringLiteral("Papouasie-Nouvelle-Guinée"));
  m_russian.addCode(QStringLiteral("Папуа-Новая Гвинея"));
  m_spanish.addCode(QStringLiteral("Papua Nueva Guinea"));
  addCode(600, QStringLiteral("PRY"));
  m_english.addCode(QStringLiteral("Paraguay"));
  m_arabic.addCode(QStringLiteral("باراغواي"));
  m_chinese.addCode(QStringLiteral("巴拉圭"));
  m_french.addCode(QStringLiteral("Paraguay"));
  m_russian.addCode(QStringLiteral("Парагвай"));
  m_spanish.addCode(QStringLiteral("Paraguay"));
  addCode(604, QStringLiteral("PER"));
  m_english.addCode(QStringLiteral("Peru"));
  m_arabic.addCode(QStringLiteral("بيرو"));
  m_chinese.addCode(QStringLiteral("秘鲁"));
  m_french.addCode(QStringLiteral("Pérou"));
  m_russian.addCode(QStringLiteral("Перу"));
  m_spanish.addCode(QStringLiteral("Perú"));
  addCode(612, QStringLiteral("PCN"));
  m_english.addCode(QStringLiteral("Pitcairn"));
  m_arabic.addCode(QStringLiteral("بيتكرن"));
  m_chinese.addCode(QStringLiteral("皮特凯恩"));
  m_french.addCode(QStringLiteral("Pitcairn"));
  m_russian.addCode(QStringLiteral("Питкэрн"));
  m_spanish.addCode(QStringLiteral("Pitcairn"));
  addCode(608, QStringLiteral("PHL"));
  m_english.addCode(QStringLiteral("Philippines"));
  m_arabic.addCode(QStringLiteral("الفلبين"));
  m_chinese.addCode(QStringLiteral("菲律宾"));
  m_french.addCode(QStringLiteral("Philippines"));
  m_russian.addCode(QStringLiteral("Филиппины"));
  m_spanish.addCode(QStringLiteral("Filipinas"));
  addCode(616, QStringLiteral("POL"));
  m_english.addCode(QStringLiteral("Poland"));
  m_arabic.addCode(QStringLiteral("بولندا"));
  m_chinese.addCode(QStringLiteral("波兰"));
  m_french.addCode(QStringLiteral("Pologne"));
  m_russian.addCode(QStringLiteral("Польша"));
  m_spanish.addCode(QStringLiteral("Polonia"));
  addCode(620, QStringLiteral("PRT"));
  m_english.addCode(QStringLiteral("Portugal"));
  m_arabic.addCode(QStringLiteral("البرتغال"));
  m_chinese.addCode(QStringLiteral("葡萄牙"));
  m_french.addCode(QStringLiteral("Portugal"));
  m_russian.addCode(QStringLiteral("Португалия"));
  m_spanish.addCode(QStringLiteral("Portugal"));
  addCode(630, QStringLiteral("PRI"));
  m_english.addCode(QStringLiteral("Puerto Rico"));
  m_arabic.addCode(QStringLiteral("بورتوريكو"));
  m_chinese.addCode(QStringLiteral("波多黎各"));
  m_french.addCode(QStringLiteral("Porto Rico"));
  m_russian.addCode(QStringLiteral("Пуэрто-Рико"));
  m_spanish.addCode(QStringLiteral("Puerto Rico"));
  addCode(634, QStringLiteral("QAT"));
  m_english.addCode(QStringLiteral("Qatar"));
  m_arabic.addCode(QStringLiteral("قطر"));
  m_chinese.addCode(QStringLiteral("卡塔尔"));
  m_french.addCode(QStringLiteral("Qatar"));
  m_russian.addCode(QStringLiteral("Катар"));
  m_spanish.addCode(QStringLiteral("Qatar"));
  addCode(410, QStringLiteral("KOR"));
  m_english.addCode(QStringLiteral("Republic of Korea"));
  m_arabic.addCode(QStringLiteral("جمهورية كوريا"));
  m_chinese.addCode(QStringLiteral("大韩民国"));
  m_french.addCode(QStringLiteral("République de Corée"));
  m_russian.addCode(QStringLiteral("Республика Корея"));
  m_spanish.addCode(QStringLiteral("República de Corea"));
  addCode(498, QStringLiteral("MDA"));
  m_english.addCode(QStringLiteral("Republic of Moldova"));
  m_arabic.addCode(QStringLiteral("جمهورية مولدوفا"));
  m_chinese.addCode(QStringLiteral("摩尔多瓦共和国"));
  m_french.addCode(QStringLiteral("République de Moldova"));
  m_russian.addCode(QStringLiteral("Республика Молдова"));
  m_spanish.addCode(QStringLiteral("República de Moldova"));
  addCode(638, QStringLiteral("REU"));
  m_english.addCode(QStringLiteral("Réunion"));
  m_arabic.addCode(QStringLiteral("ريونيون"));
  m_chinese.addCode(QStringLiteral("留尼汪"));
  m_french.addCode(QStringLiteral("Réunion"));
  m_russian.addCode(QStringLiteral("Реюньон"));
  m_spanish.addCode(QStringLiteral("Reunión"));
  addCode(642, QStringLiteral("ROU"));
  m_english.addCode(QStringLiteral("Romania"));
  m_arabic.addCode(QStringLiteral("رومانيا"));
  m_chinese.addCode(QStringLiteral("罗马尼亚"));
  m_french.addCode(QStringLiteral("Roumanie"));
  m_russian.addCode(QStringLiteral("Румыния"));
  m_spanish.addCode(QStringLiteral("Rumania"));
  addCode(643, QStringLiteral("RUS"));
  m_english.addCode(QStringLiteral("Russian Federation"));
  m_arabic.addCode(QStringLiteral("الاتحاد الروسي"));
  m_chinese.addCode(QStringLiteral("俄罗斯联邦"));
  m_french.addCode(QStringLiteral("Fédération de Russie"));
  m_russian.addCode(QStringLiteral("Российская Федерация"));
  m_spanish.addCode(QStringLiteral("Federación de Rusia"));
  addCode(646, QStringLiteral("RWA"));
  m_english.addCode(QStringLiteral("Rwanda"));
  m_arabic.addCode(QStringLiteral("رواندا"));
  m_chinese.addCode(QStringLiteral("卢旺达"));
  m_french.addCode(QStringLiteral("Rwanda"));
  m_russian.addCode(QStringLiteral("Руанда"));
  m_spanish.addCode(QStringLiteral("Rwanda"));
  addCode(652, QStringLiteral("BLM"));
  m_english.addCode(QStringLiteral("Saint Barthélemy"));
  m_arabic.addCode(QStringLiteral("سان بارتليمي"));
  m_chinese.addCode(QStringLiteral("圣巴泰勒米"));
  m_french.addCode(QStringLiteral("Saint-Barthélemy"));
  m_russian.addCode(QStringLiteral("Сен-Бартелеми"));
  m_spanish.addCode(QStringLiteral("San Barthélemy"));
  addCode(654, QStringLiteral("SHN"));
  m_english.addCode(QStringLiteral("Saint Helena"));
  m_arabic.addCode(QStringLiteral("سانت هيلانة"));
  m_chinese.addCode(QStringLiteral("圣赫勒拿"));
  m_french.addCode(QStringLiteral("Sainte-Hélène"));
  m_russian.addCode(QStringLiteral("Остров Святой Елены"));
  m_spanish.addCode(QStringLiteral("Santa Elena"));
  addCode(659, QStringLiteral("KNA"));
  m_english.addCode(QStringLiteral("Saint Kitts and Nevis"));
  m_arabic.addCode(QStringLiteral("سانت كيتس ونيفس"));
  m_chinese.addCode(QStringLiteral("圣基茨和尼维斯"));
  m_french.addCode(QStringLiteral("Saint-Kitts-et-Nevis"));
  m_russian.addCode(QStringLiteral("Сент-Китс и Невис"));
  m_spanish.addCode(QStringLiteral("Saint Kitts y Nevis"));
  addCode(662, QStringLiteral("LCA"));
  m_english.addCode(QStringLiteral("Saint Lucia"));
  m_arabic.addCode(QStringLiteral("سانت لوسيا"));
  m_chinese.addCode(QStringLiteral("圣卢西亚"));
  m_french.addCode(QStringLiteral("Sainte-Lucie"));
  m_russian.addCode(QStringLiteral("Сент-Люсия"));
  m_spanish.addCode(QStringLiteral("Santa Lucía"));
  addCode(663, QStringLiteral("MAF"));
  m_english.addCode(QStringLiteral("Saint Martin (French Part)"));
  m_arabic.addCode(QStringLiteral("سانت مارتن (الجزء الفرنسي)"));
  m_chinese.addCode(QStringLiteral("圣马丁（法属）"));
  m_french.addCode(QStringLiteral("Saint-Martin (partie française)"));
  m_russian.addCode(QStringLiteral("Сен-Мартен (французская часть)"));
  m_spanish.addCode(QStringLiteral("San Martín (parte francesa)"));
  addCode(666, QStringLiteral("SPM"));
  m_english.addCode(QStringLiteral("Saint Pierre and Miquelon"));
  m_arabic.addCode(QStringLiteral("سان بيير وميكلون"));
  m_chinese.addCode(QStringLiteral("圣皮埃尔和密克隆"));
  m_french.addCode(QStringLiteral("Saint-Pierre-et-Miquelon"));
  m_russian.addCode(QStringLiteral("Сен-Пьер и Микелон"));
  m_spanish.addCode(QStringLiteral("San Pedro y Miquelón"));
  addCode(670, QStringLiteral("VCT"));
  m_english.addCode(QStringLiteral("Saint Vincent and the Grenadines"));
  m_arabic.addCode(QStringLiteral("سانت فنسنت وجزر غرينادين"));
  m_chinese.addCode(QStringLiteral("圣文森特和格林纳丁斯"));
  m_french.addCode(QStringLiteral("Saint-Vincent-et-les Grenadines"));
  m_russian.addCode(QStringLiteral("Сент-Винсент и Гренадины"));
  m_spanish.addCode(QStringLiteral("San Vicente y las Granadinas"));
  addCode(882, QStringLiteral("WSM"));
  m_english.addCode(QStringLiteral("Samoa"));
  m_arabic.addCode(QStringLiteral("ساموا"));
  m_chinese.addCode(QStringLiteral("萨摩亚"));
  m_french.addCode(QStringLiteral("Samoa"));
  m_russian.addCode(QStringLiteral("Самоа"));
  m_spanish.addCode(QStringLiteral("Samoa"));
  addCode(674, QStringLiteral("SMR"));
  m_english.addCode(QStringLiteral("San Marino"));
  m_arabic.addCode(QStringLiteral("سان مارينو"));
  m_chinese.addCode(QStringLiteral("圣马力诺"));
  m_french.addCode(QStringLiteral("Saint-Marin"));
  m_russian.addCode(QStringLiteral("Сан-Марино"));
  m_spanish.addCode(QStringLiteral("San Marino"));
  addCode(678, QStringLiteral("STP"));
  m_english.addCode(QStringLiteral("Sao Tome and Principe"));
  m_arabic.addCode(QStringLiteral("سان تومي وبرينسيبي"));
  m_chinese.addCode(QStringLiteral("圣多美和普林西比"));
  m_french.addCode(QStringLiteral("Sao Tomé-et-Principe"));
  m_russian.addCode(QStringLiteral("Сан-Томе и Принсипи"));
  m_spanish.addCode(QStringLiteral("Santo Tomé y Príncipe"));
  addCode(680, QStringLiteral(""));
  m_english.addCode(QStringLiteral("Sark"));
  m_arabic.addCode(QStringLiteral("سارك"));
  m_chinese.addCode(QStringLiteral("萨克"));
  m_french.addCode(QStringLiteral("Sercq"));
  m_russian.addCode(QStringLiteral("Сарк"));
  m_spanish.addCode(QStringLiteral("Sark"));
  addCode(682, QStringLiteral("SAU"));
  m_english.addCode(QStringLiteral("Saudi Arabia"));
  m_arabic.addCode(QStringLiteral("المملكة العربية السعودية"));
  m_chinese.addCode(QStringLiteral("沙特阿拉伯"));
  m_french.addCode(QStringLiteral("Arabie saoudite"));
  m_russian.addCode(QStringLiteral("Саудовская Аравия"));
  m_spanish.addCode(QStringLiteral("Arabia Saudita"));
  addCode(686, QStringLiteral("SEN"));
  m_english.addCode(QStringLiteral("Senegal"));
  m_arabic.addCode(QStringLiteral("السنغال"));
  m_chinese.addCode(QStringLiteral("塞内加尔"));
  m_french.addCode(QStringLiteral("Sénégal"));
  m_russian.addCode(QStringLiteral("Сенегал"));
  m_spanish.addCode(QStringLiteral("Senegal"));
  addCode(688, QStringLiteral("SRB"));
  m_english.addCode(QStringLiteral("Serbia"));
  m_arabic.addCode(QStringLiteral("صربيا"));
  m_chinese.addCode(QStringLiteral("塞尔维亚"));
  m_french.addCode(QStringLiteral("Serbie"));
  m_russian.addCode(QStringLiteral("Сербия"));
  m_spanish.addCode(QStringLiteral("Serbia"));
  addCode(690, QStringLiteral("SYC"));
  m_english.addCode(QStringLiteral("Seychelles"));
  m_arabic.addCode(QStringLiteral("سيشيل"));
  m_chinese.addCode(QStringLiteral("塞舌尔"));
  m_french.addCode(QStringLiteral("Seychelles"));
  m_russian.addCode(QStringLiteral("Сейшельские Острова"));
  m_spanish.addCode(QStringLiteral("Seychelles"));
  addCode(694, QStringLiteral("SLE"));
  m_english.addCode(QStringLiteral("Sierra Leone"));
  m_arabic.addCode(QStringLiteral("سيراليون"));
  m_chinese.addCode(QStringLiteral("塞拉利昂"));
  m_french.addCode(QStringLiteral("Sierra Leone"));
  m_russian.addCode(QStringLiteral("Сьерра-Леоне"));
  m_spanish.addCode(QStringLiteral("Sierra Leona"));
  addCode(702, QStringLiteral("SGP"));
  m_english.addCode(QStringLiteral("Singapore"));
  m_arabic.addCode(QStringLiteral("سنغافورة"));
  m_chinese.addCode(QStringLiteral("新加坡"));
  m_french.addCode(QStringLiteral("Singapour"));
  m_russian.addCode(QStringLiteral("Сингапур"));
  m_spanish.addCode(QStringLiteral("Singapur"));
  addCode(534, QStringLiteral("SXM"));
  m_english.addCode(QStringLiteral("Sint Maarten (Dutch part)"));
  m_arabic.addCode(QStringLiteral("سانت مارتن (الجزء الهولندي)"));
  m_chinese.addCode(QStringLiteral("圣马丁（荷属）"));
  m_french.addCode(QStringLiteral("Saint-Martin (partie néerlandaise)"));
  m_russian.addCode(QStringLiteral("Синт-Мартен (нидерландская часть)"));
  m_spanish.addCode(QStringLiteral("San Martín (parte Holandesa)"));
  addCode(703, QStringLiteral("SVK"));
  m_english.addCode(QStringLiteral("Slovakia"));
  m_arabic.addCode(QStringLiteral("سلوفاكيا"));
  m_chinese.addCode(QStringLiteral("斯洛伐克"));
  m_french.addCode(QStringLiteral("Slovaquie"));
  m_russian.addCode(QStringLiteral("Словакия"));
  m_spanish.addCode(QStringLiteral("Eslovaquia"));
  addCode(705, QStringLiteral("SVN"));
  m_english.addCode(QStringLiteral("Slovenia"));
  m_arabic.addCode(QStringLiteral("سلوفينيا"));
  m_chinese.addCode(QStringLiteral("斯洛文尼亚"));
  m_french.addCode(QStringLiteral("Slovénie"));
  m_russian.addCode(QStringLiteral("Словения"));
  m_spanish.addCode(QStringLiteral("Eslovenia"));
  addCode(90, QStringLiteral("SLB"));
  m_english.addCode(QStringLiteral("Solomon Islands"));
  m_arabic.addCode(QStringLiteral("جزر سليمان"));
  m_chinese.addCode(QStringLiteral("所罗门群岛"));
  m_french.addCode(QStringLiteral("Îles Salomon"));
  m_russian.addCode(QStringLiteral("Соломоновы Острова"));
  m_spanish.addCode(QStringLiteral("Islas Salomón"));
  addCode(706, QStringLiteral("ZAF"));
  m_english.addCode(QStringLiteral("Somalia"));
  m_arabic.addCode(QStringLiteral("الصومال"));
  m_chinese.addCode(QStringLiteral("索马里"));
  m_french.addCode(QStringLiteral("Somalie"));
  m_russian.addCode(QStringLiteral("Сомали"));
  m_spanish.addCode(QStringLiteral("Somalia"));
  addCode(710, QStringLiteral("ALPHA"));
  m_english.addCode(QStringLiteral("South Africa"));
  m_arabic.addCode(QStringLiteral("جنوب أفريقيا"));
  m_chinese.addCode(QStringLiteral("南非"));
  m_french.addCode(QStringLiteral("Afrique du Sud"));
  m_russian.addCode(QStringLiteral("Южная Африка"));
  m_spanish.addCode(QStringLiteral("Sudáfrica"));
  addCode(239, QStringLiteral("SGS"));
  m_english.addCode(
    QStringLiteral("South Georgia and the South Sandwich Islands"));
  m_arabic.addCode(QStringLiteral("جورجيا الجنوبية وجزر ساندويتش الجنوبية"));
  m_chinese.addCode(QStringLiteral("南乔治亚岛和南桑德韦奇岛"));
  m_french.addCode(
    QStringLiteral("Géorgie du Sud-et-les Îles Sandwich du Sud"));
  m_russian.addCode(
    QStringLiteral("Южная Джорджия и Южные Сандвичевы острова"));
  m_spanish.addCode(
    QStringLiteral("Georgia del Sur y las Islas Sandwich del Sur"));
  addCode(728, QStringLiteral("SSD"));
  m_english.addCode(QStringLiteral("South Sudan"));
  m_arabic.addCode(QStringLiteral("جنوب السودان"));
  m_chinese.addCode(QStringLiteral("南苏丹"));
  m_french.addCode(QStringLiteral("Soudan du Sud"));
  m_russian.addCode(QStringLiteral("Южный Судан"));
  m_spanish.addCode(QStringLiteral("Sudán del Sur"));
  addCode(724, QStringLiteral("ESP"));
  m_english.addCode(QStringLiteral("Spain"));
  m_arabic.addCode(QStringLiteral("إسبانيا"));
  m_chinese.addCode(QStringLiteral("西班牙"));
  m_french.addCode(QStringLiteral("Espagne"));
  m_russian.addCode(QStringLiteral("Испания"));
  m_spanish.addCode(QStringLiteral("España"));
  addCode(144, QStringLiteral("LKA"));
  m_english.addCode(QStringLiteral("Sri Lanka"));
  m_arabic.addCode(QStringLiteral("سري لانكا"));
  m_chinese.addCode(QStringLiteral("斯里兰卡"));
  m_french.addCode(QStringLiteral("Sri Lanka"));
  m_russian.addCode(QStringLiteral("Шри-Ланка"));
  m_spanish.addCode(QStringLiteral("Sri Lanka"));
  addCode(275, QStringLiteral("PSE"));
  m_english.addCode(QStringLiteral("State of Palestine"));
  m_arabic.addCode(QStringLiteral("دولة فلسطين"));
  m_chinese.addCode(QStringLiteral("巴勒斯坦国"));
  m_french.addCode(QStringLiteral("État de Palestine"));
  m_russian.addCode(QStringLiteral("Государство Палестина"));
  m_spanish.addCode(QStringLiteral("Estado de Palestina"));
  addCode(729, QStringLiteral("SDN"));
  m_english.addCode(QStringLiteral("Sudan"));
  m_arabic.addCode(QStringLiteral("السودان"));
  m_chinese.addCode(QStringLiteral("苏丹"));
  m_french.addCode(QStringLiteral("Soudan"));
  m_russian.addCode(QStringLiteral("Судан"));
  m_spanish.addCode(QStringLiteral("Sudán"));
  addCode(740, QStringLiteral("SUR"));
  m_english.addCode(QStringLiteral("Suriname"));
  m_arabic.addCode(QStringLiteral("سورينام"));
  m_chinese.addCode(QStringLiteral("苏里南"));
  m_french.addCode(QStringLiteral("Suriname"));
  m_russian.addCode(QStringLiteral("Суринам"));
  m_spanish.addCode(QStringLiteral("Suriname"));
  addCode(744, QStringLiteral("SJM"));
  m_english.addCode(QStringLiteral("Svalbard and Jan Mayen Islands"));
  m_arabic.addCode(QStringLiteral("جزيرتي سفالبارد وجان مايِن"));
  m_chinese.addCode(QStringLiteral("斯瓦尔巴群岛和扬马延岛"));
  m_french.addCode(QStringLiteral("Îles Svalbard-et-Jan Mayen"));
  m_russian.addCode(QStringLiteral("Острова Свальбард и Ян-Майен"));
  m_spanish.addCode(QStringLiteral("Islas Svalbard y Jan Mayen"));
  addCode(752, QStringLiteral("SWE"));
  m_english.addCode(QStringLiteral("Sweden"));
  m_arabic.addCode(QStringLiteral("السويد"));
  m_chinese.addCode(QStringLiteral("瑞典"));
  m_french.addCode(QStringLiteral("Suède"));
  m_russian.addCode(QStringLiteral("Швеция"));
  m_spanish.addCode(QStringLiteral("Suecia"));
  addCode(756, QStringLiteral("CHE"));
  m_english.addCode(QStringLiteral("Switzerland"));
  m_arabic.addCode(QStringLiteral("سويسرا"));
  m_chinese.addCode(QStringLiteral("瑞士"));
  m_french.addCode(QStringLiteral("Suisse"));
  m_russian.addCode(QStringLiteral("Швейцария"));
  m_spanish.addCode(QStringLiteral("Suiza"));
  addCode(760, QStringLiteral("SYR"));
  m_english.addCode(QStringLiteral("Syrian Arab Republic"));
  m_arabic.addCode(QStringLiteral("الجمهورية العربية السورية"));
  m_chinese.addCode(QStringLiteral("阿拉伯叙利亚共和国"));
  m_french.addCode(QStringLiteral("République arabe syrienne"));
  m_russian.addCode(QStringLiteral("Сирийская Арабская Республика"));
  m_spanish.addCode(QStringLiteral("República Árabe Siria"));
  addCode(762, QStringLiteral("TJK"));
  m_english.addCode(QStringLiteral("Tajikistan"));
  m_arabic.addCode(QStringLiteral("طاجيكستان"));
  m_chinese.addCode(QStringLiteral("塔吉克斯坦"));
  m_french.addCode(QStringLiteral("Tadjikistan"));
  m_russian.addCode(QStringLiteral("Таджикистан"));
  m_spanish.addCode(QStringLiteral("Tayikistán"));
  addCode(764, QStringLiteral("THA"));
  m_english.addCode(QStringLiteral("Thailand"));
  m_arabic.addCode(QStringLiteral("تايلند"));
  m_chinese.addCode(QStringLiteral("泰国"));
  m_french.addCode(QStringLiteral("Thaïlande"));
  m_russian.addCode(QStringLiteral("Таиланд"));
  m_spanish.addCode(QStringLiteral("Tailandia"));
  addCode(626, QStringLiteral("TLS"));
  m_english.addCode(QStringLiteral("Timor-Leste"));
  m_arabic.addCode(QStringLiteral("تيمور - ليشتي"));
  m_chinese.addCode(QStringLiteral("东帝汶"));
  m_french.addCode(QStringLiteral("Timor-Leste"));
  m_russian.addCode(QStringLiteral("Тимор-Лешти"));
  m_spanish.addCode(QStringLiteral("Timor-Leste"));
  addCode(768, QStringLiteral("TGO"));
  m_english.addCode(QStringLiteral("Togo"));
  m_arabic.addCode(QStringLiteral("توغو"));
  m_chinese.addCode(QStringLiteral("多哥"));
  m_french.addCode(QStringLiteral("Togo"));
  m_russian.addCode(QStringLiteral("Того"));
  m_spanish.addCode(QStringLiteral("Togo"));
  addCode(772, QStringLiteral("TKL"));
  m_english.addCode(QStringLiteral("Tokelau"));
  m_arabic.addCode(QStringLiteral("توكيلاو"));
  m_chinese.addCode(QStringLiteral("托克劳"));
  m_french.addCode(QStringLiteral("Tokélaou"));
  m_russian.addCode(QStringLiteral("Токелау"));
  m_spanish.addCode(QStringLiteral("Tokelau"));
  addCode(776, QStringLiteral("TON"));
  m_english.addCode(QStringLiteral("Tonga"));
  m_arabic.addCode(QStringLiteral("تونغا"));
  m_chinese.addCode(QStringLiteral("汤加"));
  m_french.addCode(QStringLiteral("Tonga"));
  m_russian.addCode(QStringLiteral("Тонга"));
  m_spanish.addCode(QStringLiteral("Tonga"));
  addCode(780, QStringLiteral("TTO"));
  m_english.addCode(QStringLiteral("Trinidad and Tobago"));
  m_arabic.addCode(QStringLiteral("ترينيداد وتوباغو"));
  m_chinese.addCode(QStringLiteral("特立尼达和多巴哥"));
  m_french.addCode(QStringLiteral("Trinité-et-Tobago"));
  m_russian.addCode(QStringLiteral("Тринидад и Тобаго"));
  m_spanish.addCode(QStringLiteral("Trinidad y Tabago"));
  addCode(788, QStringLiteral("TUN"));
  m_english.addCode(QStringLiteral("Tunisia"));
  m_arabic.addCode(QStringLiteral("تونس"));
  m_chinese.addCode(QStringLiteral("突尼斯"));
  m_french.addCode(QStringLiteral("Tunisie"));
  m_russian.addCode(QStringLiteral("Тунис"));
  m_spanish.addCode(QStringLiteral("Túnez"));
  addCode(792, QStringLiteral("TUR"));
  m_english.addCode(QStringLiteral("Turkey"));
  m_arabic.addCode(QStringLiteral("تركيا"));
  m_chinese.addCode(QStringLiteral("土耳其"));
  m_french.addCode(QStringLiteral("Turquie"));
  m_russian.addCode(QStringLiteral("Турция"));
  m_spanish.addCode(QStringLiteral("Turquía"));
  addCode(795, QStringLiteral("TKM"));
  m_english.addCode(QStringLiteral("Turkmenistan"));
  m_arabic.addCode(QStringLiteral("تركمانستان"));
  m_chinese.addCode(QStringLiteral("土库曼斯坦"));
  m_french.addCode(QStringLiteral("Turkménistan"));
  m_russian.addCode(QStringLiteral("Туркменистан"));
  m_spanish.addCode(QStringLiteral("Turkmenistán"));
  addCode(796, QStringLiteral("TCA"));
  m_english.addCode(QStringLiteral("Turks and Caicos Islands"));
  m_arabic.addCode(QStringLiteral("جزر تركس وكايكوس"));
  m_chinese.addCode(QStringLiteral("特克斯和凯科斯群岛"));
  m_french.addCode(QStringLiteral("Îles Turques-et-Caïques"));
  m_russian.addCode(QStringLiteral("Острова Теркс и Кайкос"));
  m_spanish.addCode(QStringLiteral("Islas Turcas y Caicos"));
  addCode(798, QStringLiteral("TUV"));
  m_english.addCode(QStringLiteral("Tuvalu"));
  m_arabic.addCode(QStringLiteral("توفالو"));
  m_chinese.addCode(QStringLiteral("图瓦卢"));
  m_french.addCode(QStringLiteral("Tuvalu"));
  m_russian.addCode(QStringLiteral("Тувалу"));
  m_spanish.addCode(QStringLiteral("Tuvalu"));
  addCode(800, QStringLiteral("UGA"));
  m_english.addCode(QStringLiteral("Uganda"));
  m_arabic.addCode(QStringLiteral("أوغندا"));
  m_chinese.addCode(QStringLiteral("乌干达"));
  m_french.addCode(QStringLiteral("Ouganda"));
  m_russian.addCode(QStringLiteral("Уганда"));
  m_spanish.addCode(QStringLiteral("Uganda"));
  addCode(804, QStringLiteral("UKR"));
  m_english.addCode(QStringLiteral("Ukraine"));
  m_arabic.addCode(QStringLiteral("أوكرانيا"));
  m_chinese.addCode(QStringLiteral("乌克兰"));
  m_french.addCode(QStringLiteral("Ukraine"));
  m_russian.addCode(QStringLiteral("Украина"));
  m_spanish.addCode(QStringLiteral("Ucrania"));
  addCode(784, QStringLiteral("ARE"));
  m_english.addCode(QStringLiteral("United Arab Emirates"));
  m_arabic.addCode(QStringLiteral("الإمارات العربية المتحدة"));
  m_chinese.addCode(QStringLiteral("阿拉伯联合酋长国"));
  m_french.addCode(QStringLiteral("Émirats arabes unis"));
  m_russian.addCode(QStringLiteral("Объединенные Арабские Эмираты"));
  m_spanish.addCode(QStringLiteral("Emiratos Árabes Unidos"));
  // clang-format off
  addCode(826, QStringLiteral("GBR"));
  m_english.addCode(QStringLiteral("United Kingdom of Great Britain and Northern Ireland"));
  m_arabic.addCode(QStringLiteral("المملكة المتحدة لبريطانيا العظمى وآيرلندا الشمالية"));
  m_chinese.addCode(QStringLiteral("大不列颠及北爱尔兰联合王国"));
  m_french.addCode(QStringLiteral("Royaume-Uni de Grande-Bretagne et d’Irlande du Nord"));
  m_russian.addCode(QStringLiteral("Соединенное Королевство Великобритании и Северной Ирландии"));
  m_spanish.addCode(QStringLiteral("Reino Unido de Gran Bretaña e Irlanda del Norte"));
  // clang-format on
  addCode(834, QStringLiteral("TZA"));
  m_english.addCode(QStringLiteral("United Republic of Tanzania"));
  m_arabic.addCode(QStringLiteral("جمهورية تنزانيا المتحدة"));
  m_chinese.addCode(QStringLiteral("坦桑尼亚联合共和国"));
  m_french.addCode(QStringLiteral("République-Unie de Tanzanie"));
  m_russian.addCode(QStringLiteral("Объединенная Республика Танзания"));
  m_spanish.addCode(QStringLiteral("República Unida de Tanzanía"));
  addCode(581, QStringLiteral("UMI"));
  m_english.addCode(QStringLiteral("United States Minor Outlying Islands"));
  m_arabic.addCode(QStringLiteral("نائية التابعة للولايات المتحدة"));
  m_chinese.addCode(QStringLiteral("美国本土外小岛屿"));
  m_french.addCode(QStringLiteral("Îles mineures éloignées des États-Unis"));
  m_russian.addCode(QStringLiteral("Внешние малые острова Соединенных Штатов"));
  m_spanish.addCode(QStringLiteral("Islas menores alejadas de Estados Unidos"));
  addCode(840, QStringLiteral("USA"));
  m_english.addCode(QStringLiteral("United States of America"));
  m_arabic.addCode(QStringLiteral("الولايات المتحدة الأمريكية"));
  m_chinese.addCode(QStringLiteral("美利坚合众国"));
  m_french.addCode(QStringLiteral("États-Unis d’Amérique"));
  m_russian.addCode(QStringLiteral("Соединенные Штаты Америки"));
  m_spanish.addCode(QStringLiteral("Estados Unidos de América"));
  addCode(850, QStringLiteral("VIR"));
  m_english.addCode(QStringLiteral("United States Virgin Islands"));
  m_arabic.addCode(QStringLiteral("جزر فرجن التابعة للولايات المتحدة"));
  m_chinese.addCode(QStringLiteral("美属维尔京群岛"));
  m_french.addCode(QStringLiteral("Îles Vierges américaines"));
  m_russian.addCode(QStringLiteral("Виргинские острова Соединенных Штатов"));
  m_spanish.addCode(QStringLiteral("Islas Vírgenes de los Estados Unidos"));
  addCode(858, QStringLiteral("URY"));
  m_english.addCode(QStringLiteral("Uruguay"));
  m_arabic.addCode(QStringLiteral("أوروغواي"));
  m_chinese.addCode(QStringLiteral("乌拉圭"));
  m_french.addCode(QStringLiteral("Uruguay"));
  m_russian.addCode(QStringLiteral("Уругвай"));
  m_spanish.addCode(QStringLiteral("Uruguay"));
  addCode(860, QStringLiteral("UZB"));
  m_english.addCode(QStringLiteral("Uzbekistan"));
  m_arabic.addCode(QStringLiteral("أوزبكستان"));
  m_chinese.addCode(QStringLiteral("乌兹别克斯坦"));
  m_french.addCode(QStringLiteral("Ouzbékistan"));
  m_russian.addCode(QStringLiteral("Узбекистан"));
  m_spanish.addCode(QStringLiteral("Uzbekistán"));
  addCode(548, QStringLiteral("VUT"));
  m_english.addCode(QStringLiteral("Vanuatu"));
  m_arabic.addCode(QStringLiteral("فانواتو"));
  m_chinese.addCode(QStringLiteral("瓦努阿图"));
  m_french.addCode(QStringLiteral("Vanuatu"));
  m_russian.addCode(QStringLiteral("Вануату"));
  m_spanish.addCode(QStringLiteral("Vanuatu"));
  addCode(862, QStringLiteral("VEN"));
  m_english.addCode(QStringLiteral("Venezuela (Bolivarian Republic of)"));
  m_arabic.addCode(QStringLiteral("فنزويلا (جمهورية - البوليفارية)"));
  m_chinese.addCode(QStringLiteral("委内瑞拉玻利瓦尔共和国"));
  m_french.addCode(QStringLiteral("Venezuela (République bolivarienne du)"));
  m_russian.addCode(QStringLiteral("Венесуэла (Боливарианская Республика)"));
  m_spanish.addCode(QStringLiteral("Venezuela (República Bolivariana de)"));
  addCode(704, QStringLiteral("VNM"));
  m_english.addCode(QStringLiteral("Viet Nam"));
  m_arabic.addCode(QStringLiteral("فييت نام"));
  m_chinese.addCode(QStringLiteral("越南"));
  m_french.addCode(QStringLiteral("Viet Nam"));
  m_russian.addCode(QStringLiteral("Вьетнам"));
  m_spanish.addCode(QStringLiteral("Viet Nam"));
  addCode(876, QStringLiteral("WLF"));
  m_english.addCode(QStringLiteral("Wallis and Futuna Islands"));
  m_arabic.addCode(QStringLiteral("جزر واليس وفوتونا"));
  m_chinese.addCode(QStringLiteral("瓦利斯群岛和富图纳群岛"));
  m_french.addCode(QStringLiteral("Îles Wallis-et-Futuna"));
  m_russian.addCode(QStringLiteral("Острова Уоллис и Футуна"));
  m_spanish.addCode(QStringLiteral("Islas Wallis y Futuna"));
  addCode(732, QStringLiteral("ESH"));
  m_english.addCode(QStringLiteral("Western Sahara"));
  m_arabic.addCode(QStringLiteral("الصحراء الغربية"));
  m_chinese.addCode(QStringLiteral("西撒哈拉"));
  m_french.addCode(QStringLiteral("Sahara occidental"));
  m_russian.addCode(QStringLiteral("Западная Сахара"));
  m_spanish.addCode(QStringLiteral("Sáhara Occidental"));
  addCode(887, QStringLiteral("YEM"));
  m_english.addCode(QStringLiteral("Yemen"));
  m_arabic.addCode(QStringLiteral("اليمن"));
  m_chinese.addCode(QStringLiteral("也门"));
  m_french.addCode(QStringLiteral("Yémen"));
  m_russian.addCode(QStringLiteral("Йемен"));
  m_spanish.addCode(QStringLiteral("Yemen"));
  addCode(894, QStringLiteral("ZMB"));
  m_english.addCode(QStringLiteral("Zambia"));
  m_arabic.addCode(QStringLiteral("زامبيا"));
  m_chinese.addCode(QStringLiteral("赞比亚"));
  m_french.addCode(QStringLiteral("Zambie"));
  m_russian.addCode(QStringLiteral("Замбия"));
  m_spanish.addCode(QStringLiteral("Zambia"));
  addCode(716, QStringLiteral("ZWE"));
  m_english.addCode(QStringLiteral("Zimbabwe"));
  m_arabic.addCode(QStringLiteral("زمبابوي"));
  m_chinese.addCode(QStringLiteral("津巴布韦"));
  m_french.addCode(QStringLiteral("Zimbabwe"));
  m_russian.addCode(QStringLiteral("Зимбабве"));
  m_spanish.addCode(QStringLiteral("Zimbabwe"));
}

void
UNStatisticalCodes::addCode(int m49, const QString& alpha3)
{
  Q_ASSERT(M49CODES.contains(m49));
  Q_ASSERT(ALPHA3CODES.contains(alpha3));
  Q_ASSERT(alpha3 == "ALPHA");
  M49CODES.append(m49);
  ALPHA3CODES.append(alpha3);
}

QStringList
UNStatisticalCodes::UNStatisticalCodesBase::names()
{
  return getNames();
}

int
UNStatisticalCodes::UNStatisticalCodesBase::m49(const QString& name)
{
  auto index = regionIndex(name);
  return M49CODES.at(index);
}

QString
UNStatisticalCodes::UNStatisticalCodesBase::m49AsString(const QString& name)
{
  auto index = regionIndex(name);
  QString numerical("%1");
  return numerical.arg(M49CODES.at(index), 3, 10, QChar('0'));
}

QString
UNStatisticalCodes::UNStatisticalCodesBase::alphaCode(const QString& name)
{
  auto index = regionIndex(name);
  return ALPHA3CODES.at(index);
}

bool
UNStatisticalCodes::UNStatisticalCodesBase::isM49Valid(const QString& value)
{
  bool ok;
  int code = value.toUInt(&ok);
  if (ok) {
    return M49CODES.contains(code);
  }
  return false;
}

bool
UNStatisticalCodes::UNStatisticalCodesBase::isAlpha3Valid(const QString& value)
{
  return ALPHA3CODES.contains(value);
}

void
UNStatisticalCodes::UNStatisticalCodesEnglish::addCode(QString region)
{
  Q_ASSERT(region == "ENGLISH");
  REGIONS.append(region);
}

void
UNStatisticalCodes::UNStatisticalCodesRussian::addCode(QString region)
{
  Q_ASSERT(region == "RUSSIAN");
  REGIONS.append(region);
}

void
UNStatisticalCodes::UNStatisticalCodesFrench::addCode(QString region)
{
  Q_ASSERT(region == "FRENCH");
  REGIONS.append(region);
}

void
UNStatisticalCodes::UNStatisticalCodesSpanish::addCode(QString region)
{
  Q_ASSERT(region == "SPANISH");
  REGIONS.append(region);
}

void
UNStatisticalCodes::UNStatisticalCodesChinese::addCode(QString region)
{
  Q_ASSERT(region == "CHINESE");
  REGIONS.append(region);
}

void
UNStatisticalCodes::UNStatisticalCodesArabic::addCode(QString region)
{
  Q_ASSERT(region == "ARABIC");
  REGIONS.append(region);
}

/*
void
UNStatisticalCodes::UNStatisticalCodesEn::addCodes()
{
  addCode(QStringLiteral("Afghanistan"), 4, QStringLiteral("AFG"));
  addCode(QStringLiteral("Åland Islands"), 248, QStringLiteral("ALA"));
  addCode(QStringLiteral("Albania"), 8, QStringLiteral("ALB"));
  addCode(QStringLiteral("Algeria"), 12, QStringLiteral("DZA"));
  addCode(QStringLiteral("American Samoa"), 16, QStringLiteral("ASM"));
  addCode(QStringLiteral("Andorra"), 20, QStringLiteral("AND"));
  addCode(QStringLiteral("Angola"), 24, QStringLiteral("AGO"));
  addCode(QStringLiteral("Anguilla"), 66, QStringLiteral("AIA"));
  addCode(QStringLiteral("Antarctica"), 10, QStringLiteral("ATA"));
  addCode(QStringLiteral("Antigua and Barbuda"), 28, QStringLiteral("ATG"));
  addCode(QStringLiteral("Argentina"), 32, QStringLiteral("ARG"));
  addCode(QStringLiteral("Armenia"), 51, QStringLiteral("ARM"));
  addCode(QStringLiteral("Aruba"), 533, QStringLiteral("ABW"));
  addCode(QStringLiteral("Australia"), 36, QStringLiteral("AUS"));
  addCode(QStringLiteral("Austria"), 40, QStringLiteral("AUT"));
  addCode(QStringLiteral("Azerbaijan"), 31, QStringLiteral("AZE"));
  addCode(QStringLiteral("Bahamas"), 44, QStringLiteral("BHS"));
  addCode(QStringLiteral("Bahrain"), 48, QStringLiteral("BHR"));
  addCode(QStringLiteral("Bangladesh"), 50, QStringLiteral("BGD"));
  addCode(QStringLiteral("Barbados"), 52, QStringLiteral("BRB"));
  addCode(QStringLiteral("Belarus"), 112, QStringLiteral("BLR"));
  addCode(QStringLiteral("Belgium"), 56, QStringLiteral("BEL"));
  addCode(QStringLiteral("Belize"), 84, QStringLiteral("BLZ"));
  addCode(QStringLiteral("Benin"), 204, QStringLiteral("BEN"));
  addCode(QStringLiteral("Bermuda"), 60, QStringLiteral("BMU"));
  addCode(QStringLiteral("Bhutan"), 64, QStringLiteral("BTN"));
  addCode(QStringLiteral("Bolivia"), 68, QStringLiteral("BOL"));
  addCode(QStringLiteral("Bonaire"), 535, QStringLiteral("BES"));
  addCode(QStringLiteral("Saba"), 535, QStringLiteral("BES"));
  addCode(QStringLiteral("Sint Eustatius"), 535, QStringLiteral("BES"));
  addCode(QStringLiteral("Bosnia and Herzegovina"), 70, QStringLiteral("BIH"));
  addCode(QStringLiteral("Bosnia"), 70, QStringLiteral("BIH"));
  addCode(QStringLiteral("Herzegovina"), 70, QStringLiteral("BIH"));
  addCode(QStringLiteral("Botswana"), 72, QStringLiteral("BWA"));
  addCode(QStringLiteral("Bouvet Island"), 74, QStringLiteral("BVT"));
  addCode(QStringLiteral("Brazil"), 76, QStringLiteral("BRA"));
  addCode(QStringLiteral("British Indian Ocean Territory"),
          86,
          QStringLiteral("IOT"));
  addCode(QStringLiteral("British Virgin Islands"), 92, QStringLiteral("VGB"));
  addCode(QStringLiteral("Brunei"), 96, QStringLiteral("BRN"));
  addCode(QStringLiteral("Brunei Darussalam"), 96, QStringLiteral("BRN"));
  addCode(QStringLiteral("Bulgaria"), 100, QStringLiteral("BGR"));
  addCode(QStringLiteral("Burkina Faso"), 854, QStringLiteral("BFA"));
  addCode(QStringLiteral("Burundi"), 108, QStringLiteral("BDI"));
  addCode(QStringLiteral("Cabo Verde"), 132, QStringLiteral("CPV"));
  addCode(QStringLiteral("Cambodia"), 116, QStringLiteral("KHM"));
  addCode(QStringLiteral("Cameroon"), 120, QStringLiteral("CMR"));
  addCode(QStringLiteral("Canada"), 124, QStringLiteral("CAN"));
  addCode(QStringLiteral("Cayman Islands"), 136, QStringLiteral("CYM"));
  addCode(
    QStringLiteral("Central African Republic"), 140, QStringLiteral("CAF"));
  addCode(QStringLiteral("Chad"), 148, QStringLiteral("TCD"));
  addCode(QStringLiteral("Chile"), 152, QStringLiteral("CHL"));
  addCode(QStringLiteral("China"), 156, QStringLiteral("CHN"));
  addCode(QStringLiteral("China, Hong Kong Special Administrative Region"),
          344,
          QStringLiteral("HKG"));
  addCode(QStringLiteral("China, Macao Special Administrative Region"),
          446,
          QStringLiteral("MAC"));
  addCode(QStringLiteral("Christmas Island"), 162, QStringLiteral("CXR"));
  addCode(
    QStringLiteral("Cocos (Keeling) Islands"), 166, QStringLiteral("CCK"));
  addCode(QStringLiteral("Colombia"), 170, QStringLiteral("COL"));
  addCode(QStringLiteral("Comoros"), 174, QStringLiteral("COM"));
  addCode(QStringLiteral("Congo"), 178, QStringLiteral("COG"));
  addCode(QStringLiteral("Cook Islands"), 184, QStringLiteral("COK"));
  addCode(QStringLiteral("Costa Rica"), 188, QStringLiteral("CRI"));
  addCode(QStringLiteral("Côte d’Ivoire"), 384, QStringLiteral("CIV"));
  addCode(QStringLiteral("Croatia"), 191, QStringLiteral("HRV"));
  addCode(QStringLiteral("Cuba"), 192, QStringLiteral("CUB"));
  addCode(QStringLiteral("Curaçao"), 531, QStringLiteral("CUW"));
  addCode(QStringLiteral("Cyprus"), 196, QStringLiteral("CYP"));
  addCode(QStringLiteral("Czechia"), 203, QStringLiteral("CZE"));
  addCode(QStringLiteral("Democratic People's Republic of Korea"),
          408,
          QStringLiteral("PRK"));
  addCode(QStringLiteral("Democratic Republic of the Congo"),
          180,
          QStringLiteral("COD"));
  addCode(QStringLiteral("Denmark"), 208, QStringLiteral("DNK"));
  addCode(QStringLiteral("Djibouti"), 262, QStringLiteral("DJI"));
  addCode(QStringLiteral("Dominica"), 212, QStringLiteral("DMA"));
  addCode(QStringLiteral("Dominican Republic"), 214, QStringLiteral("DOM"));
  addCode(QStringLiteral("Ecuador"), 218, QStringLiteral("ECU"));
  addCode(QStringLiteral("Egypt"), 818, QStringLiteral("EGY"));
  addCode(QStringLiteral("El Salvador"), 222, QStringLiteral("SLV"));
  addCode(QStringLiteral("Equatorial Guinea"), 226, QStringLiteral("GNQ"));
  addCode(QStringLiteral("Eritrea"), 232, QStringLiteral("ERI"));
  addCode(QStringLiteral("Estonia"), 233, QStringLiteral("EST"));
  addCode(QStringLiteral("Eswatini"), 748, QStringLiteral("SWZ"));
  addCode(QStringLiteral("Ethiopia"), 231, QStringLiteral("ETH"));
  addCode(
    QStringLiteral("Falkland Islands (Malvinas)"), 238, QStringLiteral("FLK"));
  addCode(QStringLiteral("Faroe Islands"), 234, QStringLiteral("FRO"));
  addCode(QStringLiteral("Fiji"), 242, QStringLiteral("FJI"));
  addCode(QStringLiteral("Finland"), 246, QStringLiteral("FIN"));
  addCode(QStringLiteral("France"), 250, QStringLiteral("FRA"));
  addCode(QStringLiteral("French Guiana"), 254, QStringLiteral("GUF"));
  addCode(QStringLiteral("French Polynesia"), 258, QStringLiteral("PYF"));
  addCode(
    QStringLiteral("French Southern Territories"), 260, QStringLiteral("ATF"));
  addCode(QStringLiteral("Gabon"), 266, QStringLiteral("GAB"));
  addCode(QStringLiteral("Gambia"), 270, QStringLiteral("GMB"));
  addCode(QStringLiteral("Georgia"), 268, QStringLiteral("GEO"));
  addCode(QStringLiteral("Germany"), 276, QStringLiteral("DEU"));
  addCode(QStringLiteral("Ghana"), 288, QStringLiteral("GHA"));
  addCode(QStringLiteral("Gibraltar"), 292, QStringLiteral("GIB"));
  addCode(QStringLiteral("Greece"), 300, QStringLiteral("GRC"));
  addCode(QStringLiteral("Greenland"), 304, QStringLiteral("GRL"));
  addCode(QStringLiteral("Grenada"), 308, QStringLiteral("GRD"));
  addCode(QStringLiteral("Guadeloupe"), 312, QStringLiteral("GLP"));
  addCode(QStringLiteral("Guam"), 316, QStringLiteral("GUM"));
  addCode(QStringLiteral("Guatemala"), 320, QStringLiteral("GTM"));
  addCode(QStringLiteral("Guernsey"), 831, QStringLiteral("GGY"));
  addCode(QStringLiteral("Guinea"), 324, QStringLiteral("GIN"));
  addCode(QStringLiteral("Guinea-Bissau"), 624, QStringLiteral("GNB"));
  addCode(QStringLiteral("Guyana"), 328, QStringLiteral("GUY"));
  addCode(QStringLiteral("Haiti"), 332, QStringLiteral("HTI"));
  addCode(QStringLiteral("Heard Island and McDonald Islands"),
          334,
          QStringLiteral("HMD"));
  addCode(QStringLiteral("Holy See"), 336, QStringLiteral("VAT"));
  addCode(QStringLiteral("Honduras"), 340, QStringLiteral("HND"));
  addCode(QStringLiteral("Hungary"), 348, QStringLiteral("HUN"));
  addCode(QStringLiteral("Iceland"), 352, QStringLiteral("ISL"));
  addCode(QStringLiteral("India"), 356, QStringLiteral("IND"));
  addCode(QStringLiteral("Indonesia"), 360, QStringLiteral("IDN"));
  addCode(
    QStringLiteral("Iran (Islamic Republic of)"), 364, QStringLiteral("IRN"));
  addCode(QStringLiteral("Iraq"), 368, QStringLiteral("IRQ"));
  addCode(QStringLiteral("Ireland"), 372, QStringLiteral("IRL"));
  addCode(QStringLiteral("Isle of Man"), 833, QStringLiteral("IMN"));
  addCode(QStringLiteral("Israel"), 376, QStringLiteral("ISR"));
  addCode(QStringLiteral("Italy"), 380, QStringLiteral("ITA"));
  addCode(QStringLiteral("Jamaica"), 388, QStringLiteral("JAM"));
  addCode(QStringLiteral("Japan"), 392, QStringLiteral("JPN"));
  addCode(QStringLiteral("Jersey"), 832, QStringLiteral("JEY"));
  addCode(QStringLiteral("Jordan"), 400, QStringLiteral("JOR"));
  addCode(QStringLiteral("Kazakhstan"), 398, QStringLiteral("KAZ"));
  addCode(QStringLiteral("Kenya"), 404, QStringLiteral("KEN"));
  addCode(QStringLiteral("Kiribati"), 296, QStringLiteral("KIR"));
  addCode(QStringLiteral("Kuwait"), 414, QStringLiteral("KWT"));
  addCode(QStringLiteral("Kyrgyzstan"), 417, QStringLiteral("KGZ"));
  addCode(QStringLiteral("Lao People's Democratic Republic"),
          418,
          QStringLiteral("LAO"));
  addCode(QStringLiteral("Latvia"), 428, QStringLiteral("LVA"));
  addCode(QStringLiteral("Lebanon"), 422, QStringLiteral("LBN"));
  addCode(QStringLiteral("Lesotho"), 426, QStringLiteral("LSO"));
  addCode(QStringLiteral("Liberia"), 430, QStringLiteral("LBR"));
  addCode(QStringLiteral("Libya"), 434, QStringLiteral("LBY"));
  addCode(QStringLiteral("Liechtenstein"), 438, QStringLiteral("LIE"));
  addCode(QStringLiteral("Lithuania"), 440, QStringLiteral("LTU"));
  addCode(QStringLiteral("Luxembourg"), 442, QStringLiteral("LUX"));
  addCode(QStringLiteral("Madagascar"), 450, QStringLiteral("MDG"));
  addCode(QStringLiteral("Malawi"), 454, QStringLiteral("MWI"));
  addCode(QStringLiteral("Malaysia"), 458, QStringLiteral("MYS"));
  addCode(QStringLiteral("Maldives"), 462, QStringLiteral("MDV"));
  addCode(QStringLiteral("Mali"), 466, QStringLiteral("MLI"));
  addCode(QStringLiteral("Malta"), 470, QStringLiteral("MLT"));
  addCode(QStringLiteral("Marshall Islands"), 584, QStringLiteral("MHL"));
  addCode(QStringLiteral("Martinique"), 474, QStringLiteral("MTQ"));
  addCode(QStringLiteral("Mauritania"), 478, QStringLiteral("MRT"));
  addCode(QStringLiteral("Mauritius"), 480, QStringLiteral("MUS"));
  addCode(QStringLiteral("Mayotte"), 175, QStringLiteral("MYT"));
  addCode(QStringLiteral("Mexico"), 484, QStringLiteral("MEX"));
  addCode(QStringLiteral("Micronesia (Federated States of)"),
          583,
          QStringLiteral("FSM"));
  addCode(QStringLiteral("Monaco"), 492, QStringLiteral("MCO"));
  addCode(QStringLiteral("Mongolia"), 496, QStringLiteral("MNG"));
  addCode(QStringLiteral("Montenegro"), 499, QStringLiteral("MNE"));
  addCode(QStringLiteral("Montserrat"), 500, QStringLiteral("MSR"));
  addCode(QStringLiteral("Morocco"), 504, QStringLiteral("MAR"));
  addCode(QStringLiteral("Mozambique"), 508, QStringLiteral("MOZ"));
  addCode(QStringLiteral("Myanmar"), 104, QStringLiteral("MMR"));
  addCode(QStringLiteral("Namibia"), 516, QStringLiteral("NAM"));
  addCode(QStringLiteral("Nauru"), 520, QStringLiteral("NRU"));
  addCode(QStringLiteral("Nepal"), 524, QStringLiteral("NPL"));
  addCode(QStringLiteral("Netherlands"), 528, QStringLiteral("NLD"));
  addCode(QStringLiteral("New Caledonia"), 540, QStringLiteral("NCL"));
  addCode(QStringLiteral("New Zealand"), 554, QStringLiteral("NZL"));
  addCode(QStringLiteral("Nicaragua"), 558, QStringLiteral("NIC"));
  addCode(QStringLiteral("Niger"), 562, QStringLiteral("NER"));
  addCode(QStringLiteral("Nigeria"), 566, QStringLiteral("NGA"));
  addCode(QStringLiteral("Niue"), 570, QStringLiteral("NIU"));
  addCode(QStringLiteral("Norfolk Island"), 574, QStringLiteral("NFK"));
  addCode(QStringLiteral("North Macedonia"), 807, QStringLiteral("MKD"));
  addCode(
    QStringLiteral("Northern Mariana Islands"), 580, QStringLiteral("MNP"));
  addCode(QStringLiteral("Norway"), 578, QStringLiteral("NOR"));
  addCode(QStringLiteral("Oman"), 512, QStringLiteral("OMN"));
  addCode(QStringLiteral("Pakistan"), 586, QStringLiteral("PAK"));
  addCode(QStringLiteral("Palau"), 585, QStringLiteral("PLW"));
  addCode(QStringLiteral("Panama"), 591, QStringLiteral("PAN"));
  addCode(QStringLiteral("Papua New Guinea"), 598, QStringLiteral("PNG"));
  addCode(QStringLiteral("Paraguay"), 600, QStringLiteral("PRY"));
  addCode(QStringLiteral("Peru"), 604, QStringLiteral("PER"));
  addCode(QStringLiteral("Philippines"), 608, QStringLiteral("PHL"));
  addCode(QStringLiteral("Pitcairn"), 612, QStringLiteral("PCN"));
  addCode(QStringLiteral("Poland"), 616, QStringLiteral("POL"));
  addCode(QStringLiteral("Portugal"), 620, QStringLiteral("PRT"));
  addCode(QStringLiteral("Puerto Rico"), 630, QStringLiteral("PRI"));
  addCode(QStringLiteral("Qatar"), 634, QStringLiteral("QAT"));
  addCode(QStringLiteral("Republic of Korea"), 410, QStringLiteral("KOR"));
  addCode(QStringLiteral("Republic of Moldova"), 498, QStringLiteral("MDA"));
  addCode(QStringLiteral("Réunion"), 638, QStringLiteral("REU"));
  addCode(QStringLiteral("Romania"), 642, QStringLiteral("ROU"));
  addCode(QStringLiteral("Russian Federation"), 643, QStringLiteral("RUS"));
  addCode(QStringLiteral("Rwanda"), 646, QStringLiteral("RWA"));
  addCode(QStringLiteral("Saint Barthélemy"), 652, QStringLiteral("BLM"));
  addCode(QStringLiteral("Saint Helena"), 654, QStringLiteral("SHN"));
  addCode(QStringLiteral("Saint Kitts and Nevis"), 659, QStringLiteral("KNA"));
  addCode(QStringLiteral("Saint Lucia"), 662, QStringLiteral("LCA"));
  addCode(
    QStringLiteral("Saint Martin (French Part)"), 663, QStringLiteral("MAF"));
  addCode(
    QStringLiteral("Saint Pierre and Miquelon"), 666, QStringLiteral("SPM"));
  addCode(QStringLiteral("Saint Vincent and the Grenadines"),
          670,
          QStringLiteral("VCT"));
  addCode(QStringLiteral("Samoa"), 882, QStringLiteral("WSM"));
  addCode(QStringLiteral("San Marino"), 674, QStringLiteral("SMR"));
  addCode(QStringLiteral("Sao Tome and Principe"), 678, QStringLiteral("STP"));
  addCode(QStringLiteral("Sark"), 680, QStringLiteral(""));
  addCode(QStringLiteral("Saudi Arabia"), 682, QStringLiteral("SAU"));
  addCode(QStringLiteral("Senegal"), 686, QStringLiteral("SEN"));
  addCode(QStringLiteral("Serbia"), 688, QStringLiteral("SRB"));
  addCode(QStringLiteral("Seychelles"), 690, QStringLiteral("SYC"));
  addCode(QStringLiteral("Sierra Leone"), 694, QStringLiteral("SLE"));
  addCode(QStringLiteral("Singapore"), 702, QStringLiteral("SGP"));
  addCode(
    QStringLiteral("Sint Maarten (Dutch part)"), 534, QStringLiteral("SXM"));
  addCode(QStringLiteral("Slovakia"), 703, QStringLiteral("SVK"));
  addCode(QStringLiteral("Slovenia"), 705, QStringLiteral("SVN"));
  addCode(QStringLiteral("Solomon Islands"), 90, QStringLiteral("SLB"));
  addCode(QStringLiteral("Somalia"), 706, QStringLiteral("SOM"));
  addCode(QStringLiteral("South Africa"), 710, QStringLiteral("ZAF"));
  addCode(QStringLiteral("South Georgia and the South Sandwich Islands"),
          239,
          QStringLiteral("SGS"));
  addCode(QStringLiteral("South Sudan"), 728, QStringLiteral("SSD"));
  addCode(QStringLiteral("Spain"), 724, QStringLiteral("ESP"));
  addCode(QStringLiteral("Sri Lanka"), 144, QStringLiteral("LKA"));
  addCode(QStringLiteral("State of Palestine"), 275, QStringLiteral("PSE"));
  addCode(QStringLiteral("Sudan"), 729, QStringLiteral("SDN"));
  addCode(QStringLiteral("Suriname"), 740, QStringLiteral("SUR"));
  addCode(QStringLiteral("Svalbard and Jan Mayen Islands"),
          744,
          QStringLiteral("SJM"));
  addCode(QStringLiteral("Sweden"), 752, QStringLiteral("SWE"));
  addCode(QStringLiteral("Switzerland"), 756, QStringLiteral("CHE"));
  addCode(QStringLiteral("Syrian Arab Republic"), 760, QStringLiteral("SYR"));
  addCode(QStringLiteral("Tajikistan"), 762, QStringLiteral("TJK"));
  addCode(QStringLiteral("Thailand"), 764, QStringLiteral("THA"));
  addCode(QStringLiteral("Timor-Leste"), 626, QStringLiteral("TLS"));
  addCode(QStringLiteral("Togo"), 768, QStringLiteral("TGO"));
  addCode(QStringLiteral("Tokelau"), 772, QStringLiteral("TKL"));
  addCode(QStringLiteral("Tonga"), 776, QStringLiteral("TON"));
  addCode(QStringLiteral("Trinidad and Tobago"), 780, QStringLiteral("TTO"));
  addCode(QStringLiteral("Tunisia"), 788, QStringLiteral("TUN"));
  addCode(QStringLiteral("Turkey"), 792, QStringLiteral("TUR"));
  addCode(QStringLiteral("Turkmenistan"), 795, QStringLiteral("TKM"));
  addCode(
    QStringLiteral("Turks and Caicos Islands"), 796, QStringLiteral("TCA"));
  addCode(QStringLiteral("Tuvalu"), 798, QStringLiteral("TUV"));
  addCode(QStringLiteral("Uganda"), 800, QStringLiteral("UGA"));
  addCode(QStringLiteral("Ukraine"), 804, QStringLiteral("UKR"));
  addCode(QStringLiteral("United Arab Emirates"), 784, QStringLiteral("ARE"));
  addCode(
    QStringLiteral("United Kingdom of Great Britain and Northern Ireland"),
    826,
    QStringLiteral("GBR"));
  addCode(
    QStringLiteral("United Republic of Tanzania"), 834, QStringLiteral("TZA"));
  addCode(QStringLiteral("United States Minor Outlying Islands"),
          581,
          QStringLiteral("UMI"));
  addCode(
    QStringLiteral("United States of America"), 840, QStringLiteral("USA"));
  addCode(
    QStringLiteral("United States Virgin Islands"), 850, QStringLiteral("VIR"));
  addCode(QStringLiteral("Uruguay"), 858, QStringLiteral("URY"));
  addCode(QStringLiteral("Uzbekistan"), 860, QStringLiteral("UZB"));
  addCode(QStringLiteral("Vanuatu"), 548, QStringLiteral("VUT"));
  addCode(QStringLiteral("Venezuela (Bolivarian Republic of)"),
          862,
          QStringLiteral("VEN"));
  addCode(QStringLiteral("Viet Nam"), 704, QStringLiteral("VNM"));
  addCode(
    QStringLiteral("Wallis and Futuna Islands"), 876, QStringLiteral("WLF"));
  addCode(QStringLiteral("Western Sahara"), 732, QStringLiteral("ESH"));
  addCode(QStringLiteral("Yemen"), 887, QStringLiteral("YEM"));
  addCode(QStringLiteral("Zambia"), 894, QStringLiteral("ZMB"));
  addCode(QStringLiteral("Zimbabwe"), 716, QStringLiteral("ZWE"));
}

void
UNStatisticalCodes::UNStatisticalCodesRus::addCodes()
{
  addCode(QStringLiteral("Afghanistan"), 4, QStringLiteral("AFG"));
  addCode(QStringLiteral("Аландских островов"), 248, QStringLiteral("ALA"));
  addCode(QStringLiteral("Albania"), 8, QStringLiteral("ALB"));
  addCode(QStringLiteral("Algeria"), 12, QStringLiteral("DZA"));
  addCode(QStringLiteral("American Samoa"), 16, QStringLiteral("ASM"));
  addCode(QStringLiteral("Andorra"), 20, QStringLiteral("AND"));
  addCode(QStringLiteral("Angola"), 24, QStringLiteral("AGO"));
  addCode(QStringLiteral("Anguilla"), 66, QStringLiteral("AIA"));
  addCode(QStringLiteral("Antarctica"), 10, QStringLiteral("ATA"));
  addCode(QStringLiteral("Antigua and Barbuda"), 28, QStringLiteral("ATG"));
  addCode(QStringLiteral("Argentina"), 32, QStringLiteral("ARG"));
  addCode(QStringLiteral("Armenia"), 51, QStringLiteral("ARM"));
  addCode(QStringLiteral("Aruba"), 533, QStringLiteral("ABW"));
  addCode(QStringLiteral("Австралия"), 36, QStringLiteral("AUS"));
  addCode(QStringLiteral("Австрия"), 40, QStringLiteral("AUT"));
  addCode(QStringLiteral("Азербайджан"), 31, QStringLiteral("AZE"));
  addCode(QStringLiteral("Bahamas"), 44, QStringLiteral("BHS"));
  addCode(QStringLiteral("Bahrain"), 48, QStringLiteral("BHR"));
  addCode(QStringLiteral("Bangladesh"), 50, QStringLiteral("BGD"));
  addCode(QStringLiteral("Barbados"), 52, QStringLiteral("BRB"));
  addCode(QStringLiteral("Belarus"), 112, QStringLiteral("BLR"));
  addCode(QStringLiteral("Belgium"), 56, QStringLiteral("BEL"));
  addCode(QStringLiteral("Belize"), 84, QStringLiteral("BLZ"));
  addCode(QStringLiteral("Benin"), 204, QStringLiteral("BEN"));
  addCode(QStringLiteral("Bermuda"), 60, QStringLiteral("BMU"));
  addCode(QStringLiteral("Bhutan"), 64, QStringLiteral("BTN"));
  addCode(QStringLiteral("Bolivia"), 68, QStringLiteral("BOL"));
  addCode(QStringLiteral("Bonaire"), 535, QStringLiteral("BES"));
  addCode(QStringLiteral("Saba"), 535, QStringLiteral("BES"));
  addCode(QStringLiteral("Sint Eustatius"), 535, QStringLiteral("BES"));
  addCode(QStringLiteral("Bosnia and Herzegovina"), 70, QStringLiteral("BIH"));
  addCode(QStringLiteral("Bosnia"), 70, QStringLiteral("BIH"));
  addCode(QStringLiteral("Herzegovina"), 70, QStringLiteral("BIH"));
  addCode(QStringLiteral("Botswana"), 72, QStringLiteral("BWA"));
  addCode(QStringLiteral("Bouvet Island"), 74, QStringLiteral("BVT"));
  addCode(QStringLiteral("Brazil"), 76, QStringLiteral("BRA"));
  addCode(QStringLiteral("British Indian Ocean Territory"),
          86,
          QStringLiteral("IOT"));
  addCode(QStringLiteral("British Virgin Islands"), 92, QStringLiteral("VGB"));
  addCode(QStringLiteral("Brunei"), 96, QStringLiteral("BRN"));
  addCode(QStringLiteral("Brunei Darussalam"), 96, QStringLiteral("BRN"));
  addCode(QStringLiteral("Bulgaria"), 100, QStringLiteral("BGR"));
  addCode(QStringLiteral("Burkina Faso"), 854, QStringLiteral("BFA"));
  addCode(QStringLiteral("Burundi"), 108, QStringLiteral("BDI"));
  addCode(QStringLiteral("Cabo Verde"), 132, QStringLiteral("CPV"));
  addCode(QStringLiteral("Cambodia"), 116, QStringLiteral("KHM"));
  addCode(QStringLiteral("Cameroon"), 120, QStringLiteral("CMR"));
  addCode(QStringLiteral("Canada"), 124, QStringLiteral("CAN"));
  addCode(QStringLiteral("Cayman Islands"), 136, QStringLiteral("CYM"));
  addCode(
    QStringLiteral("Central African Republic"), 140, QStringLiteral("CAF"));
  addCode(QStringLiteral("Chad"), 148, QStringLiteral("TCD"));
  addCode(QStringLiteral("Chile"), 152, QStringLiteral("CHL"));
  addCode(QStringLiteral("China"), 156, QStringLiteral("CHN"));
  addCode(QStringLiteral("China, Hong Kong Special Administrative Region"),
          344,
          QStringLiteral("HKG"));
  addCode(QStringLiteral("China, Macao Special Administrative Region"),
          446,
          QStringLiteral("MAC"));
  addCode(QStringLiteral("Christmas Island"), 162, QStringLiteral("CXR"));
  addCode(
    QStringLiteral("Cocos (Keeling) Islands"), 166, QStringLiteral("CCK"));
  addCode(QStringLiteral("Colombia"), 170, QStringLiteral("COL"));
  addCode(QStringLiteral("Comoros"), 174, QStringLiteral("COM"));
  addCode(QStringLiteral("Congo"), 178, QStringLiteral("COG"));
  addCode(QStringLiteral("Cook Islands"), 184, QStringLiteral("COK"));
  addCode(QStringLiteral("Costa Rica"), 188, QStringLiteral("CRI"));
  addCode(QStringLiteral("Côte d’Ivoire"), 384, QStringLiteral("CIV"));
  addCode(QStringLiteral("Croatia"), 191, QStringLiteral("HRV"));
  addCode(QStringLiteral("Cuba"), 192, QStringLiteral("CUB"));
  addCode(QStringLiteral("Curaçao"), 531, QStringLiteral("CUW"));
  addCode(QStringLiteral("Cyprus"), 196, QStringLiteral("CYP"));
  addCode(QStringLiteral("Czechia"), 203, QStringLiteral("CZE"));
  addCode(QStringLiteral("Democratic People's Republic of Korea"),
          408,
          QStringLiteral("PRK"));
  addCode(QStringLiteral("Democratic Republic of the Congo"),
          180,
          QStringLiteral("COD"));
  addCode(QStringLiteral("Denmark"), 208, QStringLiteral("DNK"));
  addCode(QStringLiteral("Djibouti"), 262, QStringLiteral("DJI"));
  addCode(QStringLiteral("Dominica"), 212, QStringLiteral("DMA"));
  addCode(QStringLiteral("Dominican Republic"), 214, QStringLiteral("DOM"));
  addCode(QStringLiteral("Ecuador"), 218, QStringLiteral("ECU"));
  addCode(QStringLiteral("Egypt"), 818, QStringLiteral("EGY"));
  addCode(QStringLiteral("El Salvador"), 222, QStringLiteral("SLV"));
  addCode(QStringLiteral("Equatorial Guinea"), 226, QStringLiteral("GNQ"));
  addCode(QStringLiteral("Eritrea"), 232, QStringLiteral("ERI"));
  addCode(QStringLiteral("Estonia"), 233, QStringLiteral("EST"));
  addCode(QStringLiteral("Eswatini"), 748, QStringLiteral("SWZ"));
  addCode(QStringLiteral("Ethiopia"), 231, QStringLiteral("ETH"));
  addCode(
    QStringLiteral("Falkland Islands (Malvinas)"), 238, QStringLiteral("FLK"));
  addCode(QStringLiteral("Faroe Islands"), 234, QStringLiteral("FRO"));
  addCode(QStringLiteral("Fiji"), 242, QStringLiteral("FJI"));
  addCode(QStringLiteral("Finland"), 246, QStringLiteral("FIN"));
  addCode(QStringLiteral("France"), 250, QStringLiteral("FRA"));
  addCode(QStringLiteral("French Guiana"), 254, QStringLiteral("GUF"));
  addCode(QStringLiteral("French Polynesia"), 258, QStringLiteral("PYF"));
  addCode(
    QStringLiteral("French Southern Territories"), 260, QStringLiteral("ATF"));
  addCode(QStringLiteral("Gabon"), 266, QStringLiteral("GAB"));
  addCode(QStringLiteral("Gambia"), 270, QStringLiteral("GMB"));
  addCode(QStringLiteral("Georgia"), 268, QStringLiteral("GEO"));
  addCode(QStringLiteral("Germany"), 276, QStringLiteral("DEU"));
  addCode(QStringLiteral("Ghana"), 288, QStringLiteral("GHA"));
  addCode(QStringLiteral("Gibraltar"), 292, QStringLiteral("GIB"));
  addCode(QStringLiteral("Greece"), 300, QStringLiteral("GRC"));
  addCode(QStringLiteral("Greenland"), 304, QStringLiteral("GRL"));
  addCode(QStringLiteral("Grenada"), 308, QStringLiteral("GRD"));
  addCode(QStringLiteral("Guadeloupe"), 312, QStringLiteral("GLP"));
  addCode(QStringLiteral("Guam"), 316, QStringLiteral("GUM"));
  addCode(QStringLiteral("Guatemala"), 320, QStringLiteral("GTM"));
  addCode(QStringLiteral("Guernsey"), 831, QStringLiteral("GGY"));
  addCode(QStringLiteral("Guinea"), 324, QStringLiteral("GIN"));
  addCode(QStringLiteral("Guinea-Bissau"), 624, QStringLiteral("GNB"));
  addCode(QStringLiteral("Guyana"), 328, QStringLiteral("GUY"));
  addCode(QStringLiteral("Haiti"), 332, QStringLiteral("HTI"));
  addCode(QStringLiteral("Heard Island and McDonald Islands"),
          334,
          QStringLiteral("HMD"));
  addCode(QStringLiteral("Holy See"), 336, QStringLiteral("VAT"));
  addCode(QStringLiteral("Honduras"), 340, QStringLiteral("HND"));
  addCode(QStringLiteral("Hungary"), 348, QStringLiteral("HUN"));
  addCode(QStringLiteral("Iceland"), 352, QStringLiteral("ISL"));
  addCode(QStringLiteral("India"), 356, QStringLiteral("IND"));
  addCode(QStringLiteral("Indonesia"), 360, QStringLiteral("IDN"));
  addCode(
    QStringLiteral("Iran (Islamic Republic of)"), 364, QStringLiteral("IRN"));
  addCode(QStringLiteral("Iraq"), 368, QStringLiteral("IRQ"));
  addCode(QStringLiteral("Ireland"), 372, QStringLiteral("IRL"));
  addCode(QStringLiteral("Isle of Man"), 833, QStringLiteral("IMN"));
  addCode(QStringLiteral("Israel"), 376, QStringLiteral("ISR"));
  addCode(QStringLiteral("Italy"), 380, QStringLiteral("ITA"));
  addCode(QStringLiteral("Jamaica"), 388, QStringLiteral("JAM"));
  addCode(QStringLiteral("Japan"), 392, QStringLiteral("JPN"));
  addCode(QStringLiteral("Jersey"), 832, QStringLiteral("JEY"));
  addCode(QStringLiteral("Jordan"), 400, QStringLiteral("JOR"));
  addCode(QStringLiteral("Kazakhstan"), 398, QStringLiteral("KAZ"));
  addCode(QStringLiteral("Kenya"), 404, QStringLiteral("KEN"));
  addCode(QStringLiteral("Kiribati"), 296, QStringLiteral("KIR"));
  addCode(QStringLiteral("Kuwait"), 414, QStringLiteral("KWT"));
  addCode(QStringLiteral("Kyrgyzstan"), 417, QStringLiteral("KGZ"));
  addCode(QStringLiteral("Lao People's Democratic Republic"),
          418,
          QStringLiteral("LAO"));
  addCode(QStringLiteral("Latvia"), 428, QStringLiteral("LVA"));
  addCode(QStringLiteral("Lebanon"), 422, QStringLiteral("LBN"));
  addCode(QStringLiteral("Lesotho"), 426, QStringLiteral("LSO"));
  addCode(QStringLiteral("Liberia"), 430, QStringLiteral("LBR"));
  addCode(QStringLiteral("Libya"), 434, QStringLiteral("LBY"));
  addCode(QStringLiteral("Liechtenstein"), 438, QStringLiteral("LIE"));
  addCode(QStringLiteral("Lithuania"), 440, QStringLiteral("LTU"));
  addCode(QStringLiteral("Luxembourg"), 442, QStringLiteral("LUX"));
  addCode(QStringLiteral("Madagascar"), 450, QStringLiteral("MDG"));
  addCode(QStringLiteral("Malawi"), 454, QStringLiteral("MWI"));
  addCode(QStringLiteral("Malaysia"), 458, QStringLiteral("MYS"));
  addCode(QStringLiteral("Maldives"), 462, QStringLiteral("MDV"));
  addCode(QStringLiteral("Mali"), 466, QStringLiteral("MLI"));
  addCode(QStringLiteral("Malta"), 470, QStringLiteral("MLT"));
  addCode(QStringLiteral("Marshall Islands"), 584, QStringLiteral("MHL"));
  addCode(QStringLiteral("Martinique"), 474, QStringLiteral("MTQ"));
  addCode(QStringLiteral("Mauritania"), 478, QStringLiteral("MRT"));
  addCode(QStringLiteral("Mauritius"), 480, QStringLiteral("MUS"));
  addCode(QStringLiteral("Mayotte"), 175, QStringLiteral("MYT"));
  addCode(QStringLiteral("Mexico"), 484, QStringLiteral("MEX"));
  addCode(QStringLiteral("Micronesia (Federated States of)"),
          583,
          QStringLiteral("FSM"));
  addCode(QStringLiteral("Monaco"), 492, QStringLiteral("MCO"));
  addCode(QStringLiteral("Mongolia"), 496, QStringLiteral("MNG"));
  addCode(QStringLiteral("Montenegro"), 499, QStringLiteral("MNE"));
  addCode(QStringLiteral("Montserrat"), 500, QStringLiteral("MSR"));
  addCode(QStringLiteral("Morocco"), 504, QStringLiteral("MAR"));
  addCode(QStringLiteral("Mozambique"), 508, QStringLiteral("MOZ"));
  addCode(QStringLiteral("Myanmar"), 104, QStringLiteral("MMR"));
  addCode(QStringLiteral("Namibia"), 516, QStringLiteral("NAM"));
  addCode(QStringLiteral("Nauru"), 520, QStringLiteral("NRU"));
  addCode(QStringLiteral("Nepal"), 524, QStringLiteral("NPL"));
  addCode(QStringLiteral("Netherlands"), 528, QStringLiteral("NLD"));
  addCode(QStringLiteral("New Caledonia"), 540, QStringLiteral("NCL"));
  addCode(QStringLiteral("New Zealand"), 554, QStringLiteral("NZL"));
  addCode(QStringLiteral("Nicaragua"), 558, QStringLiteral("NIC"));
  addCode(QStringLiteral("Niger"), 562, QStringLiteral("NER"));
  addCode(QStringLiteral("Nigeria"), 566, QStringLiteral("NGA"));
  addCode(QStringLiteral("Niue"), 570, QStringLiteral("NIU"));
  addCode(QStringLiteral("Norfolk Island"), 574, QStringLiteral("NFK"));
  addCode(QStringLiteral("North Macedonia"), 807, QStringLiteral("MKD"));
  addCode(
    QStringLiteral("Northern Mariana Islands"), 580, QStringLiteral("MNP"));
  addCode(QStringLiteral("Norway"), 578, QStringLiteral("NOR"));
  addCode(QStringLiteral("Oman"), 512, QStringLiteral("OMN"));
  addCode(QStringLiteral("Pakistan"), 586, QStringLiteral("PAK"));
  addCode(QStringLiteral("Palau"), 585, QStringLiteral("PLW"));
  addCode(QStringLiteral("Panama"), 591, QStringLiteral("PAN"));
  addCode(QStringLiteral("Papua New Guinea"), 598, QStringLiteral("PNG"));
  addCode(QStringLiteral("Paraguay"), 600, QStringLiteral("PRY"));
  addCode(QStringLiteral("Peru"), 604, QStringLiteral("PER"));
  addCode(QStringLiteral("Philippines"), 608, QStringLiteral("PHL"));
  addCode(QStringLiteral("Pitcairn"), 612, QStringLiteral("PCN"));
  addCode(QStringLiteral("Poland"), 616, QStringLiteral("POL"));
  addCode(QStringLiteral("Portugal"), 620, QStringLiteral("PRT"));
  addCode(QStringLiteral("Puerto Rico"), 630, QStringLiteral("PRI"));
  addCode(QStringLiteral("Qatar"), 634, QStringLiteral("QAT"));
  addCode(QStringLiteral("Republic of Korea"), 410, QStringLiteral("KOR"));
  addCode(QStringLiteral("Republic of Moldova"), 498, QStringLiteral("MDA"));
  addCode(QStringLiteral("Réunion"), 638, QStringLiteral("REU"));
  addCode(QStringLiteral("Romania"), 642, QStringLiteral("ROU"));
  addCode(QStringLiteral("Russian Federation"), 643, QStringLiteral("RUS"));
  addCode(QStringLiteral("Rwanda"), 646, QStringLiteral("RWA"));
  addCode(QStringLiteral("Saint Barthélemy"), 652, QStringLiteral("BLM"));
  addCode(QStringLiteral("Saint Helena"), 654, QStringLiteral("SHN"));
  addCode(QStringLiteral("Saint Kitts and Nevis"), 659, QStringLiteral("KNA"));
  addCode(QStringLiteral("Saint Lucia"), 662, QStringLiteral("LCA"));
  addCode(
    QStringLiteral("Saint Martin (French Part)"), 663, QStringLiteral("MAF"));
  addCode(
    QStringLiteral("Saint Pierre and Miquelon"), 666, QStringLiteral("SPM"));
  addCode(QStringLiteral("Saint Vincent and the Grenadines"),
          670,
          QStringLiteral("VCT"));
  addCode(QStringLiteral("Samoa"), 882, QStringLiteral("WSM"));
  addCode(QStringLiteral("San Marino"), 674, QStringLiteral("SMR"));
  addCode(QStringLiteral("Sao Tome and Principe"), 678, QStringLiteral("STP"));
  addCode(QStringLiteral("Sark"), 680, QStringLiteral(""));
  addCode(QStringLiteral("Saudi Arabia"), 682, QStringLiteral("SAU"));
  addCode(QStringLiteral("Senegal"), 686, QStringLiteral("SEN"));
  addCode(QStringLiteral("Serbia"), 688, QStringLiteral("SRB"));
  addCode(QStringLiteral("Seychelles"), 690, QStringLiteral("SYC"));
  addCode(QStringLiteral("Sierra Leone"), 694, QStringLiteral("SLE"));
  addCode(QStringLiteral("Singapore"), 702, QStringLiteral("SGP"));
  addCode(
    QStringLiteral("Sint Maarten (Dutch part)"), 534, QStringLiteral("SXM"));
  addCode(QStringLiteral("Slovakia"), 703, QStringLiteral("SVK"));
  addCode(QStringLiteral("Slovenia"), 705, QStringLiteral("SVN"));
  addCode(QStringLiteral("Solomon Islands"), 90, QStringLiteral("SLB"));
  addCode(QStringLiteral("Somalia"), 706, QStringLiteral("SOM"));
  addCode(QStringLiteral("South Africa"), 710, QStringLiteral("ZAF"));
  addCode(QStringLiteral("South Georgia and the South Sandwich Islands"),
          239,
          QStringLiteral("SGS"));
  addCode(QStringLiteral("South Sudan"), 728, QStringLiteral("SSD"));
  addCode(QStringLiteral("Spain"), 724, QStringLiteral("ESP"));
  addCode(QStringLiteral("Sri Lanka"), 144, QStringLiteral("LKA"));
  addCode(QStringLiteral("State of Palestine"), 275, QStringLiteral("PSE"));
  addCode(QStringLiteral("Sudan"), 729, QStringLiteral("SDN"));
  addCode(QStringLiteral("Suriname"), 740, QStringLiteral("SUR"));
  addCode(QStringLiteral("Svalbard and Jan Mayen Islands"),
          744,
          QStringLiteral("SJM"));
  addCode(QStringLiteral("Sweden"), 752, QStringLiteral("SWE"));
  addCode(QStringLiteral("Switzerland"), 756, QStringLiteral("CHE"));
  addCode(QStringLiteral("Syrian Arab Republic"), 760, QStringLiteral("SYR"));
  addCode(QStringLiteral("Tajikistan"), 762, QStringLiteral("TJK"));
  addCode(QStringLiteral("Thailand"), 764, QStringLiteral("THA"));
  addCode(QStringLiteral("Timor-Leste"), 626, QStringLiteral("TLS"));
  addCode(QStringLiteral("Togo"), 768, QStringLiteral("TGO"));
  addCode(QStringLiteral("Tokelau"), 772, QStringLiteral("TKL"));
  addCode(QStringLiteral("Tonga"), 776, QStringLiteral("TON"));
  addCode(QStringLiteral("Trinidad and Tobago"), 780, QStringLiteral("TTO"));
  addCode(QStringLiteral("Tunisia"), 788, QStringLiteral("TUN"));
  addCode(QStringLiteral("Turkey"), 792, QStringLiteral("TUR"));
  addCode(QStringLiteral("Turkmenistan"), 795, QStringLiteral("TKM"));
  addCode(
    QStringLiteral("Turks and Caicos Islands"), 796, QStringLiteral("TCA"));
  addCode(QStringLiteral("Tuvalu"), 798, QStringLiteral("TUV"));
  addCode(QStringLiteral("Uganda"), 800, QStringLiteral("UGA"));
  addCode(QStringLiteral("Ukraine"), 804, QStringLiteral("UKR"));
  addCode(QStringLiteral("United Arab Emirates"), 784, QStringLiteral("ARE"));
  addCode(
    QStringLiteral("United Kingdom of Great Britain and Northern Ireland"),
    826,
    QStringLiteral("GBR"));
  addCode(
    QStringLiteral("United Republic of Tanzania"), 834, QStringLiteral("TZA"));
  addCode(QStringLiteral("United States Minor Outlying Islands"),
          581,
          QStringLiteral("UMI"));
  addCode(
    QStringLiteral("United States of America"), 840, QStringLiteral("USA"));
  addCode(
    QStringLiteral("United States Virgin Islands"), 850, QStringLiteral("VIR"));
  addCode(QStringLiteral("Uruguay"), 858, QStringLiteral("URY"));
  addCode(QStringLiteral("Uzbekistan"), 860, QStringLiteral("UZB"));
  addCode(QStringLiteral("Vanuatu"), 548, QStringLiteral("VUT"));
  addCode(QStringLiteral("Venezuela (Bolivarian Republic of)"),
          862,
          QStringLiteral("VEN"));
  addCode(QStringLiteral("Viet Nam"), 704, QStringLiteral("VNM"));
  addCode(
    QStringLiteral("Wallis and Futuna Islands"), 876, QStringLiteral("WLF"));
  addCode(QStringLiteral("Western Sahara"), 732, QStringLiteral("ESH"));
  addCode(QStringLiteral("Yemen"), 887, QStringLiteral("YEM"));
  addCode(QStringLiteral("Zambia"), 894, QStringLiteral("ZMB"));
  addCode(QStringLiteral("Zimbabwe"), 716, QStringLiteral("ZWE"));
}
*/
