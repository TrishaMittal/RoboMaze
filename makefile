INC_PATH = -I/usr/local/include/player-3.0
LIB_PATH = -L/usr/local/lib64 -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lplayercommon
ReverseMaze: ReverseMaze.c
	gcc -o ReverseMaze $(INC_PATH) ReverseMaze.c $(LIB_PATH)
clean ReverseMaze:
	rm -rf ReverseMaze

Maze: Maze.c
	gcc -o Maze $(INC_PATH) Maze.c $(LIB_PATH)
clean Maze:
	rm -rf Maze

SMaze: SMaze.c
	gcc -o SMaze $(INC_PATH) SMaze.c $(LIB_PATH)
clean SMaze:
	rm -rf SMaze

compile:
	sudo rfcomm
	sudo rfcomm bind 0 00:0A:3A:2E:C8:7D
	sudo rfcomm
	sudo chmod 666 /dev/rfcomm0
	player roomba.cfg
release:
	sudo rfcomm release 0
