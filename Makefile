all:
	g++ Main.cpp \
	-lncurses -o ventana
clean:
	rm ventana
