//
//  Move.cpp
//  testGLUT
//
//  Created by OpenLAB on 13-11-6.
//  Copyright (c) 2013年 KKStudio. All rights reserved.
//

#include <GLTools.h>
#include <GLShaderManager.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

GLBatch        squareBatch;
GLShaderManager        shaderManager_MOVE;


GLfloat blockSize = 0.1f;
GLfloat vVerts[] = { -blockSize - 0.5f, -blockSize, 0.0f,
  blockSize - 0.5f, -blockSize, 0.0f,
  blockSize - 0.5f,  blockSize, 0.0f,
  -blockSize - 0.5f,  blockSize, 0.0f};

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC_MOVE()
{
  // Black background
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f );
  
  shaderManager_MOVE.InitializeStockShaders();
  
  // Load up a triangle
  squareBatch.Begin(GL_TRIANGLE_FAN, 4);
  squareBatch.CopyVertexData3f(vVerts);
  squareBatch.End();
}

// Respond to arrow keys by moving the camera frame of reference
void BounceFunction(void)
{
  static GLfloat xDir = 1.0f;
  static GLfloat yDir = 1.0f;
  
  GLfloat stepSize = 0.005f;
  
  GLfloat blockX = vVerts[0];   // Upper left X
  GLfloat blockY = vVerts[7];  // Upper left Y
  
  blockY += stepSize * yDir;
  blockX += stepSize * xDir;
  
  // Collision detection
  if(blockX < -1.0f)
  {
    blockX = -1.0f; xDir *= -1.0f;
  }
  if(blockX > (1.0f - blockSize * 2))
  {
    blockX = 1.0f - blockSize * 2;
    xDir *= -1.0f;
  }
  if(blockY < -1.0f + blockSize * 2)
  {
    blockY = -1.0f + blockSize * 2;
    yDir *= -1.0f;
  }
  if(blockY > 1.0f)
  {
    blockY = 1.0f;
    yDir *= -1.0f;
  }
  
  // Recalculate vertex positions
  vVerts[0] = blockX;
  vVerts[1] = blockY - blockSize*2;
  
  vVerts[3] = blockX + blockSize*2;
  vVerts[4] = blockY - blockSize*2;
  
  vVerts[6] = blockX + blockSize*2;
  vVerts[7] = blockY;
  
  vVerts[9] = blockX;
  vVerts[10] = blockY;
  
  squareBatch.CopyVertexData3f(vVerts);
}





///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene_MOVE(void)
{
  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  shaderManager_MOVE.UseStockShader(GLT_SHADER_IDENTITY, vRed);
  squareBatch.Draw();
  
  // Flush drawing commands
  glutSwapBuffers();
  
  BounceFunction();
  glutPostRedisplay(); // Redraw
}

void SpecialKeys_MOVE(int key, int x, int y)
{
  GLfloat stepSize = 0.025f;
  GLfloat blockX = vVerts[0]; // Upper left X
  GLfloat blockY = vVerts[7]; // Upper left Y
  if(key == GLUT_KEY_UP) blockY += stepSize;
  if(key == GLUT_KEY_DOWN) blockY -= stepSize;
  if(key == GLUT_KEY_LEFT) blockX -= stepSize;
  if(key == GLUT_KEY_RIGHT) blockX += stepSize;
  // Collision detection
  if(blockX < -1.0f)
    blockX = -1.0f;
  if(blockX > (1.0f - blockSize * 2))
    blockX = 1.0f - blockSize * 2;;
  if(blockY < -1.0f + blockSize * 2)
    blockY = -1.0f + blockSize * 2;
  if(blockY > 1.0f)
    blockY = 1.0f;
  // Recalculate vertex positions
  vVerts[0] = blockX;
  vVerts[1] = blockY - blockSize*2;
  vVerts[3] = blockX + blockSize*2;
  vVerts[4] = blockY - blockSize*2;
  vVerts[6] = blockX + blockSize*2;
  vVerts[7] = blockY;
  vVerts[9] = blockX;
  vVerts[10] = blockY;
  squareBatch.CopyVertexData3f(vVerts);
  glutPostRedisplay(); 
}


///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize_MOVE(int w, int h)
{
  glViewport(0, 0, w, h);
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main_Move(int argc, char* argv[])
{
  gltSetWorkingDirectory(argv[0]);
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Bouncing Block");
  
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    // Problem: glewInit failed, something is seriously wrong.
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return 1;
  }
  
  glutReshapeFunc(ChangeSize_MOVE);
  glutDisplayFunc(RenderScene_MOVE);
  glutSpecialFunc(SpecialKeys_MOVE);
  SetupRC_MOVE();
  
  glutMainLoop();
  return 0;
}
