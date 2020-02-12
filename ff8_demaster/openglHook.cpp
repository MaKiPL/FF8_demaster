#include "coreHeader.h"
#include <GL/GL.h>

//could someone explain why the hell DWORD[10] when referenced by DWORD PTR is actually dumped to BYTE*????
DWORD params[10];
//is there some library for that? I'm writing this all manually...

//glFuncs used by remaster
DWORD dGlAlphaFunc;
DWORD dGlBindTexture;
DWORD dGlBlendFunc;
DWORD dGlClear;
DWORD dGlClearColor;
DWORD dGlColorMask;
DWORD dGlColorPointer;
DWORD dGlCopyTexImage2D;
DWORD dGlDeleteTextures;
DWORD dGlDepthFunc;
DWORD dGlDepthMask;
DWORD dGlDepthRange;
DWORD dGlDisable;
DWORD dGlGetError;
DWORD dGlGetBooleanv;
DWORD dGlGenTextures;
DWORD dGlFrontFace;
DWORD dGlFinish;
DWORD dGlEnableClientState;
DWORD dGlEnable;
DWORD dGlDrawElements;
DWORD dGlGetIntegerv;
DWORD dGlHint;
DWORD dGlLineWidth;
DWORD dGlLoadIdentity;
DWORD dGlLoadMatrixf;
DWORD dGlMatrixMode;
DWORD dGlOrtho;
DWORD dGlPixelStorei;
DWORD dGlPolygonMode;
DWORD dGlPopAttrib;
DWORD dGlPopMatrix;
DWORD dGlPushAttrib;
DWORD dGlPushMatrix;
DWORD dGlReadPixels;
DWORD dGlScissor;
DWORD dGlShadeModel;
DWORD dGlStencilFunc;
DWORD dGlStencilOp;
DWORD dGlTexCoordPointer;
DWORD dGlTexImage2D;
DWORD dGlTexParameteri;
DWORD dGlTexSubImage2D;
DWORD dGlVertexPointer;
DWORD dGlViewport;
DWORD dGlIsEnabled;
DWORD dGlClearDepth;
DWORD dGlGetString;
DWORD dGlGetTexImage;
DWORD dGlGetFloatv;

//jump back addressess;
DWORD jglAlphaFunc;
DWORD jglBindTexture;
DWORD jglBlendFunc;
DWORD jglClear;
DWORD jglClearColor;
DWORD jglColorMask;
DWORD jglColorPointer;
DWORD jglCopyTexImage2D;
DWORD jglDeleteTextures;
DWORD jglDepthFunc;
DWORD jglDepthMask;
DWORD jglDepthRange;
DWORD jglDisable;
DWORD jglGetError;
DWORD jglGetBooleanv;
DWORD jglGenTextures;
DWORD jglFrontFace;
DWORD jglFinish;
DWORD jglEnableClientState;
DWORD jglEnable;
DWORD jglDrawElements;
DWORD jglGetIntegerv;
DWORD jglHint;
DWORD jglLineWidth;
DWORD jglLoadIdentity;
DWORD jglLoadMatrixf;
DWORD jglMatrixMode;
DWORD jglOrtho;
DWORD jglPixelStorei;
DWORD jglPolygonMode;
DWORD jglPopAttrib;
DWORD jglPopMatrix;
DWORD jglPushAttrib;
DWORD jglPushMatrix;
DWORD jglReadPixels;
DWORD jglScissor;
DWORD jglShadeModel;
DWORD jglStencilFunc;
DWORD jglStencilOp;
DWORD jglTexCoordPointer;
DWORD jglTexImage2D;
DWORD jglTexParameteri;
DWORD jglTexSubImage2D;
DWORD jglVertexPointer;
DWORD jglViewport;
DWORD jglIsEnabled;
DWORD jglClearDepth;
DWORD jglGetString;
DWORD jglGetTexImage;
DWORD jglGetFloatv;

//this is for replacement methods if needed
void _glAlphaFunc();
void _glBindTexture();
void _glBlendFunc();
void _glClear();
void _glClearColor();
void _glColorMask();
void _glColorPointer();
void _glCopyTexImage2D();
void _glDeleteTextures();
void _glDepthFunc();
void _glDepthMask();
void _glDepthRange();
void _glDisable();
void _glGetError();
void _glGetBooleanv();
void _glGenTextures();
void _glFrontFace();
void _glFinish();
void _glEnableClientState();
void _glEnable();
void _glDrawElements();
void _glGetIntegerv();
void _glHint();
void _glLineWidth();
void _glLoadIdentity();
void _glLoadMatrixf();
void _glMatrixMode();
void _glOrtho();
void _glPixelStorei();
void _glPolygonMode();
void _glPopAttrib();
void _glPopMatrix();
void _glPushAttrib();
void _glPushMatrix();
void _glReadPixels();
void _glScissor();
void _glShadeModel();
void _glStencilFunc();
void _glStencilOp();
void _glTexCoordPointer();
void _glTexImage2D();
void _glTexParameteri();
void _glTexSubImage2D();
void _glVertexPointer();
void _glViewport();
void _glIsEnabled();
void _glClearDepth();
void _glGetString();
void _glGetTexImage();
void _glGetFloatv();

