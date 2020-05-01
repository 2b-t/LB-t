# Makefile
# Tobit Flatscher - github.com/2b-t (2020)

# Compiler settings (alternatively: export COMPILER=)
COMPILER = ICC

# Define directories
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

# Define source files to be compiled
SOURCES  = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
INCLUDES = $(wildcard $(SRCDIR)/*.hpp) $(wildcard $(SRCDIR)/*/*.hpp)
OBJECTS  = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
PROGRAM	 = main.$(COMPILER)

# compiler flags
WARNINGS   = -Wall -pedantic -Wextra -Weffc++ -Woverloaded-virtual  -Wfloat-equal -Wshadow -Wredundant-decls -Winline -fmax-errors=1
CXXFLAGS  += -O3 -std=c++17 -flto -funroll-all-loops -finline-functions -mavx2 -march=native -DNDEBUG
LINKFLAGS += -O3 -flto

# compiler settings
ifeq ($(COMPILER),ICC)
	# Intel compiler ICC
	CXX        = icpc
	LINKER     = icpc
	CXXFLAGS  += -fopenmp
	LINKFLAGS += -qopenmp
	COMPILER   = ICC
else
	# Gnu Compiler GCC
	CXX        = g++
	LINKER     = g++
	CXXFLAGS  += -fopenmp
	LINKFLAGS += -lgomp
	COMPILER   = GCC
endif

# Make commands
default: $(BINDIR)/$(PROGRAM)

$(BINDIR)/$(PROGRAM): $(OBJECTS)
	@mkdir -p $(@D)
	$(LINKER)  $(OBJECTS)  $(LINKFLAGS) -o $@
	@echo "Linking complete!"
	
$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	@rm -f $(BINDIR)/$(PROGRAM) $(OBJECTS)

run: clean $(BINDIR)/$(PROGRAM)
	./$(BINDIR)/$(PROGRAM)

doc:
	doxygen Doxyfile

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<

info:
	@echo "Show optimization flags"
	$(CXX) --version
	@echo $(CXXFLAGS)
