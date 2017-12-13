#include <string>
#include <iostream>
#include <fstream>

#include "CustomRenderer.h"

#define DEBUG

CustomRenderer::CustomRenderer()
{
  modelLoader = new DataLoader();
  sliceLoader = new DataLoader();
  stepSize = 0.004f;
  mouseLButtonDown = false;
  mouseRButtonDown = false;
  dataLinked = false;
  volumeInited = false;
  sliceInited = false;
  isCoordShown = false;
  isCoordInited = false;

  hAngle = 0;
  vAngle = 0;

  pos = glm::vec3(1, 1, 2);
  distDelta = 0.5f;

  opacity = 0.0f;
  clipPlaneXL = clipPlaneYL = clipPlaneZL = -100.0f;
  clipPlaneXR = clipPlaneYR = clipPlaneZR = 100.0f;

  n = m = 30;
  radius = 0.2f;
}

void CustomRenderer::UpdateWheelPos(int wheelPos)
{
  int sign = wheelPos > 0 ? -1 : 1;
  pos *= (1.0f + sign * distDelta / pos.length());
}


CustomRenderer::~CustomRenderer()
{

}




void CustomRenderer::CustomInit(void)
{  
}

void CustomRenderer::GetEllipseData(int *center, float *scale, float *angles)
{
  center[0] = (int)(spherePos.x * dataW);
  center[1] = (int)(spherePos.y * dataH);
  center[2] = (int)(spherePos.z * dataD);

  scale[0] = sphereScale.x;
  scale[1] = sphereScale.y;
  scale[2] = sphereScale.z;

  angles[0] = sphereRot.x;
  angles[1] = sphereRot.y;
}


void CustomRenderer::CustomCoordInit(void)
{
  /*** Init VBOs ***/
  GLfloat vertices[18] = {
    0.0, 0.0, 0.0,// 1.0, 0.0, 0.0,
    5.0, 0.0, 0.0,// 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0,// 0.0, 1.0, 0.0,
    0.0, 5.0, 0.0,// 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0,// 0.0, 0.0, 1.0,
    0.0, 0.0, 5.0//, 0.0, 0.0, 1.0
  };  
  
  GLuint gbo;

  glGenBuffers(1, &gbo);  
  glGenVertexArrays(1, &sysCoordVAO);
  glBindVertexArray(sysCoordVAO);

  glBindBuffer(GL_ARRAY_BUFFER, gbo);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);      
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLfloat *)NULL);
  glEnableVertexAttribArray(0);
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  //glEnableVertexAttribArray(1);  

  isCoordInited = true;
}


void CustomRenderer::CustomSphereInit(void)
{
  
  int vertInd = 0;
  float theta = 0, phy = 0;  
  int index = 0;
  GLfloat *vertices = new GLfloat[n * m * 6];
  GLuint *indices = new GLuint[(n - 1) * (m - 1) * 6];

  for (int i = 0; i < n; i++)
  {
    theta = (float)PI * i / (n - 1);
    for (int j = 0; j < m; j++)
    {
      phy = 2.0f * (float)PI * j / (m - 1);

      // Pos
      vertices[vertInd + 0] = 0 + radius * sin(theta) * cos(phy);
      vertices[vertInd + 1] = 0 + radius * cos(theta);
      vertices[vertInd + 2] = 0 + radius * sin(theta) * sin(phy);
      // Color (red)
      vertices[vertInd + 3] = 250;
      vertices[vertInd + 4] = 0;
      vertices[vertInd + 5] = 0;
      vertInd += 6;
    }
  }   

  for (int i = 0, j = 0; i <= n * m; i++)
  {
    if (i + m < n * m && (i + 1) % m != 0)
    {
      indices[j + 0] = i + m + 1;
      indices[j + 1] = i;
      indices[j + 2] = i + m;
      indices[j + 3] = i + m + 1;
      indices[j + 4] = i + 1;
      indices[j + 5] = i;

      j += 6;
    }
  }    


  GLuint gbo[2];

  glGenBuffers(2, gbo);
  GLuint vertexBuffObject = gbo[0];
  GLuint indexBuffObject = gbo[1];

  glGenVertexArrays(1, &sphereVAO);
  glBindVertexArray(sphereVAO);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffObject);
  glBufferData(GL_ARRAY_BUFFER, (m * n * 6) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  // used in glDrawElement()
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((m - 1) * (n - 1) * 6) * sizeof(GLuint), indices, GL_STATIC_DRAW);

  // vao like a closure binding 3 buffer object: verlocdat vercoldat and veridxdat  
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)NULL);
  glEnableVertexAttribArray(0);  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);  

  spherePos = glm::vec3(0.5f, 0.5f, 0.5f);
  sphereScale = glm::vec3(1.0f, 1.0f, 1.0f);
  sphereRot = glm::vec2(0.0f, 0.0f);

  delete[] vertices;
  delete[] indices;  
}


