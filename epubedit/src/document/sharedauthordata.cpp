#include "document/sharedauthordata.h"

SharedAuthorData::SharedAuthorData()
  : m_uid(-1)
{}

SharedAuthorData::SharedAuthorData(const SharedAuthorData& other)
  : QSharedData(other)
  , m_uid(other.m_uid)
  , m_display_name(other.m_display_name)
  , m_forename(other.m_forename)
  , m_middlenames(other.m_middlenames)
  , m_surname(other.m_surname)
  , m_file_as(other.m_file_as)
  , m_surname_last(other.m_surname_last)
  , m_word_list(other.m_word_list)
  , m_website(other.m_website)
  , m_wikipedia(other.m_wikipedia)
  , m_books(other.m_books)
  , m_pixmap(other.m_pixmap)
{}

SharedAuthorData::~SharedAuthorData() {}
