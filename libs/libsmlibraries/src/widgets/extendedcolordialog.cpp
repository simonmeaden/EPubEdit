#include "widgets/extendedcolordialog.h"
#include "utilities/x11colors.h"

#include <QtDebug>

//! \brief The color dialog allows the user to set/recover primary and secondary
//! colors.
//!
//! \sa color()
//! \sa setColor()
//! \sa textColor()
//! \sa setTextColor()

//namespace {

///*!
// * \internal
// * \class ColorDropDisplay extendedcolordialog.h "include/widgets/extendedcolordialog.h"
// */
//class ColorDropDisplay : public QFrame
//{
//  Q_OBJECT
//  //  enum Side
//  //  {
//  //    Left,  //!< The left hand (primary) side is specified
//  //    Right, //!< The right hand (secondary) side is specified
//  //  };

//public:
//  ColorDropDisplay(const QColor& color,
//                   const QColor& dropColor,
//                   QWidget* parent = nullptr);

//  void setPrimaryColor(const QColor& color, const QString& name);
//  void setPrimaryTextColor(const QColor& color, const QString& name);
//  void setSecondaryColor(const QColor& color, const QString& name);
//  void setSecondaryTextColor(const QColor& color, const QString& name);

//  static const QString DISPLAYLABELRIGHT;
//  static const QString DISPLAYLABELLEFT;
//  static const QString DISPLAYBORDER;

//  QColor primaryColor() const;
//  QColor secondaryColor() const;
//  QString primaryName() const;
//  QString secondaryName() const;

//signals:
//  void primaryColorChanged(const QColor& color, const QString& name);
//  void primaryTextColorChanged(const QColor& color, const QString& name);
//  void secondaryColorChanged(const QColor& color, const QString& name);
//  void secondaryTextColorChanged(const QColor& color, const QString& name);

//protected:
//  void dragEnterEvent(QDragEnterEvent* event) override;
//  void dropEvent(QDropEvent* event) override;

//private:
//  QLabel* m_left;
//  QLabel* m_right;
//  QColor m_primaryColor;
//  QColor m_primaryTextColor;
//  QString m_primaryName;
//  QString m_primaryTextName;
//  QColor m_secondaryColor;
//  QColor m_secondaryTextColor;
//  QString m_secondaryName;
//  QString m_secondaryTextName;
//  bool m_colorSet, m_dropColorSet;

//  QString calculateTextString(ColorType type);
//  QString primaryColorToStyle();
//  QString secondaryColorToStyle();
//};

//class ColorDragModel : public QAbstractTableModel
//{
//  Q_OBJECT

//  struct Data
//  {
//    Data(const QString& n, const QColor& f, const QColor& b)
//      : name(n)
//      , fore(f)
//      , back(b)
//    {}
//    QString name;
//    QColor fore;
//    QColor back;
//  };
//  typedef Data* Row;
//  typedef Row* Column;

//public:
//  ColorDragModel(int rows, int columns);
//  ~ColorDragModel();

//  Qt::ItemFlags flags(const QModelIndex& index) const override;
//  int rowCount(const QModelIndex& = QModelIndex()) const override;
//  int columnCount(const QModelIndex& = QModelIndex()) const override;
//  QVariant data(const QModelIndex& index,
//                int role = Qt::DisplayRole) const override;
//  QVariant headerData(int,
//                      Qt::Orientation,
//                      int = Qt::DisplayRole) const override;
//  void setColorData(int row,
//                    int column,
//                    const QString& name,
//                    const QColor& back,
//                    const QColor& fore);
//  QModelIndex index(int row,
//                    int column,
//                    const QModelIndex& = QModelIndex()) const override;
//  QModelIndex parent(const QModelIndex&) const override;

//private:
//  Column* m_data;
//  int m_rows = 0;
//  int m_columns = 0;
//};

//struct ColorDragData
//{
//  int r;
//  int g;
//  int b;
//  QString name;
//};

//QDataStream&
//operator<<(QDataStream& out, const ColorDragData& a);
//QDataStream&
//operator>>(QDataStream& in, ColorDragData& a);

//class ColorDragTable : public QTableView
//{
//  Q_OBJECT
//public:
//  ColorDragTable(int rows, int columns, QWidget* parent = nullptr);

//  QString name(const QModelIndex& index);
//  QColor foreground(const QModelIndex& index);
//  QColor background(const QModelIndex& index);
//  void setData(int row, int column, bool x11, const QString& back);

//protected:
//  void mousePressEvent(QMouseEvent* event) override;
//  void mouseMoveEvent(QMouseEvent* event) override;
//  void dragEnterEvent(QDragEnterEvent* event) override;
//  void dragMoveEvent(QDragMoveEvent*) override;

//private:
//  QPoint m_dragStartPosition;
//  QColor m_color;
//  QString m_name;
//  ColorDragModel* m_model;
//  QSize m_size;
//  QPixmap m_pixmap;
//};

//Q_DECLARE_METATYPE(ColorDragData)


const QString ColorDropDisplay::DISPLAYLABELRIGHT =
  "QLabel {"
  "color: %1;"
  "background-color: %2;"
  "border: 0px;"
  "border-top-left-radius: 0px;"
  "border-bottom-left-radius: 0px;"
  "border-top-right-radius: 25px;"
  "border-bottom-right-radius: 25px"
  "}";
const QString ColorDropDisplay::DISPLAYLABELLEFT =
  "QLabel {"
  "color: %1;"
  "background-color: %2;"
  "border: 0px;"
  "border-top-left-radius: 25px;"
  "border-bottom-left-radius: 25px;"
  "border-top-right-radius: 0px;"
  "border-bottom-right-radius: 0px"
  "}";
const QString ColorDropDisplay::DISPLAYBORDER =
  "QFrame {"
  "border: 2px solid darkslategray; "
  "border-radius: 25px;"
  "}";
const QString ExtendedColorDialog::HASHCOLOR = "#%1%2%3";
const QString ExtendedColorDialog::HASHACOLOR = "#%1%2%3%4";
const QString ExtendedColorDialog::RGBCOLOR = "rgb(%1, %2, %3)";
const QString ExtendedColorDialog::RGBACOLOR = "rgba(%1, %2, %3, %4%)";
const QString ExtendedColorDialog::HSLCOLOR = "hsl(%1, %2, %3)";
const QString ExtendedColorDialog::HSLACOLOR = "hsla(%1, %2, %3, %4%)";
const QString ExtendedColorDialog::HSVCOLOR = "hsv(%1, %2, %3)";
const QString ExtendedColorDialog::HSVACOLOR = "hsva(%1, %2, %3,%4%)";

/// \endcond

ExtendedColorDialog::ExtendedColorDialog(QWidget* parent)
  : QDialog(parent)
  , m_primaryColor(QColor(Qt::white))
  , m_secondaryColor(QColor(Qt::white))
{
  initGui();
}

ExtendedColorDialog::ExtendedColorDialog(const QColor& primaryColor,
                                         QWidget* parent)
  : QDialog(parent)
  , m_primaryColor(primaryColor)
  , m_secondaryColor(Qt::white)
{
  initGui();
  setPrimaryColor(primaryColor, "white");
}

ExtendedColorDialog::ExtendedColorDialog(const QString& primaryColor,
                                         QWidget* parent)
  : QDialog(parent)
{
  initGui();
  auto color = QColorConstants::svgOrX11Color(primaryColor);
  setPrimaryColor(color, primaryColor);
  setSecondaryColor(color, primaryColor);
}

ExtendedColorDialog::ExtendedColorDialog(const QColor& primaryColor,
                                         const QColor& secondaryColor,
                                         QWidget* parent)
  : QDialog(parent)
{
  initGui();
  auto pColor = QColorConstants::svgOrX11Name(primaryColor);
  setPrimaryColor(primaryColor, pColor);
  pColor = QColorConstants::svgOrX11Name(secondaryColor);
  setSecondaryColor(secondaryColor, pColor);
}

ExtendedColorDialog::ExtendedColorDialog(const QString& primaryColor,
                                         const QString& secondaryColor,
                                         QWidget* parent)
  : QDialog(parent)
{
  initGui();
  auto color = QColorConstants::svgOrX11Color(primaryColor);
  setPrimaryColor(color, primaryColor);
  color = QColorConstants::svgOrX11Color(secondaryColor);
  if (color == Qt::white) {
    setSecondaryColor(color, "white");
  } else {
    setSecondaryColor(color, secondaryColor);
  }
}

