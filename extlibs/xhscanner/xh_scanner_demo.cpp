// xh_scanner_demo.cpp : Defines the entry point for the console application.
//

#include "stdio.h"

#include "xh_scanner.h"


struct str_istream: public markup::instream
{
  const char* p;
  const char* end;

  str_istream(const char* src): p(src), end(src + strlen(src)) {}
  virtual wchar_t get_char() { return p < end? *p++: 0; }
};


int main(int argc, char* argv[])
{
  str_istream si(
    "<html><body><p align=right dir='rtl'>Begin &amp; back</p>"
    "<a href=http://terrainformatica.com/index.php?a=1&b=2>link</a></body></html>");
  markup::scanner sc(si);
  bool in_text = false;
  while(true)
  {
    int t = sc.get_token();
    switch(t)
    {
      case markup::scanner::TT_ERROR:
        printf("ERROR\n");
        break;
      case markup::scanner::TT_EOF:
        printf("EOF\n");
        goto FINISH;
      case markup::scanner::TT_TAG_START:
        printf("TAG START:%s\n", sc.get_tag_name());
        break;
      case markup::scanner::TT_TAG_END:
        printf("TAG END:%s\n", sc.get_tag_name());
        break;
      case markup::scanner::TT_ATTR:
        printf("\tATTR:%s=%S\n", sc.get_attr_name(), sc.get_value());
        break;
      case markup::scanner::TT_WORD: 
      case markup::scanner::TT_SPACE:
        printf("{%S}\n", sc.get_value());
        break;
    }
  }
FINISH:
  printf("--------------------------\n");
  return 0;
}


