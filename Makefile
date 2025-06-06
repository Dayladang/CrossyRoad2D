# Compiler
CXX       := C:/msys64/ucrt64/bin/g++.exe

# Compiler flags và Linker flags
CXXFLAGS   := -std=c++17 -Wall -Wextra -g -Isrc/include
LDLIBS  := -Lsrc/lib \
             -lmingw32 -lSDL2main -lSDL2 \
             -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Danh sách tất cả .cpp (tự động lấy các file trong ECS/)
SRC      := $(wildcard *.cpp) $(wildcard ECS/*.cpp)
OBJ      := $(SRC:.cpp=.o)
EXEC     := CrossyRoad.exe

.PHONY: all clean debug

all: $(EXEC)

debug:
	@echo "CXX       = $(CXX)"
	@echo "CXXFLAGS   = $(CXXFLAGS)"
	@echo "LDLIBS  = $(LDLIBS)"
	@echo "SRC      = $(SRC)"
	@echo "OBJ      = $(OBJ)"
	@echo "EXEC     = $(EXEC)"

# Link tất cả .o thành .exe
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)
	@echo "Built $@"
	-del /Q $(subst /,\,$(OBJ))

# Biên dịch từng .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build và chạy chương trình
b: $(EXEC)
	$(EXEC)

# Chạy chương trình
run:
	@if exist $(EXEC) ( \
		$(EXEC) \
	) else ( \
		echo "$(EXEC) does not exist. Run 'make' first." \
	)

# Xóa các file .o và .exe
clean:
	@echo "Cleaning build artifacts..."
	-del /Q $(subst /,\,$(OBJ)) $(EXEC)

# Dừng chương trình
kill:
	@echo "Stopping program..."
	@taskkill /F /IM $(EXEC) >nul 2>&1 || echo "No process found."
	@echo "Program stopped."