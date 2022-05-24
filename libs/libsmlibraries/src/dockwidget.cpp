#include "dockwidget.h"

//====================================================================
//=== DockWidget
//====================================================================
DockWidget::DockWidget(QWidget* parent)
  : QWidget{ parent }
{
  setContentsMargins(0, 0, 0, 0);
  setAutoFillBackground(true);

  initGui();
}

QWidget*
DockWidget::centreWidget() const
{
  return m_centreWidget;
}

void
DockWidget::setCentreWidget(QWidget* newCentreWidget)
{
  m_centreWidget = newCentreWidget;
  m_layout->addWidget(m_centreWidget, DockLayout::Center);
}

void
DockWidget::initGui()
{
  m_layout = new DockLayout;
  m_layout->setSpacing(0);
  m_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(m_layout);
}

DockToolbar*
DockWidget::addNewToolbar(DockLayout::Position position)
{
  auto toolbar = m_toolbars.value(position, nullptr);
  if (toolbar)
    return toolbar;

  toolbar = new DockToolbar(position, this);
  m_toolbars.insert(position, toolbar);

  switch (position) {
    case DockLayout::West: {
      m_layout->addWidget(toolbar, DockLayout::West);
      break;
    }
    case DockLayout::East: {
      m_layout->addWidget(toolbar, DockLayout::East);
      break;
    }
    case DockLayout::North: {
      m_layout->addWidget(toolbar, DockLayout::North);
      break;
    }
    case DockLayout::South: {
      m_layout->addWidget(toolbar, DockLayout::South);
      break;
    }
  }
  repaint();
  return toolbar;
}

void
DockWidget::addToolbar(DockLayout::Position position)
{
  addNewToolbar(position);
}

void
DockWidget::removeToolbar(DockLayout::Position position)
{
  auto toolbar = m_toolbars.value(position, nullptr);
  if (!toolbar)
    return;

  m_layout->removeWidget(toolbar);
  m_toolbars.remove(position);
  toolbar->deleteLater();
  repaint();
}

void
DockWidget::setToolbarPosition(DockLayout::Position oldPosition,
                               DockLayout::Position newPosition)
{
  auto oldbar = m_toolbars.value(oldPosition, nullptr);
  if (!oldbar)
    return;

  auto newbar = m_toolbars.value(newPosition, nullptr);
  if (newbar)
    return;

  oldbar->setDockPosition(newPosition);
  m_toolbars.remove(oldPosition);
  m_toolbars.insert(newPosition, oldbar);
  m_layout->removeWidget(oldbar);
  switch (newPosition) {
    case DockLayout::East: {
      m_layout->addWidget(oldbar, DockLayout::West);
      break;
    }
    case DockLayout::West: {
      m_layout->addWidget(oldbar, DockLayout::East);
      break;
    }
    case DockLayout::North: {
      m_layout->addWidget(oldbar, DockLayout::North);
      break;
    }
    case DockLayout::South: {
      m_layout->addWidget(oldbar, DockLayout::South);
      break;
    }
  }
  repaint();
}

void
DockWidget::addFooter()
{
  if (!m_footer) {
    m_footer = new DockFooter(this);
    m_layout->addWidget(m_footer, DockLayout::South);
    repaint();
  }
}

void
DockWidget::addHeader()
{
  if (!m_header) {
    m_header = new DockHeader(this);
    m_layout->addWidget(m_header, DockLayout::North);
    repaint();
  }
}

WidgetWrapper*
DockWidget::addToolbarIconButton(DockLayout::Position toolbarPos,
                                 WidgetPosition pos,
                                 QImage icon,
                                 const QString& tooltip)
{
  auto toolbar = m_toolbars.value(toolbarPos, nullptr);
  if (!toolbar) {
    toolbar = addNewToolbar(toolbarPos);
  }

  if (toolbar) {
    auto widget = toolbar->addIconButton(pos, icon, tooltip);
    return widget;
  }
  return nullptr;
}

WidgetWrapper*
DockWidget::addFooterIconButton(WidgetPosition pos,
                                QImage icon,
                                const QString& tooltip)
{
  WidgetWrapper* widget = nullptr;
  if (!m_footer) {
    m_footer = new DockFooter(this);
  }
  widget = m_footer->addIconButton(pos, icon, tooltip);
  return widget;
}

WidgetWrapper*
DockWidget::addHeaderIconButton(WidgetPosition pos,
                                QImage icon,
                                const QString& tooltip)
{
  WidgetWrapper* widget = nullptr;
  if (!m_header) {
    m_header = new DockHeader(this);
  }
  widget = m_header->addIconButton(pos, icon, tooltip);
  return widget;
}

WidgetWrapper*
DockWidget::addToolbarIconTextButton(DockLayout::Position toolbarPos,
                                     WidgetPosition position,
                                     QImage icon,
                                     const QString& text,
                                     Arrangement textPos,
                                     const QString& tooltip)
{
  auto toolbar = m_toolbars.value(toolbarPos, nullptr);
  if (!toolbar) {
    toolbar = addNewToolbar(toolbarPos);
  }

  if (toolbar) {
    auto widget =
      toolbar->addIconTextButton(position, icon, text, textPos, tooltip);
    return widget;
  }
  return nullptr;
}

WidgetWrapper*
DockWidget::addHeaderIconTextButton(WidgetPosition position,
                                    QImage icon,
                                    const QString& text,
                                    Arrangement textPos,
                                    const QString& tooltip)
{
  WidgetWrapper* widget = nullptr;
  //  if (!m_header) {
  //    m_header = new HeaderWidget(this);
  //  }
  widget = m_header->addIconTextButton(position, icon, text, textPos, tooltip);
  return widget;
}

WidgetWrapper*
DockWidget::addFooterIconTextButton(WidgetPosition position,
                                    QImage icon,
                                    const QString& text,
                                    Arrangement textPos,
                                    const QString& tooltip)
{
  WidgetWrapper* widget = nullptr;
  widget = m_footer->addIconTextButton(position, icon, text, textPos, tooltip);
  return widget;
}

WidgetWrapper*
DockWidget::addToolbarTextButton(DockLayout::Position toolbarPos,
                                 WidgetPosition position,
                                 const QString& text,
                                 const QString& tooltip)
{
  auto toolbar = m_toolbars.value(toolbarPos, nullptr);
  if (!toolbar) {
    toolbar = addNewToolbar(toolbarPos);
  }

  if (toolbar) {
    auto widget = toolbar->addTextButton(position, text, tooltip);
    return widget;
  }
  return nullptr;
}

