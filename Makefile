CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra

IMGUI_DIR := imgui

SIM_SRC := $(wildcard src/sim/*.cpp) \
  $(wildcard src/sim/parts/*.cpp) \
  $(wildcard src/sim/parts/ic/*.cpp) \
  $(wildcard src/sim/util/*.cpp)

IMGUI_SRC := $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/imgui_demo.cpp \
  $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

SRC := src/main.cpp $(SIM_SRC) $(IMGUI_SRC)

INCLUDES := -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -Isrc

CXXFLAGS += $(shell pkg-config --cflags glfw3)
LIBS     := $(shell pkg-config --libs glfw3) -lopengl32 -lgdi32

TARGET := app.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

clean:
	rm -f $(TARGET)
