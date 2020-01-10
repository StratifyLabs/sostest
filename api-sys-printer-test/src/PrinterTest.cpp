
#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include "PrinterTest.hpp"

PrinterTest::PrinterTest()
{

}


bool PrinterTest::execute(u32 o_flags){
   Printer p;

   Vector<String> levels = {
      "default", "fatal", "error", "warning", "info", "message", "debug"
   };

   for(const auto & level: levels){
      if( level != "default" ){
         p.set_verbose_level(level);
      }

      printf("start --------------------------\n");
      p.fatal("level %s fatal", level.cstring());
      p.error("level %s error", level.cstring());
      p.warning("level %s warning", level.cstring());
      p.info("level %s info", level.cstring());
      p.message("level %s message", level.cstring());
      p.debug("level %s debug", level.cstring());
      printf("end --------------------------\n");

   }


   Data data(32);

   data.fill<u32>(0x11223344);
   p.set_flags(Printer::print_8 | Printer::print_hex) << data;
   p.set_flags(Printer::print_16 | Printer::print_hex) << data;
   p.set_flags(Printer::print_32 | Printer::print_hex) << data;
   p.set_flags(Printer::print_8 | Printer::print_unsigned) << data;
   p.set_flags(Printer::print_16 | Printer::print_unsigned) << data;
   p.set_flags(Printer::print_32 | Printer::print_unsigned) << data;
   p.set_flags(Printer::print_8 | Printer::print_signed) << data;
   p.set_flags(Printer::print_16 | Printer::print_signed) << data;
   p.set_flags(Printer::print_32 | Printer::print_signed) << data;


   return true;
}