void HookOpenGL()
{
	//copy&paste horror below
	if(NULL == (dGlAlphaFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glAlphaFunc"))) {OutputDebug("GetProcAddress failed at glAlphaFunc"); return; }
	if(NULL == (dGlBindTexture = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glBindTexture"))) {OutputDebug("GetProcAddress failed at glBindTexture"); return; }
	if(NULL == (dGlBlendFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glBlendFunc"))) {OutputDebug("GetProcAddress failed at glBlendFunc"); return; }
	if(NULL == (dGlClear = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glClear"))) {OutputDebug("GetProcAddress failed at glClear"); return; }
	if(NULL == (dGlClearColor = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glClearColor"))) {OutputDebug("GetProcAddress failed at glClearColor"); return; }
	if(NULL == (dGlColorMask = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glColorMask"))) {OutputDebug("GetProcAddress failed at glColorMask"); return; }
	if(NULL == (dGlColorPointer = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glColorPointer"))) {OutputDebug("GetProcAddress failed at glColorPointer"); return; }
	if(NULL == (dGlCopyTexImage2D = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glCopyTexImage2D"))) {OutputDebug("GetProcAddress failed at glCopyTexImage2D"); return; }
	if(NULL == (dGlDeleteTextures = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDeleteTextures"))) {OutputDebug("GetProcAddress failed at glDeleteTextures"); return; }
	if(NULL == (dGlDepthFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDepthFunc"))) {OutputDebug("GetProcAddress failed at glDepthFunc"); return; }
	if(NULL == (dGlDepthMask = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDepthMask"))) {OutputDebug("GetProcAddress failed at glDepthMask"); return; }
	if(NULL == (dGlDepthRange = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDepthRange"))) {OutputDebug("GetProcAddress failed at glDepthRange"); return; }
	if(NULL == (dGlDisable = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDisable"))) {OutputDebug("GetProcAddress failed at glDisable"); return; }
	if(NULL == (dGlGetError = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetError"))) {OutputDebug("GetProcAddress failed at glGetError"); return; }
	if(NULL == (dGlGetBooleanv = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetBooleanv"))) {OutputDebug("GetProcAddress failed at glGetBooleanv"); return; }
	if(NULL == (dGlGenTextures = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGenTextures"))) {OutputDebug("GetProcAddress failed at glGenTextures"); return; }
	if(NULL == (dGlFrontFace = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glFrontFace"))) {OutputDebug("GetProcAddress failed at glFrontFace"); return; }
	if(NULL == (dGlFinish = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glFinish"))) {OutputDebug("GetProcAddress failed at glFinish"); return; }
	if(NULL == (dGlEnableClientState = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glEnableClientState"))) {OutputDebug("GetProcAddress failed at glEnableClientState"); return; }
	if(NULL == (dGlEnable = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glEnable"))) {OutputDebug("GetProcAddress failed at glEnable"); return; }
	if(NULL == (dGlDrawElements = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDrawElements"))) {OutputDebug("GetProcAddress failed at glDrawElements"); return; }
	if(NULL == (dGlGetIntegerv = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetIntegerv"))) {OutputDebug("GetProcAddress failed at glGetIntegerv"); return; }
	if(NULL == (dGlHint = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glHint"))) {OutputDebug("GetProcAddress failed at glHint"); return; }
	if(NULL == (dGlLineWidth = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glLineWidth"))) {OutputDebug("GetProcAddress failed at glLineWidth"); return; }
	if(NULL == (dGlLoadIdentity = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glLoadIdentity"))) {OutputDebug("GetProcAddress failed at glLoadIdentity"); return; }
	if(NULL == (dGlLoadMatrixf = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glLoadMatrixf"))) {OutputDebug("GetProcAddress failed at glLoadMatrixf"); return; }
	if(NULL == (dGlMatrixMode = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glMatrixMode"))) {OutputDebug("GetProcAddress failed at glMatrixMode"); return; }
	if(NULL == (dGlOrtho = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glOrtho"))) {OutputDebug("GetProcAddress failed at glOrtho"); return; }
	if(NULL == (dGlPixelStorei = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPixelStorei"))) {OutputDebug("GetProcAddress failed at glPixelStorei"); return; }
	if(NULL == (dGlPolygonMode = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPolygonMode"))) {OutputDebug("GetProcAddress failed at glPolygonMode"); return; }
	if(NULL == (dGlPopAttrib = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPopAttrib"))) {OutputDebug("GetProcAddress failed at glPopAttrib"); return; }
	if(NULL == (dGlPopMatrix = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPopMatrix"))) {OutputDebug("GetProcAddress failed at glPopMatrix"); return; }
	if(NULL == (dGlPushAttrib = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPushAttrib"))) {OutputDebug("GetProcAddress failed at glPushAttrib"); return; }
	if(NULL == (dGlPushMatrix = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPushMatrix"))) {OutputDebug("GetProcAddress failed at glPushMatrix"); return; }
	if(NULL == (dGlReadPixels = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glReadPixels"))) {OutputDebug("GetProcAddress failed at glReadPixels"); return; }
	if(NULL == (dGlScissor = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glScissor"))) {OutputDebug("GetProcAddress failed at glScissor"); return; }
	if(NULL == (dGlShadeModel = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glShadeModel"))) {OutputDebug("GetProcAddress failed at glShadeModel"); return; }
	if(NULL == (dGlStencilFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glStencilFunc"))) {OutputDebug("GetProcAddress failed at glStencilFunc"); return; }
	if(NULL == (dGlStencilOp = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glStencilOp"))) {OutputDebug("GetProcAddress failed at glStencilOp"); return; }
	if(NULL == (dGlTexCoordPointer = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexCoordPointer"))) {OutputDebug("GetProcAddress failed at glTexCoordPointer"); return; }
	if(NULL == (dGlTexImage2D = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexImage2D"))) {OutputDebug("GetProcAddress failed at glTexImage2D"); return; }
	if(NULL == (dGlTexParameteri = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexParameteri"))) {OutputDebug("GetProcAddress failed at glTexParameteri"); return; }
	if(NULL == (dGlTexSubImage2D = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexSubImage2D"))) {OutputDebug("GetProcAddress failed at glTexSubImage2D"); return; }
	if(NULL == (dGlVertexPointer = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glVertexPointer"))) {OutputDebug("GetProcAddress failed at glVertexPointer"); return; }
	if(NULL == (dGlViewport = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glViewport"))) {OutputDebug("GetProcAddress failed at glViewport"); return; }
	if(NULL == (dGlIsEnabled = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glIsEnabled"))) {OutputDebug("GetProcAddress failed at glIsEnabled"); return; }
	if(NULL == (dGlClearDepth = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glClearDepth"))) {OutputDebug("GetProcAddress failed at glClearDepth"); return; }
	if(NULL == (dGlGetString = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetString"))) {OutputDebug("GetProcAddress failed at glGetString"); return; }
	if(NULL == (dGlGetTexImage = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetTexImage"))) {OutputDebug("GetProcAddress failed at glGetTexImage"); return; }
	if(NULL == (dGlGetFloatv = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetFloatv"))) {OutputDebug("GetProcAddress failed at glGetFloatv"); return; }

	jglClearColor = (DWORD)InjectJMP(dGlClearColor, (DWORD)_glClearColor);
	jglTexImage2D = (DWORD)InjectJMP(dGlTexImage2D, (DWORD)_glTexImage2D);
	jglLoadMatrixf = (DWORD)InjectJMP(dGlLoadMatrixf, (DWORD)_glLoadMatrixf);
	jglOrtho = (DWORD)InjectJMP(dGlOrtho, (DWORD)_glOrtho);
}

void __glOrtho()
{
	//GLfloat matrixModel[16], matrixProjection[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, matrixModel);
	//glGetFloatv(GL_PROJECTION_MATRIX, matrixProjection);
	//double* left = (double*) (params[0]);
	//double* right = left + 1;
	//double* bottom = left + 2;
	//double* top = left + 3;
	//double* zNear = left + 4;
	//double* zFar = left + 5;
	//char localn[256];
	//sprintf(localn, "glOrtho: left: %lf right: %lf bottom: %lf top: %lf zNear: %lf zFar: %lf\n",
	//	*left, *right, *bottom, *top, *zNear, *zFar);
	//OutputDebug(localn);
	//sprintf(localn, "glOrtho/MODELVIEW: \n  %f %f %f %f\n  %f %f %f %f\n  %f %f %f %f\n  %f %f %f %f\n",
	//	matrixModel[0], matrixModel[1], matrixModel[2], matrixModel[3],
	//	matrixModel[4], matrixModel[5], matrixModel[6], matrixModel[7],
	//	matrixModel[8], matrixModel[9], matrixModel[10], matrixModel[11],
	//	matrixModel[12], matrixModel[13], matrixModel[14], matrixModel[15]);
	//OutputDebug(localn);
	//sprintf(localn, "glOrtho/PROJECTION: \n  %f %f %f %f\n  %f %f %f %f\n  %f %f %f %f\n  %f %f %f %f\n",
	//	matrixProjection[0], matrixProjection[1], matrixProjection[2], matrixProjection[3],
	//	matrixProjection[4], matrixProjection[5], matrixProjection[6], matrixProjection[7],
	//	matrixProjection[8], matrixProjection[9], matrixProjection[10], matrixProjection[11],
	//	matrixProjection[12], matrixProjection[13], matrixProjection[14], matrixProjection[15]);
	//OutputDebug(localn);
	return;
}
__declspec(naked) void _glOrtho()
{
	__asm
	{
		LEA EAX, [ESP + 4] //left
		MOV params[0], EAX
		LEA EAX, [ESP + 12] //right
		MOV params[4], EAX
		LEA EAX, [ESP + 20] //bottom
		MOV params[8], EAX
		LEA EAX, [ESP + 28] //top
		MOV params[12], EAX
		LEA EAX, [ESP + 36] //zNear
		MOV params[16], EAX
		LEA EAX, [ESP + 44] //zFar
		MOV params[20], EAX
		CALL __glOrtho
		NOP
		NOP
		PUSH EBP
		MOV EBP, ESP
		JMP jglOrtho
	}
}

void __glTexImage2D()
{
	
	char localn[256];
	sprintf(localn, "glTexImage2D: target: %X level: %X internal: %X width: %X height: %X border: %X format: %X type: %X data: %X\n",
		params[0], params[1], params[2], params[3],
		params[4], params[5], params[6], params[7], params[8], params[9]);
	OutputDebug(localn);
	return;
}
__declspec(naked) void _glTexImage2D()
{
	__asm
	{
		MOV EAX, [ESP + 4] //target
		MOV params[0], EAX
		MOV EAX, [ESP + 8] //level
		MOV params[4], EAX
		MOV EAX, [ESP + 12] //internalformat
		MOV params[8], EAX
		MOV EAX, [ESP + 16] //width
		MOV params[12], EAX
		MOV EAX, [ESP + 20] //height
		MOV params[16], EAX
		MOV EAX, [ESP + 24] //border
		MOV params[20], EAX
		MOV EAX, [ESP + 28] //format
		MOV params[24], EAX
		MOV EAX, [ESP + 32] //type
		MOV params[28], EAX
		MOV EAX, [ESP + 36] //void* pixels
		MOV params[32], EAX
		CALL __glTexImage2D
		NOP
		NOP
		PUSH EBP
		MOV EBP, ESP
		JMP jglTexImage2D
	}
}

void __glLoadMatrixf()
{
	//char localn[256];
	//float* m11 = (float*)params[0];
	//float* m12 = m11 + 1;
	//float* m13 = m11 + 2;
	//float* m14 = m11 + 3;
	//float* m21 = m11 + 4;
	//float* m22 = m11 + 5;
	//float* m23 = m11 + 6;
	//float* m24 = m11 + 7;
	//float* m31 = m11 + 8;
	//float* m32 = m11 + 9;
	//float* m33 = m11 + 10;
	//float* m34 = m11 + 11;
	//float* m41 = m11 + 12;
	//float* m42 = m11 + 13;
	//float* m43 = m11 + 14;
	//float* m44 = m11 + 15;
	//sprintf(localn, "glLoadMatrix:\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n",
	//	m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
	//OutputDebug(localn);
	return;
}
__declspec(naked) void _glLoadMatrixf()
{
	__asm
	{
		MOV EAX, [ESP + 4]
		MOV params[0], EAX //m11
		CALL __glLoadMatrixf
		/*MOV EAX, params[0]
		MOV[ESP + 4], EAX*/
		NOP
		NOP
		PUSH EBP
		MOV EBP, ESP
		JMP jglLoadMatrixf
	}
}

void __glClearColor()
{
	//char localn[256];
	//sprintf(localn, "glClearColor: %d %d %d %d\n", params[0], params[1], params[2], params[3]);
	//OutputDebug(localn);
	//params[2] = 0x3F800000;
	return;
}
__declspec(naked) void _glClearColor()
{
	__asm
	{
		MOV EAX, [ESP+4]
		MOV params[0], EAX
		MOV EAX, [ESP+8]
		MOV params[4], EAX
		MOV EAX, [ESP+12]
		MOV params[8], EAX
		MOV EAX, [ESP+16]
		MOV params[12], EAX
		CALL __glClearColor
		MOV EAX, params[12]
		MOV [ESP+16], EAX
		MOV EAX, params[8]
		MOV[ESP + 12], EAX
		MOV EAX, params[4]
		MOV[ESP + 8], EAX
		MOV EAX, params[0]
		MOV[ESP + 4], EAX
		NOP
		NOP
		PUSH EBP
		MOV EBP, ESP
		JMP jglClearColor
	}
}