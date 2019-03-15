PROJECT = vector
BUILD   = g++
MAKEDIR = mkdir
REMOVE  = rm -rf
OUT_DIR = bin
OUTPUT  = ./$(OUT_DIR)/$(PROJECT)

DBGFLAGS = -g
CFLAGS  = -flto -std=c++14
#-I.\include
LDFLAGS = -flto
#-L. -l:pdcurses.a -s

EXCLUDE = catch.cpp
TARGETS = main.cpp tests.cpp benchmarks.cpp
ALLSRCS = $(wildcard *.cpp)
SOURCES = $(filter-out $(TARGETS) $(EXCLUDE),$(ALLSRCS))
OBJECTS = $(SOURCES:.cpp=.o)

.SECONDEXPANSION:
.PHONY: all cov test bench debug build list clean

all: build clean

list:
	@printf "Targets: $(TARGETS)\nSources: $(SOURCES)\nObjects: $(OBJECTS)\n"

# cov: CFLAGS = -fprofile-arcs -ftest-coverage
# cov: LDFLAGS += -fprofile-arcs -ftest-coverage
cov: CFLAGS = -O0 -g -coverage -fno-inline
cov: LDFLAGS += -coverage
cov: test
	@$(OUTPUT)
	@gcov tests.cpp > "coverage.txt"

# test: CFLAGS += $(DBGFLAGS)
test: SOURCES += catch.cpp
test: $(OUT_DIR) catch.o tests

debug: CFLAGS += $(DBGFLAGS)
debug: build

build: $(OUT_DIR) main

bench: $(OUT_DIR) benchmarks


$(OUT_DIR):
	$(MAKEDIR) $(OUT_DIR)

$(TARGETS:.cpp=): $(OBJECTS) $$(@).o
	$(BUILD) -o $(OUTPUT) $(OBJECTS) $(@).o $(LDFLAGS)
# $(BUILD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(BUILD) -o $@ -c $< $(CFLAGS)

clean:
	$(REMOVE) *.o *.gc*
