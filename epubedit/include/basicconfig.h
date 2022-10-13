#ifndef BASICCONFIG_H
#define BASICCONFIG_H

#include <QObject>
#include <QStandardPaths>

#include "qyamlcpp.h"

class BCP47Languages;
class BasicConfig : public QObject
{
  Q_OBJECT
public:
  explicit BasicConfig(QObject* parent = nullptr);
   BasicConfig(BCP47Languages* languages, QObject* parent = nullptr);
  ~BasicConfig();

  virtual bool save(const QString& filename) = 0;
  virtual bool load(const QString& filename) = 0;

  const QString& configDirectory() const;
  void setConfigDirectory(const QString& ConfigDirectory);

  const QString& homeDirectory() const;
  void setHomeDirectory(const QString& homeDirectory);

  BCP47Languages* languages() const;

  int statusTimeout() const;
  void setStatusTimeout(int value);

  static const int StatusTimeout = 20;

signals:
  void sendStatusMessage(const QString& message, int timeout);
  void sendLogMessage(const QString& message);

protected:
  QString m_configDirectory;
  QString m_homeDirectory;
  BCP47Languages* m_languages;
  int m_statusTimeout; // timeout in seconds
  bool m_modified = false;

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);

  static const QString STATUS_TIMEOUT;
};

#endif // BASICCONFIG_H