void
ExtendedColorDialog::initGui()
{
  setWindowTitle(tr("Choose Color"));

  m_tabs = new QTabWidget(this);
  auto layout = new QGridLayout;
  layout->addWidget(m_tabs, 0, 0);
  layout->addWidget((m_display = createColorDisplay()), 1, 0);
  layout->addWidget(createBtnBox(), 2, 0);
  setLayout(layout);

  m_colorDlg = new QColorDialog(m_primaryColor, this);
  m_tabs->setTabToolTip(0, tr("Standard colour dialog"));
  m_colorDlg->setOptions(QColorDialog::ShowAlphaChannel |
                         QColorDialog::DontUseNativeDialog |
                         QColorDialog::NoButtons);
  m_colorDlg->setCurrentColor(QColor(0, 0, 0, 0));
  m_tabs->addTab(m_colorDlg, "Chooser");
  connect(m_colorDlg,
          &QColorDialog::currentColorChanged,
          this,
          &ExtendedColorDialog::dialogColorHasChanged);

  m_tabs->addTab(initSvgBlueGreen(), "SVG B/G");
  m_tabs->setTabToolTip(1, tr("SVG blue and green Colour shades"));
  m_tabs->addTab(initSvgRedYellowBrown(), "SVG R/Y/Brn");
  m_tabs->setTabToolTip(2, tr("SVG red, yellow and brown colour shades"));
  m_tabs->addTab(initX11BlueGreen(), "X11 Blue/Green");
  m_tabs->setTabToolTip(3, tr("X11 blue and green Colour shades"));
  m_tabs->addTab(initX11Red(), "X11 Red/Violet");
  m_tabs->setTabToolTip(4, tr("X11 red, yellow and brown colour shades"));
  m_tabs->addTab(initX11YellowBrown(), "X11 Yellow/Brown");
  m_tabs->setTabToolTip(5, tr("X11 red, yellow and brown colour shades"));
  m_tabs->addTab(initX11MonoFrame(), "X11 Mono");
  m_tabs->setTabToolTip(6, tr("X11 monochrome colour shades"));
}

void
ExtendedColorDialog::colorClicked(const QModelIndex& index)
{
  auto table = qobject_cast<ColorDragTable*>(sender());
  if (index.isValid()) {
    m_primaryColor = table->background(index);
    m_primaryName = table->name(index);
    m_display->setPrimaryColor(m_primaryColor, m_primaryName);
  }
}

void
ExtendedColorDialog::primaryColorHasChanged(const QColor& color,
                                            const QString& name)
{
  // receives color/name from display.
  m_primaryColor = color;
  m_primaryName = name;
  emit primaryColorChanged(m_primaryColor, m_primaryName);
}

void
ExtendedColorDialog::dialogColorHasChanged(const QColor& color)
{
  m_primaryColor = color;
  m_primaryName = QColorConstants::svgOrX11Name(color);
  m_display->setPrimaryColor(m_primaryColor, m_primaryName);
}

void
ExtendedColorDialog::secondaryColorHasChanged(const QColor& color,
                                              const QString& name)
{
  // receives color/name from display.
  m_secondaryColor = color;
  m_secondaryName = name;
  emit primaryColorChanged(m_secondaryColor, m_secondaryName);
}

QDialogButtonBox*
ExtendedColorDialog::createBtnBox()
{
  auto btnbox =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btnbox,
          &QDialogButtonBox::accepted,
          this,
          &ExtendedColorDialog::acceptChanges);
  connect(btnbox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  return btnbox;
}

ColorDropDisplay*
ExtendedColorDialog::createColorDisplay()
{
  auto display = new ColorDropDisplay(m_primaryColor, m_secondaryColor, this);
  display->setMinimumSize(QSize(300, 90));
  display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  connect(display,
          &ColorDropDisplay::primaryColorChanged,
          this,
          &ExtendedColorDialog::primaryColorHasChanged);
  connect(display,
          &ColorDropDisplay::primaryTextColorChanged,
          this,
          &ExtendedColorDialog::primaryTextColorHasChanged);
  connect(display,
          &ColorDropDisplay::secondaryColorChanged,
          this,
          &ExtendedColorDialog::secondaryColorHasChanged);
  connect(display,
          &ColorDropDisplay::secondaryTextColorChanged,
          this,
          &ExtendedColorDialog::secondaryTextColorHasChanged);
  return display;
}

ColorDragTable*
ExtendedColorDialog::createColorTable(int columns)
{
  auto table = new ColorDragTable(17, columns, this);
  table->setContextMenuPolicy(Qt::CustomContextMenu);
  table->setSelectionMode(QTableView::NoSelection);
  //  table->setSelectionBehavior(QTableView::SelectItems);
  table->setDragDropMode(QTableView::DragOnly);
  table->horizontalHeader()->setVisible(false);
  table->verticalHeader()->setVisible(false);
  table->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  table->setStyleSheet("QTableView { gridline-color: white; }");
  connect(
    table, &ColorDragTable::clicked, this, &ExtendedColorDialog::colorClicked);
  connect(table,
          &QTableView::customContextMenuRequested,
          this,
          &ExtendedColorDialog::customMenuRequested);
  return table;
}

void
ExtendedColorDialog::customMenuRequested(QPoint pos)
{
  auto table = qobject_cast<ColorDragTable*>(sender());
  if (table) {
    auto index = table->indexAt(pos);

    auto menu = new QMenu(this);
    auto act = new QAction(tr("Set Primary Colour"), this);
    act->setData(index);
    connect(act,
            &QAction::triggered,
            this,
            &ExtendedColorDialog::primaryColorMenuClicked);
    menu->addAction(act);
    act = new QAction(tr("Set Primary Text Colour"), this);
    act->setData(index);
    connect(act,
            &QAction::triggered,
            this,
            &ExtendedColorDialog::primaryTextColorMenuClicked);
    menu->addAction(act);
    act = new QAction(tr("Set Secondary Colour"), this);
    act->setData(index);
    connect(act,
            &QAction::triggered,
            this,
            &ExtendedColorDialog::secondaryColorMenuClicked);
    menu->addAction(act);
    act = new QAction(tr("Set Secondary Text Colour"), this);
    act->setData(index);
    connect(act,
            &QAction::triggered,
            this,
            &ExtendedColorDialog::secondaryTextColorMenuClicked);
    menu->addAction(act);
    menu->popup(table->viewport()->mapToGlobal(pos));
  }
}

void
ExtendedColorDialog::primaryTextColorHasChanged(const QColor& color,
                                                const QString& name)
{
  m_primaryTextColor = color;
  m_primaryTextName = name;
  emit primaryTextColorChanged(color, name);
}

void
ExtendedColorDialog::secondaryTextColorHasChanged(const QColor& color,
                                                  const QString& name)
{
  m_secondaryTextColor = color;
  m_secondaryTextName = name;
  emit secondaryTextColorChanged(color, name);
}

void
ExtendedColorDialog::primaryColorMenuClicked()
{
  auto act = dynamic_cast<QAction*>(sender());
  auto index = act->data().value<QModelIndex>();
  if (index.isValid()) {
    m_primaryColor = index.data(Qt::BackgroundRole).value<QColor>();
    m_primaryName = QColorConstants::svgOrX11Name(m_primaryColor);
    m_display->setPrimaryColor(m_primaryColor, m_primaryName);
  }
}

void
ExtendedColorDialog::secondaryColorMenuClicked()
{
  auto act = dynamic_cast<QAction*>(sender());
  auto index = act->data().value<QModelIndex>();
  if (index.isValid()) {
    m_secondaryColor = index.data(Qt::BackgroundRole).value<QColor>();
    m_secondaryName = QColorConstants::svgOrX11Name(m_secondaryColor);
    m_display->setSecondaryColor(m_secondaryColor, m_secondaryName);
  }
}

void
ExtendedColorDialog::primaryTextColorMenuClicked()
{
  auto act = dynamic_cast<QAction*>(sender());
  auto index = act->data().value<QModelIndex>();
  if (index.isValid()) {
    m_primaryTextColor = index.data(Qt::BackgroundRole).value<QColor>();
    m_primaryTextName = QColorConstants::svgOrX11Name(m_primaryTextColor);
    m_display->setPrimaryTextColor(m_primaryTextColor, m_primaryTextName);
  }
}

