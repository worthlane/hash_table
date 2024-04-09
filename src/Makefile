CXX = g++-12
ASM = nasm
EXECUTABLE = table
CXXFLAGS =  -D _DEBUG -O3 -mavx2 -D NDEBUG -ggdb3 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts       \
			-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal  \
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline          \
			-Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked            \
			-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo  \
			-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn                \
			-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default      \
			-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast           \
			-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing   \
			-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation    \
			-fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192         \
			-Wstack-usage=8192 -fPIE -Werror=vla

HOME = $(shell pwd)
CXXFLAGS += -I $(HOME)

BUILD_DIR = build/bin
OBJECTS_DIR = build

SOURCES = main.cpp

OBJECTS = $(SOURCES:%.cpp=$(OBJECTS_DIR)/%.o)

HASHTABLE_DIR = hashtable
HASHTABLE_SRC = hashtable.cpp hash.cpp
HASHTABLE_ASM = findkey.s
HASHTABLE_OBJ = $(HASHTABLE_SRC:%.cpp=$(OBJECTS_DIR)/%.o)
HASHTABLE_OBJ += $(HASHTABLE_ASM:%.s=$(OBJECTS_DIR)/%.o)

TESTING_DIR = testing
TESTING_SRC = testing.cpp
TESTING_ASM = tick.s
TESTING_OBJ = $(TESTING_SRC:%.cpp=$(OBJECTS_DIR)/%.o)
TESTING_OBJ += $(TESTING_ASM:%.s=$(OBJECTS_DIR)/%.o)

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HASHTABLE_OBJ) $(TESTING_OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(OBJECTS_DIR)/%.o : $(HASHTABLE_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(OBJECTS_DIR)/%.o : $(HASHTABLE_DIR)/%.s
	$(ASM) -f elf64 $^ -o $@

$(OBJECTS_DIR)/%.o : $(TESTING_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(OBJECTS_DIR)/%.o : $(TESTING_DIR)/%.s
	$(ASM) -f elf64 $^ -o $@

$(OBJECTS_DIR)/%.o : %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

.PHONY: clean makedirs

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS_DIR)/*.o

makedirs:
	mkdir -p $(BUILD_DIR)
