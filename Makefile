SOURCE = main.cpp
PROGRAM = stereogram
SOURCE_V = viewer.cpp
PROGRAM_V = viewer

all: $(PROGRAM) $(PROGRAM_V)

$(PROGRAM): $(SOURCE)
	$(CXX) -W -Wall $(SOURCE) -o $(PROGRAM)

$(PROGRAM_V): $(SOURCE_V)
	$(CXX) -W -Wall $(SOURCE_V) -o $(PROGRAM_V)

clean:
	rm -f $(PROGRAM) $(PROGRAM_V)
