#include "coreHeader.h"

//is there some library for that? I'm writing this all manually...

//glFuncs used by remaster
DWORD glAlphaFunc;
DWORD glBindTexture;
DWORD glBlendFunc;
DWORD glClear;
DWORD glClearColor;
DWORD glColorMask;
DWORD glColorPointer;
DWORD glCopyTexImage2D;
DWORD glDeleteTextures;
DWORD glDepthFunc;
DWORD glDepthMask;
DWORD glDepthRange;
DWORD glDisable;
DWORD glGetError;
DWORD glGetBooleanv;
DWORD glGenTextures;
DWORD glFrontFace;
DWORD glFinish;
DWORD glEnableClientState;
DWORD glEnable;
DWORD glDrawElements;
DWORD glGetIntegerv;
DWORD glHint;
DWORD glLineWidth;
DWORD glLoadIdentity;
DWORD glLoadMatrixf;
DWORD glMatrixMode;
DWORD glOrtho;
DWORD glPixelStorei;
DWORD glPolygonMode;
DWORD glPopAttrib;
DWORD glPopMatrix;
DWORD glPushAttrib;
DWORD glPushMatrix;
DWORD glReadPixels;
DWORD glScissor;
DWORD glShadeModel;
DWORD glStencilFunc;
DWORD glStencilOp;
DWORD glTexCoordPointer;
DWORD glTexImage2D;
DWORD glTexParameteri;
DWORD glTexSubImage2D;
DWORD glVertexPointer;
DWORD glViewport;
DWORD glIsEnabled;
DWORD glClearDepth;
DWORD glGetString;
DWORD glGetTexImage;
DWORD glGetFloatv;

