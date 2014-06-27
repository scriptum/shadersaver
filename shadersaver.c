
#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "shadersaver.h"

const GLchar *std_vertex_shader = "varying vec2 TexCoord;"
                                "void main()"
                                "{"
                                "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
                                "gl_FrontColor = gl_Color;"
                                "TexCoord = gl_MultiTexCoord0.xy;"
                                "}";

GLuint shader;

enum {
	WIDTH = 320,
	HEIGHT = 240
};

static int compile(GLuint s)
{
	GLint compiled;
	GLint blen = 0;
	GLsizei slen = 0;
	GLchar *compiler_log = NULL;
	glCompileShader(s);
	glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		glGetShaderiv(s, GL_INFO_LOG_LENGTH , &blen);
		if(blen > 1)
		{
			compiler_log = malloc(blen);
			glGetShaderInfoLog(s, blen,
			 &slen, compiler_log);
			
			
			fprintf(stderr, "Error while compiling s %s: \n", compiler_log);
			free(compiler_log);
		}
		return 0;
	}
	return 1;
}

static void initShader()
{
	GLuint v, f;
	const GLsizei count = 1;
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	//printf("%d %d\n", v, f);
	glShaderSource(v, count, &std_vertex_shader, NULL);
	glShaderSource(f, count, &frag_source, NULL);
	if(0 == compile(v))
	{
		return;
	}
	if(0 == compile(f))
	{
		glDeleteShader(v);
		return;
	}
	shader = glCreateProgram();
	glAttachShader(shader, v);
	glAttachShader(shader, f);
	glLinkProgram(shader);

	glDeleteShader(v);
	glDeleteShader(f);
}


void display(void)
{
	static float time = 0.f;
	static float prev_time = 0.f;
	float current_time = (float)(clock()/(float)CLOCKS_PER_SEC);
	static int count = 0;
	glUseProgram(shader);
	GLint var = glGetUniformLocation(shader, "iGlobalTime");
	glUniform1f(var, current_time);
	time += current_time - prev_time;
	count++;
	if(time > 1.0f)
	{
		printf("FPS: %d\n", count);
		count = 0;
		time -= 1.0f;
	}
	prev_time = current_time;
	var = glGetUniformLocation(shader, "iResolution");
	glUniform2f(var, WIDTH, HEIGHT);
	var = glGetUniformLocation(shader, "iMouse");
	glUniform2f(var, 0.f, 0.f);
	glBegin(GL_QUADS);
	glVertex2f(0,1);
	glVertex2f(1,1);
	glVertex2f(1,0);
	glVertex2f(0,0);
	glEnd();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(argv[0]);
	glOrtho(0,1,0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
	initShader();
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

