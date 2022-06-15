#ifndef NAVITEM_H
#define NAVITEM_H

#include <QList>
#include <QObject>
#include <QString>

class NavBase
{
public:
  NavBase();
  virtual ~NavBase();

  NavBase* child() const;
  void setChild(NavBase* child);

  NavBase* parent() const;
  void setParent(NavBase* parent);

private:
  NavBase* m_child = nullptr;
  NavBase* m_parent = nullptr;
};

class NavAttribute /*: public NavBase*/
{
public:
  const QString& name() const;
  void setName(const QString& newName);

  const QString& value() const;
  void setValue(const QString& newValue);

private:
  QString m_name;
  QString m_value;
};

class NavTag : public NavBase
{
public:
  ~NavTag();

  const QString& type() const;
  void setType(const QString& newType);

  const QString& value() const;
  void setValue(const QString& newValue);

  QList<NavAttribute*> attributes() const;
  NavAttribute* attribute(int index) const;
  void addAttribute(NavAttribute* newAttributes);
  void removeAttribute(NavAttribute* attribute);
  void removeAttribute(int index);
  int attributeCount();

  NavTag* parent() const;
  void setParent(NavTag* newParent);

  NavTag* child() const;
  void setChild(NavTag* newChild);

private:
  NavTag* m_parent = nullptr;
  NavTag* m_child = nullptr;
  QString m_type;
  QString m_value;
  QList<NavAttribute*> m_attributes;
};

class NavItem : public NavBase
{
public:
  NavItem();
  NavItem(const NavItem* other)
    : NavBase()
  {
    m_child = other->m_child;
    m_parent = other->m_parent;
    m_tags = other->m_tags;
  }
  ~NavItem();

  virtual int columnCount() { return -1; }

  QList<NavTag*> tags() const;
  NavTag* tag(int index) const;
  void addTag(NavTag* tag);
  void removeTag(NavTag* tag);
  void removeTag(int index);
  int tagCount();

  QString findFirstHref();

protected:
  NavItem* m_child = nullptr;
  NavItem* m_parent = nullptr;
  QList<NavTag*> m_tags;
};

class NavValueItem : public NavItem
{
public:
  NavValueItem();

  int columnCount();

  const QString& value() const;
  void setValue(const QString& newValue);

private:
  QString m_value;
};

class NavListItem : public NavItem
{
public:
  NavListItem();
  ~NavListItem();

  int row() const;
  int columnCount();

  QList<NavItem*> items() const;
  NavItem* item(int index) const;
  void addItem(NavItem* item);
  void removeItem(NavItem* item);
  void removeItem(int index);
  int itemCount();

private:
  QList<NavItem*> m_items;
};
Q_DECLARE_METATYPE(NavAttribute);
Q_DECLARE_METATYPE(NavTag);
Q_DECLARE_METATYPE(NavItem);
Q_DECLARE_METATYPE(NavValueItem);
Q_DECLARE_METATYPE(NavListItem);

#endif // NAVITEM_H
