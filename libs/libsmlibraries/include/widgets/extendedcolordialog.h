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

/*!
 * \enum ColorType extendedcolordialog.h
 *
 * Defines whether colors are primary or secondary.
 */
enum ColorType
{
  Primary,   //!< Used to set and return colors for the primary (left) display
  Secondary, //!< Used to set and return colors for the secondary (right)
             //!< display
};

// clang-format off

// These are declared in the cpp file as they are just internal classes.
class ColorDropDisplay;
class ColorDragModel;
struct ColorDragData;
class ColorDragTable;

/*!
   \file extendedcolordialog.h extendedcolordialog.cpp
   \class ExtendedColorDialog extendedcolordialog.h "include/widgets/extendedcolordialog.h"
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
   \license{The MIT License}
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
