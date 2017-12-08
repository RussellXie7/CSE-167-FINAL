#include "CameraTexture"

CameraTexture::CameraTexture(int priority) {
  this->priority = priority;
  
  // generate texture
  glGenTexture(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::width / 2, Window::height / 2, 0
      GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glGenFrameBuffers(1, &FBO);
  glBindFrameBuffer(GL_FRAMEBUFFER, FBO);
  glGenRenderBuffer(1, &depthBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 
      Window::width / 2, Window::height / 2);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
      GL_TEXTURE_2D, texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D, texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, texture, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CameraTexture::takeSnapShot(glm::vec3 camPos, glm::vec3 camLookAt, glm::vec4 clipPlane) {
  // store window parameter
  glm::mat4 windowV = Window::V;
  Window::V = glm::lookAt(camPos, camLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

  glEnable(GL_CLIP_DISTANCE_0);

  // set clip distance for all shader
  for (int i = 0; i < Window::shaderNum; i++) {
    glUniform4f(glGetUniformLocation(Window::shader[i], "clipPlane"),
        clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
  }

  glViewPort(0, 0, Window::width/2, Window::height/2);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFERm GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, texture, 0);

  Window::render(priority);

  glDisable(GL_CLIP_DISTANCE_0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, Window::width, Window::height);
  Window::V = windowV;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}
