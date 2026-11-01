CXX      ?= g++
STD      := -std=c++17
WARN     := -Wall -Wextra -Wpedantic
INPUT    = entrada.in

ifeq ($(SANITIZE),1)
  OPT    := -O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer
  BIN    := programa_debug
  OBJDIR := obj/debug
else
  OPT    := -O2
  BIN    := programa
  OBJDIR := obj/release
endif

SRC      := $(wildcard src/*.cpp)
OBJ      := $(SRC:src/%.cpp=$(OBJDIR)/%.o)
DEP      := $(OBJ:.o=.d)
CPPFLAGS := -Iinclude -MMD -MP
CXXFLAGS := $(STD) $(WARN) $(OPT)

TESTS    := $(sort $(wildcard tests/*.in) $(wildcard ../../testes/*.in))

.PHONY: build run test debug clean

build: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run:
	@$(MAKE) --no-print-directory -s build >&2
	@./$(BIN) $(INPUT)

debug:
	@$(MAKE) --no-print-directory SANITIZE=1 build

test: build
	@fail=0; total=0; \
	for f in $(TESTS); do \
	  exp="$${f%.in}.out"; \
	  [ -f "$$exp" ] || continue; \
	  total=$$((total+1)); \
	  if ./$(BIN) "$$f" | cmp -s - "$$exp"; then echo "OK     $$f"; \
	  else echo "FAILED $$f"; fail=$$((fail+1)); fi; \
	done; \
	echo "$$((total-fail))/$$total tests passed"; \
	[ $$fail -eq 0 ]

clean:
	rm -rf obj programa programa_debug

-include $(DEP)