void
ExtendedColorDialog::secondaryTextColorMenuClicked()
{
  auto act = dynamic_cast<QAction*>(sender());
  auto index = act->data().value<QModelIndex>();
  if (index.isValid()) {
    m_secondaryTextColor = index.data(Qt::BackgroundRole).value<QColor>();
    m_secondaryTextName = QColorConstants::svgOrX11Name(m_secondaryTextColor);
    m_display->setSecondaryTextColor(m_secondaryTextColor, m_secondaryTextName);
  }
}

ColorDragTable*
ExtendedColorDialog::initSvgBlueGreen()
{
  auto table = createColorTable();

  int row = 0, column = 0;
  table->setData(row++, column, false, "aliceblue");
  table->setData(row++, column, false, "azure");
  table->setData(row++, column, false, "lightcyan");
  table->setData(row++, column, false, "paleturquoise");
  table->setData(row++, column, false, "powderblue");
  table->setData(row++, column, false, "lightblue");
  table->setData(row++, column, false, "lightskyblue");
  table->setData(row++, column, false, "skyblue");
  table->setData(row++, column, false, "cornflowerblue");
  table->setData(row++, column, false, "steelblue");
  table->setData(row++, column, false, "dodgerblue");
  table->setData(row++, column, false, "royalblue");
  table->setData(row++, column, false, "deepskyblue");
  table->setData(row++, column, false, "mediumslateblue");
  table->setData(row++, column, false, "slateblue");
  table->setData(row++, column, false, "indigo");
  table->setData(row++, column, false, "darkslateblue");

  row = 0;
  column++;
  table->setData(row++, column, false, "mediumblue");
  table->setData(row++, column, false, "darkblue");
  table->setData(row++, column, false, "navy");
  table->setData(row++, column, false, "blue");
  table->setData(row++, column, false, "midnightblue");
  table->setData(row++, column, false, "aquamarine");
  table->setData(row++, column, false, "mediumaquamarine");
  table->setData(row++, column, false, "cyan");
  table->setData(row++, column, false, "aqua");
  table->setData(row++, column, false, "turquoise");
  table->setData(row++, column, false, "mediumturquoise");
  table->setData(row++, column, false, "darkturquoise");
  table->setData(row++, column, false, "lightseagreen");
  table->setData(row++, column, false, "cadetblue");
  table->setData(row++, column, false, "darkcyan");
  table->setData(row++, column, false, "teal");
  table->setData(row++, column, false, "darkslategray");

  row = 0;
  column++;
  table->setData(row++, column, false, "honeydew");
  table->setData(row++, column, false, "limegreen");
  table->setData(row++, column, false, "darkseagreen");
  table->setData(row++, column, false, "palegreen");
  table->setData(row++, column, false, "lightgreen");
  table->setData(row++, column, false, "springgreen");
  table->setData(row++, column, false, "lime");
  table->setData(row++, column, false, "forestgreen");
  table->setData(row++, column, false, "green");
  table->setData(row++, column, false, "darkgreen");
  table->setData(row++, column, false, "aquamarine");
  table->setData(row++, column, false, "mediumaquamarine");
  table->setData(row++, column, false, "mediumseagreen");
  table->setData(row++, column, false, "mediumspringgreen");
  table->setData(row++, column, false, "seagreen");
  table->setData(row++, column, false, "greenyellow");
  table->setData(row++, column, false, "lawngreen");

  row = 0;
  column++;
  table->setData(row++, column, false, "chartreuse");
  table->setData(row++, column, false, "yellowgreen");
  table->setData(row++, column, false, "olivedrab");
  table->setData(row++, column, false, "olive");
  table->setData(row++, column, false, "darkolivegreen");

  return table;
}

ColorDragTable*
ExtendedColorDialog::initSvgRedYellowBrown()
{
  auto table = createColorTable();

  int row = 0, column = 0;

  table->setData(row++, column, false, "seashell");
  table->setData(row++, column, false, "lavenderblush");
  table->setData(row++, column, false, "mistyrose");
  table->setData(row++, column, false, "lightsalmon");
  table->setData(row++, column, false, "darksalmon");
  table->setData(row++, column, false, "salmon");
  table->setData(row++, column, false, "tomato");
  table->setData(row++, column, false, "red");
  table->setData(row++, column, false, "pink");
  table->setData(row++, column, false, "crimson");
  table->setData(row++, column, false, "indianred");
  table->setData(row++, column, false, "lavender");
  table->setData(row++, column, false, "hotpink");
  table->setData(row++, column, false, "lightpink");
  table->setData(row++, column, false, "palevioletred");
  table->setData(row++, column, false, "fuchsia");
  table->setData(row++, column, false, "magenta");

  row = 0;
  column++;
  table->setData(row++, column, false, "deeppink");
  table->setData(row++, column, false, "mediumvioletred");
  table->setData(row++, column, false, "thistle");
  table->setData(row++, column, false, "plum");
  table->setData(row++, column, false, "violet");
  table->setData(row++, column, false, "orchid");
  table->setData(row++, column, false, "mediumorchid");
  table->setData(row++, column, false, "darkmagenta");
  table->setData(row++, column, false, "purple");
  table->setData(row++, column, false, "mediumpurple");
  table->setData(row++, column, false, "darkorchid");
  table->setData(row++, column, false, "darkviolet");
  table->setData(row++, column, false, "blueviolet");
  table->setData(row++, column, false, "ivory");
  table->setData(row++, column, false, "beige");
  table->setData(row++, column, false, "lightyellow");
  table->setData(row++, column, false, "linen");

  row = 0;
  column++;
  table->setData(row++, column, false, "palegoldenrod");
  table->setData(row++, column, false, "cornsilk");
  table->setData(row++, column, false, "floralwhite");
  table->setData(row++, column, false, "lemonchiffon");
  table->setData(row++, column, false, "lightgoldenrodyellow");
  table->setData(row++, column, false, "moccasin");
  table->setData(row++, column, false, "wheat");
  table->setData(row++, column, false, "khaki");
  table->setData(row++, column, false, "darkkhaki");
  table->setData(row++, column, false, "gold");
  table->setData(row++, column, false, "goldenrod");
  table->setData(row++, column, false, "darkgoldenrod");
  table->setData(row++, column, false, "yellow");
  table->setData(row++, column, false, "orange");
  table->setData(row++, column, false, "darkorange");
  table->setData(row++, column, false, "orangered");
  table->setData(row++, column, false, "oldlace");

  row = 0;

  column++;
  table->setData(row++, column, false, "antiquewhite");
  table->setData(row++, column, false, "papayawhip");
  table->setData(row++, column, false, "peachpuff");
  table->setData(row++, column, false, "bisque");
  table->setData(row++, column, false, "navajowhite");
  table->setData(row++, column, false, "blanchedalmond");
  table->setData(row++, column, false, "burlywood");
  table->setData(row++, column, false, "tan");
  table->setData(row++, column, false, "sandybrown");
  table->setData(row++, column, false, "peru");
  table->setData(row++, column, false, "sienna");
  table->setData(row++, column, false, "chocolate");
  table->setData(row++, column, false, "saddlebrown");
  table->setData(row++, column, false, "coral");
  table->setData(row++, column, false, "lightcoral");
  table->setData(row++, column, false, "rosybrown");
  table->setData(row++, column, false, "brown");

  row = 0;
  column++;
  table->setData(row++, column, false, "firebrick");
  table->setData(row++, column, false, "darkred");
  table->setData(row++, column, false, "maroon");

  return table;
}

