CC := gcc
LD := gcc
TARGET := stree

SRC := src
OBJ := obj

OBJECTS := $(OBJ)/stree.o
DEPS := $(OBJECTS:.o=.d)

CFLAGS  :=
LDFLAGS := -lc

.PHONY: all
all: $(TARGET)

ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif

$(TARGET): $(OBJECTS)
	@echo [LD] -o $@ $^
	@$(LD) $(LDFLAGS) -o $@ $^

$(DEPS) $(OBJECTS): | $(OBJ)/

$(OBJ)/%.o: $(SRC)/%.c
	@echo [CC] -o $@ $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.d: $(SRC)/%.c
	@echo [DEP] $<
	@set -e; rm -f $@; \
	$(CC) -MM -MF $@ $(CFLAGS) $<;\
	sed -i 's,\($*\)\.o[\s:]*,\1.o $@: ,g' $@;

%/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(OBJ) $(TARGET)

