#include "basewidget.h"
#include "x11colors.h"

//====================================================================
//=== BaseWidget
//====================================================================
BaseWidget::BaseWidget(WidgetPosition position, QWidget* parent)
  : QWidget{ parent }
  , m_widgetPosition(position)
  , m_backColor(QColor(64, 65, 66))
  , m_hoverBackColor(QColorConstants::X11::grey43)
  , m_selectedColor(QColorConstants::X11::grey18)
  , m_spacerColor(QColorConstants::X11::DimGrey)
{
  switch (m_widgetPosition) {
    case Left:
    case Right: {
      m_width = WIDTH;
      m_maxWidgetWidth = m_width;
      break;
    }
    case Top:
    case Bottom: {
      m_height = HEIGHT;
      m_maxWidgetHeight = m_height;
      break;
    }
  }
  auto f = font();
  f.setPointSize(8);
  setFont(f);
}

BaseWidget::~BaseWidget()
{
  WidgetWrapper* l;
  while ((l = takeAt(0)))
    delete l;
}

WidgetWrapper*
BaseWidget::addIconButton(WidgetPosition pos,
                          QImage icon,
                          const QString& tooltip)
{
  WidgetWrapper* w = createWidgetWrapper(
    Button, pos, icon, QString(), IconOnly, tooltip);
  calcWidgetSizes();
  connect(w, &WidgetWrapper::widgetChanged, this, &BaseWidget::calcWidgetSizes);
  return w;
}

WidgetWrapper*
BaseWidget::addIconTextButton(WidgetPosition pos,
                              QImage icon,
                              const QString& text,
                              Arrangement textPos,
                              const QString& tooltip)
{
  WidgetWrapper* w =
    createWidgetWrapper(Button, pos, icon, text, textPos, tooltip);
  calcWidgetSizes();
  connect(w, &WidgetWrapper::widgetChanged, this, &BaseWidget::calcWidgetSizes);
  return w;
}

WidgetWrapper*
BaseWidget::addTextButton(WidgetPosition pos,
                          const QString& text,
                          const QString& tooltip)
{
  WidgetWrapper* w = createWidgetWrapper(
    Button, pos, QImage(), text, TextOnly, tooltip);
  calcWidgetSizes();
  connect(w, &WidgetWrapper::widgetChanged, this, &BaseWidget::calcWidgetSizes);
  return w;
}

WidgetWrapper*
BaseWidget::addSpacer(WidgetPosition pos)
{
  WidgetWrapper* w =
    createWidgetWrapper(Spacer, pos, QImage(), QString(), IconOnly, QString());
  return w;
}

WidgetWrapper*
BaseWidget::addCustomWidget(CustomWidgetWrapper* w)
{
  m_widgets.append(w);
  return w;
}

void
BaseWidget::setText(int index, const QString& text)
{
  auto w = at(index);
  if (w) {
    switch (w->type()) {
      case Button: {
        auto tb = qobject_cast<ButtonWrapper*>(w);
        tb->setText(text);
        break;
      }
      default: {
      }
    }
  }
}

void
BaseWidget::setToolTip(int index, const QString& tooltip)
{
  auto w = at(index);
  if (w)
    w->setTooltip(tooltip);
}

WidgetWrapper*
BaseWidget::createWidgetWrapper(WidgetType type,
                                WidgetPosition pos,
                                QImage image,
                                const QString& text,
                                Arrangement textPos,
                                const QString& tooltip)
{
  WidgetWrapper* wrapper;

  switch (type) {
    case Button: {
      auto bw = new ButtonWrapper(this);
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
      m_maxWidgetHeight = (height > m_maxWidgetHeight ? height : m_maxWidgetHeight);

      break;
    }
    case List: {
      // TODO
      wrapper = nullptr;
      break;
    }
    case Spacer: {
      auto sp = new SpacerWrapper(this);
      wrapper = sp;

      break;
    }
    default:
      wrapper = nullptr;
      break;
  }

  if (wrapper) {
    wrapper->setType(type);
    wrapper->setPosition(pos);
    m_widgets.append(wrapper);

    setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  }

  return wrapper;
}

WidgetWrapper*
BaseWidget::takeAt(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetWrapper* layoutStruct = m_widgets.takeAt(index);
    return layoutStruct;
  }
  return nullptr;
}

WidgetWrapper*
BaseWidget::at(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetWrapper* layoutStruct = m_widgets.at(index);
    return layoutStruct;
  }
  return nullptr;
}

bool
BaseWidget::replace(int index, WidgetWrapper* w)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.replace(index, w);
    return true;
  }
  return false;
}

bool
BaseWidget::widgetEnabled(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isEnabled();
  }
  return false;
}

