CC = g++
CPPFILES = $(shell find src/ -type f -name '*.cpp')
CFILES = $(shell find src/ -type f -name '*.c')
OFILES = $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

TARGET = build/gitman

.PHONY = clean all

$(TARGET): $(OFILES)
	@echo [LD] $(TARGET)
	@$(CC) $(OFILES) -o $@

%.o: %.cpp
	@echo [CC] $@
	@$(CC) -Isrc/include/ -c $< -o $@

%.o: %.c
	@echo [CC] $@
	@$gcc -Isrc/include -c $< -o $@

clean:
	@echo [CLEAN]
	@rm $(OFILES) $(TARGET)

all: clean $(TARGET)

