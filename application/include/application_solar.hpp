#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scenegraph.hpp"
#include "geometrynode.hpp"
#include "cameranode.hpp"
#include "pointLightNode.hpp"
#include "texture_loader.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;

  //draw planets
  void planetRendering() const;

  //load textures into container 
  void loadTextures();

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeFramebuffer();//buffer
  void initializeScreenQuad();//quad
  //initializes the SceneGraph with all the planets
  SceneGraph initializeSceneGraph() const;
  //initialize textures for further use
  void initializeTextures();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  void initializeStars();
  void drawStars() const;

  void initializeSkyBox();
  void drawSkybox() const;
  void makeQuad() const;

  //initializes framebuffer
  void initializeFramebuffer();

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object skybox_object;
  texture_object skybox_tex_obj; 

  // buffer objects
  renderbuffer_object rb_object;
  framebuffer_object fb_object;

  texture_object fb_tex_object;
  texture_object quad_tex_object;

  // quad object
  model_object quad_object;

  GLenum draw_buffers[1];
  GLenum status; 

  //container for stars
  std::vector<float> stars_;
  //container for textures
  //std::vector<texture> texContainer_;
  std::vector<pixel_data> texContainer_;

  //container to store texture objects
  std::vector<texture_object> texObj_;

  //skybox
  std::vector<pixel_data> skybox_container;

  //objects for framebuffer
  framebuffer_object fbo_obj;
  renderbuffer_object ren_obj;

  //texture object for framebuffer
  texture_object fbo_tex;


  //lightsource for solarsystem 
  PointLightNode light_;

  //ShaderMode for switching between blinn phong and cell-shading
  int shader_Mode;

  //post-processing effect 
  bool greyscale_Mode = false;
  bool horizontal_Mode = false;
  bool vertical_Mode = false;
  bool blur_Mode = false;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  SceneGraph solar_;
};

#endif