bool
BaseWidget::enableWidget(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setEnabled(value);
    repaint();
    return true;
  }
  return false;
}

bool
BaseWidget::selected(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isSelected();
  }
  return false;
}

bool
BaseWidget::setSelected(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setSelected(value);
    repaint();
    return true;
  }
  return false;
}

QMargins
BaseWidget::widgetMargins(int index)
{
  auto w = at(index);
  if (w) {
    return w->margins();
  }
  return QMargins();
}

void
BaseWidget::setWidgetMargins(int index,
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
BaseWidget::calcWidgetSizes()
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
BaseWidget::calcMaxContentsSize()
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
BaseWidget::calcMaxMargins()
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
BaseWidget::setPreferredSize(int width, int height)
{
  m_width = width;
  m_height = height;
  if (m_widgetPosition == Left || m_widgetPosition == Right) {
    setFixedWidth(m_width);
    m_maxWidgetWidth=m_width;
  } else {
    setFixedHeight(m_height);
    m_maxWidgetHeight = m_height;
  }
}

void
BaseWidget::setPreferredSize(QSize size)
{
  setPreferredSize(size.width(), size.height());
}

void
BaseWidget::setPreferredWidth(int width)
{
  m_width = width;
}

void
BaseWidget::setPreferredHeight(int height)
{
  m_height = height;
}

QSize
BaseWidget::sizeHint() const
{
  return QSize(m_width, m_height);
}

WidgetPosition
BaseWidget::widgetPosition() const
{
  return m_widgetPosition;
}

int
BaseWidget::maxWidth() const
{
  return m_maxWidgetWidth;
}

int
BaseWidget::maxHeight() const
{
  return m_maxWidgetHeight;
}

void
BaseWidget::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  auto min = 1;
  auto max =
    (m_widgetPosition == Top || m_widgetPosition == Bottom ? s.width()
                                                           : s.height());
  m_frameRect = QRect(0, 0, s.width(), s.height());
  for (auto& w : m_widgets) {
    auto r = w->rect();
    w->resizeEvent(r, min, max);
    w->setRect(r);
  }
}

void
BaseWidget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.fillRect(m_frameRect, m_backColor);

  painter.save();
  auto pen = painter.pen();
  pen.setColor(QColor("black"));
  pen.setWidth(1);
  painter.setPen(pen);
  painter.drawLine(0, 0, m_maxWidgetWidth, 0);
  painter.restore();

  for (auto& w : m_widgets) {
    w->paintWidget(painter);
  }
}

void
BaseWidget::hoverEnterEvent(QHoverEvent* event)
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
BaseWidget::hoverLeaveEvent(QHoverEvent* /*event*/)
{
  for (auto& w : m_widgets) {
    w->setHoverOver(false);
    QToolTip::hideText();
  }
  repaint();
}

void
BaseWidget::hoverMoveEvent(QHoverEvent* event)
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
BaseWidget::mousePressEvent(QMouseEvent* event)
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
BaseWidget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
  // TODO possibly remove if not used.
}

bool
BaseWidget::event(QEvent* event)
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
//=== FooterWidget
//====================================================================
FooterWidget::FooterWidget(QWidget* parent)
  : BaseWidget(Bottom, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

//====================================================================
//=== HeaderWidget
//====================================================================
HeaderWidget::HeaderWidget(QWidget* parent)
  : BaseWidget(Top, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

//====================================================================
//=== ToolbarWidget
//====================================================================
ToolbarWidget::ToolbarWidget(WidgetPosition position, QWidget* parent)
  : BaseWidget(position, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

WidgetPosition ToolbarWidget::position() { return m_widgetPosition; }

void ToolbarWidget::setPosition(WidgetPosition position)
{
  m_widgetPosition = position;
  repaint();
}

//====================================================================
//=== LeftToolbarWidget
//====================================================================
//LeftToolbarWidget::LeftToolbarWidget(QWidget* parent)
//  : ToolbarWidget(WidgetPosition::Left, parent)
//{
//  setPreferredSize(WIDTH, HEIGHT);
//  setMouseTracking(true);
//  setAttribute(Qt::WA_Hover);
//  setAutoFillBackground(true);
//  setContentsMargins(0, 0, 0, 0);
//}

//====================================================================
//=== RightToolbarWidget
//====================================================================
//RightToolbarWidget::RightToolbarWidget(QWidget* parent)
//  : ToolbarWidget(WidgetPosition::Right, parent)
//{
//  setPreferredSize(WIDTH, HEIGHT);
//  setMouseTracking(true);
//  setAttribute(Qt::WA_Hover);
//  setAutoFillBackground(true);
//  setContentsMargins(0, 0, 0, 0);
//}
