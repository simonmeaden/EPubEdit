#ifndef SVGCOLORNAMEDIALOG_H
#define SVGCOLORNAMEDIALOG_H

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QCheckBox>
#include <QColorDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDrag>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QRadioButton>
#include <QStringList>
#include <QTabWidget>
#include <QTableWidget>
#include <QtMath>

#include <bits/functional_hash.h>

//! \brief The color dialog allows the user to set/recover primary and secondary
//! colors.
//!
//! \sa color()
//! \sa setColor()
//! \sa textColor()
//! \sa setTextColor()
enum ColorType
{
  Primary,   //!< Used to set and return colors for the primary (left) display
  Secondary, //!< Used to set and return colors for the secondary (right)
             //!< display
};

/// \cond DO_NOT_DOCUMENT

class ColorDropDisplay : public QFrame
{
  Q_OBJECT
  //  enum Side
  //  {
  //    Left,  //!< The left hand (primary) side is specified
  //    Right, //!< The right hand (secondary) side is specified
  //  };

public:
  ColorDropDisplay(const QColor& color,
                   const QColor& dropColor,
                   QWidget* parent = nullptr);

  void setPrimaryColor(const QColor& color, const QString& name);
  void setPrimaryTextColor(const QColor& color, const QString& name);
  void setSecondaryColor(const QColor& color, const QString& name);
  void setSecondaryTextColor(const QColor& color, const QString& name);

  static const QString DISPLAYLABELRIGHT;
  static const QString DISPLAYLABELLEFT;
  static const QString DISPLAYBORDER;

  QColor primaryColor() const;
  QColor secondaryColor() const;
  QString primaryName() const;
  QString secondaryName() const;

signals:
  void primaryColorChanged(const QColor& color, const QString& name);
  void primaryTextColorChanged(const QColor& color, const QString& name);
  void secondaryColorChanged(const QColor& color, const QString& name);
  void secondaryTextColorChanged(const QColor& color, const QString& name);

protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;

private:
  QLabel* m_left;
  QLabel* m_right;
  QColor m_primaryColor;
  QColor m_primaryTextColor;
  QString m_primaryName;
  QString m_primaryTextName;
  QColor m_secondaryColor;
  QColor m_secondaryTextColor;
  QString m_secondaryName;
  QString m_secondaryTextName;
  bool m_colorSet, m_dropColorSet;

  QString calculateTextString(ColorType type);
  QString primaryColorToStyle();
  QString secondaryColorToStyle();
};

class ColorDragModel : public QAbstractTableModel
{
  Q_OBJECT

  struct Data
  {
    Data(const QString& n, const QColor& f, const QColor& b)
      : name(n)
      , fore(f)
      , back(b)
    {}
    QString name;
    QColor fore;
    QColor back;
  };
  typedef Data* Row;
  typedef Row* Column;

public:
  ColorDragModel(int rows, int columns);
  ~ColorDragModel();

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  int columnCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int,
                      Qt::Orientation,
                      int = Qt::DisplayRole) const override;
  void setColorData(int row,
                    int column,
                    const QString& name,
                    const QColor& back,
                    const QColor& fore);
  QModelIndex index(int row,
                    int column,
                    const QModelIndex& = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex&) const override;

private:
  Column* m_data;
  int m_rows = 0;
  int m_columns = 0;
};

struct ColorDragData
{
  int r;
  int g;
  int b;
  QString name;
};
Q_DECLARE_METATYPE(ColorDragData)

QDataStream&
operator<<(QDataStream& out, const ColorDragData& a);
QDataStream&
operator>>(QDataStream& in, ColorDragData& a);

class ColorDragTable : public QTableView
{
  Q_OBJECT
public:
  ColorDragTable(int rows, int columns, QWidget* parent = nullptr);

  QString name(const QModelIndex& index);
  QColor foreground(const QModelIndex& index);
  QColor background(const QModelIndex& index);
  void setData(int row, int column, bool x11, const QString& back);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent*) override;

