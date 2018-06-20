CC := g++
FLAGS := -std=c++11 -w -g
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main: $(BUILD_DIR)/Date.o $(BUILD_DIR)/main.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/Service.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $(BUILD_DIR)/Date.o $(BUILD_DIR)/main.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/Service.o -o $@

$(BUILD_DIR)/Date.o: $(SRC_DIR)/Date.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Date.cpp

$(BUILD_DIR)/User.o: $(SRC_DIR)/User.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/User.cpp

$(BUILD_DIR)/Meeting.o: $(SRC_DIR)/Meeting.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Meeting.cpp

$(BUILD_DIR)/Storage.o: $(SRC_DIR)/Storage.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Storage.cpp

$(BUILD_DIR)/Service.o: $(SRC_DIR)/AgendaService.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaService.cpp

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/main.cpp
clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)