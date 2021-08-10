/*
   Copyright 2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#ifndef LABELLEDCOMBOBOX_H
#define LABELLEDCOMBOBOX_H

#include "abstractlabelledwidget.h"
#include "sm_widgets_global.h"
#include <QComboBox>

/*!
   \file labelledcombobox.h labelledcombobox.cpp
   \class LabelledComboBox labelledcombobox.h
   \brief The LabelledComboBox is a wrapper for QLabel and QComboBox that
          provides a labelled combo box.
   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   All properties of the QComboBox are copied across to this widget.
*/
class SM_WIDGETS_SHARED_EXPORT LabelledComboBox : public AbstractLabelledWidget
{
  /*!
     \property LabelledComboBox::editable()
     \brief This property holds whether the combo box can be edited by the user

     @reimplements QComboBox::editable
     @accessor %isEditable(), %setEditable()
  */
  Q_PROPERTY(bool editable READ isEditable WRITE setEditable)

  /*!
     \property LabelledComboBox::count()
     \brief This property holds the number of items in the combobox

     @reimplements QComboBox::count
     @accessor %count()
  */
  Q_PROPERTY(int count READ count)

  /*!
     \property LabelledComboBox::currentText()
     \brief This property holds the current text

     @reimplements QComboBox::currentText
     @accessor %currentText(), %setCurrentText()
     @notifier currentTextChanged()
  */
  Q_PROPERTY(QString currentText READ currentText WRITE setCurrentText NOTIFY
               currentTextChanged USER true)

  /*!
     \property LabelledComboBox::currentIndex()
     \brief This property holds the index of the current item in the combobox.

     @reimplements QComboBox::currentIndex
     @accessor %currentIndex(), %setCurrentIndex()
     @notifier currentTextChanged()
  */
  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY
               currentIndexChanged)

  /*!
     \property LabelledComboBox::currentData()
     \brief This property holds the data for the current item

     @reimplements QComboBox::currentData
     @accessor %currentData()
  */
  Q_PROPERTY(QVariant currentData READ currentData)

  /*!
     \property LabelledComboBox::maxVisibleItems()
     \brief This property holds the maximum allowed size on screen of the
     combo box, measured in items

     @reimplements QComboBox::maxVisibleItems
     @accessor %maxVisibleItems(void), %setMaxVisibleItems()
  */
  Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems)

  /*!
     \property LabelledComboBox::maxCount()
     \brief This property holds the maximum number of items allowed in the
     combobox

     @reimplements QComboBox::maxCount
     @accessor %maxCount(void), %setMaxCount()
  */
  Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)

  /*!
     \property LabelledComboBox::insertPolicy()
     \brief This property holds the policy used to determine where user-inserted
     items should appear in the combobox

     @reimplements QComboBox::insertPolicy
     @accessor %insertPolicy(), %setInsertPolicy()
  */
  Q_PROPERTY(QComboBox::InsertPolicy insertPolicy READ insertPolicy WRITE
               setInsertPolicy)

  /*!
     \property LabelledComboBox::sizeAdjustPolicy()
     \brief This property holds the minimum number of characters that should fit
     into the combobox.

     @reimplements QComboBox::sizeAdjustPolicy
     @accessor %sizeAdjustPolicy(), %setSizeAdjustPolicy()
  */
  Q_PROPERTY(QComboBox::SizeAdjustPolicy sizeAdjustPolicy READ sizeAdjustPolicy
               WRITE setSizeAdjustPolicy)

  /*!
     \property LabelledComboBox::minimumContentsLength()
     \brief This property holds the minimum number of characters that should fit
     into the combobox.

     @reimplements QComboBox::minimumContentsLength
     @accessor %minimumContentsLength(), %setMinimumContentsLength()
  */
  Q_PROPERTY(int minimumContentsLength READ minimumContentsLength WRITE
               setMinimumContentsLength)

  /*!
     \property LabelledComboBox::iconSize()
     \brief TThis property holds the size of the icons shown in the combobox.

     @reimplements QComboBox::iconSize
     @accessor %iconSize(), %setIconSize()
  */
  Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  /*!
     \property LabelledComboBox::placeholderText()
     \brief Sets a placeholderText text shown when no valid index is set

     @reimplements QComboBox::placeholderText
     @accessor %placeholderText(), %setPlaceholderText()
  */
  Q_PROPERTY(
    QString placeholderText READ placeholderText WRITE setPlaceholderText)
