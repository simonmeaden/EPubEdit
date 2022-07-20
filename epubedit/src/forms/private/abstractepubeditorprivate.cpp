#include "abstractepubeditorprivate.h"
#include "config.h"
#include "forms/codeedit.h"
#include "forms/epubedit.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

AbstractEPubEditorPrivate::AbstractEPubEditorPrivate(PConfig config,
                                                     AbstractEPubEditor* parent)
  : HeaderWidgetPrivate(parent)
  , m_config(config)
{
}

const QString&
AbstractEPubEditorPrivate::href() const
{
  return m_href;
}

void
AbstractEPubEditorPrivate::loadHref(const QString& href)
{
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    if (m_editor) {
      auto e = qobject_cast<EPubEdit*>(m_editor);
      if (e) { // is a text editor
        e->setHtml(text);
        m_href = href;
      } else { // must be a code editor
        auto c = qobject_cast<CodeEdit*>(m_editor);
        if (c) {
          c->setPlainText(text);
        }
      }
    }
  }
}

QTextDocument* AbstractEPubEditorPrivate::document() {
  auto e = qobject_cast<EPubEdit*>(m_editor);
  if (e)
  return e->document();
  else {
    auto c = qobject_cast<CodeEdit*>(m_editor);
    if (c) {
      return c->document();
    }
  }
  return nullptr;
}

QTextCursor
AbstractEPubEditorPrivate::currentCursor()
{
  auto e = qobject_cast<EPubEdit*>(m_editor);
  if (e)
  return e->textCursor();
  else {
    auto c = qobject_cast<CodeEdit*>(m_editor);
    if (c) {
      return c->textCursor();
    }
  }
  return QTextCursor();
}
