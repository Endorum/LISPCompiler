CXX := c++
CXXFLAGS := -Wall -std=c++20

# Contain path for any includes (headers)
# Depending on your platform: Include a path to boost, on linux should be 
# /usr/local/include, on mac could be /opt/homebrew/include
INCLUDES := -I./SFML/include -I/opt/homebrew/include

# Contains libraries we need to (-L is directory search path, -l is lib)
LDFLAGS := -L/usr/local/lib -L./SFML/lib -L/opt/homebrew/lib
LDLIBS := -lsfml-system -lsfml-window -lsfml-graphics

SRCDIR := ./src
BUILDDIR := ./build
VPATH := src

# search all .cpp files and add corresponding .o-files to GAME_OBJECTS
SRCFILES := $(shell find $(SRCDIR) -name '*.cpp')
GAME_OBJECTS := $(SRCFILES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
DEPENDENCIES := $(SRCFILES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.d)
    
game: $(GAME_OBJECTS) 
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)
	
-include $(DEPENDENCIES)

# On a Mac: "brew install sfml" will install the library.
getSFML:
	@echo "Downloading SFML"
	curl https://www.sfml-dev.org/files/SFML-3.0.0-linux-gcc-64-bit.tar.gz --output SFML.tar.gz
	@echo "Unpacking the lib"
	tar -xf SFML.tar.gz
	@echo "Removing download"
	rm SFML.tar.gz
	@echo "Renaming folder"
	mv SFML-3.0.0 SFML
	@echo "Finished!"

tests: $(GAME_OBJECTS) test_call.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)
    
$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	test ! -f game || rm game
	test ! -f tests || rm tests
	rm $(GAME_OBJECTS) $(DEPENDENCIES)