#endif

  Q_OBJECT

protected:
  /// \cond DO_NOT_DOCUMENT
  struct WidgetFactory : AbstractLabelledWidget::WidgetFactory
  {
    virtual QWidget* newWidget(QWidget* parent) const
    {
      return new QComboBox(parent);
    }
  };

  explicit LabelledComboBox(WidgetFactory const& factory,
                            const QString& labelText,
                            QWidget* parent = nullptr);

  /// \endcond

public:
  //! \brief Constructor for LabelledComboBox.
  //!
  //! \param labelText the label text string.
  //! \param parent the parent widget.
  explicit LabelledComboBox(const QString& labelText,
                            QWidget* parent = nullptr);

  //! Changes the default factory combo box with a custom supplied one.
  void setCustomComboBox(QComboBox* replacement);

  //! @reimplements QComboBox::addItem(const QString& const QVariant&).
  void addItem(const QString& labelText, const QVariant& userdata = QVariant());

  //! @reimplements QComboBox::addItem(const QIcon&,const QString& const
  //! QVariant&).
  void addItem(const QIcon& icon,
               const QString& labelText,
               const QVariant& userdata = QVariant());

  //! @reimplements QComboBox::addItems(const QStringList&).
  void addItems(const QStringList& items);

  //! @reimplements QComboBox::completer() const.
  QCompleter* completer() const;

  //! @reimplements QComboBox::setCompleter().
  void setCompleter(QCompleter* completer);

  //! @reimplements QComboBox::count() const.
  int count() const;

  //! @reimplements QComboBox::maxCount() const.
  int maxCount() const;

  //! @reimplements QComboBox::setMaxCount().
  void setMaxCount(int max);

  //! @reimplements QComboBox::currentData() const.
  QVariant currentData(int role = Qt::UserRole) const;

  //! @reimplements QComboBox::currentIndex() const.
  int currentIndex() const;

  //! @reimplements QComboBox::currentText() const.
  QString currentText() const;

  //! @reimplements QComboBox::duplicatesEnabled().
  bool duplicatesEnabled() const;

  //! @reimplements QComboBox::setDuplicatesEnabled().
  void setDuplicatesEnabled(bool enable);

  //! @reimplements QComboBox::findData() const.
  int findData(const QVariant& data,
               int role = Qt::UserRole,
               Qt::MatchFlags flags = static_cast<Qt::MatchFlags>(
                 Qt::MatchExactly | Qt::MatchCaseSensitive)) const;

  //! @reimplements QComboBox::findText() const.
  int findText(const QString& labelText,
               Qt::MatchFlags flags = Qt::MatchExactly |
                                      Qt::MatchCaseSensitive) const;

  //! @reimplements QComboBox::iconSize() const.
  QSize iconSize() const;

  //! @reimplements QComboBox::hidePopup() const.
  void hidePopup() const;

  //! @reimplements QComboBox::insertItem(int index, const QString&, const
  //! QVariant&).
  void insertItem(int index,
                  const QString& labelText,
                  const QVariant& userData = QVariant());

  //! @reimplements QComboBox::insertItem(int index, const QIcon&, const
  //! QString&, const QVariant&).
  void insertItem(int index,
                  const QIcon& icon,
                  const QString& labelText,
                  const QVariant& userData = QVariant());

  //! @reimplements QComboBox::insertItems().
  void insertItems(int index, const QStringList& list);

  //! @reimplements QComboBox::insertPolicy() const.
  QComboBox::InsertPolicy insertPolicy() const;

  //! @reimplements QComboBox::insertSeparator().
  void insertSeparator(int index);

  //! @reimplements QComboBox::isEditable() const.
  bool isEditable() const;

  //! @reimplements QComboBox::itemData() const.
  QVariant itemData(int index, int role = Qt::UserRole) const;

  //! @reimplements QComboBox::itemDelegate() const.
  QAbstractItemDelegate* itemDelegate() const;

  //! @reimplements QComboBox::itemIcon() const.
  QIcon itemIcon(int index) const;

  //! @reimplements QComboBox::itemText() const.
  QString itemText(int index) const;

  //! @reimplements QComboBox::lineEdit() const.
  QLineEdit* lineEdit() const;

  //! @reimplements QComboBox::maxVisibleItems() const.
  int maxVisibleItems() const;

  //! @reimplements QComboBox::minimumContentsLength() const.
  int minimumContentsLength() const;

  //! @reimplements QComboBox::model() const.
  QAbstractItemModel* model() const;

  //! @reimplements QComboBox::modelColumn() const.
  int modelColumn() const;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  //! @reimplements QComboBox::placeholderText() const.
  QString placeholderText() const;

  //! @reimplements QComboBox::setPlaceholderText().
  void setPlaceholderText(const QString& placeholderText);
