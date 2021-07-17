#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QStandardPaths>

class Config
{
public:
  Config();
  ~Config();


  QString getConfigDir() const;
  void setConfigDir(const QString &value);
  QString getConfigFile() const;
  void setConfigFile(const QString &value);

  int getStatusTimeout() const;
  void setStatusTimeout(int value);

private:
  QString configDir;
  QString configFile;

  int statusTimeout = 20; // timeout in seconds
};

#endif // CONFIG_H
