#include <iostream>
#include "frontend/XMLConfigReader.h"
#include "input/CsvReader.h"
#include "output/CsvWriter.h"
#include "openpivotlib/OpenPivotContext.h"
#include "openpivotlib/Settings.h"
#include "openpivotlib/PivotTable.h"
#include "OptionParser.h"
using namespace std;

int main (int argc, char * const argv[]) {
  int EXIT_STATUS = 0;
  openpivot::OptionParser parser;
  parser.setUsageString("openpivot -p <CONFIGFILE> -o <OUTPUTFILE> <INPUTFILE>");
  parser.addArgumentOption('o', "output", "name of the output file");
  parser.addArgumentOption('p', "params", "name of param file");

  parser.parse(argc, argv);
  if (parser.getArguments().size() == 0 
      || parser.getArgumentOptionVal("output") == std::string()
      || parser.getArgumentOptionVal("params") == std::string()) {
    std::cerr << "Wrong arguments" << std::endl;
    parser.usage();
  }
  try
  {
  
    openpivot::OpenPivotContext context;
    openpivot::XMLConfigReader xmlReader;
    xmlReader.setContext(&context);
    std::string configFile = parser.getArgumentOptionVal("params");
    xmlReader.setInputFile(configFile);
    xmlReader.read();
    
    openpivot::CsvReader reader;
    std::string inputFile = parser.getArguments()[0];
    reader.setInput(inputFile.c_str());
    reader.setContext(&context);
    reader.read();
    reader.clean();
    
    openpivot::CsvWriter writer;
    std::string writerFile = parser.getArgumentOptionVal("output");
    writer.setOutput(writerFile.c_str());
    writer.setContext(&context);
    writer.dump();
  }
  catch (openpivot::Exception & exc)
  {
    std::cerr << "Error : " << exc.getMessage() << std::endl;
    EXIT_STATUS = 1;
  }
  return EXIT_STATUS;
}
