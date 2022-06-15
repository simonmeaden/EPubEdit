#include "document/sharedbookdata.h"

SharedBookData::SharedBookData()
  : m_uid(-1)
  , m_seriesIndex(0)
  , m_currentSpineLineNo(-1)
  , m_modified(false)
  , m_readPosition(0)
  , m_showContentsList(false)
{}

SharedBookData::SharedBookData(const SharedBookData &other)
  : QSharedData(other)
  , m_uid(other.m_uid)
  , m_filename(other.m_filename)
  , m_title(other.m_title)
  , m_series(other.m_series)
  , m_seriesIndex(other.m_seriesIndex)
  , m_currentSpineId(other.m_currentSpineId)
  , m_currentSpineLineNo(other.m_currentSpineLineNo)
  , m_dateLastRead(other.m_dateLastRead)
  , m_readPosition(other.m_readPosition)
  , m_showContentsList(other.m_showContentsList)
{}

SharedBookData::~SharedBookData() {}