ColorDragTable*
ExtendedColorDialog::initX11BlueGreen()
{
  auto table = createColorTable();

  int row = 0, column = 0;
  table->setData(row++, column, true, "mint cream");
  table->setData(row++, column, true, "alice blue");
  table->setData(row++, column, true, "azure");
  table->setData(row++, column, true, "azure2");
  table->setData(row++, column, true, "azure3");
  table->setData(row++, column, true, "azure4");
  table->setData(row++, column, true, "light cyan");
  table->setData(row++, column, true, "pale turquoise");
  table->setData(row++, column, true, "PaleTurquoise1");
  table->setData(row++, column, true, "PaleTurquoise2");
  table->setData(row++, column, true, "PaleTurquoise3");
  table->setData(row++, column, true, "PaleTurquoise4");
  table->setData(row++, column, true, "powder blue");
  table->setData(row++, column, true, "cornflower blue");
  table->setData(row++, column, true, "deep sky blue");
  table->setData(row++, column, true, "DeepSkyBlue2");
  table->setData(row++, column, true, "DeepSkyBlue3");
  table->setData(row++, column, true, "DeepSkyBlue4");

  row = 0;
  column++;
  table->setData(row++, column, true, "light blue");
  table->setData(row++, column, true, "LightBlue1");
  table->setData(row++, column, true, "LightBlue2");
  table->setData(row++, column, true, "LightBlue3");
  table->setData(row++, column, true, "LightBlue4");
  table->setData(row++, column, true, "light steel blue");
  table->setData(row++, column, true, "LightSteelBlue1");
  table->setData(row++, column, true, "LightSteelBlue2");
  table->setData(row++, column, true, "LightSteelBlue3");
  table->setData(row++, column, true, "LightSteelBlue4");
  table->setData(row++, column, true, "dark slate gray");
  table->setData(row++, column, true, "DarkSlateGray1");
  table->setData(row++, column, true, "DarkSlateGray2");
  table->setData(row++, column, true, "DarkSlateGray3");
  table->setData(row++, column, true, "DarkSlateGray4");
  table->setData(row++, column, true, "light sky blue");
  table->setData(row++, column, true, "LightSkyBlue1");

  row = 0;
  column++;
  table->setData(row++, column, true, "LightSkyBlue2");
  table->setData(row++, column, true, "LightSkyBlue3");
  table->setData(row++, column, true, "LightSkyBlue4");
  table->setData(row++, column, true, "sky blue");
  table->setData(row++, column, true, "SkyBlue2");
  table->setData(row++, column, true, "SkyBlue3");
  table->setData(row++, column, true, "SkyBlue4");
  table->setData(row++, column, true, "SteelBlue1");
  table->setData(row++, column, true, "SteelBlue2");
  table->setData(row++, column, true, "SteelBlue3");
  table->setData(row++, column, true, "steel blue");
  table->setData(row++, column, true, "SteelBlue4");
  table->setData(row++, column, true, "royal blue");
  table->setData(row++, column, true, "RoyalBlue1");
  table->setData(row++, column, true, "RoyalBlue2");
  table->setData(row++, column, true, "RoyalBlue3");
  table->setData(row++, column, true, "RoyalBlue4");

  row = 0;
  column++;
  table->setData(row++, column, true, "blue");
  table->setData(row++, column, true, "medium blue");
  table->setData(row++, column, true, "dark blue");
  table->setData(row++, column, true, "navy");
  table->setData(row++, column, true, "midnight blue");
  table->setData(row++, column, true, "medium slate blue");
  table->setData(row++, column, true, "slate blue");
  table->setData(row++, column, true, "dark slate blue");
  table->setData(row++, column, true, "dodger blue");
  table->setData(row++, column, true, "DodgerBlue2");
  table->setData(row++, column, true, "DodgerBlue3");
  table->setData(row++, column, true, "DodgerBlue4");
  table->setData(row++, column, true, "cyan");
  table->setData(row++, column, true, "cyan2");
  table->setData(row++, column, true, "cyan3");
  table->setData(row++, column, true, "cyan4");
  table->setData(row++, column, true, "turquoise");

  row = 0;
  column++;
  table->setData(row++, column, true, "turquoise1");
  table->setData(row++, column, true, "turquoise2");
  table->setData(row++, column, true, "turquoise3");
  table->setData(row++, column, true, "turquoise4");
  table->setData(row++, column, true, "dark turquoise");
  table->setData(row++, column, true, "light sea green");
  table->setData(row++, column, true, "dark cyan");
  table->setData(row++, column, true, "lime green");
  table->setData(row++, column, true, "dark sea green");
  table->setData(row++, column, true, "pale green");
  table->setData(row++, column, true, "light green");
  table->setData(row++, column, true, "spring green");
  table->setData(row++, column, true, "forest green");
  table->setData(row++, column, true, "green");
  table->setData(row++, column, true, "dark green");
  table->setData(row++, column, true, "medium sea green");
  table->setData(row++, column, true, "sea green");

  row = 0;
  column++;
  table->setData(row++, column, true, "aquamarine");
  table->setData(row++, column, true, "aquamarine2");
  table->setData(row++, column, true, "aquamarine3");
  table->setData(row++, column, true, "aquamarine4");
  table->setData(row++, column, true, "medium aquamarine");
  table->setData(row++, column, true, "medium spring green");
  table->setData(row++, column, true, "green yellow");
  table->setData(row++, column, true, "lawn green");
  table->setData(row++, column, true, "chartreuse");
  table->setData(row++, column, true, "chartreuse2");
  table->setData(row++, column, true, "chartreuse3");
  table->setData(row++, column, true, "chartreuse4");
  table->setData(row++, column, true, "yellow green");
  table->setData(row++, column, true, "olive drab");
  table->setData(row++, column, true, "dark olive green");

  return table;
}

ColorDragTable*
ExtendedColorDialog::initX11Red()
{
  auto table = createColorTable(10);

  int row = 0, column = 0;
  table->setData(row++, column, true, "seashell1");
  table->setData(row++, column, true, "seashell2");
  table->setData(row++, column, true, "seashell3");
  table->setData(row++, column, true, "seashell4");
  table->setData(row++, column, true, "lavender");
  table->setData(row++, column, true, "LavenderBlush1");
  table->setData(row++, column, true, "LavenderBlush2");
  table->setData(row++, column, true, "LavenderBlush3");
  table->setData(row++, column, true, "LavenderBlush4");
  table->setData(row++, column, true, "misty rose");
  table->setData(row++, column, true, "MistyRose2");
  table->setData(row++, column, true, "MistyRose3");
  table->setData(row++, column, true, "MistyRose4");
  table->setData(row++, column, true, "LightSalmon1");
  table->setData(row++, column, true, "LightSalmon2");
  table->setData(row++, column, true, "LightSalmon3");
  table->setData(row++, column, true, "LightSalmon4");

  row = 0;
  column++;
  table->setData(row++, column, true, "dark salmon");
  table->setData(row++, column, true, "salmon1");
  table->setData(row++, column, true, "salmon2");
  table->setData(row++, column, true, "salmon3");
  table->setData(row++, column, true, "salmon4");
  table->setData(row++, column, true, "tomato1");
  table->setData(row++, column, true, "tomato2");
  table->setData(row++, column, true, "tomato3");
  table->setData(row++, column, true, "tomato4");
  table->setData(row++, column, true, "red1");
  table->setData(row++, column, true, "red2");
  table->setData(row++, column, true, "red3");
  table->setData(row++, column, true, "red4");
  table->setData(row++, column, true, "pink1");
  table->setData(row++, column, true, "pink2");
  table->setData(row++, column, true, "pink3");
  table->setData(row++, column, true, "pink4");

  row = 0;
  column++;
  table->setData(row++, column, true, "indian red");
  table->setData(row++, column, true, "IndianRed1");
  table->setData(row++, column, true, "IndianRed2");
  table->setData(row++, column, true, "IndianRed3");
  table->setData(row++, column, true, "IndianRed4");
  table->setData(row++, column, true, "coral");
  table->setData(row++, column, true, "coral2");
  table->setData(row++, column, true, "coral3");
  table->setData(row++, column, true, "coral4");
  table->setData(row++, column, true, "light coral");
  table->setData(row++, column, true, "rosy brown");
  table->setData(row++, column, true, "RosyBrown1");
  table->setData(row++, column, true, "RosyBrown2");
  table->setData(row++, column, true, "RosyBrown3");
  table->setData(row++, column, true, "RosyBrown4");
  table->setData(row++, column, true, "brown");
  table->setData(row++, column, true, "brown2");

  row = 0;
  column++;
  table->setData(row++, column, true, "brown3");
  table->setData(row++, column, true, "brown4");
  table->setData(row++, column, true, "firebrick");
  table->setData(row++, column, true, "firebrick2");
  table->setData(row++, column, true, "firebrick3");
  table->setData(row++, column, true, "firebrick4");
  table->setData(row++, column, true, "dark red");
  table->setData(row++, column, true, "maroon");
  table->setData(row++, column, true, "maroon2");
  table->setData(row++, column, true, "maroon3");
  table->setData(row++, column, true, "maroon4");
  table->setData(row++, column, true, "hot pink");
  table->setData(row++, column, true, "HotPink1");
  table->setData(row++, column, true, "HotPink2");
  table->setData(row++, column, true, "HotPink3");
  table->setData(row++, column, true, "HotPink4");
  table->setData(row++, column, true, "deep pink");

  row = 0;
  column++;
  table->setData(row++, column, true, "DeepPink2");
  table->setData(row++, column, true, "DeepPink3");
  table->setData(row++, column, true, "DeepPink4");
  table->setData(row++, column, true, "light pink");
  table->setData(row++, column, true, "LightPink1");
  table->setData(row++, column, true, "LightPink2");
  table->setData(row++, column, true, "LightPink3");
  table->setData(row++, column, true, "LightPink4");
  table->setData(row++, column, true, "pale violet red");
  table->setData(row++, column, true, "PaleVioletRed1");
  table->setData(row++, column, true, "PaleVioletRed2");
  table->setData(row++, column, true, "PaleVioletRed3");
  table->setData(row++, column, true, "PaleVioletRed4");
  table->setData(row++, column, true, "magenta");
  table->setData(row++, column, true, "magenta2");
  table->setData(row++, column, true, "magenta3");
  table->setData(row++, column, true, "magenta4");

  row = 0;
  column++;
  table->setData(row++, column, true, "medium violet red");
  table->setData(row++, column, true, "thistle");
  table->setData(row++, column, true, "thistle2");
  table->setData(row++, column, true, "thistle3");
  table->setData(row++, column, true, "thistle4");
  table->setData(row++, column, true, "plum");
  table->setData(row++, column, true, "plum2");
  table->setData(row++, column, true, "plum3");
  table->setData(row++, column, true, "plum4");
  table->setData(row++, column, true, "violet");
  table->setData(row++, column, true, "orchid");
  table->setData(row++, column, true, "orchid2");
  table->setData(row++, column, true, "orchid3");
  table->setData(row++, column, true, "orchid4");
  table->setData(row++, column, true, "medium orchid");
  table->setData(row++, column, true, "MediumOrchid1");
  table->setData(row++, column, true, "MediumOrchid2");

  row = 0;
  column++;
  table->setData(row++, column, true, "MediumOrchid3");
  table->setData(row++, column, true, "MediumOrchid4");
  table->setData(row++, column, true, "dark magenta");
  table->setData(row++, column, true, "purple");
  table->setData(row++, column, true, "purple2");
  table->setData(row++, column, true, "purple3");
  table->setData(row++, column, true, "purple4");
  table->setData(row++, column, true, "medium purple");
  table->setData(row++, column, true, "MediumPurple1");
  table->setData(row++, column, true, "MediumPurple2");
  table->setData(row++, column, true, "MediumPurple3");
  table->setData(row++, column, true, "MediumPurple4");
  table->setData(row++, column, true, "dark orchid");
  table->setData(row++, column, true, "DarkOrchid1");
  table->setData(row++, column, true, "DarkOrchid2");
  table->setData(row++, column, true, "DarkOrchid3");
  table->setData(row++, column, true, "DarkOrchid4");

  row = 0;
  column++;
  table->setData(row++, column, true, "dark violet");

  return table;
}

