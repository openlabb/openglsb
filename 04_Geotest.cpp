// GeoTest.cpp
// OpenGL SuperBible
// Demonstrates OpenGL Backface culling, depth testing, and polygon mode
// Program by Richard S. Wright Jr.
#include <GLTools.h>        // OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


GLFrame             viewFrame;
GLFrustum           viewFrustum_GEOTEST;
GLTriangleBatch     torusBatch;
GLMatrixStack       modelViewMatix;
GLMatrixStack       projectionMatrix_GEOTEST;
GLGeometryTransform transformPipeline_GEOTEST;
GLShaderManager     shaderManager_GEOTEST;

// Flags for effects
int iCull = 0;
int iDepth = 0;

///////////////////////////////////////////////////////////////////////////////
// Reset flags as appropriate in response to menu selections
void ProcessMenu_GEOTEST(int value)
{
  switch(value)
  {
    case 1:
      iDepth = !iDepth;
      break;
      
    case 2:
      iCull = !iCull;
      break;
      
    case 3:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
      
    case 4:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
      
    case 5:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
  }
  
  glutPostRedisplay();
}


// Called to draw scene
void RenderScene_GEOTEST(void)
{
  // Clear the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Turn culling on if flag is set
  if(iCull)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
  
  // Enable depth testing if flag is set
  if(iDepth)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
  
  
  modelViewMatix.PushMatrix(viewFrame);
  
  GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  //shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vRed);
  shaderManager_GEOTEST.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline_GEOTEST.GetModelViewMatrix(), transformPipeline_GEOTEST.GetProjectionMatrix(), vRed);
  
  
  torusBatch.Draw();
  
  modelViewMatix.PopMatrix();
  
  
  glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context.
void SetupRC_GEOTEST()
{
  // Black background
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f );
  
  shaderManager_GEOTEST.InitializeStockShaders();
  viewFrame.MoveForward(7.0f);
  
  // Make the torus
  gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
  
  glPointSize(4.0f);
}

void SpecialKeys_GEOTEST(int key, int x, int y)
{
  if(key == GLUT_KEY_UP)
    viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
  
  if(key == GLUT_KEY_DOWN)
    viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
  
  if(key == GLUT_KEY_LEFT)
    viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
  
  if(key == GLUT_KEY_RIGHT)
    viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
  
  // Refresh the Window
  glutPostRedisplay();
}


void ChangeSize_GEOTEST(int w, int h)
{
  // Prevent a divide by zero
  if(h == 0)
    h = 1;
  
  // Set Viewport to window dimensions
  glViewport(0, 0, w, h);
  
  viewFrustum_GEOTEST.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
  
  projectionMatrix_GEOTEST.LoadMatrix(viewFrustum_GEOTEST.GetProjectionMatrix());
  transformPipeline_GEOTEST.SetMatrixStacks(modelViewMatix, projectionMatrix_GEOTEST);
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main_GEOTEST(int argc, char* argv[])
{
  gltSetWorkingDirectory(argv[0]);
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Geometry Test Program");
  glutReshapeFunc(ChangeSize_GEOTEST);
  glutSpecialFunc(SpecialKeys_GEOTEST);
  glutDisplayFunc(RenderScene_GEOTEST);
  
  // Create the Menu
  glutCreateMenu(ProcessMenu_GEOTEST);
  glutAddMenuEntry("Toggle depth test",1);
  glutAddMenuEntry("Toggle cull backface",2);
  glutAddMenuEntry("Set Fill Mode", 3);
  glutAddMenuEntry("Set Line Mode", 4);
  glutAddMenuEntry("Set Point Mode", 5);
  
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
    return 1;
  }
  
  SetupRC_GEOTEST();
  
  glutMainLoop();
  return 0;
}