WidgetWrapper*
DockWidget::addHeaderTextButton(WidgetPosition position,
                                const QString& text,
                                const QString& tooltip)
{
  WidgetWrapper* widget = nullptr;
  if (!m_header) {
    m_header = new DockHeader(this);
  }
  widget = m_header->addTextButton(position, text, tooltip);
  return widget;
}

WidgetWrapper*
DockWidget::addFooterTextButton(WidgetPosition position,
                                const QString& text,
                                const QString& tooltip)
{
  WidgetWrapper* widget = nullptr;
  if (!m_footer) {
    m_footer = new DockFooter(this);
  }
  widget = m_footer->addTextButton(position, text, tooltip);
  return widget;
}

WidgetWrapper*
DockWidget::addToolbarSpacer(DockLayout::Position toolbarPos,
                             WidgetPosition position)
{
  auto toolbar = m_toolbars.value(toolbarPos, nullptr);
  if (!toolbar) {
    toolbar = addNewToolbar(toolbarPos);
  }

  if (toolbar) {
    auto widget = toolbar->addSpacer(position);
    return widget;
  }
  return nullptr;
}

WidgetWrapper*
DockWidget::addHeaderSpacer(WidgetPosition position)
{
  WidgetWrapper* widget = nullptr;
  if (!m_header) {
    m_header = new DockHeader(this);
  }
  widget = m_header->addSpacer(position);
  return widget;
}

WidgetWrapper*
DockWidget::addFooterSpacer(WidgetPosition position)
{
  WidgetWrapper* widget = nullptr;
  if (!m_footer) {
    m_footer = new DockFooter(this);
  }
  widget = m_footer->addSpacer(position);
  return widget;
}

void
DockWidget::setCornerType(CornerType topLeft,
                          CornerType topRight,
                          CornerType bottomLeft,
                          CornerType bottomRight)
{
  m_layout->setCornerType(topLeft, topRight, bottomLeft, bottomRight);
  DockWidgetItem* widget;
  if (topLeft != None) {
    widget = new DockBoxCorner(DockLayout::NorthEast, this);
    m_layout->addWidget(widget, DockLayout::NorthEast);
  }
  if (topRight != None) {
    widget = new DockBoxCorner(DockLayout::NorthWest, this);
    m_layout->addWidget(widget, DockLayout::NorthWest);
  }
  if (bottomLeft != None) {
    widget = new DockBoxCorner(DockLayout::SouthEast, this);
    m_layout->addWidget(widget, DockLayout::SouthEast);
  }
  if (bottomRight != None) {
    widget = new DockBoxCorner(DockLayout::SouthWest, this);
    m_layout->addWidget(widget, DockLayout::SouthWest);
  }
}

//====================================================================
//=== WidgetWrapper
//====================================================================
WidgetWrapper::WidgetWrapper(QObject* parent)
  : QObject(parent)
  , m_backColor(QColor(64, 65, 66))
  , m_hoverBackColor(QColorConstants::X11::grey43)
  , m_selectedColor(QColorConstants::X11::grey18)
  , m_margins(QMargins(LEFTMARGIN, TOPMARGIN, RIGHTMARGIN, BOTTOMMARGIN))
{}

bool
WidgetWrapper::isEnabled() const
{
  return m_enabled;
}

void
WidgetWrapper::setEnabled(bool newEnabled)
{
  m_enabled = newEnabled;
}

const QRect&
WidgetWrapper::rect() const
{
  return m_rect;
}

void
WidgetWrapper::setRect(const QRect& newRect)
{
  m_rect = newRect;
}

WidgetType
WidgetWrapper::type() const
{
  return m_type;
}

void
WidgetWrapper::setType(WidgetType newType)
{
  m_type = newType;
}

WidgetPosition
WidgetWrapper::widgetPosition() const
{
  return m_widgetPosition;
}

void
WidgetWrapper::setWidgetPosition(WidgetPosition newPosition)
{
  m_widgetPosition = newPosition;
  emit widgetChanged();
}

Arrangement
WidgetWrapper::arrangement() const
{
  return m_arrangement;
}

void
WidgetWrapper::setArrangement(Arrangement newArrangement)
{
  m_arrangement = newArrangement;
  emit widgetChanged();
}

const QString&
WidgetWrapper::tooltip() const
{
  return m_tooltip;
}

void
WidgetWrapper::setTooltip(const QString& newTooltip)
{
  m_tooltip = newTooltip;
}

bool
WidgetWrapper::isHoverOver() const
{
  return m_hoverOver;
}

void
WidgetWrapper::setHoverOver(bool newHoverOver)
{
  m_hoverOver = newHoverOver;
}

void
WidgetWrapper::paintBackground(QPainter& painter)
{
  painter.save();
  if (isHoverOver()) {
    painter.fillRect(m_rect, m_hoverBackColor);
  } else if (isSelected()) {
    painter.fillRect(m_rect, m_selectedColor);
  } else {
    painter.fillRect(m_rect, m_backColor);
  }
  painter.restore();
}

const QBrush&
WidgetWrapper::backColor() const
{
  return m_backColor;
}

void
WidgetWrapper::setBackColor(const QBrush& newBackColor)
{
  m_backColor = newBackColor;
}

const QBrush&
WidgetWrapper::hoverBackColor() const
{
  return m_hoverBackColor;
}

void
WidgetWrapper::setHoverBackColor(const QBrush& newHoverBackColor)
{
  m_hoverBackColor = newHoverBackColor;
}

const QBrush&
WidgetWrapper::selectedColor() const
{
  return m_selectedColor;
}

void
WidgetWrapper::setSelectedColor(const QBrush& newSelectedColor)
{
  m_selectedColor = newSelectedColor;
}

const QSize&
WidgetWrapper::content() const
{
  return m_content;
}

int
WidgetWrapper::halfDifference(int large, int small)
{
  return int((large - small) / 2);
}

bool
WidgetWrapper::isSelected() const
{
  return m_selected;
}

void
WidgetWrapper::setSelected(bool newSelected)
{
  m_selected = newSelected;
}

const QMargins&
WidgetWrapper::margins() const
{
  return m_margins;
}

void
WidgetWrapper::setMargins(const QMargins& newMargins)
{
  m_margins = newMargins;
  emit widgetChanged();
}

