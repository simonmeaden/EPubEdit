#ifndef KEYMAP_H
#define KEYMAP_H

#include <QKeySequence>
#include <QMap>

#include "qyamlcpp.h"

/*!
  \ingroup utilities
  \file keymap.h
  \brief stores a set of Key maps for storage in a configuration file.
  \author Simon Meaden
  \date 2022-08-10

  Not really intended for anyone else to use as you can see from to
  paucity of mapping keys.

  Data is stored in a QMap<KeyEventMapper, KeyMapper> and the main part
  of the code is to allow the data to be stored/recovered from a YAML file
  using yaml-cpp.
 */

/*!
 * \enum KeyEventMapper
 *
 * Defines the key for a map of key/modifier structures.
 */
enum KeyEventMapper
{
  NoKey, //!< No key was pressed.

  PreviousBookmark, //!< Go to the previous bookmark
  NextBookmark,     //!< Go to the next bookmark.
  GoToBookmark,     //!< Go to selected bookmark.
  AddBookmark,      //!< Add a new bookmark.
  ToggleBookmark,   //!< Toggle current bookmark.
  EditBookmark,     //!< Edit bookmark.
  RemoveBookmark,   //!< Remove current bookmark.
  ClearBookmarks,   //!< Clears all bookmarks.

  NextTag,     //!< Go to the previous tag
  PreviousTag, //!< Go to the next tag.

  Options, //!< Calls options dialog
};

/*!
 * \struct KeyMapper
 * \brief Stores a key/modifier value for a key press.
 */
struct KeyMapper
{

  //! Default constructor
  KeyMapper() {}
  //! Constructor for KeyMapper specifying Key value and modifiers
  KeyMapper(Qt::Key key, Qt::KeyboardModifiers modifiers)
  {
    this->key = key;
    this->modifiers = modifiers;
  }

  //! Validity check for KeyMapper
  bool isValid()
  {
    if (key != Qt::Key_unknown)
      return true;
    return false;
  }

  QKeySequence keySequence() { return QKeySequence(key, modifiers); }

  //! QKey value
  Qt::Key key = Qt::Key_unknown;
  //! Key modifiers
  Qt::KeyboardModifiers modifiers = Qt::NoModifier;
};
Q_DECLARE_METATYPE(KeyMapper)

namespace YAML {

//! \brief A yaml-cpp emitter operator for KeyMapper.
Emitter&
operator<<(Emitter& out, const KeyMapper v);

//! \brief A yaml-cpp emitter operator for KeyEventMapper.
Emitter&
operator<<(Emitter& out, const KeyEventMapper v);

//! \brief A yaml-cpp emitter operator for  QMap<KeyEventMapper, KeyMapper>.
Emitter&
operator<<(Emitter& out, const QMap<KeyEventMapper, KeyMapper>& v);

//! \brief A yaml-cpp convertion class for KeyEventMapper.
template<>
struct convert<KeyEventMapper>
{
  //! encode function for QString
  static Node encode(const KeyEventMapper rhs);

  //! decode function for QString
  static bool decode(const Node& node, KeyEventMapper& rhs);
};

//! \brief A yaml-cpp convertion class for KeyMapper.
template<>
struct convert<KeyMapper>
{
  //! encode function for QString
  static Node encode(const KeyMapper rhs);

  //! decode function for QString
  static bool decode(const Node& node, KeyMapper rhs);
};

} // end of namespace YAML

#endif // KEYMAP_H
