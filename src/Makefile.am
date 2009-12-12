bin_PROGRAMS = openpivot
openpivot_SOURCES = \
	app/main.cpp \
	app/OptionParser.cpp \
	frontend/XMLConfigReader.cpp \
	input/CsvReader.cpp \
	output/CsvWriter.cpp \
	openpivotlib/AggregationColumn.cpp \
	openpivotlib/PivotTable.cpp \
	openpivotlib/Accumulator.cpp \
	openpivotlib/Settings.cpp \
	openpivotlib/OpenPivotContext.cpp \
	openpivotlib/Utils.cpp

AM_CXXFLAGS= -I${GOOGLE_SPARSE_INCLUDES} ${XML_CPPFLAGS} ${LIBXML2_INCLUDES}
AM_LDFLAGS=${LIBXML2_LIBS}