void CustomRenderer::CustomCubeInit()
{   
  /*** Init VBOs ***/
  GLfloat vertices[24] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 1.0
  };
  // draw the six faces of the boundbox by drawwing triangles
  // draw it contra-clockwise
  // front: 1 5 7 3
  // back: 0 2 6 4
  // left：0 1 3 2
  // right:7 5 4 6    
  // up: 2 3 7 6
  // down: 1 0 4 5
  GLuint indices[36] = {
    1,5,7,
    7,3,1,
    0,2,6,
    6,4,0,
    0,1,3,
    3,2,0,
    7,5,4,
    4,6,7,
    2,3,7,
    7,6,2,
    1,0,4,
    4,5,1
  };

  GLuint gbo[2];

  glGenBuffers(2, gbo);
  GLuint vertexdat = gbo[0];
  GLuint veridxdat = gbo[1];
  glBindBuffer(GL_ARRAY_BUFFER, vertexdat);
  glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  // used in glDrawElement()
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &boxVAO);
  // vao like a closure binding 3 buffer object: verlocdat vercoldat and veridxdat
  glBindVertexArray(boxVAO);
  glEnableVertexAttribArray(0); // for vertexloc
  glEnableVertexAttribArray(1); // for vertexcol

  glBindBuffer(GL_ARRAY_BUFFER, vertexdat);  // the vertex location is the same as the vertex color
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat *)NULL);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat *)NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);     
}


void CustomRenderer::CustomSlicesInit()
{
  float sliceStep = 0.1f, currStep = 0.0f;  
  int texW = 0, texH = 0, slicesNum = 0, sliceSize = 0, currSlicePos = 0;
  unsigned char *sliceData = sliceLoader->GenerateDataArray(&texW, &texH, &slicesNum);
  
  numOfSlices = slicesNum;
  sliceSize = texW * texH;

  slicesVAO = new GLuint[slicesNum];  
  glGenVertexArrays(slicesNum, slicesVAO);  
  currStep = -slicesNum * sliceStep / 2.0f;

  for (int i = 0; i < slicesNum; i++)
  {
    /*** Init VBOs ***/
    GLfloat vertices[20] = {
      0.0, 0.0, currStep, 0.0, 0.0,
      0.0, 1.0, currStep, 0.0, 1.0,
      1.0, 0.0, currStep, 1.0, 0.0,
      1.0, 1.0, currStep, 1.0, 1.0,
    };

    GLuint indices[6] = {      
      0,1,3,
      3,2,0,      
    };

    GLuint gbo[2];

    glGenBuffers(2, gbo);
    GLuint vertexBuffObject = gbo[0];
    GLuint indexBuffObject = gbo[1];
    
    glBindVertexArray(slicesVAO[i]);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffObject);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);                

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    currStep += sliceStep;
  }   
  
  glGenTextures(1, &slicesTextureArray);       
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, slicesTextureArray);  
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R8, texW, texH, slicesNum);
  
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, texW, texH, slicesNum - 1, GL_RED, GL_UNSIGNED_BYTE, sliceData);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, slicesNum - 1, texW, texH, 1, GL_RED, GL_UNSIGNED_BYTE, sliceData + sliceSize * (slicesNum - 1));
    
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

  delete[] sliceData;
}


void CustomRenderer::LoadData(const char *filePath, bool showCube, long dataLen)
{
  const char *loadedFilePath = filePath;
  int errCode;

  if (dataLen == 0)
  {
    if (filePath != 0)
      memcpy(savedFilePath, filePath, 512 * sizeof(char));
    else
      loadedFilePath = savedFilePath;
  }    

  isShowCube = showCube;    
  if (showCube && !volumeInited)
  {
    if (dataLen != 0)
      errCode = modelLoader->LoadDataInternal(filePath, dataLen);
    else 
      errCode = modelLoader->LoadData(loadedFilePath);
    CustomCubeInit();    
    InitVolumeShaders();
    CustomSphereInit();
  }
  else if (!sliceInited)
  {
    if (dataLen != 0)
      errCode = sliceLoader->LoadDataInternal(filePath, dataLen);
    else
      errCode = sliceLoader->LoadData(loadedFilePath);     
    CustomSlicesInit();
    InitSliceShaders();
  }

  if (isCoordShown && !isCoordInited)
    CustomCoordInit();  
  dataLinked = true;
}

