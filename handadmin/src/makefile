# compiler
COMPILER = g++

# flags
CFLAGS = -c -Wall
LMFLAGS = -lm

ifeq "$(OS)"  "Windows_NT"

# folders
BIN_FOLDER = ..\bin_win
OBJS_FOLDER = ..\objs_win
LIB_FOLDER = ..\..\qbAPI\lib_win

else

# folders
BIN_FOLDER = ../bin_unix
OBJS_FOLDER = ../objs_unix
LIB_FOLDER = ../../qbAPI/lib_unix

endif



all:handmove handmoveadmin handparam hand_demo


handmove:$(OBJS_FOLDER)/handmove.o $(BIN_FOLDER)
	$(COMPILER) $(OBJS_FOLDER)/handmove.o      $(LIB_FOLDER)/libqbmove_comm.a -o $(BIN_FOLDER)/handmove      $(LMFLAGS)

handmoveadmin:$(OBJS_FOLDER)/handmoveadmin.o $(BIN_FOLDER)
	$(COMPILER) $(OBJS_FOLDER)/handmoveadmin.o $(LIB_FOLDER)/libqbmove_comm.a -o $(BIN_FOLDER)/handmoveadmin $(LMFLAGS)

handparam:$(OBJS_FOLDER)/handparam.o $(BIN_FOLDER)
	$(COMPILER) $(OBJS_FOLDER)/handparam.o     $(LIB_FOLDER)/libqbmove_comm.a -o $(BIN_FOLDER)/handparam     $(LMFLAGS)

hand_demo:$(OBJS_FOLDER)/hand_demo.o $(BIN_FOLDER)
	$(COMPILER) $(OBJS_FOLDER)/hand_demo.o     $(LIB_FOLDER)/libqbmove_comm.a -o $(BIN_FOLDER)/hand_demo     $(LMFLAGS)




$(OBJS_FOLDER)/handmove.o:handmove.c $(OBJS_FOLDER)
	$(COMPILER) $(CFLAGS) $(PFLAGS) handmove.c -o $(OBJS_FOLDER)/handmove.o

$(OBJS_FOLDER)/handmoveadmin.o:handmoveadmin.c $(OBJS_FOLDER)
	$(COMPILER) $(CFLAGS) handmoveadmin.c -o $(OBJS_FOLDER)/handmoveadmin.o

$(OBJS_FOLDER)/handparam.o:handparam.c $(OBJS_FOLDER)
	$(COMPILER) $(CFLAGS) handparam.c -o $(OBJS_FOLDER)/handparam.o

$(OBJS_FOLDER)/hand_demo.o:hand_demo.c $(OBJS_FOLDER)
	$(COMPILER) $(CFLAGS) $(PFLAGS) hand_demo.c -o $(OBJS_FOLDER)/hand_demo.o



clean:
ifeq "$(OS)"  "Windows_NT"
	rmdir /Q /S $(OBJS_FOLDER)
	rmdir /Q /S $(BIN_FOLDER)
else
	rm -rf $(BIN_FOLDER) $(OBJS_FOLDER)
endif

$(OBJS_FOLDER):
	mkdir $(OBJS_FOLDER)

$(BIN_FOLDER):
	mkdir $(BIN_FOLDER)
