/*
  Copyright 2020 Simon Meaden

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "labelledcombobox.h"

LabelledComboBox::LabelledComboBox(const QString& labelText, QWidget* parent)
  : LabelledComboBox(LabelledComboBox::WidgetFactory(), labelText, parent)
{}

LabelledComboBox::LabelledComboBox(const WidgetFactory& factory,
                                   const QString& labelText,
                                   QWidget* parent)
  : AbstractLabelledWidget(labelText, parent, factory)
{
  LabelledComboBox::initGui();
}

void
LabelledComboBox::setCustomComboBox(QComboBox* replacement)
{
  m_widget->deleteLater();
  m_widget = replacement;
  LabelledComboBox::initGui();
}

void
LabelledComboBox::addItem(const QString& text, const QVariant& userdata)
{
  qobject_cast<QComboBox*>(m_widget)->addItem(text, userdata);
}

void
LabelledComboBox::addItem(const QIcon& icon,
                          const QString& text,
                          const QVariant& userdata)
{
  qobject_cast<QComboBox*>(m_widget)->addItem(icon, text, userdata);
}

void
LabelledComboBox::addItems(const QStringList& items)
{
  qobject_cast<QComboBox*>(m_widget)->addItems(items);
}

QCompleter*
LabelledComboBox::completer() const
{
  return qobject_cast<QComboBox*>(m_widget)->completer();
}

void
LabelledComboBox::setCompleter(QCompleter* completer)
{
  qobject_cast<QComboBox*>(m_widget)->setCompleter(completer);
}

int
LabelledComboBox::count() const
{
  return qobject_cast<QComboBox*>(m_widget)->count();
}

int
LabelledComboBox::maxCount() const
{
  return qobject_cast<QComboBox*>(m_widget)->maxCount();
}

void
LabelledComboBox::setMaxCount(int max)
{
  qobject_cast<QComboBox*>(m_widget)->setMaxCount(max);
}

QVariant
LabelledComboBox::currentData(int role) const
{
  return qobject_cast<QComboBox*>(m_widget)->currentData(role);
}

int
LabelledComboBox::currentIndex() const
{
  return qobject_cast<QComboBox*>(m_widget)->currentIndex();
}

QString
LabelledComboBox::currentText() const
{
  return qobject_cast<QComboBox*>(m_widget)->currentText();
}

void
LabelledComboBox::setCurrentIndex(int index)
{
  qobject_cast<QComboBox*>(m_widget)->setCurrentIndex(index);
}

void
LabelledComboBox::setCurrentText(const QString& text)
{
  qobject_cast<QComboBox*>(m_widget)->setCurrentText(text);
}

bool
LabelledComboBox::duplicatesEnabled() const
{
  return qobject_cast<QComboBox*>(m_widget)->duplicatesEnabled();
}

void
LabelledComboBox::setDuplicatesEnabled(bool enable)
{
  qobject_cast<QComboBox*>(m_widget)->setDuplicatesEnabled(enable);
}

int
LabelledComboBox::findData(const QVariant& data,
                           int role,
                           Qt::MatchFlags flags) const
{
  return qobject_cast<QComboBox*>(m_widget)->findData(data, role, flags);
}

int
LabelledComboBox::findText(const QString& text, Qt::MatchFlags flags) const
{
  return qobject_cast<QComboBox*>(m_widget)->findText(text, flags);
}

QSize
LabelledComboBox::iconSize() const
{
  return qobject_cast<QComboBox*>(m_widget)->iconSize();
}

void
LabelledComboBox::hidePopup() const
{
  qobject_cast<QComboBox*>(m_widget)->hidePopup();
}

void
LabelledComboBox::insertItem(int index,
                             const QString& text,
                             const QVariant& userData)
{
  qobject_cast<QComboBox*>(m_widget)->insertItem(index, text, userData);
}

void
LabelledComboBox::insertItem(int index,
                             const QIcon& icon,
                             const QString& text,
                             const QVariant& userData)
{
  qobject_cast<QComboBox*>(m_widget)->insertItem(index, icon, text, userData);
}

void
LabelledComboBox::insertItems(int index, const QStringList& list)
{
  qobject_cast<QComboBox*>(m_widget)->insertItems(index, list);
}

QComboBox::InsertPolicy
LabelledComboBox::insertPolicy() const
{
  return qobject_cast<QComboBox*>(m_widget)->insertPolicy();
}

void
LabelledComboBox::insertSeparator(int index)
{
  qobject_cast<QComboBox*>(m_widget)->insertSeparator(index);
}

bool
LabelledComboBox::isEditable() const
{
  return qobject_cast<QComboBox*>(m_widget)->isEditable();
}

QVariant
LabelledComboBox::itemData(int index, int role) const
{
  return qobject_cast<QComboBox*>(m_widget)->itemData(index, role);
}

QAbstractItemDelegate*
LabelledComboBox::itemDelegate() const
{
  return qobject_cast<QComboBox*>(m_widget)->itemDelegate();
}

QIcon
LabelledComboBox::itemIcon(int index) const
{
  return qobject_cast<QComboBox*>(m_widget)->itemIcon(index);
}

QString
LabelledComboBox::itemText(int index) const
{
  return qobject_cast<QComboBox*>(m_widget)->itemText(index);
}

QLineEdit*
LabelledComboBox::lineEdit() const
{
  return qobject_cast<QComboBox*>(m_widget)->lineEdit();
}

int
LabelledComboBox::maxVisibleItems() const
{
  return qobject_cast<QComboBox*>(m_widget)->maxVisibleItems();
}

int
LabelledComboBox::minimumContentsLength() const
{
  return qobject_cast<QComboBox*>(m_widget)->minimumContentsLength();
}

QAbstractItemModel*
LabelledComboBox::model() const
{
  return qobject_cast<QComboBox*>(m_widget)->model();
}

int
LabelledComboBox::modelColumn() const
{
  return qobject_cast<QComboBox*>(m_widget)->modelColumn();
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
// these are only available in 15.0.0 and greater.
QString
LabelledComboBox::placeholderText() const
{
  return qobject_cast<QComboBox*>(m_widget)->placeholderText();
}

void
LabelledComboBox::setPlaceholderText(const QString& placeholderText)
{
  qobject_cast<QComboBox*>(m_widget)->setPlaceholderText(placeholderText);
}
#endif

void
LabelledComboBox::removeItem(int index)
{
  qobject_cast<QComboBox*>(m_widget)->removeItem(index);
}

QModelIndex
LabelledComboBox::rootModelIndex() const
{
  return qobject_cast<QComboBox*>(m_widget)->rootModelIndex();
}

void
LabelledComboBox::setEditable(bool editable)
{
  qobject_cast<QComboBox*>(m_widget)->setEditable(editable);
}

void
LabelledComboBox::setIconSize(const QSize& size)
{
  qobject_cast<QComboBox*>(m_widget)->setIconSize(size);
}

void
LabelledComboBox::setInsertPolicy(QComboBox::InsertPolicy policy)
{
  qobject_cast<QComboBox*>(m_widget)->setInsertPolicy(policy);
}

void
LabelledComboBox::setItemData(int index, const QVariant& value, int role)
{
  qobject_cast<QComboBox*>(m_widget)->setItemData(index, value, role);
}

void
LabelledComboBox::setItemDelegate(QAbstractItemDelegate* delegate)
{
  qobject_cast<QComboBox*>(m_widget)->setItemDelegate(delegate);
}

void
LabelledComboBox::setItemIcon(int index, const QIcon& icon)
{
  qobject_cast<QComboBox*>(m_widget)->setItemIcon(index, icon);
}

void
LabelledComboBox::setItemText(int index, const QString& text)
{
  qobject_cast<QComboBox*>(m_widget)->setItemText(index, text);
}

void
LabelledComboBox::setLineEdit(QLineEdit* edit)
{
  qobject_cast<QComboBox*>(m_widget)->setLineEdit(edit);
}

void
LabelledComboBox::setMaxVisibleItems(int maxItems)
{
  qobject_cast<QComboBox*>(m_widget)->setMaxVisibleItems(maxItems);
}

void
LabelledComboBox::setMinimumContentsLength(int characters)
{
  qobject_cast<QComboBox*>(m_widget)->setMinimumContentsLength(characters);
}

void
LabelledComboBox::setModel(QAbstractItemModel* model)
{
  qobject_cast<QComboBox*>(m_widget)->setModel(model);
}

void
LabelledComboBox::setModelColumn(int visibleColumn)
{
  qobject_cast<QComboBox*>(m_widget)->setModelColumn(visibleColumn);
}

void
LabelledComboBox::setRootModelIndex(const QModelIndex& index)
{
  qobject_cast<QComboBox*>(m_widget)->setRootModelIndex(index);
}

void
LabelledComboBox::setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy policy)
{
  qobject_cast<QComboBox*>(m_widget)->setSizeAdjustPolicy(policy);
}

void
LabelledComboBox::setValidator(const QValidator* validator)
{
  qobject_cast<QComboBox*>(m_widget)->setValidator(validator);
}

void
LabelledComboBox::setView(QAbstractItemView* itemView)
{
  qobject_cast<QComboBox*>(m_widget)->setView(itemView);
}

void
LabelledComboBox::showPopup()
{
  qobject_cast<QComboBox*>(m_widget)->showPopup();
}

QComboBox::SizeAdjustPolicy
LabelledComboBox::sizeAdjustPolicy() const
{
  return qobject_cast<QComboBox*>(m_widget)->sizeAdjustPolicy();
}

const QValidator*
LabelledComboBox::validator() const
{
  return qobject_cast<QComboBox*>(m_widget)->validator();
}

QAbstractItemView*
LabelledComboBox::view() const
{
  return qobject_cast<QComboBox*>(m_widget)->view();
}

void
LabelledComboBox::initGui(const QString&)
{
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<int>(&QComboBox::activated),
          this,
          &LabelledComboBox::activated);
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<int>(&QComboBox::currentIndexChanged),
          this,
          &LabelledComboBox::currentIndexChanged);
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<const QString&>(&QComboBox::currentTextChanged),
          this,
          &LabelledComboBox::currentTextChanged);
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<const QString&>(&QComboBox::editTextChanged),
          this,
          &LabelledComboBox::editTextChanged);
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<int>(&QComboBox::highlighted),
          this,
          &LabelledComboBox::highlighted);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<const QString&>(&QComboBox::textActivated),
          this,
          &LabelledComboBox::textActivated);
#else
  connect(reinterpret_cast<QComboBox*>(m_widget),
          qOverload<const QString&>(&QComboBox::activated),
          this,
          &LabelledComboBox::textActivated);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  connect(qobject_cast<QComboBox*>(m_widget),
          qOverload<const QString&>(&QComboBox::textHighlighted),
          this,
          &LabelledComboBox::textHighlighted);
#else
  connect(reinterpret_cast<QComboBox*>(m_widget),
          qOverload<const QString&>(&QComboBox::highlighted),
          this,
          &LabelledComboBox::textHighlighted);
#endif
}

/*====================================================================*/