void CustomRenderer::CleanData()
{
  // Volume cleaning

  // Slice cleaning
}


void CustomRenderer::CoordSetup()
{
  if (!isCoordInited)
    CustomCoordInit();
}


void CustomRenderer::InitVolumeTexture(void)
{
  dataW = 0;
  dataH = 0;
  dataD = 0;
  unsigned char *data = modelLoader->GenerateDataArray(&dataW, &dataH, &dataD);

  volumeTexture = LoadVolumeTexture(data, dataW, dataH, dataD);
  delete[] data;
}


void CustomRenderer::InitVolumeShaders(void)
{
  /*** Init shaders ***/
  backShaderProgram = LoadShader("shader\\backface.vert", "shader\\backface.frag");
  frontShaderProgram = LoadShader("shader\\frontface.vert", "shader\\frontface.frag");
  volumeShaderProgram = LoadShader("shader\\raycasting.vert", "shader\\raycasting.frag");

  /*** Init textures ***/
  backfaceTexture = InitTexture(w, h);
  frontfaceTexture = InitTexture(w, h);

  InitVolumeTexture();

  /* Init buffers */
  InitFrameBuffer(w, h);

  InitShaderParams();

  volumeInited = true;
}


void CustomRenderer::InitSliceShaders(void)
{
  // Shader params
  slicesShaderProgram = LoadShader("shader\\slices.vert", "shader\\slices.frag");
  
  mvpSliceParam = glGetUniformLocation(slicesShaderProgram, "MVP");
  screenSizeSliceParam = glGetUniformLocation(slicesShaderProgram, "ScreenSize");
  opacitySliceParam = glGetUniformLocation(slicesShaderProgram, "Opacity");
  currSampleSliceParam = glGetUniformLocation(slicesShaderProgram, "CurrSample");
  texArraySliceParam = glGetUniformLocation(slicesShaderProgram, "TexArray");

  sliceInited = true;
}


void CustomRenderer::InitFrameBuffer(GLuint texWidth, GLuint texHeight)
{  
  GLuint depthBuffer[2], backDepthBuffer, frontDepthBuffer,
    frameBuffer[2];

  GLenum err = glGetError();

  glGenRenderbuffers(2, depthBuffer);
  backDepthBuffer = depthBuffer[0];
  frontDepthBuffer = depthBuffer[1];
  glBindRenderbuffer(GL_RENDERBUFFER, backDepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);

  glBindRenderbuffer(GL_RENDERBUFFER, frontDepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);

  // attach the texture and the depth buffer to the framebuffer
  glGenFramebuffers(2, frameBuffer);
  backFrameBuffer = frameBuffer[0];
  frontFrameBuffer = frameBuffer[1];
  glBindFramebuffer(GL_FRAMEBUFFER, backFrameBuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backfaceTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, backDepthBuffer);
        
  glBindFramebuffer(GL_FRAMEBUFFER, frontFrameBuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frontfaceTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frontDepthBuffer);


  #ifdef DEBUG  

  GLenum complete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (complete != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "framebuffer is not complete" << std::endl;
    exit(EXIT_FAILURE);
  }
  

  #endif // DEBUG  

  glEnable(GL_DEPTH_TEST);
}


void CustomRenderer::CustomRender(void)
{
  if (!dataLinked)
  {
    glViewport(0, 0, w, h);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    return;
  }

  if (isShowCube)
    CustomCubeRender();
  else
    CustomSlicesRender();       
}

