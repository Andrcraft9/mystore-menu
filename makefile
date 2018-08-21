all:
	g++-8 -std=c++17 -o mystore-menu main.cpp menu.h textbox.h mystore.h -lncursesw -lmenuw -lstdc++fs
	
