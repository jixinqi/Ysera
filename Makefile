CXX                            = clang++ -std=c++11
CXXFLAGS                       = -Wall
LDFLAGS                        = -lboost_system -pthread

SRC_DIR                        = ./src:./src/ys
OBJ_DIR                        = ./obj
BIN_DIR                        = ./bin

vpath %.cpp                    $(SRC_DIR)
.PHONY                         : clean

default                        : $(BIN_DIR)/Ysera


$(BIN_DIR)/Ysera               : $(OBJ_DIR)/main.o \
                                 $(OBJ_DIR)/ys_listener.o \
                                 $(OBJ_DIR)/ys_server.o \
                                 $(OBJ_DIR)/ys_session.o \
                                 $(OBJ_DIR)/ys_response.o \
                                 $(OBJ_DIR)/ys_request.o \
                                 $(OBJ_DIR)/ys_router.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o               : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean :
	-rm -rf $(OBJ_DIR)/*
	-rm -rf $(BIN_DIR)/*

