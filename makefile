CXX = g++-8
OPT = -std=c++17 -Iinclude

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
	src/nstools.cpp     \
	src/mystore.cpp     \
	src/menu.cpp        \
	src/notes.cpp       \
	src/textbox.cpp     \
	src/todomenu.cpp    \
	src/todonotes.cpp   \
	src/storemenu.cpp   \
	src/storenotes.cpp


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
