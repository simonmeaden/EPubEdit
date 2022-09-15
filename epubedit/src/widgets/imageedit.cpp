#include "widgets/imageedit.h"
#include "config.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

ImageEdit::ImageEdit(PConfig config, QWidget* parent)
  : QWidget{ parent }
  , m_config(config)
{
}

const QString
ImageEdit::href() const
{
  return m_href;
}

void
ImageEdit::loadHref(const QString& href)
{
  // TODO load image file
}

void
ImageEdit::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus())
    emit gotFocus(this);

  QWidget::focusInEvent(event);
}

void
ImageEdit::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus())
    emit lostFocus(this);

  QWidget::focusOutEvent(event);
}

void ImageEdit::mousePressEvent(QMouseEvent *event)
{
  emit mouseClicked(event->pos());
  QWidget::mousePressEvent(event);
}

void ImageEdit::mouseReleaseEvent(QMouseEvent *event)
{
  setFocus();
  QWidget::mouseReleaseEvent(event);
}