ColorDragTable*
ExtendedColorDialog::initX11YellowBrown()
{
  auto table = createColorTable();

  int row = 0, column = 0;

  table->setData(row++, column, true, "ivory");
  table->setData(row++, column, true, "ivory2");
  table->setData(row++, column, true, "ivory3");
  table->setData(row++, column, true, "ivory4");
  table->setData(row++, column, true, "beige");
  table->setData(row++, column, true, "light yellow");
  table->setData(row++, column, true, "LightYellow2");
  table->setData(row++, column, true, "LightYellow3");
  table->setData(row++, column, true, "LightYellow4");
  table->setData(row++, column, true, "pale goldenrod");
  table->setData(row++, column, true, "cornsilk");
  table->setData(row++, column, true, "cornsilk2");
  table->setData(row++, column, true, "cornsilk3");
  table->setData(row++, column, true, "cornsilk4");
  table->setData(row++, column, true, "lemon chiffon");
  table->setData(row++, column, true, "LemonChiffon2");
  table->setData(row++, column, true, "LemonChiffon3");

  row = 0;
  column++;
  table->setData(row++, column, true, "LemonChiffon4");
  table->setData(row++, column, true, "light goldenrod yellow");
  table->setData(row++, column, true, "moccasin");
  table->setData(row++, column, true, "wheat");
  table->setData(row++, column, true, "khaki");
  table->setData(row++, column, true, "khaki2");
  table->setData(row++, column, true, "khaki3");
  table->setData(row++, column, true, "khaki4");
  table->setData(row++, column, true, "dark khaki");
  table->setData(row++, column, true, "gold");
  table->setData(row++, column, true, "gold2");
  table->setData(row++, column, true, "gold3");
  table->setData(row++, column, true, "gold4");
  table->setData(row++, column, true, "goldenrod");
  table->setData(row++, column, true, "goldenrod");
  table->setData(row++, column, true, "goldenrod2");
  table->setData(row++, column, true, "goldenrod3");

  row = 0;
  column++;
  table->setData(row++, column, true, "goldenrod4");
  table->setData(row++, column, true, "dark goldenrod");
  table->setData(row++, column, true, "yellow");
  table->setData(row++, column, true, "yellow2");
  table->setData(row++, column, true, "yellow3");
  table->setData(row++, column, true, "yellow4");
  table->setData(row++, column, true, "orange");
  table->setData(row++, column, true, "orange2");
  table->setData(row++, column, true, "orange3");
  table->setData(row++, column, true, "orange4");
  table->setData(row++, column, true, "dark orange");
  table->setData(row++, column, true, "DarkOrange1");
  table->setData(row++, column, true, "DarkOrange2");
  table->setData(row++, column, true, "DarkOrange3");
  table->setData(row++, column, true, "DarkOrange4");
  table->setData(row++, column, true, "orange red");
  table->setData(row++, column, true, "OrangeRed2");

  row = 0;
  column++;
  table->setData(row++, column, true, "OrangeRed3");
  table->setData(row++, column, true, "OrangeRed4");
  table->setData(row++, column, true, "floral white");
  table->setData(row++, column, true, "antique white");
  table->setData(row++, column, true, "AntiqueWhite1");
  table->setData(row++, column, true, "AntiqueWhite2");
  table->setData(row++, column, true, "AntiqueWhite3");
  table->setData(row++, column, true, "AntiqueWhite4");
  table->setData(row++, column, true, "peach puff");
  table->setData(row++, column, true, "PeachPuff2");
  table->setData(row++, column, true, "PeachPuff3");
  table->setData(row++, column, true, "PeachPuff4");
  table->setData(row++, column, true, "bisque");
  table->setData(row++, column, true, "bisque2");
  table->setData(row++, column, true, "bisque3");
  table->setData(row++, column, true, "bisque4");
  table->setData(row++, column, true, "navajo white");

  row = 0;
  column++;
  table->setData(row++, column, true, "NavajoWhite2");
  table->setData(row++, column, true, "NavajoWhite3");
  table->setData(row++, column, true, "NavajoWhite4");
  table->setData(row++, column, true, "burlywood");
  table->setData(row++, column, true, "burlywood2");
  table->setData(row++, column, true, "burlywood3");
  table->setData(row++, column, true, "burlywood4");
  table->setData(row++, column, true, "tan2");
  table->setData(row++, column, true, "tan3");
  table->setData(row++, column, true, "tan4");
  table->setData(row++, column, true, "sandy brown");
  table->setData(row++, column, true, "sienna");
  table->setData(row++, column, true, "sienna2");
  table->setData(row++, column, true, "sienna3");
  table->setData(row++, column, true, "sienna4");

  row = 0;
  column++;
  table->setData(row++, column, true, "chocolate");
  table->setData(row++, column, true, "chocolate2");
  table->setData(row++, column, true, "chocolate3");
  table->setData(row++, column, true, "chocolate4");
  table->setData(row++, column, true, "saddle brown");

  return table;
}

