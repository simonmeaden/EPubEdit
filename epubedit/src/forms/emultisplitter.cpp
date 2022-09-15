#include "emultisplitter.h"
#include "epubeditor.h"

EMultiSplitter::EMultiSplitter(PConfig config, QWidget* parent)
  : MultiSplitter(parent)
  , m_config(config)
{
  createEditor();
}

QWidget*
EMultiSplitter::createSplit(Qt::Orientation orientation,
                            AbstractEPubEditor* editor)
{
  connect(editor,
          &AbstractEPubEditor::sendLogMessage,
          this,
          &EMultiSplitter::sendLogMessage);
  return MultiSplitter::createSplit(orientation, editor);
}

EPubEditor*
EMultiSplitter::createEditor()
{
  auto current = currentEditor();
  EPubEditor* editor=nullptr;

  if (current)
    editor = current->clone(dynamic_cast<IEPubEditor*>(editor));
  else
    editor = new EPubEditor(m_config, this);

  setCurrentWidget(editor);

  connect(
    editor, &EPubEditor::gotFocus, this, &MultiSplitter::setCurrentWidget);
  connect(editor, &EPubEditor::splitWidget, this, &EMultiSplitter::createSplit);
  connect(editor,
          &EPubEditor::splitToWindow,
          this,
          &MultiSplitter::openToNewWindow);
  return editor;
}

EPubEditor*
EMultiSplitter::currentEditor()
{
  return qobject_cast<EPubEditor*>(m_currentWidget);
}

QList<AbstractEPubEditor*>
EMultiSplitter::widgets()
{
  QList<AbstractEPubEditor*> list;
  for (auto widget :m_widgetMap.keys() ) {
    auto aee = qobject_cast<AbstractEPubEditor*>(widget);
    if (aee) {
      list.append(aee);
    }
  }
  return list;
}

void
EMultiSplitter::createSplitterData(QSplitter* splitter)
{
  for (auto i = 0; i < splitter->count(); i++) {
    auto w = splitter->widget(i);
    auto s = qobject_cast<QSplitter*>(w);
    if (s) {

    } else {
      auto e = qobject_cast<AbstractEPubEditor*>(w);
      if (e) {
//      auto href = e->editor();
      }
    }
  }
}

