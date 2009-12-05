#include <iostream>
#include "frontend/XMLConfigReader.h"
#include "input/CsvReader.h"
#include "output/CsvWriter.h"
#include "openpivotlib/OpenPivotContext.h"
#include "openpivotlib/Settings.h"
#include "openpivotlib/PivotTable.h"
int main (int argc, char * const argv[]) {
  int EXIT_STATUS = 0;
  op::OpenPivotContext context;
  op::XMLConfigReader xmlReader;
  xmlReader.setContext(&context);
  xmlReader.setInputFile(argv[1]);
  try
  {
    xmlReader.read();
    op::CsvReader reader;
    reader.setInput(argv[2]);
    reader.setContext(&context);
    reader.read();
    reader.clean();
    
    op::CsvWriter writer;
    writer.setOutput(argv[3]);
    writer.setContext(&context);
    writer.dump();
  }
  catch (op::Exception & exc)
  {
    std::cerr << "Error : " << exc.getMessage() << std::endl;
    EXIT_STATUS = 1;
  }
  return EXIT_STATUS;
}
