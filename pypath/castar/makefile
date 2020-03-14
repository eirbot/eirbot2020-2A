SUFFIX := $(shell python3-config --extension-suffix)
PY_LIBS := $(shell python3 -m pybind11 --includes)
# project name (generate executable with this name)
TARGET   = castar$(SUFFIX)

CC       = g++
# compiling flags here
CFLAGS   = -O3 -Wall -Werror -shared -g -std=c++11 -fPIC $(PY_LIBS)

LINKER   = g++
# linking flags here
LFLAGS   = -Wall -shared -fPIC $(PY_LIBS)

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"
    
.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"