private:
  QPoint m_dragStartPosition;
  QColor m_color;
  QString m_name;
  ColorDragModel* m_model;
  QSize m_size;
  QPixmap m_pixmap;
};

/// \endcond DO_NOT_DOCUMENT
// clang-format off

/*!
   \file extendedcolordialog.h extendedcolordialog.cpp
   \class ExtendedColorDialog extendedcolordialog.h
   \brief The ExtendedColorDialog class provides an extension of the standard
          QColorDialog class that includes selection pages for both the SVG color
          name set and the X11 color name set.

  The dialog allows the user to display two different color fields with a text color
  so that thet can be compared. Colors can be recovered using the color() and textColor()
  methods and initially set using the setColor() and setTextColor() methods. The dialog
  allows the user to switch between a standard color selection page and various SVG and
  X11 color sslection pages. Clicking on a color will set the primary (left) display.
  Alternatively the user can drag colors to either left or right display, or use the
  context menu to set the color, or text color of either.


   \since 5.7.0
   \license The MIT License
   \copyright © 2020 - 2021 Simon Meaden. All rights reserved.


*/
// clang-format on

class ExtendedColorDialog : public QDialog
{
  Q_OBJECT

public:
  //! \brief Constructor for ExtendedColorDialog.
  //!
  //! Display background and text default to white on white.
  //! \param parent - the parent widget.
  ExtendedColorDialog(QWidget* parent = nullptr);

  //! \brief Constructor for ExtendedColorDialog.
  //!
  //! Display primary (left) is set to primaryColor, with the text color set
  //! to a complementary color. Text displays the color hash name \#ffffff
  //! and a matching SVG or X11 color name if one exists.
  //! \param primaryColor - the initial color as a QColor object.
  //! \param parent - the parent widget.
  ExtendedColorDialog(const QColor& primaryColor, QWidget* parent = nullptr);

  //! \brief Constructor for ExtendedColorDialog.
  //!
  //! Display primary (left) is set to primaryColor, with the text color set
  //! to a complementary color. Text displays the color hash name \#ffffff
  //! and a matching SVG or X11 color name if one exists. If the string
  //! is invalid (either invalid name or hash name) it will default to
  //! white on white.
  //! \param primaryColor - the initial primary color as a name string. This can
  //! be either an SVG or X11 name string or a color hash \#ffffff string.
  //! \param parent - the parent widget.
  ExtendedColorDialog(const QString& primaryColor, QWidget* parent = nullptr);

  //! \brief Constructor for ExtendedColorDialog.
  //!
  //! Display primary (left) is set to primaryColor, with the text color set
  //! to a complementary color. Text displays the color hash name \#ffffff
  //! and a matching SVG or X11 color name if one exists.
  //! \param primaryColor - the initial primary color as a QColor object.
  //! \param secondaryColor - the initial secondary color as a QColor object.
  //! \param parent - the parent widget.
  ExtendedColorDialog(const QColor& primaryColor,
                      const QColor& secondaryColor,
                      QWidget* parent = nullptr);

  //! \brief Constructor for ExtendedColorDialog.
  //!
  //! Display primary (left) is set to primaryColor, with the text color set
  //! to a complementary color. Text displays the color hash name \#ffffff
  //! and a matching SVG or X11 color name if one exists. If the string
  //! is invalid (either invalid name or hash name) it will default to
  //! white on white.
  //! \param primaryColor - the initial primary color as a name string. This can
  //! \param secondaryColor - the initial secondary color as a name string. This
  //! can be either an SVG or X11 name string or a color hash \#ffffff string.
  //! \param parent - the parent widget.
  ExtendedColorDialog(const QString& primaryColor,
                      const QString& secondaryColor,
                      QWidget* parent = nullptr);

