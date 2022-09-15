#include "utilities/keymap.h"

//====================================================================
//=== QYamlCpp extensions.
//====================================================================
namespace YAML {

Emitter&
operator<<(Emitter& out, const KeyMapper v)
{
  out << Key << "key" << Value << v.key << Key << "modifier"
      << Value << v.modifiers;
  return out;
}

Emitter&
operator<<(Emitter& out, const KeyEventMapper v)
{
  out << Value << int(v);
  return out;
}

Emitter&
operator<<(Emitter& out, const QMap<KeyEventMapper, KeyMapper>& v)
{
  //  out << BeginMap;
  for (auto it = v.cbegin(); it != v.cend(); ++it) {
    auto key = it.key();
    auto value = it.value();
    out << Key << int(key);
    out << Value;
    out << BeginMap;
    out << Key << "key" << Value << value.key;
    out << Key << "modifier" << Value << value.modifiers;
    out << EndMap;
  }
  //  out << YAML::EndMap;
  return out;
}

Node
convert<KeyEventMapper>::encode(const KeyEventMapper rhs)
{
  Node node;
  node = int(rhs);
  return node;
}

bool
convert<KeyEventMapper>::decode(const Node& node, KeyEventMapper& rhs)
{
  if (!node.IsScalar()) {
    return false;
  }

  rhs = KeyEventMapper(node.as<int>());

  return true;
}

Node
convert<KeyMapper>::encode(const KeyMapper rhs)
{
  Node node;
  node["key"] = int(rhs.key);
  node["modifier"] = int(rhs.modifiers);
  return node;
}

bool
convert<KeyMapper>::decode(const Node& node, KeyMapper rhs)
{
  if (!node.IsMap()) {
    return false;
  }

  auto key = node["key"].as<int>();
  auto value = node["modifier"].as<int>();
  rhs.key = Qt::Key(key);
  rhs.modifiers = Qt::KeyboardModifiers(value);

  return true;
}

} // end of namespace YAML

////====================================================================
////=== KeyMap
////====================================================================
//KeyMap::KeyMap()
//  : QMap<KeyEventMapper, KeyMapper>()
//{
//}
