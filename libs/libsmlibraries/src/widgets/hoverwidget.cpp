#include "widgets/hoverwidget.h"
#include "utilities/ContainerUtil.h"
#include "utilities/characters.h"
#include "utilities/x11colors.h"

//====================================================================
//=== HoverWidget
//====================================================================
HoverWidget::HoverWidget(QWidget* parent)
  : QFrame{ parent }
  , m_background(QColorConstants::X11::lightyellow)
  , m_margins(QMargins(3, 3, 3, 8))
{
  buildGui();
}

HoverWidget::HoverWidget(const QString& title,
                         const QString& text,
                         QWidget* parent)
  : QFrame{ parent }
  , m_background(QColorConstants::X11::lightyellow)
  , m_margins(QMargins(3, 3, 3, 3))
{
  buildGui();
  setTitle(title);
  setText(text);
}

void
HoverWidget::buildGui()
{
  setAttribute(Qt::WA_Hover);
  setMouseTracking(true);
  setWindowFlags(Qt::CustomizeWindowHint);
  //  setContentsMargins(0, 0, 0, 0);

  setStyleSheet("QFrame {"
                "border: 1px solid black;"
                "margin: 0px;"
                "padding: 0px;"
                "background: #FFFFE0;"
                "}"
                "QPlainTextEdit {"
                "border: 0px;"
                "}");
  m_copyIcon = QPixmap(":/icon/Copy");
  m_copyPressedIcon = QPixmap(":/icon/CopyPressed");
  m_iconRect.setSize(m_copyIcon.size());

  auto layout = new QGridLayout;
  setLayout(layout);

  m_editor = new QPlainTextEdit(this);
  m_highlighter = new HoverHighlighter(m_editor->document());
  layout->addWidget(m_editor, 0, 0);

  auto act = new QAction(this);
  act->setIcon(m_copyIcon);
  m_copyBtn = new QToolButton(this);
  m_copyBtn->setDefaultAction(act);
  connect(act, &QAction::triggered, this, &HoverWidget::buttonClicked);
  layout->addWidget(m_copyBtn, 0, 1, Qt::AlignTop);
}

void
HoverWidget::setTitle(const QString& title)
{
  m_editor->clear();
  m_title = title;
  addData();
}

void
HoverWidget::addData()
{
  m_highlighter->lines.clear();
  auto i = 0;
  auto lines = m_title.split(Characters::NEWLINE);
  for (auto& line : lines) {
    m_editor->appendPlainText(line);
    m_highlighter->lines.insert(i++, Title);
  }
  lines = m_text.split(Characters::NEWLINE);
  for (auto& line : lines) {
    m_editor->appendPlainText(line);
    m_highlighter->lines.insert(i++, Text);
  }
}

void
HoverWidget::setText(const QString& text)
{
  m_editor->clear();
  m_text = text;
  addData();
}

void
HoverWidget::timedout()
{
  hide();
  emit finished();
}


bool
HoverWidget::event(QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverLeave:
      hoverLeave(static_cast<QHoverEvent*>(event));
      return true;
    default:
      break;
  }
  return QFrame::event(event);
}

void
HoverWidget::hoverLeave(QHoverEvent* /*event*/)
{
  // should already have been deleted but to be safe
  hide();
  emit finished();
}

void
HoverWidget::buttonClicked(bool)
{
  m_pressed = true;

  QClipboard* clipboard = QApplication::clipboard();

  clipboard->setText(m_editor->toPlainText(), QClipboard::Clipboard);

  if (clipboard->supportsSelection()) {
    clipboard->setText(m_editor->toPlainText(), QClipboard::Selection);
  }

#if defined(Q_OS_LINUX)
  QThread::msleep(1); // workaround for copied text not being available...
#endif

  hide();
}

void
HoverWidget::setSpacer(int spacer)
{
  m_spacer = spacer;
}

bool
HoverWidget::isEmpty()
{
  if (m_editor->toPlainText().isEmpty()) {
    return true;
  }
  return false;
}

void
HoverWidget::setMargins(const QMargins& Margins)
{
  m_margins = Margins;
}

void
HoverWidget::show(int timeout)
{
  if (timeout > 0) {
    auto* m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &HoverWidget::timedout);
    m_timer->start(timeout);
    QFrame::show();
  } else {
    QFrame::show();
  }
}

//====================================================================
//=== HoverWidget::HoverHighlighter
//====================================================================
HoverWidget::HoverHighlighter::HoverHighlighter(QTextDocument* document)
  : QSyntaxHighlighter(document)
  , m_backgroundColor(QColorConstants::X11::lightyellow)
  , m_foregroundColor(Qt::black)
{
  m_titleFormat.setForeground(m_foregroundColor);
  m_titleFormat.setBackground(m_backgroundColor);
  m_titleFormat.setFontWeight(QFont::Bold);

  m_textFormat.setForeground(m_foregroundColor);
  m_textFormat.setBackground(m_backgroundColor);
  m_textFormat.setFontWeight(QFont::Normal);
}

void
HoverWidget::HoverHighlighter::highlightBlock(const QString& text)
{
  auto block = currentBlock();
  int line = block.blockNumber();
  for (auto [key, value] : asKeyValueRange(lines)) {
    if (key == line) {
      switch (value) {
        case Title:
          setFormat(0, text.length(), m_titleFormat);
          break;
        case Text:
          setFormat(0, text.length(), m_textFormat);
          break;
      }
    }
  }
}
