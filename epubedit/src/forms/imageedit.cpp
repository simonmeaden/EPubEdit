#include "imageedit.h"
#include "config.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

ImageEdit::ImageEdit(PConfig config, QWidget* parent)
  : QWidget{ parent }
  , m_config(config)
{
}

const QString &ImageEdit::href() const
{
  return m_href;
}

void ImageEdit::loadHref(const QString &href)
{
  // TODO load image file
}
