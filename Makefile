file=screenSaver
$(file).scr: $(file).c
	gcc -std=c99 -L/usr/local/lib -I/z/1A/sasano/freeglut-2.8.1/include $(file).c -o $(file).scr -lscrnsave -lglu32 -lglut -lgdi32 -lopengl32
s:
	./$(file).scr s