int
WidgetWrapper::leftMargin()
{
  return m_margins.left();
}

int
WidgetWrapper::rightMargin()
{
  return m_margins.right();
}

int
WidgetWrapper::topMargin()
{
  return m_margins.top();
}

int
WidgetWrapper::bottomMargin()
{
  return m_margins.bottom();
}

void
WidgetWrapper::setLeftMargin(int margin)
{
  m_margins.setLeft(margin);
  emit widgetChanged();
}

void
WidgetWrapper::setRightMargin(int margin)
{
  m_margins.setRight(margin);
  emit widgetChanged();
}

void
WidgetWrapper::setTopMargin(int margin)
{
  m_margins.setTop(margin);
  emit widgetChanged();
}

void
WidgetWrapper::setBottomMargin(int margin)
{
  m_margins.setBottom(margin);
  emit widgetChanged();
}

//====================================================================
//=== ButtonWrapper
//====================================================================
ButtonWidget::ButtonWidget(QObject* parent)
  : WidgetWrapper(parent)
  , m_textColor(QColorConstants::X11::LightGrey)
{}

const QRect&
ButtonWidget::iconRect() const
{
  return m_iconRect;
}

void
ButtonWidget::setIconRect(const QRect& newIconRect)
{
  m_iconRect = newIconRect;
}

const QIcon&
ButtonWidget::icon() const
{
  return m_icon;
}

void
ButtonWidget::setIcon(const QIcon& newIcon)
{
  m_icon = newIcon;
}

const QSize&
ButtonWidget::iconSize() const
{
  return m_iconSize;
}

void
ButtonWidget::setIconSize(const QSize& newIconSize)
{
  m_iconSize = newIconSize;
  emit widgetChanged();
}

int
ButtonWidget::spacer() const
{
  return m_spacer;
}

void
ButtonWidget::setSpacer(int newSpacer)
{
  m_spacer = newSpacer;
}

const QSize
ButtonWidget::calcSize()
{
  int width = 0, height = 0, value = 0;
  int iconWidth = 0, iconHeight = 0, textWidth = 0, textHeight = 0;
  auto fm = qobject_cast<DockWidgetItem*>(parent())->fontMetrics();

  if (!icon().isNull()) {
    iconWidth = iconSize().width();
    iconHeight = iconSize().height();
  }

  if (!text().isEmpty()) {
    textWidth = fm.horizontalAdvance(text());
    textHeight = fm.height();
  }

  switch (arrangement()) {
    case TextAboveIcon:
    case TextBelowIcon: {
      value = (iconWidth > textWidth ? iconWidth : textWidth);
      width = (value > width ? value : width);
      m_content.setWidth(value);
      value = iconHeight + spacer() + textHeight;
      m_content.setHeight(value);
      height = (value > height ? value : height);
      break;
    }
    case TextToLeft:
    case TextToRight: {
      value = iconWidth + spacer() + textWidth;
      m_content.setWidth(value);
      width = (value > width ? value : width);
      value = (iconHeight > textHeight ? iconHeight : textHeight);
      height = (value > height ? value : height);
      m_content.setHeight(height);
      break;
    }
    case IconOnly: {
      width = (iconWidth > width ? iconWidth : width);
      m_content.setWidth(width);
      height = (iconHeight > height ? iconHeight : height);
      m_content.setHeight(height);
      break;
    }
    case TextOnly:
      width = (textWidth > width ? textWidth : width);
      m_content.setWidth(width);
      height = (textHeight > height ? textHeight : height);
      m_content.setHeight(height);
      break;
    case NoArrangement:
      break;
  }
  return QSize(width, height);
}

void
ButtonWidget::paintWidget(QPainter& painter)
{
  painter.save();
  paintBackground(painter);

  if (!m_icon.isNull()) {
    auto pixmap = icon().pixmap(m_iconSize,
                                isEnabled() ? QIcon::Normal : QIcon::Disabled,
                                isSelected() ? QIcon::On : QIcon::Off);
    painter.drawPixmap(m_iconRect.topLeft(), pixmap);
  }

  if (!m_text.isEmpty()) {
    auto pen = painter.pen();
    pen.setColor(m_textColor);
    painter.setPen(pen);
    painter.drawText(m_textRect.left(), m_textRect.bottom(), m_text);
  }

  painter.restore();
}