ColorDragTable*
ExtendedColorDialog::initX11MonoFrame()
{
  auto table = createColorTable();

  auto row = 0;
  auto column = 0;
  table->setData(row++, column, true, "grey0");
  table->setData(row++, column, true, "grey1");
  table->setData(row++, column, true, "grey2");
  table->setData(row++, column, true, "grey3");
  table->setData(row++, column, true, "grey4");
  table->setData(row++, column, true, "grey5");
  table->setData(row++, column, true, "grey6");
  table->setData(row++, column, true, "grey7");
  table->setData(row++, column, true, "grey8");
  table->setData(row++, column, true, "grey9");
  table->setData(row++, column, true, "grey10");
  table->setData(row++, column, true, "grey11");
  table->setData(row++, column, true, "grey12");
  table->setData(row++, column, true, "grey13");
  table->setData(row++, column, true, "grey14");
  table->setData(row++, column, true, "grey15");
  table->setData(row++, column, true, "grey16");

  row = 0;
  column++;
  table->setData(row++, column, true, "grey17");
  table->setData(row++, column, true, "grey18");
  table->setData(row++, column, true, "grey19");
  table->setData(row++, column, true, "grey20");
  table->setData(row++, column, true, "grey21");
  table->setData(row++, column, true, "grey22");
  table->setData(row++, column, true, "grey23");
  table->setData(row++, column, true, "grey24");
  table->setData(row++, column, true, "grey25");
  table->setData(row++, column, true, "grey26");
  table->setData(row++, column, true, "grey27");
  table->setData(row++, column, true, "grey28");
  table->setData(row++, column, true, "grey29");
  table->setData(row++, column, true, "grey30");
  table->setData(row++, column, true, "grey31");
  table->setData(row++, column, true, "grey32");
  table->setData(row++, column, true, "grey33");

  row = 0;
  column++;
  table->setData(row++, column, true, "grey34");
  table->setData(row++, column, true, "grey35");
  table->setData(row++, column, true, "grey36");
  table->setData(row++, column, true, "grey37");
  table->setData(row++, column, true, "grey38");
  table->setData(row++, column, true, "grey39");
  table->setData(row++, column, true, "grey40");
  table->setData(row++, column, true, "grey41");
  table->setData(row++, column, true, "grey42");
  table->setData(row++, column, true, "grey43");
  table->setData(row++, column, true, "grey44");
  table->setData(row++, column, true, "grey45");
  table->setData(row++, column, true, "grey46");
  table->setData(row++, column, true, "grey47");
  table->setData(row++, column, true, "grey48");
  table->setData(row++, column, true, "grey49");
  table->setData(row++, column, true, "grey50");

  row = 0;
  column++;
  table->setData(row++, column, true, "grey51");
  table->setData(row++, column, true, "grey52");
  table->setData(row++, column, true, "grey53");
  table->setData(row++, column, true, "grey54");
  table->setData(row++, column, true, "grey55");
  table->setData(row++, column, true, "grey56");
  table->setData(row++, column, true, "grey57");
  table->setData(row++, column, true, "grey58");
  table->setData(row++, column, true, "grey59");
  table->setData(row++, column, true, "grey60");
  table->setData(row++, column, true, "grey61");
  table->setData(row++, column, true, "grey62");
  table->setData(row++, column, true, "grey63");
  table->setData(row++, column, true, "grey64");
  table->setData(row++, column, true, "grey65");
  table->setData(row++, column, true, "grey66");
  table->setData(row++, column, true, "grey67");

  row = 0;
  column++;
  table->setData(row++, column, true, "grey68");
  table->setData(row++, column, true, "grey69");
  table->setData(row++, column, true, "grey70");
  table->setData(row++, column, true, "grey71");
  table->setData(row++, column, true, "grey72");
  table->setData(row++, column, true, "grey73");
  table->setData(row++, column, true, "grey74");
  table->setData(row++, column, true, "grey75");
  table->setData(row++, column, true, "grey76");
  table->setData(row++, column, true, "grey77");
  table->setData(row++, column, true, "grey78");
  table->setData(row++, column, true, "grey79");
  table->setData(row++, column, true, "grey80");
  table->setData(row++, column, true, "grey81");
  table->setData(row++, column, true, "grey82");
  table->setData(row++, column, true, "grey83");
  table->setData(row++, column, true, "grey84");

  row = 0;
  column++;
  table->setData(row++, column, true, "grey85");
  table->setData(row++, column, true, "grey86");
  table->setData(row++, column, true, "grey87");
  table->setData(row++, column, true, "grey88");
  table->setData(row++, column, true, "grey89");
  table->setData(row++, column, true, "grey90");
  table->setData(row++, column, true, "grey91");
  table->setData(row++, column, true, "grey92");
  table->setData(row++, column, true, "grey93");
  table->setData(row++, column, true, "grey94");
  table->setData(row++, column, true, "grey95");
  table->setData(row++, column, true, "grey96");
  table->setData(row++, column, true, "grey97");
  table->setData(row++, column, true, "grey98");
  table->setData(row++, column, true, "grey99");
  table->setData(row++, column, true, "grey100");

  row = 0;
  column++;
  table->setData(row++, column, true, "white");
  table->setData(row++, column, true, "snow");
  table->setData(row++, column, true, "ghost white");
  table->setData(row++, column, true, "white smoke");
  table->setData(row++, column, true, "snow2");
  table->setData(row++, column, true, "snow3");
  table->setData(row++, column, true, "snow4");
  table->setData(row++, column, true, "gainsboro");
  table->setData(row++, column, true, "light gray");
  table->setData(row++, column, true, "dark gray");
  table->setData(row++, column, true, "light slate gray");
  table->setData(row++, column, true, "slate gray");
  table->setData(row++, column, true, "gray");
  table->setData(row++, column, true, "dim gray");
  table->setData(row++, column, true, "black");

  return table;
}

QColor
ExtendedColorDialog::color(ColorType type) const
{
  if (type == Secondary)
    return m_secondaryColor;
  return m_primaryColor;
}

QColor
ExtendedColorDialog::textColor(ColorType type) const
{
  if (type == Secondary)
    return m_secondaryTextColor;
  return m_primaryTextColor;
}

void
ExtendedColorDialog::setColor(ColorType type, const QColor& color)
{
  auto name = QColorConstants::svgOrX11Name(color);
  if (type == Secondary) {
    setSecondaryColor(color, name);
  } else {
    setPrimaryColor(color, name);
  }
}

void
ExtendedColorDialog::setColor(ColorType type, const QString& name)
{
  auto color = QColorConstants::svgOrX11Color(name);
  if (type == Secondary) {
    setSecondaryColor(color, name);
  } else {
    setPrimaryColor(color, name);
  }
}

void
ExtendedColorDialog::setPrimaryColor(QColor color, QString name)
{
  m_primaryColor = color;
  m_primaryName = name;
  m_colorDlg->setCurrentColor(color);
  m_display->setPrimaryColor(color, name);
}

void
ExtendedColorDialog::setPrimaryTextColor(QColor color, QString name)
{
  m_primaryTextColor = color;
  m_primaryTextName = name;
  m_display->setPrimaryTextColor(color, name);
}

void
ExtendedColorDialog::setSecondaryColor(QColor color, QString name)
{
  m_secondaryColor = color;
  m_secondaryName = name;
  m_display->setSecondaryColor(color, name);
}

void
ExtendedColorDialog::setSecondaryTextColor(QColor color, QString name)
{
  m_secondaryTextColor = color;
  m_secondaryTextName = name;
  m_display->setSecondaryTextColor(color, name);
}

void
ExtendedColorDialog::setTextColor(ColorType type, const QColor& color)
{
  auto name = QColorConstants::svgOrX11Name(color);
  if (type == Secondary) {
    setSecondaryTextColor(color, name);
  } else {
    setPrimaryTextColor(color, name);
  }
}

void
ExtendedColorDialog::setTextColor(ColorType type, const QString& name)
{
  auto color = QColorConstants::svgOrX11Color(name);
  if (type == Secondary) {
    setSecondaryTextColor(color, name);
  } else {
    setPrimaryTextColor(color, name);
  }
}

QString
ExtendedColorDialog::rgb(ColorType type, int alpha) const
{
  QString name;
  QColor color = (type == Primary ? m_primaryColor : m_secondaryColor);
  int a = (alpha == -1 ? 100 : alpha);
  bool showAlpha = (alpha == -1 ? false : true);
  if (showAlpha) {
    name = ExtendedColorDialog::RGBACOLOR.arg(color.red())
             .arg(color.green())
             .arg(color.blue())
             .arg(a);
  } else {
    name = ExtendedColorDialog::RGBCOLOR.arg(color.red())
             .arg(color.green())
             .arg(color.blue());
  }
  if (name.isEmpty() && color.isValid()) {
    if (showAlpha) {
      return hash(type, alpha);
    } else {
      return hash(type);
    }
  }
  return name;
}

