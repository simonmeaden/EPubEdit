#include "codeedit.h"
#include "config.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

CodeEdit::CodeEdit(PConfig config, QWidget* parent)
  : QPlainTextEdit(parent)
  , m_config(config)
{
}

QTextDocument*
CodeEdit::document()
{
  return QPlainTextEdit::document();
}

QTextCursor
CodeEdit::currentCursor()
{
  return QPlainTextEdit::textCursor();
}

const QString&
CodeEdit::href() const
{
  return m_href;
}

void
CodeEdit::loadHref(const QString& href)
{
  m_href = href;
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    setPlainText(text);
  }
}

void
CodeEdit::setText(const QString& text)
{
  setPlainText(text);
}

void
CodeEdit::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus())
    emit gotFocus(this);

  QPlainTextEdit::focusInEvent(event);
}

void
CodeEdit::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus())
    emit lostFocus(this);

  QPlainTextEdit::focusOutEvent(event);
}

void
CodeEdit::mousePressEvent(QMouseEvent* event)
{
  emit mouseClicked(event->pos());
  QPlainTextEdit::mousePressEvent(event);
}

void
CodeEdit::mouseReleaseEvent(QMouseEvent* event)
{
  // TODO possibly remove
  setFocus();
  QPlainTextEdit::mouseReleaseEvent(event);
}
