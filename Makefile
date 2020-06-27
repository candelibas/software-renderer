build:
	gcc \
	-Wfatal-errors \
	-std=c99 \
	./src/*.c \
	-I"C:\libsdl\include" \
	-L"C:\libsdl\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o renderer.exe

run:
	renderer.exe

clean:
	del renderer.exe