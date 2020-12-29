CXX = g++ -std=c++17
CXXFLAGS = -Wall -g -Og `pkg-config --libs --cflags libzip`
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC=crack

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

class:
	@curl -s https://raw.githubusercontent.com/dandrewbarlow/cpp-template/class/my_class.cpp > my_class.cpp
	@curl -s https://raw.githubusercontent.com/dandrewbarlow/cpp-template/class/my_class.h > my_class.h
	@read -p "Enter class name: " className; \
		sed -i "" -e "s/my_class/$$className/g" my_class.h; \
		sed -i "" -e "s/my_class/$$className/g" my_class.cpp; \
		mv my_class.cpp $$className.cpp; \
		mv my_class.h $$className.h

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)

test:
	time python3 crack.py
