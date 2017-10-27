#version 400

in vec3 EntryPointT;
in vec4 ExitPointCoord;

uniform sampler2D ExitPoints;
uniform sampler2D EntryPoints;
uniform sampler3D VolumeTex;
uniform float StepSize;
uniform vec2 ScreenSize;
uniform vec3 ClipPlaneL;
uniform vec3 ClipPlaneR;
uniform float Opacity;
uniform vec3 SpherePos;
uniform vec3 SphereScale;
uniform mat4 SphereRotMatrix;

out vec4 FragColor;

void main()
{       
    float sphereRad = 0.2;
    
    // screencoordinates
    vec2 tc = gl_FragCoord.st / ScreenSize; //float3 tc = pix.screenSpacePos.xyz / pix.screenSpacePos.w * 0.5 + 0.5;
    
    // get front, back pos for ray in [0, 1] cube
    vec3 backObj = texture(ExitPoints, tc).xyz;
    vec3 frontObj = texture(EntryPoints, tc).xyz;

    // ray throush the volume
    vec3 dir = backObj - frontObj;
    float dist = distance(frontObj, backObj);
    float step = StepSize;
    vec3 stepDir = step * dir, 
      spherePosVec = vec3(0),
      sphereScaleVec = SphereScale * sphereRad;

    // walk along the ray sampling the volume
    vec3 pos = frontObj;
    float alpha = 0,       
      density = 0,      
      compositeTransparency = 1;
    vec3 color = vec3(0),
      compositeColor = vec3(0);          
    vec4 sampledColor = vec4(0, 0, 0, 1), 
      rotPos = vec4(0);

    for (int i = 0; i < 450; i++)
    {
      if (distance(pos, backObj) < step * 0.5) 
        break; // check when reach the back  

      if (pos.x < ClipPlaneL.x || pos.y < ClipPlaneL.y || pos.z < ClipPlaneL.z ||
          pos.x > ClipPlaneR.x || pos.y > ClipPlaneR.y || pos.z > ClipPlaneR.z)
      {
        pos += stepDir;
        continue;
      }
      
      density = texture(VolumeTex, pos).r;                 
      
      sampledColor = vec4(density, density, density, (1.0 - Opacity) * 0.5);            
      alpha = density * sampledColor.w;

      spherePosVec = pos - SpherePos;
      rotPos = SphereRotMatrix * vec4(spherePosVec, 1);      
      if ((rotPos.x * rotPos.x) / (sphereScaleVec.x * sphereScaleVec.x) + 
          (rotPos.y * rotPos.y) / (sphereScaleVec.y * sphereScaleVec.y) + 
          (rotPos.z * rotPos.z) / (sphereScaleVec.z * sphereScaleVec.z) <= 1)      
        sampledColor += vec4(0.8, 0.0, 0.0, 0.0);
      
      compositeColor += alpha * sampledColor.xyz * compositeTransparency;
      compositeTransparency *= (1 - alpha);
      
      pos += stepDir;
    }
    color = compositeColor;
    // temp color    

  FragColor = vec4(color, 1 - compositeTransparency);
  //FragColor = vec4(dist, 0.0, 0.0, 1.0); 
  //FragColor = vec4(frontObj, 1.0);
  //FragColor = vec4(backObj, 1.0);
}