  //! \brief Returns the color as a QColor object.
  //!
  //! This would be the 'background' property in a stylesheet.
  //!
  //! type must be either ColorType::Primary for the left half of the display,
  //! or ColorType::Secondary for the right half.
  //!
  //! If no color has been selected or  'Cancel' has been pressed then the
  //! color will be Invalid.
  //!
  //! \param type - either ColorType::Primary or ColorType::Secondary.
  //! \sa rgb()
  //! \sa hsv()
  //! \sa hsl()
  //! \sa name()
  //! \sa hash()
  QColor color(ColorType type) const;

  //! \brief Returns the the text color as a QColor object.
  //!
  //! This would be the 'color' property in a stylesheet.
  //!
  //! type must be either ColorType::Primary for the left half of the display,
  //! or ColorType::Secondary for the right half.
  //!
  //! If no color has been selected or  'Cancel' has been pressed then the
  //! color will be Invalid.
  //!
  //! \param type - either ColorType::Primary or ColorType::Secondary.
  //! \sa rgb()
  //! \sa hsv()
  //! \sa hsl()
  //! \sa name()
  //! \sa hash()
  QColor textColor(ColorType type) const;

  //! \brief Sets the initial background color as a QColor object.
  //!
  //! type must be either ColorType::Primary for the left half of the display,
  //! or ColorType::Secondary for the right half.
  //!
  //! If this color has a matching SVG or X11 name this will be set
  //! automatically.
  //! \param type - either ColorType::Primary or ColorType::Secondary.
  //! \param color - a QColor object.
  void setColor(ColorType type, const QColor& color);

  //! \brief Sets the initial background color.
  //!
  //! If the name is a valid SVG or X11 name, or a color hash \#ffffff string
  //! the display will be set to this backfground color, otherwise it will be
  //! ignored.
  //!
  //! type must be either ColorType::Primary for the left half of the display,
  //! or ColorType::Secondary for the right half.
  //!
  //! If this color has a matching SVG or X11 name this will be displayed.
  //! \param type - either ColorType::Primary or ColorType::Secondary.
  //! \param name - an SVG or X11 color name or a color hash \#ffffff string.
  void setColor(ColorType type, const QString& name);

  //! \brief Sets the initial text color as a QColor object.
  //!
  //! type must be either ColorType::Primary for the left half of the display,
  //! or ColorType::Secondary for the right half.
  //!
  //! \param type - either ColorType::Primary or ColorType::Secondary.
  //! \param color - a QColor object.
  void setTextColor(ColorType type, const QColor& color);

  //! Sets the initial text color as a name string.
  //!
  //! type must be either ColorType::Primary for the left half of the display,
  //! or ColorType::Secondary for the right half.

  //! If the name is a valid SVG or X11 name, or a color hash \#ffffff string
  //! the display will be set to this backfground color, otherwise it will be
  //! ignored.
  //!
  //! \param type - either ColorType::Primary or ColorType::Secondary.
  //! \param name - an SVG or X11 color name or a color hash \#ffffff string.
  void setTextColor(ColorType type, const QString& name);

  //! Returns the selected color as a stylesheet %rgb(int, int, int) or
  //! %rgba(int, int, int, int) string.
  //!
  //! The type should be either ColorType::Primary or ColorType::Secondary.
  //!
  //! If alpha is specified an rgba() form will be returned.
  //! The value of alpha must it must be between 0 (transparent) and 100
  //! (opaque), the default, otherwise it will be ignored. If no color has been
  //! selected or  'Cancel' has been pressed then an empty string is returned.
  QString rgb(ColorType type, int alpha = -1) const;

  //! Returns the selected color as a stylesheet %hsv(int, int, int) or an
  //! %hsva(int, int, int, int) string.
  //!
  //! If alpha is specified an %hsva() form will be returned. The
  //! value of alpha must it must be between 0 (transparent) and 100 (opaque),
  //! the default, otherwise it will be ignored. If no color has been selected
  //! or  'Cancel' has been pressed then an empty string is returned.
  QString hsv(ColorType type, int alpha = 100) const;

