CXX = g++-8
OPT = -std=c++17

INC_FILES =      \
	nstools.h    \
	mystore.h    \
	menu.h       \
	notes.h      \
	textbox.h    \
	todomenu.h   \
	todonotes.h  \
	storemenu.h  \
	storenotes.h

SRC_FILES =         \
	nstools.cpp     \
	mystore.cpp     \
	menu.cpp        \
	notes.cpp       \
	textbox.cpp     \
	todomenu.cpp    \
	todonotes.cpp   \
	storemenu.cpp   \
	storenotes.cpp


all:
	$(CXX) $(OPT) -o mystore-menu main.cpp $(SRC_FILES) -lncursesw -lmenuw -lstdc++fs

#.cpp.o:
#	$(CXX) $(OPT) $^ -c -o $@
#
#main.o: main.cpp
#	$(CXX) $(OPT) -c -o main.o main.cpp
#
#store: main.o 
#	$(CXX) $(OPT) -o mystore-menu main.o -lncursesw -lmenuw -lstdc++fs
