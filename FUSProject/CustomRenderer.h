#ifndef _CUSTOMRENDERER_H_
#define _CUSTOMRENDERER_H_

#include "OGLRenderer.h"
#include "DataLoader.h"

#include "glm\glm.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"

#define PI (float)3.14159265358979323846

class CustomRenderer : public OGLRenderer
{
public:  
  DataLoader *modelLoader;
  DataLoader *sliceLoader;

  bool mouseLButtonDown, mouseRButtonDown, 
    isShowCube, 
    volumeInited, sliceInited,
    isCoordShown, isCoordInited;

  float opacity;
  float clipPlaneXL, clipPlaneXR;
  float clipPlaneYL, clipPlaneYR;
  float clipPlaneZL, clipPlaneZR;

  CustomRenderer();

  ~CustomRenderer();  

  void LoadData(const char *filePath, bool showCube, long dataLen = 0);

  void UpdateWheelPos(int wheelPos);
  
  void CoordSetup();

  void CleanData(void);

  void GetEllipseData(int *center, float *scale, float *angles);

  /* Custom functions */

  void CustomInit(void);
  void CustomCoordInit(void);
  void CustomCubeInit(void);
  void CustomSphereInit(void);
  void CustomSlicesInit(void);

  void CustomRender(void);
  void CustomCubeRender(void);
  void CustomSlicesRender(void);

  void CustomResponse(void);
  void CustomReshape(void);
  void CustomClose(void);

  /* OGL-related variables*/
  GLuint boxVAO;  

  GLuint backShaderProgram;
  GLuint frontShaderProgram;
  GLuint volumeShaderProgram;

  GLuint backfaceTexture;
  GLuint frontfaceTexture;  
  GLuint volumeTexture;

  GLuint backFrameBuffer;
  GLuint frontFrameBuffer;  

  GLint mvpBackfaceParam;
  GLint mvpFrontfaceParam;
  GLint mvpVolumeParam;
  GLint screenSizeParam;
  GLint stepSizeParam;
  GLint clipPlaneParamL;
  GLint clipPlaneParamR;
  GLint opacityParam;
  GLint backfaceTextureParam;
  GLint frontfaceTextureParam;  
  GLint volumeTextureParam;  

  
  GLuint *slicesVAO;
  int numOfSlices;
  GLuint slicesTextureArray;
  
  GLuint slicesShaderProgram;
  GLint mvpSliceParam;
  GLint screenSizeSliceParam;
  GLint opacitySliceParam;
  GLint currSampleSliceParam;
  GLint texArraySliceParam;


  GLuint sphereVAO;
  int n, m;
  float radius;
  glm::vec3 spherePos;
  glm::vec3 sphereScale;
  glm::vec2 sphereRot;
  
  GLint sphereRotMatrixParam;
  GLint spherePosParam;
  GLint sphereScaleParam;
  

  GLuint sysCoordVAO;

private:  
  char savedFilePath[512];

  bool dataLinked;
  float stepSize;

  float hAngle, vAngle;
  float distDelta;
  glm::vec3 pos;  

  int dataW, dataH, dataD;

  float m_xRot;
  float m_yRot;
  float m_zRot;

  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);

  GLuint LoadShader(const char *vertexShaderPath, const char *fragmentShaderPath);

  GLuint InitTexture(GLuint texWidth, GLuint texHeight);

  void InitVolumeTexture(void);

  void InitVolumeShaders(void);

  void InitSliceShaders(void);

  void InitFrameBuffer(GLuint texWidth, GLuint texHeight);

  void InitShaderParams();

  void SetShaderParams();

  GLuint LoadVolumeTexture(byte *data, int dataWidth, int dataHeight, int dataDepth);

  const char *ReadShaderFile(const char *filePath);
};


#endif /* _CUSTOMRENDERER_H_ */