#ifndef EPUBTITLE_H
#define EPUBTITLE_H

#include <QObject>
#include <QSharedPointer>

class EPubTitle : public QObject
{
  Q_OBJECT
public:
  explicit EPubTitle(QObject* parent = nullptr);

  QString id();
  void setId(QString id);
  QString title() const;
  void setTitle(const QString& title);

signals:

private:
  QString m_id;
  QString m_title;
};
typedef QSharedPointer<EPubTitle> Title;
typedef QList<Title> TitleList;
typedef QMap<int, Title> OrderedTitleMap;

#endif // EPUBTITLE_H
