#include "epubedit.h"
#include "config.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

EPubEdit::EPubEdit(PConfig config, QWidget* parent)
  : QTextEdit(parent)
{
}

QTextDocument*
EPubEdit::document()
{
  return QTextEdit::document();
}

QTextCursor
EPubEdit::currentCursor()
{
  return QTextEdit::textCursor();
}

const QString&
EPubEdit::href() const
{
  return m_href;
}

void
EPubEdit::loadHref(const QString& href)
{
  m_href = href;
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    setHtml(text);
  }
}

void
EPubEdit::setText(const QString& text)
{
  setHtml(text);
}

void
EPubEdit::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus())
    emit gotFocus(this);

  QTextEdit::focusInEvent(event);
}

void
EPubEdit::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus())
    emit lostFocus(this);

  QTextEdit::focusOutEvent(event);
}

void
EPubEdit::mousePressEvent(QMouseEvent* event)
{
  emit mouseClicked(event->pos());
  QTextEdit::mousePressEvent(event);
}

void
EPubEdit::mouseReleaseEvent(QMouseEvent* event)
{
  // TODO possibly remove
  setFocus();
  QTextEdit::mouseReleaseEvent(event);
}
