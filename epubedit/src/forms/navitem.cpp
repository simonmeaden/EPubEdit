#include "navitem.h"

//====================================================================
//=== NavBase
//====================================================================
NavBase::NavBase() {}

NavBase::~NavBase() {}

NavBase*
NavBase::child() const
{
  return m_child;
}

void
NavBase::setChild(NavBase* child)
{
  m_child = child;
}

NavBase*
NavBase::parent() const
{
  return m_parent;
}

void
NavBase::setParent(NavBase* parent)
{
  m_parent = parent;
}

//====================================================================
//=== NavAttribute
//====================================================================
const QString&
NavAttribute::name() const
{
  return m_name;
}

void
NavAttribute::setName(const QString& newName)
{
  m_name = newName;
}

const QString&
NavAttribute::value() const
{
  return m_value;
}

void
NavAttribute::setValue(const QString& newValue)
{
  m_value = newValue;
}

//====================================================================
//=== NavTag
//====================================================================
NavTag::~NavTag()
{
  qDeleteAll(m_attributes);
}

const QString&
NavTag::type() const
{
  return m_type;
}

void
NavTag::setType(const QString& newType)
{
  m_type = newType;
}

const QString&
NavTag::value() const
{
  return m_value;
}

void
NavTag::setValue(const QString& newValue)
{
  m_value = newValue;
}

QList<NavAttribute*>
NavTag::attributes() const
{
  return m_attributes;
}

NavAttribute*
NavTag::attribute(int index) const
{
  if (index >= 0 && index < m_attributes.size())
    return m_attributes.at(index);
  return nullptr;
}

void
NavTag::addAttribute(NavAttribute* newAttributes)
{
  m_attributes.append(newAttributes);
}

void
NavTag::removeAttribute(NavAttribute* attribute)
{
  m_attributes.removeOne(attribute);
}

void
NavTag::removeAttribute(int index)
{
  m_attributes.removeAt(index);
}

int
NavTag::attributeCount()
{
  return m_attributes.count();
}

NavTag*
NavTag::parent() const
{
  return m_parent;
}

void
NavTag::setParent(NavTag* newParent)
{
  m_parent = newParent;
}

NavTag*
NavTag::child() const
{
  return m_child;
}

void
NavTag::setChild(NavTag* newChild)
{
  m_child = newChild;
}

//====================================================================
//=== NavItem
//====================================================================
NavItem::NavItem()
  : NavBase()
{}

NavItem::~NavItem()
{
  qDeleteAll(m_tags);
}

QList<NavTag*>
NavItem::tags() const
{
  return m_tags;
}

NavTag*
NavItem::tag(int index) const
{
  if (index >= 0 && index < m_tags.size())
    return m_tags.at(index);
  return nullptr;
}

void
NavItem::addTag(NavTag* tag)
{
  m_tags.append(tag);
}

void
NavItem::removeTag(NavTag* tag)
{
  m_tags.removeOne(tag);
}

void
NavItem::removeTag(int index)
{
  m_tags.removeAt(index);
}

int
NavItem::tagCount()
{
  return m_tags.count();
}

QString NavItem::findFirstHref()
{
  for (auto& tag : tags()) {
    for (auto& attribute : tag->attributes()) {
      if (attribute->name()=="href") {
        return attribute->value();
      }
    }
  }
  return QString();
}

//====================================================================
//=== NavValueItem
//====================================================================
NavValueItem::NavValueItem()
  : NavItem()
{}

const QString&
NavValueItem::value() const
{
  return m_value;
}

void
NavValueItem::setValue(const QString& newValue)
{
  m_value = newValue;
}

int
NavValueItem::columnCount()
{
  return m_tags.isEmpty() ? 0 : 1;
}

//====================================================================
//=== NavListItem
//====================================================================
NavListItem::NavListItem()
  : NavItem()
{}

NavListItem::~NavListItem()
{
  qDeleteAll(m_items);
}

int
NavListItem::row() const
{
  auto parentList = dynamic_cast<NavListItem*>(parent());
  if (parentList) {
    return parentList->items().indexOf(const_cast<NavListItem*>(this));
  }
  return 0;
}

int
NavListItem::columnCount()
{
  return m_items.isEmpty() ? 0 : 1;
}

QList<NavItem*>
NavListItem::items() const
{
  return m_items;
}

NavItem*
NavListItem::item(int index) const
{
  if (index >= 0 && index < m_items.size())
    return m_items.at(index);
  return nullptr;
}

void
NavListItem::addItem(NavItem* item)
{
  m_items.append(item);
}

void
NavListItem::removeItem(NavItem* item)
{
  m_items.removeOne(item);
}

void
NavListItem::removeItem(int index)
{
  m_items.removeAt(index);
}

int
NavListItem::itemCount()
{
  return m_items.count();
}