QString
ExtendedColorDialog::hsv(ColorType type, int alpha) const
{
  QString name;
  QColor color = (type == Primary ? m_primaryColor : m_secondaryColor);
  int a = (alpha == -1 ? 100 : alpha);
  bool showAlpha = (alpha == -1 ? false : true);
  if (showAlpha) {
    name = ExtendedColorDialog::HSVACOLOR.arg(color.hsvHue())
             .arg(color.hsvSaturation())
             .arg(color.value())
             .arg(a);
  } else {
    name = ExtendedColorDialog::HSVCOLOR.arg(color.hsvHue())
             .arg(color.hsvSaturation())
             .arg(color.value());
  }
  if (name.isEmpty() && color.isValid()) {
    if (showAlpha) {
      return hash(type, alpha);
    } else {
      return hash(type);
    }
  }
  return name;
}

QString
ExtendedColorDialog::hsl(ColorType type, int alpha) const
{
  QString name;
  QColor color = (type == Primary ? m_primaryColor : m_secondaryColor);
  int a = (alpha == -1 ? 100 : alpha);
  bool showAlpha = (alpha == -1 ? false : true);
  if (showAlpha) {
    name = ExtendedColorDialog::HSVACOLOR.arg(color.hslHue())
             .arg(color.hslSaturation())
             .arg(color.value())
             .arg(a);
  } else {
    name = ExtendedColorDialog::HSVCOLOR.arg(color.hslHue())
             .arg(color.hslSaturation())
             .arg(color.value());
  }
  if (name.isEmpty() && color.isValid()) {
    if (showAlpha) {
      return hash(type, alpha);
    } else {
      return hash(type);
    }
  }
  return name;
}

QString
ExtendedColorDialog::name(ColorType type) const
{
  QString name = (type == Primary ? m_primaryName : m_secondaryName);
  QColor color = (type == Primary ? m_primaryColor : m_secondaryColor);
  if (color.isValid()) {
    if (name.isEmpty()) {
      QString name = QColorConstants::X11::name(color);
      if (name.isEmpty()) {
        return hash(Primary);
      }
    }
    return name;
  }
  return QString();
}

QString
ExtendedColorDialog::hash(ColorType type, int alpha) const
{
  QColor color = (type == Primary ? m_primaryColor : m_secondaryColor);
  bool showAlpha = (alpha == -1 ? false : true);
  if (color.isValid()) {
    if (showAlpha)
      return color.name(QColor::HexArgb);
    return color.name(QColor::HexRgb);
  }
  return QString();
}

// QString
// ExtendedColorDialog::svgOrX11Name(const QColor& color)
//{
//  auto name = QColorConstants::Svg::name(color);
//  if (!name.isEmpty()) {
//    return name;
//  } else {
//    name = QColorConstants::X11::name(color);
//    if (!name.isEmpty()) {
//      return name;
//    }
//  }
//  return QString();
//}

// QColor
// ExtendedColorDialog::svgOrX11Color(const QString& initialColor)
//{
//  auto color = QColorConstants::Svg::color(initialColor);
//  if (color.isValid()) {
//    return color;
//  } else {
//    color = QColorConstants::X11::color(initialColor);
//    if (color.isValid()) {
//      return color;
//    }
//  }
//  return Qt::white;
//}

// bool
// ExtendedColorDialog::isDark(const QColor& color)
//{
//  auto r = color.redF();
//  auto g = color.greenF();
//  auto b = color.blueF();

//  // Calculate the Perceived brightness 0 - 1.0.
//  // the darker it is the lower the value
//  auto p = qSqrt(r * r * Pr + g * g * Pg + b * b * Pb);

//  if (p < 0.5) {
//    return true;
//  }
//  return false;
//}

QSize
ExtendedColorDialog::sizeHint() const
{
  return QSize(900, 1200);
}

void
ExtendedColorDialog::acceptChanges()
{
  if (m_primaryColor.isValid()) {
    accept();
  } else {
    reject();
  }
}

ColorDropDisplay::ColorDropDisplay(const QColor& color,
                                   const QColor& dropColor,
                                   QWidget* parent)
  : QFrame(parent)
  , m_primaryColor(color)
  , m_secondaryColor(dropColor)
  , m_colorSet(false)
  , m_dropColorSet(false)
{
  if (color != Qt::white)
    m_colorSet = true;
  if (dropColor != Qt::white)
    m_dropColorSet = true;
  setStyleSheet(DISPLAYBORDER);
  setAcceptDrops(true);

  auto layout = new QHBoxLayout;
  layout->setSpacing(0);
  setLayout(layout);
  m_left = new QLabel(this);
  m_left->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_left->setStyleSheet(primaryColorToStyle());
  layout->addWidget(m_left);
  m_right = new QLabel(this);
  m_right->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_right->setStyleSheet(secondaryColorToStyle());
  layout->addWidget(m_right);
}

QString
ColorDropDisplay::calculateTextString(ColorType type)
{
  QColor color = (type == Primary ? m_primaryColor : m_secondaryColor);
  QColor tColor = (type == Primary ? m_primaryTextColor : m_secondaryTextColor);
  QString name = (type == Primary ? m_primaryName : m_secondaryName);
  QString tName = (type == Primary ? m_primaryTextName : m_secondaryTextName);
  QString displayText, cStr, bStr;
  if (color.isValid()) {
    if (tColor.isValid()) {
      if (tName.isEmpty())
        cStr = "color: " + tColor.name(QColor::HexRgb);
      else
        cStr = "color: " + tColor.name(QColor::HexRgb) + "(" + tName + ")";
    }
    if (name.isEmpty())
      bStr = "background: " + color.name(QColor::HexRgb);
    else
      bStr = "background: " + color.name(QColor::HexRgb) + "(" + name + ")";
    if (!cStr.isEmpty() && !bStr.isEmpty())
      displayText = cStr + "<br>" + bStr;
    else if (!cStr.isEmpty())
      displayText = cStr;
    else if (!bStr.isEmpty())
      displayText = bStr;
  }
  return displayText;
}

void
ColorDropDisplay::setPrimaryColor(const QColor& color, const QString& name)
{
  m_primaryColor = color;
  m_primaryName = name;
  m_left->setText(calculateTextString(Primary));
  m_left->setStyleSheet(primaryColorToStyle());
  m_colorSet = true;
  emit primaryColorChanged(color, name);
}

void
ColorDropDisplay::setPrimaryTextColor(const QColor& color, const QString& name)
{
  m_primaryTextColor = color;
  m_primaryTextName = name;
  m_left->setText(calculateTextString(Primary));
  m_left->setStyleSheet(primaryColorToStyle());
  m_colorSet = true;
  emit primaryTextColorChanged(color, name);
}

void
ColorDropDisplay::setSecondaryColor(const QColor& color, const QString& name)
{
  m_secondaryColor = color;
  m_secondaryName = name;
  m_right->setText(calculateTextString(Secondary));
  m_right->setStyleSheet(secondaryColorToStyle());
  m_dropColorSet = true;
  emit secondaryColorChanged(color, name);
}

void
ColorDropDisplay::setSecondaryTextColor(const QColor& color,
                                        const QString& name)
{
  m_secondaryTextColor = color;
  m_secondaryTextName = name;
  m_right->setText(calculateTextString(Secondary));
  m_right->setStyleSheet(secondaryColorToStyle());
  m_dropColorSet = true;
  emit secondaryTextColorChanged(color, name);
}

void
ColorDropDisplay::dragEnterEvent(QDragEnterEvent* event)
{
  auto md = event->mimeData();
  if (md->hasFormat("colordata") || md->hasColor())
    event->acceptProposedAction();
  else
    event->ignore();
}

void
ColorDropDisplay::dropEvent(QDropEvent* event)
{
  auto pos = event->position().toPoint();//position().toPoint();
  auto md = event->mimeData();
  if (md->hasFormat("colordata")) {
    auto mimeData = event->mimeData();
    if (mimeData) {
      ColorDragData data;
      auto array = mimeData->data("colordata");
      QDataStream ds(&array, QIODevice::ReadWrite);
      ds.setVersion(QDataStream::Qt_5_15);
      ds >> data;
      auto name = QString(data.name);
      auto color = QColor(data.r, data.g, data.b);
      if (m_left->geometry().contains(pos))
        setPrimaryColor(color, name);
      else if (m_right->geometry().contains(pos))
        setSecondaryColor(color, name);
    }
  } else if (md->hasColor()) {
    auto vColor = md->colorData();
    if (vColor.isValid()) {
      auto color = vColor.value<QColor>();
      auto name = QColorConstants::svgOrX11Name(color);
      if (m_left->geometry().contains(pos))
        setPrimaryColor(color, QString());
      else if (m_right->geometry().contains(pos))
        setSecondaryColor(color, name);
    }
  }
}

