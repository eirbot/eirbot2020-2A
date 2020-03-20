SUFFIX := $(shell python3-config --extension-suffix)
PY_LIBS := $(shell python3 -m pybind11 --includes)

TEST = no

# project name (generate executable with this name)
ifeq ($(TEST),yes)
TARGET   = castar
else
TARGET   = castar$(SUFFIX)
endif
CC       = g++
LINKER   = g++
# compiling flags here
ifeq ($(TEST),yes)
	CFLAGS   = -O3 -Wall -Werror -g -std=c++11 -DTEST
	LFLAGS   = -Wall
else
	CFLAGS   = -O3 -Wall -Werror -shared -g -std=c++11 -fPIC $(PY_LIBS)
	LFLAGS   = -Wall -shared -fPIC $(PY_LIBS)
endif


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