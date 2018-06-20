CC := g++
FLAGS := -std=c++11 -w 
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/Add.o $(BUILD_DIR)/Subtract.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $(BUILD_DIR)/main.o $(BUILD_DIR)/Add.o $(BUILD_DIR)/Subtract.o -o $@

$(BUILD_DIR)/Add.o: $(SRC_DIR)/functionAdd.cpp $(INC_DIR)/functionAdd.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/functionAdd.cpp

$(BUILD_DIR)/Subtract.o: $(SRC_DIR)/functionSubtract.cpp $(INC_DIR)/functionSubtract.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/functionSubtract.cpp

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/functionSubtract.hpp $(INC_DIR)/functionAdd.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/main.cpp


clean:
	@rm -f $(BUILD_DIR)/Add.o
	@rm -f $(BUILD_DIR)/Subtract.o
	@rm -f $(BUILD_DIR)/main.o
	@rm -f $(BIN_DIR)/main