  //! Returns the selected color as a stylesheet %hsl(int, int, int) or
  //! %hsla(int, int, int, int) form string.
  //!
  //! If alpha is specified an %hsla() form will be returned.
  //! The value of alpha must it must be between 0 (transparent) and 100
  //! (opaque), the default, otherwise it will be ignored. If no color has been
  //! selected or  'Cancel' has been pressed then an empty string is returned.
  QString hsl(ColorType type, int alpha = 100) const;

  //! Returns the selected color as a stylesheet name string.
  //!
  //! If no name has been selected then the color will be tested
  //! against all possible X11 name strings and if one matches this
  //! name will be returned, otherwise an empty string will be returned.
  //! If 'Cancel' has been pressed then an empty string is returned.
  QString name(ColorType type) const;

  //! Returns the selected color as a \#ffffff style string.
  //!
  //! If alpha is specified a \#ffffffff form will be returned. The value of
  //! alpha must it must be between 0 (transparent)
  //! and 100 (opaque), the default, otherwise it will be ignored.
  //! If no name has been selected or  'Cancel' has been pressed then
  //! an empty string is returned.
  QString hash(ColorType type, int alpha = false) const;

signals:
  //! This signal is emitted whenever the primary color changes in the dialog.
  //! The current color is specified by color.
  void primaryColorChanged(const QColor& color, const QString& name);
  //! This signal is emitted whenever the primary text color changes in the
  //! dialog. The current color is specified by color.
  void primaryTextColorChanged(const QColor& color, const QString& name);
  //! This signal is emitted whenever the secondary color changes in the dialog.
  //! The current color is specified by color.
  void secondaryColorChanged(const QColor& color, const QString& name);
  //! This signal is emitted whenever the secondary text color changes in the
  //! dialog. The current color is specified by color.
  void secondaryTextColorChanged(const QColor& color, const QString& name);

protected:
  //! overides QWidget::sizeHint()
  QSize sizeHint() const override;

private:
  QTabWidget* m_tabs;
  QColor m_primaryColor;
  QColor m_primaryTextColor;
  QColor m_secondaryColor;
  QColor m_secondaryTextColor;
  QString m_primaryName;
  QString m_primaryTextName;
  QString m_secondaryTextName;
  QString m_secondaryName;
  QColorDialog* m_colorDlg;
  ColorDropDisplay* m_display;
  //  Tabs m_currentTab;

  //  void acceptColor();
  void acceptChanges();
  void initGui();
  ColorDragTable* initSvgBlueGreen();
  ColorDragTable* initSvgGreen();
  ColorDragTable* initSvgRedYellowBrown();
  ColorDragTable* initX11BlueGreen();
  ColorDragTable* initX11Red();
  ColorDragTable* initX11YellowBrown();
  ColorDragTable* initX11MonoFrame();
  QDialogButtonBox* createBtnBox();
  ColorDropDisplay* createColorDisplay();
  ColorDragTable* createColorTable(int columns = 7);
  void customMenuRequested(QPoint pos);
  void primaryColorHasChanged(const QColor& color, const QString& name);
  void dialogColorHasChanged(const QColor& color);
  void secondaryColorHasChanged(const QColor& color, const QString& name);
  void primaryTextColorHasChanged(const QColor& color, const QString& name);
  void secondaryTextColorHasChanged(const QColor& color, const QString& name);

  void setPrimaryColor(QColor color, QString name);
  void setPrimaryTextColor(QColor color, QString name);
  void setSecondaryColor(QColor color, QString name);
  void setSecondaryTextColor(QColor color, QString name);

  void colorClicked(const QModelIndex& index);
  void primaryColorMenuClicked();
  void primaryTextColorMenuClicked();
  void secondaryColorMenuClicked();
  void secondaryTextColorMenuClicked();

  static const QString HASHACOLOR;
  static const QString HASHCOLOR;
  static const QString RGBCOLOR;
  static const QString RGBACOLOR;
  static const QString HSLCOLOR;
  static const QString HSLACOLOR;
  static const QString HSVCOLOR;
  static const QString HSVACOLOR;
};

#endif // SVGCOLORNAMEDIALOG_H