void CustomRenderer::CustomCubeRender(void)
{   
  //  transform the box
  glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)w / h, 0.1f, 400.f);
  glm::mat4 view = glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f - (m_xRot / 16.0f)), glm::vec3(0.1f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_yRot / 16.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_zRot / 16.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f, -0.5f, -0.5f));

  glm::mat4 mvp = projection * view * model;

  glEnable(GL_DEPTH_TEST);
  // render to texture
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, backfaceTexture);
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, w, h);
  glUseProgram(backShaderProgram);

  glUniformMatrix4fv(mvpBackfaceParam, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(boxVAO);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
  glDisable(GL_CULL_FACE);

  glUseProgram(0);

  /*** Front buffer ***/

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frontfaceTexture);
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, w, h);
  glUseProgram(frontShaderProgram);

  glUniformMatrix4fv(mvpFrontfaceParam, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(boxVAO);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
  glDisable(GL_CULL_FACE);

  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, w, h);    
  

  if (isCoordShown)
  {
    glUseProgram(frontShaderProgram);

    glUniformMatrix4fv(mvpFrontfaceParam, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(sysCoordVAO);
    glDrawElements(GL_LINES, 3, GL_UNSIGNED_INT, (GLuint *)NULL);

    glUseProgram(0);
  }

  /*** Volume shader step ***/

  glm::mat4 sphereRotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-sphereRot.x), glm::vec3(0.1f, 0.0f, 0.0f));
  sphereRotMatrix = glm::rotate(sphereRotMatrix, glm::radians(-sphereRot.y), glm::vec3(0.0f, 1.0f, 0.0f));  

  glUseProgram(volumeShaderProgram);

  glUniformMatrix4fv(sphereRotMatrixParam, 1, GL_FALSE, &sphereRotMatrix[0][0]);
  glUniformMatrix4fv(mvpVolumeParam, 1, GL_FALSE, &mvp[0][0]);
  SetShaderParams();

  glBindVertexArray(boxVAO);  
  
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);  
  glDisable(GL_BLEND);

  glEnable(GL_CULL_FACE);
  glUseProgram(0);  
}

void CustomRenderer::CustomSlicesRender(void)
{
  // transform world
  glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)w / h, 0.1f, 400.f);
  glm::mat4 view = glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f - (m_xRot / 16.0f)), glm::vec3(0.1f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_yRot / 16.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_zRot / 16.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));

  glm::mat4 mvp = projection * view * model;

  glEnable(GL_DEPTH_TEST);
  // render to texture    

  glViewport(0, 0, w, h);
  glUseProgram(slicesShaderProgram);  
  
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, slicesTextureArray);
  glUniform1i(texArraySliceParam, 0);

  glUniformMatrix4fv(mvpSliceParam, 1, GL_FALSE, &mvp[0][0]);
  glUniform2f(screenSizeSliceParam, (float)w, (float)h);
  glUniform1f(opacitySliceParam, opacity);          
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (int i = numOfSlices - 1; i >= 0; i--)
  {
    glUniform1f(currSampleSliceParam, (float)i);
    glBindVertexArray(slicesVAO[i]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLuint *)NULL);
  }  

  glDisable(GL_BLEND);  
  glUseProgram(0);
}



static void qNormalizeAngle(int &angle)
{
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}


void CustomRenderer::setXRotation(int angle)
{
  qNormalizeAngle(angle);
  if (angle != m_xRot) 
  {
    m_xRot = angle;    
  }
}

void CustomRenderer::setYRotation(int angle)
{
  qNormalizeAngle(angle);
  if (angle != m_yRot) 
  {
    m_yRot = angle;  
  }
}

void CustomRenderer::setZRotation(int angle)
{
  qNormalizeAngle(angle);
  if (angle != m_zRot) 
  {
    m_zRot = angle;    
  }
}


void CustomRenderer::CustomResponse(void)
{
  /*if (!mouseLButtonDown)
    return;*/
  
  static const float rotSpeed = 0.5f;  

  if (mouseLButtonDown) 
  {
    setXRotation(m_xRot + 8 * dmy);
    setYRotation(m_yRot + 8 * dmx);
  }
  else if (mouseRButtonDown) 
  {
    setXRotation(m_xRot + 8 * dmy);
    setZRotation(m_zRot + 8 * dmx);
  }

  /*hAngle += dmx / 1000.0f;
  vAngle += dmy / 1000.0f;

  if (hAngle > 360.0f)
    hAngle -= 360.0f;
  else if (hAngle < 0.0f)
    hAngle += 360.0f;
  if (vAngle > 80.0f)
    vAngle = 80.0f;
  else if (vAngle < -80.0f)
    vAngle = -80.0f;*/
}


void CustomRenderer::CustomReshape(void)
{
  if (volumeInited)
  {
    glBindTexture(GL_TEXTURE_2D, backfaceTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, frontfaceTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, backFrameBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, frontFrameBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }
}


void CustomRenderer::CustomClose(void)
{
  glDeleteFramebuffers(1, &backFrameBuffer);
  glDeleteFramebuffers(1, &frontFrameBuffer);
}

GLuint CustomRenderer::LoadShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Read shaders
  const char *vertShaderSrc = ReadShaderFile(vertexShaderPath);
  const char *fragShaderSrc = ReadShaderFile(fragmentShaderPath);  

  // Compile vertex shader  
  glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
  glCompileShader(vertShader);

  #ifdef DEBUG

  int success;
  char infoLog[512];
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  #endif // DEBUG  

  // Compile fragment shader
  glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
  glCompileShader(fragShader);  
  
  #ifdef DEBUG
    
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  #endif // DEBUG
  

  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  #ifdef DEBUG
   
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  #endif // DEBUG  

  delete[] vertShaderSrc;
  delete[] fragShaderSrc;

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  return program;
}

