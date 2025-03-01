CXX = g++
SOURCES = rewardCoverage.cpp
OBJECTS = pso.o
EXECUTABLE = pso

.PHONY: all clean

all: $(EXECUTABLE)
	@rm -f $(OBJECTS)

CXXFLAGS = -std=c++17 -O3

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

pso.o: rewardCoverage.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