void HookOpenGL()
{
	//copy&paste horror below
	if(NULL == (glAlphaFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glAlphaFunc"))) {OutputDebug("GetProcAddress failed at glAlphaFunc"); return; }
	if(NULL == (glBindTexture = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glBindTexture"))) {OutputDebug("GetProcAddress failed at glBindTexture"); return; }
	if(NULL == (glBlendFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glBlendFunc"))) {OutputDebug("GetProcAddress failed at glBlendFunc"); return; }
	if(NULL == (glClear = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glClear"))) {OutputDebug("GetProcAddress failed at glClear"); return; }
	if(NULL == (glClearColor = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glClearColor"))) {OutputDebug("GetProcAddress failed at glClearColor"); return; }
	if(NULL == (glColorMask = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glColorMask"))) {OutputDebug("GetProcAddress failed at glColorMask"); return; }
	if(NULL == (glColorPointer = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glColorPointer"))) {OutputDebug("GetProcAddress failed at glColorPointer"); return; }
	if(NULL == (glCopyTexImage2D = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glCopyTexImage2D"))) {OutputDebug("GetProcAddress failed at glCopyTexImage2D"); return; }
	if(NULL == (glDeleteTextures = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDeleteTextures"))) {OutputDebug("GetProcAddress failed at glDeleteTextures"); return; }
	if(NULL == (glDepthFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDepthFunc"))) {OutputDebug("GetProcAddress failed at glDepthFunc"); return; }
	if(NULL == (glDepthMask = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDepthMask"))) {OutputDebug("GetProcAddress failed at glDepthMask"); return; }
	if(NULL == (glDepthRange = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDepthRange"))) {OutputDebug("GetProcAddress failed at glDepthRange"); return; }
	if(NULL == (glDisable = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDisable"))) {OutputDebug("GetProcAddress failed at glDisable"); return; }
	if(NULL == (glGetError = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetError"))) {OutputDebug("GetProcAddress failed at glGetError"); return; }
	if(NULL == (glGetBooleanv = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetBooleanv"))) {OutputDebug("GetProcAddress failed at glGetBooleanv"); return; }
	if(NULL == (glGenTextures = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGenTextures"))) {OutputDebug("GetProcAddress failed at glGenTextures"); return; }
	if(NULL == (glFrontFace = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glFrontFace"))) {OutputDebug("GetProcAddress failed at glFrontFace"); return; }
	if(NULL == (glFinish = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glFinish"))) {OutputDebug("GetProcAddress failed at glFinish"); return; }
	if(NULL == (glEnableClientState = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glEnableClientState"))) {OutputDebug("GetProcAddress failed at glEnableClientState"); return; }
	if(NULL == (glEnable = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glEnable"))) {OutputDebug("GetProcAddress failed at glEnable"); return; }
	if(NULL == (glDrawElements = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glDrawElements"))) {OutputDebug("GetProcAddress failed at glDrawElements"); return; }
	if(NULL == (glGetIntegerv = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetIntegerv"))) {OutputDebug("GetProcAddress failed at glGetIntegerv"); return; }
	if(NULL == (glHint = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glHint"))) {OutputDebug("GetProcAddress failed at glHint"); return; }
	if(NULL == (glLineWidth = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glLineWidth"))) {OutputDebug("GetProcAddress failed at glLineWidth"); return; }
	if(NULL == (glLoadIdentity = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glLoadIdentity"))) {OutputDebug("GetProcAddress failed at glLoadIdentity"); return; }
	if(NULL == (glLoadMatrixf = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glLoadMatrixf"))) {OutputDebug("GetProcAddress failed at glLoadMatrixf"); return; }
	if(NULL == (glMatrixMode = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glMatrixMode"))) {OutputDebug("GetProcAddress failed at glMatrixMode"); return; }
	if(NULL == (glOrtho = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glOrtho"))) {OutputDebug("GetProcAddress failed at glOrtho"); return; }
	if(NULL == (glPixelStorei = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPixelStorei"))) {OutputDebug("GetProcAddress failed at glPixelStorei"); return; }
	if(NULL == (glPolygonMode = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPolygonMode"))) {OutputDebug("GetProcAddress failed at glPolygonMode"); return; }
	if(NULL == (glPopAttrib = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPopAttrib"))) {OutputDebug("GetProcAddress failed at glPopAttrib"); return; }
	if(NULL == (glPopMatrix = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPopMatrix"))) {OutputDebug("GetProcAddress failed at glPopMatrix"); return; }
	if(NULL == (glPushAttrib = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPushAttrib"))) {OutputDebug("GetProcAddress failed at glPushAttrib"); return; }
	if(NULL == (glPushMatrix = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glPushMatrix"))) {OutputDebug("GetProcAddress failed at glPushMatrix"); return; }
	if(NULL == (glReadPixels = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glReadPixels"))) {OutputDebug("GetProcAddress failed at glReadPixels"); return; }
	if(NULL == (glScissor = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glScissor"))) {OutputDebug("GetProcAddress failed at glScissor"); return; }
	if(NULL == (glShadeModel = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glShadeModel"))) {OutputDebug("GetProcAddress failed at glShadeModel"); return; }
	if(NULL == (glStencilFunc = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glStencilFunc"))) {OutputDebug("GetProcAddress failed at glStencilFunc"); return; }
	if(NULL == (glStencilOp = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glStencilOp"))) {OutputDebug("GetProcAddress failed at glStencilOp"); return; }
	if(NULL == (glTexCoordPointer = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexCoordPointer"))) {OutputDebug("GetProcAddress failed at glTexCoordPointer"); return; }
	if(NULL == (glTexImage2D = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexImage2D"))) {OutputDebug("GetProcAddress failed at glTexImage2D"); return; }
	if(NULL == (glTexParameteri = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexParameteri"))) {OutputDebug("GetProcAddress failed at glTexParameteri"); return; }
	if(NULL == (glTexSubImage2D = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glTexSubImage2D"))) {OutputDebug("GetProcAddress failed at glTexSubImage2D"); return; }
	if(NULL == (glVertexPointer = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glVertexPointer"))) {OutputDebug("GetProcAddress failed at glVertexPointer"); return; }
	if(NULL == (glViewport = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glViewport"))) {OutputDebug("GetProcAddress failed at glViewport"); return; }
	if(NULL == (glIsEnabled = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glIsEnabled"))) {OutputDebug("GetProcAddress failed at glIsEnabled"); return; }
	if(NULL == (glClearDepth = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glClearDepth"))) {OutputDebug("GetProcAddress failed at glClearDepth"); return; }
	if(NULL == (glGetString = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetString"))) {OutputDebug("GetProcAddress failed at glGetString"); return; }
	if(NULL == (glGetTexImage = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetTexImage"))) {OutputDebug("GetProcAddress failed at glGetTexImage"); return; }
	if(NULL == (glGetFloatv = (DWORD)GetProcAddress((HMODULE)OPENGL_HANDLE, "glGetFloatv"))) {OutputDebug("GetProcAddress failed at glGetFloatv"); return; }

	
}