QString
ColorDropDisplay::primaryColorToStyle(/*const QColor& backColor,
                                      const QColor& foreColor*/)
{
  if (m_primaryColor.isValid()) {
    QString textColor;
    if (m_primaryTextColor.isValid()) {
      textColor = m_primaryTextColor.name(QColor::HexRgb);
    } else {
      if (QColorConstants::isDark(m_primaryColor))
        textColor = "white";
      else
        textColor = "black";
    }
    auto val =
      DISPLAYLABELLEFT.arg(textColor, m_primaryColor.name(QColor::HexRgb));
    return val;
  }
  auto val = DISPLAYLABELLEFT.arg("white", "white");
  return val;
}

QString
ColorDropDisplay::secondaryColorToStyle(/*const QColor& backColor,
                                        const QColor& foreColor*/)
{
  if (m_secondaryColor.isValid()) {
    QString textColor;
    if (m_secondaryTextColor.isValid()) {
      textColor = m_secondaryTextColor.name(QColor::HexRgb);
    } else {
      if (QColorConstants::isDark(m_secondaryColor))
        textColor = "white";
      else
        textColor = "black";
    }
    auto val =
      DISPLAYLABELRIGHT.arg(textColor, m_secondaryColor.name(QColor::HexRgb));
    return val;
  }
  auto val = DISPLAYLABELRIGHT.arg("white", "white");
  return val;
}

QString
ColorDropDisplay::secondaryName() const
{
  return m_secondaryName;
}

QString
ColorDropDisplay::primaryName() const
{
  return m_primaryName;
}

QColor
ColorDropDisplay::secondaryColor() const
{
  return m_secondaryColor;
}

QColor
ColorDropDisplay::primaryColor() const
{
  return m_primaryColor;
}

ColorDragTable::ColorDragTable(int rows, int columns, QWidget* parent)
  : QTableView(parent)
{
  m_model = new ColorDragModel(rows, columns);
  setModel(m_model);
  setStyleSheet("QTableView { gridline-color: white; }");
}

QString
ColorDragTable::name(const QModelIndex& index)
{
  auto data = m_model->data(index, Qt::DisplayRole);
  auto name = data.toString();
  return name;
}

QColor
ColorDragTable::foreground(const QModelIndex& index)
{
  auto data = m_model->data(index, Qt::ForegroundRole);
  return data.value<QColor>();
}

QColor
ColorDragTable::background(const QModelIndex& index)
{
  auto data = m_model->data(index, Qt::BackgroundRole);
  return data.value<QColor>();
}

void
ColorDragTable::setData(int row, int column, bool x11, const QString& back)
{
  QColor background, foreground;
  if (x11) {
    background = QColorConstants::X11::color(back);
  } else {
    background = QColorConstants::Svg::color(back);
  }
  if (QColorConstants::isDark(background))
    foreground = Qt::white;
  else
    foreground = Qt::black;
  m_model->setColorData(row, column, back, background, foreground);
  auto index = m_model->index(row, column);
  if (index.isValid())
    dataChanged(index, index);
}

void
ColorDragTable::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    auto pos = event->pos();
    auto index = indexAt(pos);
    if (index.isValid()) {
      m_dragStartPosition = pos;
      m_color = background(index);
      m_name = name(index);
      auto w = columnWidth(index.column());
      auto h = rowHeight(index.row());
      m_size = QSize(w, h);
    } else {
      m_color = QColor();
    }
  }
  QTableView::mousePressEvent(event);
}

void
ColorDragTable::mouseMoveEvent(QMouseEvent* event)
{
  auto mousePos = event->pos();
  if (!(event->buttons() & Qt::LeftButton))
    return;
  if ((mousePos - m_dragStartPosition).manhattanLength() <
      QApplication::startDragDistance())
    return;

  if (m_color.isValid()) {
    auto drag = new QDrag(this);
    m_pixmap = QPixmap(m_size);
    m_pixmap.fill(m_color);
    drag->setPixmap(m_pixmap);

    ColorDragData data;
    data.r = m_color.red();
    data.g = m_color.green();
    data.b = m_color.blue();
    if (!m_name.isEmpty())
      data.name = m_name;
    else
      data.name = QColorConstants::svgOrX11Name(m_color);
    QByteArray array;
    QDataStream ds(&array, QIODevice::ReadWrite);
    ds.setVersion(QDataStream::Qt_5_15);
    ds << data;

    m_pixmap = QPixmap();
    auto mimeData = new QMimeData();
    mimeData->setData("colordata", array);
    drag->setMimeData(mimeData);
    drag->setHotSpot(
      QPoint(drag->pixmap().width() / 2, drag->pixmap().height()));

    drag->exec(Qt::CopyAction);
  }
  // dont want the table to do any selection drag.
  // QTableView::mouseMoveEvent(event);
}

void
ColorDragTable::dragEnterEvent(QDragEnterEvent* event)
{
  event->ignore();
}

void
ColorDragTable::dragMoveEvent(QDragMoveEvent* /*event*/)
{
  // does nothing but it blocks the default qtableview
  // drag operations which we don't want.
}

ColorDragModel::ColorDragModel(int rows, int columns)
{
  m_data = new Column[rows];
  for (int row = 0; row < rows; ++row) {
    m_data[row] = new Row[rows];
    for (int column = 0; column < columns; ++column) {
      m_data[row][column] = nullptr;
    }
  }

  m_rows = rows;
  m_columns = columns;
}

ColorDragModel::~ColorDragModel()
{
  for (int row = 0; row < m_rows; ++row) {
    auto rowData = m_data[row];
    for (int column = 0; column < m_columns; ++column) {
      auto data = rowData[column];
      if (data)
        delete data;
    }
    delete[] rowData;
  }
  delete[] m_data;
}

Qt::ItemFlags
ColorDragModel::flags(const QModelIndex& index) const
{
  auto name = data(index, Qt::DisplayRole).toString();
  if (name.isEmpty())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable;
}

int
ColorDragModel::rowCount(const QModelIndex& /*parent*/) const
{
  return m_rows;
}

int
ColorDragModel::columnCount(const QModelIndex& /*parent*/) const
{
  return m_columns;
}

QVariant
ColorDragModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  auto row = index.row();
  auto column = index.column();
  auto data = m_data[row][column];
  if (data) {
    if (role == Qt::DisplayRole) {
      return data->name;
    } else if (role == Qt::ForegroundRole) {
      return m_data[row][column]->fore;
    } else if (role == Qt::BackgroundRole) {
      return m_data[row][column]->back;
    }
  }
  return QVariant();
}

QVariant
ColorDragModel::headerData(int /*section*/,
                           Qt::Orientation /*orientation*/,
                           int /*role*/) const
{
  return QVariant();
}

void
ColorDragModel::setColorData(int row,
                             int column,
                             const QString& name,
                             const QColor& back,
                             const QColor& fore)
{
  if (row >= rowCount() || column >= columnCount()) {
    return;
  }
  auto data = m_data[row][column];
  if (data) {
    data->name = name;
    data->back = back;
    data->fore = fore;
  } else {
    data = new Data(name, fore, back);
    m_data[row][column] = data;
  }
}

QModelIndex
ColorDragModel::index(int row, int column, const QModelIndex& /*parent*/) const
{
  if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount()) {
    if (m_data[row][column]) {
      auto newIndex = createIndex(row, column, m_data[row][column]);
      return newIndex;
    }
  }
  return QModelIndex();
}

QModelIndex
ColorDragModel::parent(const QModelIndex& /*child*/) const
{
  return QModelIndex();
}

QDataStream&
operator<<(QDataStream& out, const ColorDragData& data)
{
  out << data.r << data.g << data.b << data.name;
  return out;
}

QDataStream&
operator>>(QDataStream& in, ColorDragData& data)
{
  in >> data.r >> data.g >> data.b >> data.name;
  return in;
}
