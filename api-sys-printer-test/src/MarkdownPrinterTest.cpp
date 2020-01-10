#include "MarkdownPrinterTest.hpp"

MarkdownPrinterTest::MarkdownPrinterTest()
{

}

bool MarkdownPrinterTest::execute(u32 o_flags){

   MarkdownPrinter p;


   p.open_header("Header 1");
   p.open_header("Header 2");
   p.open_header("Header 3");
   p.open_header("Header 4");
   p.open_header("Header 5");
   p.open_header("Header 6");
   p.close_header();
   p.close_header();
   p.close_header();
   p.close_header();
   p.close_header();
   p.close_header();

   p.open_header("Header 1");
   p.open_paragraph();
   p << "This is in a paragraph.";
   p << "This is in a paragraph.";
   p << "This is in a paragraph.";
   p << "This is in a paragraph.";
   p.close_paragraph();

   p.open_list(MarkdownPrinter::list_unordered);
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p.close_list();

   p.open_header("list");
   p.open_list(MarkdownPrinter::list_unordered);
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p << "This is in an unordered list.";
   p.close_list();
   p.open_list(MarkdownPrinter::list_ordered);
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p.close_list();

   p.open_list(MarkdownPrinter::list_ordered);
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p << "This is in an ordered list.";
   p.close_list();

   p.open_blockquote();
   p << "This is in a blockquote.";
   p << "This is in a blockquote.";
   p << "This is in a blockquote.";
   p.open_blockquote();
   p << "This is in a double blockquote.";
   p << "This is in a double blockquote.";
   p.close_blockquote();

   p.close_header();

   return true;
}
