#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,solar_{}
{
  initializeGeometry();
  initializeShaderPrograms();
  //initializeSceneGraph();
  solar_ = initializeSceneGraph();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

SceneGraph ApplicationSolar::initializeSceneGraph() {
  GeometryNode* sun = new GeometryNode{"sun", 1.0f, 0.0f,0.0f};  
  SceneGraph solarsystem {"solarsystem", sun};
  //model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
/*
  planet sun{1.0f, 0.0f, 0.0f};
  planet Merkur(0.383f, 3.012f, 0.5f);
  planet Venus(0.950f, 1.177f, 0.723f);
  planet Erde(1.0f, 1.0f, 1.0f);
  planet Mars(0.583f, 0.53f, 1.524f);
  planet Jupiter(10.97f, 0.084f, 4.2f);
  planet Saturn(9.14f, 0.0339f, 6.54f);
  planet Uranus(3.98f, 0.0119f, 8.19f);
  planet Neptun(3.87f, 0.006f, 9.1f);*/

  GeometryNode* mercury = new GeometryNode{"mercury", 0.383f, 3.012f, 0.5f};
  sun->addChildren(mercury);
  //sun->setGeometry(planet_model);
  GeometryNode* venus = new GeometryNode{"venus",0.950f, 1.177f, 0.723f};
  sun->addChildren(venus);
  GeometryNode* earth = new GeometryNode{"earth", 1.0f, 1.0f, 1.0f};
  sun->addChildren(earth);
  GeometryNode* moon = new GeometryNode{"moon", 0.003f, 1.0f, 1.01f};
  earth->addChildren(moon);
  GeometryNode* mars = new GeometryNode{"mars", 0.583f, 0.53f, 1.524f};
  sun->addChildren(mars);
  GeometryNode* jupiter = new GeometryNode{"jupiter", 10.97f, 0.084f, 4.2f};
  sun->addChildren(jupiter);
  GeometryNode* saturn = new GeometryNode{"saturn", 9.14f, 0.0339f, 6.54f};
  sun->addChildren(saturn);
  GeometryNode* uranus = new GeometryNode{"uranus", 3.98f, 0.0119f, 8.19f};
  sun->addChildren(uranus);
  GeometryNode* neptune = new GeometryNode{"neptune",3.87f, 0.006f, 9.1f};
  sun->addChildren(neptune);
/*
  for(auto& i: solarsystem.getRoot()->getListOfChildren()){
    i->setGeometry(planet_model); //not quite working yet
  }*/
  return solarsystem;

}


void ApplicationSolar::render() const {

  for (auto& i: solar_.getRoot()->getListOfChildren()) {

  
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL)
  }
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  //moves forwards
  if ((key == GLFW_KEY_W || key == GLFW_KEY_UP)  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  //moves backwards
  else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN)  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  //moves to the left
  else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT)  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.1f});
    uploadView();
  }
  //moves to the right
  else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.1f});
    uploadView();
  }
  //moves up
  else if (key == GLFW_KEY_U  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, 0.1f});
    uploadView();
  }
  //moves down
  else if (key == GLFW_KEY_SPACE  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.1f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling; not perfected yet
  m_view_transform = glm::rotate(m_view_transform, float(pos_x)/100, glm::fvec3{0.0f, -1.0f, 0.0f});
  m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, float(pos_y)/100, 0.0f});
  uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}