GLuint CustomRenderer::InitTexture(GLuint texWidth, GLuint texHeight)
{
  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texWidth, texHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  
  return texture;
}

void CustomRenderer::InitShaderParams()
{
  mvpBackfaceParam = glGetUniformLocation(backShaderProgram, "MVP");
  mvpFrontfaceParam = glGetUniformLocation(frontShaderProgram, "MVP");
  mvpVolumeParam = glGetUniformLocation(volumeShaderProgram, "MVP");
  screenSizeParam = glGetUniformLocation(volumeShaderProgram, "ScreenSize");  
  stepSizeParam = glGetUniformLocation(volumeShaderProgram, "StepSize");  
  clipPlaneParamL = glGetUniformLocation(volumeShaderProgram, "ClipPlaneL");
  clipPlaneParamR = glGetUniformLocation(volumeShaderProgram, "ClipPlaneR");
  opacityParam = glGetUniformLocation(volumeShaderProgram, "Opacity");
  backfaceTextureParam = glGetUniformLocation(volumeShaderProgram, "ExitPoints");  
  frontfaceTextureParam = glGetUniformLocation(volumeShaderProgram, "EntryPoints");
  volumeTextureParam = glGetUniformLocation(volumeShaderProgram, "VolumeTex");  
  spherePosParam = glGetUniformLocation(volumeShaderProgram, "SpherePos");
  sphereScaleParam = glGetUniformLocation(volumeShaderProgram, "SphereScale");
  sphereRotMatrixParam = glGetUniformLocation(volumeShaderProgram, "SphereRotMatrix");
}


void CustomRenderer::SetShaderParams()
{    
  glUniform2f(screenSizeParam, (float)w, (float)h);    
  
  glUniform1f(stepSizeParam, stepSize);

  glUniform1f(opacityParam, opacity);

  glUniform3f(clipPlaneParamL, clipPlaneXL, clipPlaneYL, clipPlaneZL);

  glUniform3f(clipPlaneParamR, clipPlaneXR, clipPlaneYR, clipPlaneZR);      

  glUniform3f(spherePosParam, spherePos.x, spherePos.y, spherePos.z);

  glUniform3f(sphereScaleParam, sphereScale.x, sphereScale.y, sphereScale.z);  

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, backfaceTexture);
  glUniform1i(backfaceTextureParam, 0);
  
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, frontfaceTexture);
  glUniform1i(frontfaceTextureParam, 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, volumeTexture);
  glUniform1i(volumeTextureParam, 2);

}

int write_file_cpp(unsigned char *pOutData, int out_len)
{
  std::basic_ofstream<unsigned char> file_stream;

  file_stream.open("out.txt", std::ios::out);

  if (!file_stream.is_open())
  {
    std::cout << stderr << "Can't open output file !\n";
    exit(1);
  }
  else
  {
    file_stream.write(pOutData, out_len);
  }

  file_stream.close();

  return 0;
}

GLuint CustomRenderer::LoadVolumeTexture(byte *data, int dataWidth, int dataHeight, int dataDepth)
{
  GLuint texture;

  glGenTextures(1, &texture);  
  glBindTexture(GL_TEXTURE_3D, texture);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, dataWidth, dataHeight, dataDepth, 0, GL_RED, GL_UNSIGNED_BYTE, data);

  return texture;
}

const char *CustomRenderer::ReadShaderFile(const char *filePath)
{  
  char *shaderData;
  unsigned totalLen = 0;
  std::string content;
  std::ifstream fileStream(filePath, std::ios::in);

  if (!fileStream.is_open())
  {
    std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
    return "";
  }

  std::string line = "";
  while (!fileStream.eof()) 
  {
    std::getline(fileStream, line);
    totalLen += line.length() + 1;
    content.append(line + "\n");
  }

  fileStream.close();

  shaderData = new char[content.size() + 1];
  strcpy_s(shaderData, content.size() + 1, content.c_str());  

  return shaderData;
}