#endif

  //! @reimplements QComboBox::removeItem().
  void removeItem(int index);

  //! @reimplements QComboBox::rootModelIndex() const.
  QModelIndex rootModelIndex() const;

  //! @reimplements QComboBox::setEditable().
  void setEditable(bool editable);

  //! @reimplements QComboBox::setIconSize().
  void setIconSize(const QSize& size);

  //! @reimplements QComboBox::setInsertPolicy().
  void setInsertPolicy(QComboBox::InsertPolicy policy);

  //! @reimplements QComboBox::setItemData().
  void setItemData(int index, const QVariant& value, int role = Qt::UserRole);

  //! @reimplements QComboBox::setItemDelegate().
  void setItemDelegate(QAbstractItemDelegate* delegate);

  //! @reimplements QComboBox::setItemIcon().
  void setItemIcon(int index, const QIcon& icon);

  //! @reimplements QComboBox::setItemText().
  void setItemText(int index, const QString& labelText);

  //! @reimplements QComboBox::setLineEdit().
  void setLineEdit(QLineEdit* edit);

  //! @reimplements QComboBox::setMaxVisibleItems().
  void setMaxVisibleItems(int maxItems);

  //! @reimplements QComboBox::setMinimumContentsLength().
  void setMinimumContentsLength(int characters);

  //! @reimplements QComboBox::setModel().
  void setModel(QAbstractItemModel* model);

  //! @reimplements QComboBox::setModelColumn().
  void setModelColumn(int visibleColumn);

  //! @reimplements QComboBox::setRootModelIndex().
  void setRootModelIndex(const QModelIndex& index);

  //! @reimplements QComboBox::setSizeAdjustPolicy().
  void setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy policy);

  //! @reimplements QComboBox::setValidator().
  void setValidator(const QValidator* validator);

  //! @reimplements QComboBox::setView().
  void setView(QAbstractItemView* itemView);

  //! @reimplements QComboBox::showPopup().
  void showPopup();

  //! @reimplements QComboBox::sizeAdjustPolicy() const.
  QComboBox::SizeAdjustPolicy sizeAdjustPolicy() const;

  //! @reimplements QComboBox::validator() const.
  const QValidator* validator() const;

  //! @reimplements QComboBox::view() const.
  QAbstractItemView* view() const;

  //! @reimplements QComboBox::setCurrentText().
  void setCurrentText(const QString& labelText);

  //! @reimplements QComboBox::setCurrentIndex().
  void setCurrentIndex(int index);

signals:
  /*!
     \fn LabelledComboBox::activated(int index)

     \brief This signal is sent when the user chooses an item in the combobox.

     @from QComboBox::activated.
  */
  void activated(int index);
  /*!
     \fn LabelledComboBox::currentIndexChanged(int index)

     @from QComboBox::currentIndexChanged.
     @notprop currentIndex.
  */
  void currentIndexChanged(int index);
  /*!
     \fn LabelledComboBox::currentTextChanged(int index)

     @from QComboBox::currentTextChanged.
     @notprop currentText.
  */
  void currentTextChanged(const QString& labelText);
  /*!
     \fn LabelledComboBox::editTextChanged(const QString& text)

     @from QComboBox::editTextChanged.
  */
  void editTextChanged(const QString& labelText);
  /*!
     \fn LabelledComboBox::highlighted(int index)

     @from QComboBox::highlighted.
  */
  void highlighted(int index);
  /*!
     \fn LabelledComboBox::textActivated(const QString& text)

     @from QComboBox::textActivated.
  */
  void textActivated(const QString& labelText);
  /*!
     \fn LabelledComboBox::textHighlighted(const QString& text)

     @from QComboBox::textHighlighted.
  */
  void textHighlighted(const QString& labelText);

private:
  // overridding initGui(QString) from AbstractLabelledWidget
  void initGui(const QString& = QString());
};

#endif // LABELLEDCOMBOBOX_H