void
ButtonWidget::resizeEvent(QRect& r, int& min, int& max)
{
  auto p = qobject_cast<DockWidgetItem*>(parent());
  auto dockPosition = p->dockPosition();
  auto iconHeight = 0;
  auto iconWidth = 0;
  auto iconLeft = 0;
  auto iconTop = 0;
  auto textHeight = 0;
  auto textWidth = 0;
  auto textLeft = 0;
  auto textTop = 0;
  auto fm = p->fontMetrics();

  iconHeight = m_iconSize.height();
  iconWidth = m_iconSize.width();
  textHeight = fm.height();
  textWidth = fm.horizontalAdvance(m_text);

  switch (arrangement()) {
    case TextBelowIcon: {
      switch (dockPosition) {
        case DockLayout::East:
        case DockLayout::West: {
          if (widgetPosition() == Start) {
            r.setTop(min);
            iconTop = min + halfDifference(p->maxHeight(), m_content.height());
            min += p->maxHeight();
          } else if (widgetPosition() == End) {
            max -= p->maxHeight();
            r.setTop(max);
            iconTop = max + halfDifference(p->maxHeight(), m_content.height());
          }
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          textTop = iconTop + iconHeight + spacer();
          textLeft = halfDifference(p->maxWidth(), textWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case DockLayout::North:
        case DockLayout::South: {
          if (widgetPosition() == Start) {
            r.setLeft(min);
            iconLeft = min + halfDifference(p->maxWidth(), m_content.width());
            textLeft = min + halfDifference(p->maxWidth(), textWidth);
            min += p->maxWidth();
          } else if (widgetPosition() == End) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + halfDifference(p->maxWidth(), m_content.width());
            textLeft = max + halfDifference(p->maxWidth(), textWidth);
          }
          iconTop = halfDifference(p->maxHeight(), m_content.height());
          textTop = iconTop + iconHeight + spacer();
          min += p->maxWidth();
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case TextAboveIcon: {
      switch (dockPosition) {
        case DockLayout::East:
        case DockLayout::West: {
          if (widgetPosition() == Start) {
            r.setTop(min);
            textTop = min + topMargin();
            min += p->maxHeight();
          } else if (widgetPosition() == End) {
            max -= p->maxHeight();
            r.setTop(max);
            textTop = max + topMargin();
          }
          textLeft = halfDifference(p->maxWidth(), textWidth);
          iconTop = textTop + textHeight + spacer();
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case DockLayout::North:
        case DockLayout::South: {
          if (widgetPosition() == Start) {
            r.setLeft(min);
            textLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (widgetPosition() == End) {
            max -= p->maxWidth();
            r.setLeft(max);
            textLeft = max + leftMargin();
          }
          textTop = halfDifference(p->maxHeight(), textHeight);
          iconTop = textTop + textHeight + spacer();
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case TextToRight: {
      switch (dockPosition) {
        case DockLayout::East:
        case DockLayout::West: {
          if (widgetPosition() == Start) {
            r.setTop(min);
            textTop = min + halfDifference(p->maxHeight(), textHeight);
            iconTop = min + halfDifference(p->maxHeight(), iconHeight);
            min += p->maxHeight();
          } else if (widgetPosition() == End) {
            max -= p->maxHeight();
            r.setTop(max);
            textTop = max + halfDifference(p->maxHeight(), textHeight);
            iconTop = max + halfDifference(p->maxHeight(), iconHeight);
          }
          iconLeft =
            halfDifference(p->maxWidth(), textWidth + spacer() + iconWidth);
          textLeft = iconLeft + iconWidth + spacer();
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case DockLayout::North:
        case DockLayout::South: {
          if (widgetPosition() == Start) {
            r.setLeft(min);
            iconLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (widgetPosition() == End) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + leftMargin();
          }
          iconTop = halfDifference(p->maxHeight(), iconHeight);
          textLeft = iconLeft + iconWidth + spacer();
          textTop = halfDifference(p->maxHeight(), textHeight);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case TextToLeft: {
      switch (dockPosition) {
        case DockLayout::East:
        case DockLayout::West: {
          if (widgetPosition() == Start) {
            r.setTop(min);
            textTop = min + halfDifference(p->maxHeight(), textHeight);
            iconTop = min + halfDifference(p->maxHeight(), iconHeight);
            min += p->maxHeight();
          } else if (widgetPosition() == End) {
            max -= p->maxHeight();
            r.setTop(max);
            textTop = max + halfDifference(p->maxHeight(), textHeight);
            iconTop = max + halfDifference(p->maxHeight(), iconHeight);
          }
          textLeft =
            halfDifference(p->maxWidth(), textWidth + spacer() + iconWidth);
          iconLeft = textLeft + textWidth + spacer();
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case DockLayout::North:
        case DockLayout::South: {
          if (widgetPosition() == Start) {
            r.setLeft(min);
            textLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (widgetPosition() == End) {
            max -= p->maxWidth();
            r.setLeft(max);
            textLeft = max + leftMargin();
          }
          textTop = halfDifference(p->maxHeight(), textHeight);
          iconTop = textTop + textHeight + spacer();
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case IconOnly: {
      switch (dockPosition) {
        case DockLayout::East:
        case DockLayout::West: {
          if (widgetPosition() == Start) {
            r.setTop(min);
            iconTop = min + halfDifference(p->maxHeight(), iconHeight);
            min += p->maxHeight();
          } else if (widgetPosition() == End) {
            max -= p->maxHeight();
            r.setTop(max);
            iconTop = max + halfDifference(p->maxHeight(), iconHeight);
          }
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          break;
        }
        case DockLayout::North:
        case DockLayout::South: {
          if (widgetPosition() == Start) {
            r.setLeft(min);
            iconLeft = min + halfDifference(p->maxWidth(), iconWidth);
            min += p->maxWidth();
          } else if (widgetPosition() == End) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + halfDifference(p->maxWidth(), iconWidth);
          }
          iconTop = halfDifference(p->maxHeight(), iconHeight);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          break;
        }
      }
      break;
    }
    case TextOnly: {
      switch (dockPosition) {
        case DockLayout::East:
        case DockLayout::West: {
          textLeft = halfDifference(p->maxWidth(), textWidth);
          if (widgetPosition() == Start) {
            textTop = min + halfDifference(p->maxHeight(), textHeight);
            r.setTop(min);
            min += p->maxHeight();
          } else if (widgetPosition() == End) {
            max -= p->maxHeight();
            textTop = max + halfDifference(p->maxHeight(), textHeight);
            r.setTop(max);
          }
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case DockLayout::North:
        case DockLayout::South: {
          textTop = halfDifference(p->maxHeight(), textHeight);
          textLeft = min + halfDifference(p->maxWidth(), textWidth);
          if (widgetPosition() == Start) {
            r.setLeft(min);
            min += p->maxWidth();
          } else if (widgetPosition() == End) {
            max -= p->maxWidth();
            r.setLeft(max);
          }
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  r.setWidth(p->maxWidth());
  r.setHeight(p->maxHeight());
}

const QColor&
ButtonWidget::textColor() const
{
  return m_textColor;
}

void
ButtonWidget::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

const QString&
ButtonWidget::text() const
{
  return m_text;
}

void
ButtonWidget::setText(const QString& newText)
{
  m_text = newText;
  emit widgetChanged();
}

const QRect&
ButtonWidget::textRect() const
{
  return m_textRect;
}

void
ButtonWidget::setTextRect(const QRect& newTextRect)
{
  m_textRect = newTextRect;
}

//====================================================================
//=== SpacerWrapper
//====================================================================
SpacerWidget::SpacerWidget(QObject* parent)
  : WidgetWrapper(parent)
  , m_color(QColorConstants::X11::DimGray)
{}

const QSize
SpacerWidget::calcSize()
{
  auto p = qobject_cast<DockWidget*>(parent());
  return QSize(p->width(), p->height());
}

const QColor&
SpacerWidget::color() const
{
  return m_color;
}

void
SpacerWidget::setColor(const QColor& newColor)
{
  m_color = newColor;
}

int
SpacerWidget::thickness() const
{
  return m_thickness;
}

void
SpacerWidget::setThickness(int newThickness)
{
  m_thickness = newThickness;
}

void
SpacerWidget::paintWidget(QPainter& painter)
{
  painter.save();
  paintBackground(painter);
  auto pen = painter.pen();
  pen.setColor(m_color);
  pen.setWidth(thickness());
  painter.setPen(pen);
  auto t = m_rect.top() + m_margins.top();
  painter.drawLine(m_rect.left(), t, m_rect.right(), t);
  painter.restore();
}

void
SpacerWidget::resizeEvent(QRect& r, int& min, int& max)
{
  auto p = qobject_cast<DockWidgetItem*>(parent());
  auto dockPosition = p->dockPosition();

  switch (dockPosition) {
    case DockLayout::East:
    case DockLayout::West: {
      r.setX(0);
      auto h = topMargin() + thickness() + bottomMargin();
      if (widgetPosition() == Start) {
        r.setTop(min);
        min += h;
      } else if (widgetPosition() == End) {
        max -= h;
        r.setTop(max);
      }
      r.setWidth(p->maxWidth());
      r.setHeight(h);
      break;
    }
    case DockLayout::North:
    case DockLayout::South: {
      // TODO
      break;
    }
  }
}

//====================================================================
//=== BaseDockWidget
//====================================================================
DockWidgetItem::DockWidgetItem(DockLayout::Position position, QWidget* parent)
  : QWidget{ parent }
  , m_dockPosition(position)
  , m_backColor(QColor(64, 65, 66))
  , m_hoverBackColor(QColorConstants::X11::grey43)
  , m_selectedColor(QColorConstants::X11::grey18)
  , m_spacerColor(QColorConstants::X11::DimGrey)
{
  switch (m_dockPosition) {
    case DockLayout::West:
    case DockLayout::East: {
      m_width = WIDTH;
      m_maxWidgetWidth = m_width;
      break;
    }
    case DockLayout::North:
    case DockLayout::South: {
      m_height = HEIGHT;
      m_maxWidgetHeight = m_height;
      break;
    }
  }
  auto f = font();
  f.setPointSize(8);
  setFont(f);
}

DockWidgetItem::~DockWidgetItem()
{
  WidgetWrapper* l;
  while ((l = takeAt(0)))
    delete l;
}

WidgetWrapper*
DockWidgetItem::addIconButton(WidgetPosition pos,
                              QImage icon,
                              const QString& tooltip)
{
  WidgetWrapper* w =
    createWidgetWrapper(Button, pos, icon, QString(), IconOnly, tooltip);
  calcWidgetSizes();
  connect(
    w, &WidgetWrapper::widgetChanged, this, &DockWidgetItem::calcWidgetSizes);
  return w;
}

WidgetWrapper*
DockWidgetItem::addIconTextButton(WidgetPosition pos,
                                  QImage icon,
                                  const QString& text,
                                  Arrangement textPos,
                                  const QString& tooltip)
{
  WidgetWrapper* w =
    createWidgetWrapper(Button, pos, icon, text, textPos, tooltip);
  calcWidgetSizes();
  connect(
    w, &WidgetWrapper::widgetChanged, this, &DockWidgetItem::calcWidgetSizes);
  return w;
}

WidgetWrapper*
DockWidgetItem::addTextButton(WidgetPosition pos,
                              const QString& text,
                              const QString& tooltip)
{
  WidgetWrapper* w =
    createWidgetWrapper(Button, pos, QImage(), text, TextOnly, tooltip);
  calcWidgetSizes();
  connect(
    w, &WidgetWrapper::widgetChanged, this, &DockWidgetItem::calcWidgetSizes);
  return w;
}

WidgetWrapper*
DockWidgetItem::addSpacer(WidgetPosition pos)
{
  WidgetWrapper* w =
    createWidgetWrapper(Spacer, pos, QImage(), QString(), IconOnly, QString());
  return w;
}

WidgetWrapper*
DockWidgetItem::addCustomWidget(CustomWidget* w)
{
  m_widgets.append(w);
  return w;
}

void
DockWidgetItem::setText(int index, const QString& text)
{
  auto w = at(index);
  if (w) {
    switch (w->type()) {
      case Button: {
        auto tb = qobject_cast<ButtonWidget*>(w);
        tb->setText(text);
        break;
      }
      default: {
      }
    }
  }
}

void
DockWidgetItem::setToolTip(int index, const QString& tooltip)
{
  auto w = at(index);
  if (w)
    w->setTooltip(tooltip);
}

WidgetWrapper*
DockWidgetItem::createWidgetWrapper(WidgetType type,
                                    WidgetPosition pos,
                                    QImage image,
                                    const QString& text,
                                    Arrangement textPos,
                                    const QString& tooltip)
{
  WidgetWrapper* wrapper;

  switch (type) {
    case Button: {
      auto bw = new ButtonWidget(this);
      bw->setIcon(QIcon(QPixmap::fromImage(image)));
      bw->setIconSize(image.size());
      bw->setText(text);
      bw->setArrangement(textPos);
      bw->setTooltip(tooltip);
      wrapper = bw;

      auto fm = fontMetrics();
      auto width = fm.horizontalAdvance(text);
      width = (width > bw->iconSize().width() ? width : bw->iconSize().width());
      m_maxWidgetWidth = (width > m_maxWidgetWidth ? width : m_maxWidgetWidth);
      auto height = bw->topMargin() + fm.height() + bw->spacer() +
                    bw->iconSize().height() + bw->bottomMargin();
      m_maxWidgetHeight =
        (height > m_maxWidgetHeight ? height : m_maxWidgetHeight);

      break;
    }
    case List: {
      // TODO
      wrapper = nullptr;
      break;
    }
    case Spacer: {
      auto sp = new SpacerWidget(this);
      wrapper = sp;

      break;
    }
    default:
      wrapper = nullptr;
      break;
  }

  if (wrapper) {
    wrapper->setType(type);
    wrapper->setWidgetPosition(pos);
    m_widgets.append(wrapper);

    setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  }

  return wrapper;
}

WidgetWrapper*
DockWidgetItem::takeAt(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetWrapper* layoutStruct = m_widgets.takeAt(index);
    return layoutStruct;
  }
  return nullptr;
}

WidgetWrapper*
DockWidgetItem::at(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetWrapper* layoutStruct = m_widgets.at(index);
    return layoutStruct;
  }
  return nullptr;
}

bool
DockWidgetItem::replace(int index, WidgetWrapper* w)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.replace(index, w);
    return true;
  }
  return false;
}

bool
DockWidgetItem::widgetEnabled(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isEnabled();
  }
  return false;
}

bool
DockWidgetItem::enableWidget(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setEnabled(value);
    repaint();
    return true;
  }
  return false;
}

bool
DockWidgetItem::selected(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isSelected();
  }
  return false;
}

bool
DockWidgetItem::setSelected(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setSelected(value);
    repaint();
    return true;
  }
  return false;
}

QMargins
DockWidgetItem::widgetMargins(int index)
{
  auto w = at(index);
  if (w) {
    return w->margins();
  }
  return QMargins();
}

void
DockWidgetItem::setWidgetMargins(int index,
                                 int left,
                                 int top,
                                 int right,
                                 int bottom)
{
  auto w = at(index);
  if (w) {
    w->setLeftMargin(left);
    w->setTopMargin(top);
    w->setRightMargin(right);
    w->setBottomMargin(bottom);
  }
  repaint();
}

/*
 * Scans the sub widgets to find the maximum widget size. This means that all
 * the sub-widgets are the same size, at least if they are buttons. Other
 * sub-widgets may or may not affect this value.
 */
void
DockWidgetItem::calcWidgetSizes()
{
  auto maxMargins = calcMaxMargins();
  auto maxContentsSize = calcMaxContentsSize();
  auto w = maxMargins.left() + maxContentsSize.width() + maxMargins.right();
  auto h = maxMargins.top() + maxContentsSize.height() + maxMargins.bottom();

  m_maxWidgetWidth = (w > m_maxWidgetWidth ? w : m_maxWidgetWidth);
  m_maxWidgetHeight = (h > m_maxWidgetHeight ? h : m_maxWidgetHeight);
  resize(m_maxWidgetWidth, m_maxWidgetHeight);
}

/*
 * Returns the size of the contents of the sub widget, including spacers.
 */
QSize
DockWidgetItem::calcMaxContentsSize()
{
  auto width = 0;
  auto height = 0;

  for (auto& w : m_widgets) {
    auto size = w->calcSize();
    width = (size.width() > width ? size.width() : width);
    height = (size.height() > height ? size.height() : height);
  }
  return QSize(width, height);
}

QMargins
DockWidgetItem::calcMaxMargins()
{
  QMargins maxMargins;
  for (auto& w : m_widgets) {
    maxMargins.setLeft(w->leftMargin() > maxMargins.left() ? w->leftMargin()
                                                           : maxMargins.left());
    maxMargins.setRight(w->rightMargin() > maxMargins.right()
                          ? w->rightMargin()
                          : maxMargins.right());
    maxMargins.setTop(w->topMargin() > maxMargins.top() ? w->topMargin()
                                                        : maxMargins.top());
    maxMargins.setBottom(w->bottomMargin() > maxMargins.bottom()
                           ? w->bottomMargin()
                           : maxMargins.bottom());
  }
  return maxMargins;
}

void
DockWidgetItem::setPreferredSize(int width, int height)
{
  m_width = width;
  m_height = height;
  if (m_dockPosition == DockLayout::East ||
      m_dockPosition == DockLayout::West) {
    setFixedWidth(m_width);
    m_maxWidgetWidth = m_width;
  } else {
    setFixedHeight(m_height);
    m_maxWidgetHeight = m_height;
  }
}

void
DockWidgetItem::setPreferredSize(QSize size)
{
  setPreferredSize(size.width(), size.height());
}

void
DockWidgetItem::setPreferredWidth(int width)
{
  m_width = width;
}

void
DockWidgetItem::setPreferredHeight(int height)
{
  m_height = height;
}

QSize
DockWidgetItem::sizeHint() const
{
  return m_frameRect.size();
}

DockLayout::Position
DockWidgetItem::dockPosition() const
{
  return m_dockPosition;
}

int
DockWidgetItem::maxWidth() const
{
  return m_maxWidgetWidth;
}

int
DockWidgetItem::maxHeight() const
{
  return m_maxWidgetHeight;
}

void
DockWidgetItem::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  auto min = 1;
  auto max =
    (m_dockPosition == DockLayout::North || m_dockPosition == DockLayout::South
       ? s.width() - TOOLBAR_ENDER
       : s.height() - TOOLBAR_ENDER);
  m_frameRect = QRect(0, 0, s.width(), s.height());
  for (auto& w : m_widgets) {
    auto r = w->rect();
    w->resizeEvent(r, min, max);
    w->setRect(r);
  }
}

void
DockWidgetItem::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.fillRect(m_frameRect, m_backColor);

  for (auto& w : m_widgets) {
    w->paintWidget(painter);
  }
}

void
DockWidgetItem::hoverEnterEvent(QHoverEvent* event)
{
  // TODO tooltips not working on hover.
  for (auto& w : m_widgets) {
    auto p = event->pos();
    if (w->rect().contains(p)) {
      w->setHoverOver(true);
      QToolTip::showText(p, w->tooltip(), this, w->rect());
      repaint();
    } else {
      w->setHoverOver(false);
      repaint();
    }
  }
}

void
DockWidgetItem::hoverLeaveEvent(QHoverEvent* /*event*/)
{
  for (auto& w : m_widgets) {
    w->setHoverOver(false);
    QToolTip::hideText();
  }
  repaint();
}

void
DockWidgetItem::hoverMoveEvent(QHoverEvent* event)
{
  for (auto& w : m_widgets) {
    if (w->rect().contains(event->pos())) {
      w->setHoverOver(true);
      repaint();
    } else {
      w->setHoverOver(false);
      repaint();
    }
  }
}

void
DockWidgetItem::mousePressEvent(QMouseEvent* event)
{
  for (int i = 0; i < m_widgets.size(); i++) {
    auto w = m_widgets.at(i);
    if (w->rect().contains(event->pos())) {
      if (w->isEnabled()) {
        switch (w->type()) {
          case Button: {
            emit w->widgetClicked();
            break;
          }
          default:
            break;
        }
      }
    }
  }
}

void
DockWidgetItem::mouseReleaseEvent(QMouseEvent* /*event*/)
{
  // TODO possibly remove if not used.
}

bool
DockWidgetItem::event(QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverEnter:
      hoverEnterEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverLeave:
      hoverLeaveEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverMove:
      hoverMoveEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    default:
      break;
  }
  return QWidget::event(event);
}

//====================================================================
//=== DockFooter
//====================================================================
DockFooter::DockFooter(QWidget* parent)
  : DockWidgetItem(DockLayout::South, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

void
DockFooter::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  auto pen = painter.pen();
  pen.setColor(QColor("black"));
  pen.setWidth(1);
  painter.setPen(pen);
  painter.drawLine(0, 0, 0, m_maxWidgetHeight);

  DockWidgetItem::paintEvent(event);
}

//====================================================================
//=== DockHeader
//====================================================================
DockHeader::DockHeader(QWidget* parent)
  : DockWidgetItem(DockLayout::North, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

void
DockHeader::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  auto pen = painter.pen();
  pen.setColor(QColor("black"));
  pen.setWidth(1);
  painter.setPen(pen);
  painter.drawLine(0, 0, 0, m_maxWidgetHeight);

  DockWidgetItem::paintEvent(event);
}

//====================================================================
//=== DockToolbar
//====================================================================
DockToolbar::DockToolbar(DockLayout::Position position, QWidget* parent)
  : DockWidgetItem(position, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

DockLayout::Position
DockToolbar::dockPosition()
{
  return m_dockPosition;
}

void
DockToolbar::setDockPosition(DockLayout::Position position)
{
  m_dockPosition = position;
  repaint();
}

void
DockToolbar::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  auto pen = painter.pen();
  pen.setColor(QColor("black"));
  pen.setWidth(1);
  painter.setPen(pen);
  painter.drawLine(0, 0, m_maxWidgetWidth, 0);

  DockWidgetItem::paintEvent(event);
}

//====================================================================
//=== BoxWidget
//====================================================================
DockBoxCorner::DockBoxCorner(DockLayout::Position position, QWidget* parent)
  : DockWidgetItem(position, parent)
{}

void
DockBoxCorner::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.fillRect(m_frameRect, m_backColor);
}

void
DockBoxCorner::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  m_frameRect = QRect(0, 0, s.width(), s.height());
}

DockLayout::Position
DockBoxCorner::dockPosition()
{
  return m_dockPosition;
}

void
DockBoxCorner::setDockPosition(DockLayout::Position position)
{
  m_dockPosition = position;
  repaint();
}

//====================================================================
//=== DockWidthCorner
//====================================================================
DockWidthCorner::DockWidthCorner(DockLayout::Position position, QWidget* parent)
  : DockWidgetItem(position, parent)
{}

void
DockWidthCorner::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.fillRect(m_frameRect, m_backColor);
}

void
DockWidthCorner::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  m_frameRect = QRect(0, 0, s.width(), s.height());
}

DockLayout::Position
DockWidthCorner::dockPosition()
{
  return m_dockPosition;
}

void
DockWidthCorner::setDockPosition(DockLayout::Position position)
{
  m_dockPosition = position;
  repaint();
}

//====================================================================
//=== DockHeightCorner
//====================================================================
DockHeightCorner::DockHeightCorner(DockLayout::Position position,
                                   QWidget* parent)
  : DockWidgetItem(position, parent)
{}

void
DockHeightCorner::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.fillRect(m_frameRect, m_backColor);
}

void
DockHeightCorner::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  m_frameRect = QRect(0, 0, s.width(), s.height());
}

DockLayout::Position
DockHeightCorner::dockPosition()
{
  return m_dockPosition;
}

void
DockHeightCorner::setDockPosition(DockLayout::Position position)
{
  m_dockPosition = position;
  repaint();
}

//====================================================================
//=== DockLayout
//====================================================================
DockLayout::DockLayout(QWidget* parent, const QMargins& margins, int spacing)
  : QLayout(parent)
{
  setContentsMargins(margins);
  setSpacing(spacing);
}

DockLayout::DockLayout(int spacing)
{
  setSpacing(spacing);
}

DockLayout::~DockLayout()
{
  QLayoutItem* l;
  while ((l = takeAt(0)))
    delete l;
}

void
DockLayout::addItem(QLayoutItem* item)
{
  add(item, West);
}

void
DockLayout::addWidget(QWidget* widget, Position position)
{
  add(new QWidgetItem(widget), position);
}

Qt::Orientations
DockLayout::expandingDirections() const
{
  return Qt::Horizontal | Qt::Vertical;
}

bool
DockLayout::hasHeightForWidth() const
{
  return false;
}

int
DockLayout::count() const
{
  return m_items.size();
}

QLayoutItem*
DockLayout::itemAt(int index) const
{
  ItemWrapper* wrapper = m_items.value(index);
  return wrapper ? wrapper->item : nullptr;
}

QSize
DockLayout::minimumSize() const
{
  return calculateSize(MinimumSize);
}

void
DockLayout::setGeometry(const QRect& rect)
{
  ItemWrapper* center = nullptr;

  auto eastWidth = 0;
  auto westWidth = 0;
  auto northHeight = 0;
  auto southHeight = 0;
  auto centreHeight = rect.height();
  auto centreWidth = rect.width();

  QLayout::setGeometry(rect);

  for (auto& wrapper : m_items) {
    QLayoutItem* item = wrapper->item;
    Position position = wrapper->position;
    auto h = item->sizeHint().height();
    auto w = item->sizeHint().width();

    switch (position) {
      case DockLayout::North:
        northHeight += w;
        centreHeight -= w;
        break;
      case DockLayout::South:
        southHeight += h;
        centreHeight -= h;
        break;
      case DockLayout::East:
        eastWidth += w;
        centreWidth -= w;
        break;
      case DockLayout::West:
        westWidth += w;
        centreWidth -= w;
        break;
      default:
        break;
    }
  }

  for (auto& wrapper : m_items) {
    QLayoutItem* item = wrapper->item;
    Position position = wrapper->position;
    switch (position) {
      case DockLayout::NorthEast:
        switch (m_northEast) {
          case Box: {
            item->setGeometry(
              QRect(rect.x() + centreWidth, rect.y(), eastWidth, northHeight));
            break;
          }
          case Height: {
            item->setGeometry(
              QRect(rect.x() + centreWidth, rect.y(), 0, northHeight));
            break;
          }
          case Width: {
            item->setGeometry(
              QRect(rect.x() + centreWidth, rect.y(), eastWidth, 0));
            break;
          }
          default:
            break;
        }
      case DockLayout::NorthWest:
        switch (m_northWest) {
          case Box: {
            item->setGeometry(
              QRect(rect.x(), rect.y(), westWidth, northHeight));
            break;
          }
          case Height: {
            item->setGeometry(QRect(rect.x(), rect.y(), 0, northHeight));
            break;
          }
          case Width: {
            item->setGeometry(QRect(rect.x(), rect.y(), westWidth, 0));
            break;
          }
          case None:
            break;
        }
        break;
      case DockLayout::SouthEast:
        switch (m_southEast) {
          case Box: {
            item->setGeometry(QRect(rect.x() + centreWidth,
                                    rect.y() + centreHeight,
                                    eastWidth,
                                    southHeight));
            break;
          }
          case Height: {
            item->setGeometry(QRect(
              rect.x() + centreWidth, rect.y() + centreHeight, 0, southHeight));
            break;
          }
          case Width: {
            item->setGeometry(QRect(
              rect.x() + centreWidth, rect.y() + centreHeight, eastWidth, 0));
            break;
          }
          case None:
            break;
        }
        break;
      case DockLayout::SouthWest:
        switch (m_southEast) {
          case Box: {
            item->setGeometry(
              QRect(rect.x(), rect.y() + centreHeight, westWidth, southHeight));
            break;
          }
          case Height: {
            item->setGeometry(
              QRect(rect.x(), rect.y() + centreHeight, 0, southHeight));
            break;
          }
          case Width: {
            item->setGeometry(
              QRect(rect.x(), rect.y() + centreHeight, westWidth, 0));
            break;
          }
          case None:
            break;
        }
        break;
      default:
        break;
    }
  }

  for (auto wrapper : m_items) {
    QLayoutItem* item = wrapper->item;
    Position position = wrapper->position;
    switch (position) {
      case DockLayout::West:
        item->setGeometry(
          QRect(rect.x(), rect.y() + centreHeight, westWidth, 0));
        break;
      case DockLayout::North:
        break;
      case DockLayout::South:
        break;
      case DockLayout::East:
        break;
      case DockLayout::Center:
        item->setGeometry(QRect(rect.x() + westWidth,
                                rect.y() + northHeight,
                                centreWidth,
                                centreHeight));
        break;
      default:
        break;
    }
  }

  //  for (auto wrapper : m_items) {
  //    QLayoutItem* item = wrapper->item;
  //    Position position = wrapper->position;

  //    if (position == North) {
  //      item->setGeometry(
  //        QRect(rect.x(), northHeight, rect.width(),
  //        item->sizeHint().height()));

  //      northHeight += item->geometry().height() + spacing();
  //    } else if (position == South) {
  //      item->setGeometry(QRect(item->geometry().x(),
  //                              item->geometry().y(),
  //                              rect.width(),
  //                              item->sizeHint().height()));

  //      southHeight += item->geometry().height() + spacing();

  //      item->setGeometry(
  //        QRect(rect.x(),
  //              rect.y() + rect.height() - southHeight + spacing(),
  //              item->geometry().width(),
  //              item->geometry().height()));
  //    } else if (position == Center) {
  //      center = wrapper;
  //    }
  //  }

  //  centreHeight = rect.height() - northHeight - southHeight;

  //  for (i = 0; i < m_items.size(); ++i) {
  //    ItemWrapper* wrapper = m_items.at(i);
  //    QLayoutItem* item = wrapper->item;
  //    Position position = wrapper->position;

  //    if (position == West) {
  //      //      auto s = item->sizeHint();
  //      //      auto sp = spacing();
  //      item->setGeometry(QRect(rect.x() + westWidth,
  //                              northHeight,
  //                              item->sizeHint().width(),
  //                              centreHeight));

  //      westWidth += item->geometry().width() + spacing();
  //    } else if (position == East) {
  //      item->setGeometry(QRect(item->geometry().x(),
  //                              item->geometry().y(),
  //                              item->sizeHint().width(),
  //                              centreHeight));

  //      eastWidth += item->geometry().width() + spacing();

  //      item->setGeometry(QRect(rect.x() + rect.width() - eastWidth +
  //      spacing(),
  //                              northHeight,
  //                              item->geometry().width(),
  //                              item->geometry().height()));
  //    }
  //  }

  //  if (center)
  //    center->item->setGeometry(QRect(westWidth,
  //                                    northHeight,
  //                                    rect.width() - eastWidth - westWidth,
  //                                    centreHeight));
}

QSize
DockLayout::sizeHint() const
{
  return calculateSize(SizeHint);
}

QLayoutItem*
DockLayout::takeAt(int index)
{
  if (index >= 0 && index < m_items.size()) {
    ItemWrapper* layoutStruct = m_items.takeAt(index);
    return layoutStruct->item;
  }
  return nullptr;
}

void
DockLayout::add(QLayoutItem* item, Position position)
{
  m_items.append(new ItemWrapper(item, position));
}

CornerType
DockLayout::topLeft() const
{
  return m_northWest;
}

void
DockLayout::setTopLeft(CornerType type)
{
  m_northWest = type;
}

CornerType
DockLayout::topRight() const
{
  return m_northEast;
}

void
DockLayout::setTopRight(CornerType type)
{
  m_northEast = type;
}

CornerType
DockLayout::bottomLeft() const
{
  return m_southWest;
}

void
DockLayout::setBottomLeft(CornerType type)
{
  m_southWest = type;
}

CornerType
DockLayout::bottomRight() const
{
  return m_southEast;
}

void
DockLayout::setBottomRight(CornerType type)
{
  m_southEast = type;
}

void
DockLayout::setCornerType(CornerType type)
{
  m_northWest = type;
  m_northEast = type;
  m_southWest = type;
  m_southEast = type;
}

void
DockLayout::setCornerType(CornerType topLeft,
                          CornerType topRight,
                          CornerType bottomLeft,
                          CornerType bottomRight)
{
  m_northWest = topLeft;
  m_northEast = topRight;
  m_southWest = bottomLeft;
  m_southEast = bottomRight;
}

QSize
DockLayout::calculateSize(SizeType sizeType) const
{
  QSize totalSize;

  for (int i = 0; i < m_items.size(); ++i) {
    ItemWrapper* wrapper = m_items.at(i);
    Position position = wrapper->position;
    QSize itemSize;

    if (sizeType == MinimumSize)
      itemSize = wrapper->item->minimumSize();
    else // (sizeType == SizeHint)
      itemSize = wrapper->item->sizeHint();

    if (position == North || position == South || position == Center)
      totalSize.rheight() += itemSize.height();

    if (position == West || position == East || position == Center)
      totalSize.rwidth() += itemSize.width();
  }
  return totalSize;
}
