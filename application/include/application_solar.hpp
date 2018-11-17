#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scenegraph.hpp"
#include "geometrynode.hpp"
#include "cameranode.hpp"
#include "pointLightNode.hpp"

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
  void planetRendering(Node* i) const;
  //void drawPlanets() const;

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  //initializes the SceneGraph with all the planets
  SceneGraph initializeSceneGraph() const;
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  void initializeStars();
  void drawStars() const;

  // cpu representation of model
  model_object planet_object;
  model_object star_object;

  //container for stars
  std::vector<float> stars_;

  PointLightNode light_;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  SceneGraph solar_;
};

#endif