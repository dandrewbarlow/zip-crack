CXX = g++ -std=c++17
CXXFLAGS = -Wall -g -Og `pkg-config --libs --cflags libzip`
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC=crack

go:
	go build crack.go

test:
	time ./crack -f secret.txt.zip > /dev/null
	time python3 crack.py -s > /dev/null


# C++ stuff
# Abandoned for me, maybe useful for aliens studying our culture in a thousand years?
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)

