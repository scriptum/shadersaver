TARGET=shadersaver

%:%.c shader.frag Makefile
	awk 'BEGIN{print "const GLchar * frag_source = ";print "\"uniform float iGlobalTime;\\nuniform vec2 iResolution;\\nuniform vec2 iMouse;\\n\"";}{printf("\"%s\\n\"\n",$$0)}END{printf(";")}' shader.frag > $@.h
	gcc -Os -s $< -I/usr/include/GL/ -lGL -lGLU -lglut -o $@

all: $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).h