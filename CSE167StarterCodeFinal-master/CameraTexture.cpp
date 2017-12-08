#include "CameraTexture.h"

CameraTexture::CameraTexture(int priority) {
  this->priority = priority;
  
  // generate texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::width, Window::height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glGenRenderbuffers(1, &depthBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 
      Window::width, Window::height);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
      GL_TEXTURE_2D, texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D, depthBuf);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, texture, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CameraTexture::takeSnapShot(glm::vec3 camPos, glm::vec3 camLookAt, 
    glm::vec3 camUp, glm::vec4 clipPlane) {
  // store window parameter
  glm::mat4 windowV = Window::V;
  Window::V = glm::lookAt(camPos, camLookAt, camUp);

  glEnable(GL_CLIP_DISTANCE0);
  // set clip distance for all shader
  for (int i = 0; i < Window::shaderNum; i++) {
    glUseProgram(Window::shader[i]);
    glUniform4f(glGetUniformLocation(Window::shader[i], "clipPlane"),
        clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
    glUseProgram(0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, texture, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  Window::render(priority);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  Window::V = windowV;
  glDisable(GL_CLIP_DISTANCE0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}
