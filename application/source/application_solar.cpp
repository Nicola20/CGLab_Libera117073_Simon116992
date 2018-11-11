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
 //,star_object{}
 //,stars_{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeStars();
  solar_ = initializeSceneGraph();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);

  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

SceneGraph ApplicationSolar::initializeSceneGraph() const {
  //initializes solarsystemgraph
  GeometryNode* sun = new GeometryNode{"sun", 1.0f, 0.0f, 0.0f};  
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

  SceneGraph solarsystem {"solarsystem", sun};
  return solarsystem;
}


void ApplicationSolar::render() const {

  //auto sol = solar_.getRoot();
/*
  while(sol->getListOfChildren().size() > 0){
      auto i = sol->getFirstChild(sol);
      planetRendering(i);
      sol = i->getFirstChild(i);
  }*/
  //for now its just traversing through a list not the actual tree
  for(auto i : solar_.getRoot()->getListOfChildren()) {
      planetRendering(i);
  }
  //drawPlanets();
  drawStars();

}

//assignment 1 draw planets
void ApplicationSolar::planetRendering(Node* i) const {

        model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
          // bind shader to upload uniforms
        glUseProgram(m_shaders.at("planet").handle);

        glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()*(i->getRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});
         //model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f+((*i).getDistance()), 0.0f, 1.0f}); //o,o ,-1*distance
         model_matrix = glm::translate (model_matrix, glm::vec3{0.0f, 0.0f, -1.0f*i->getDistance()});
         glm::vec3 planetSize {i->getDiameter(), i->getDiameter(), i->getDiameter()};
         //model_matrix = glm::scale(model_matrix, planetSize);

        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

        // extra matrix for normal transformation to keep them orthogonal to surface
        glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

          // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    //}
}

void ApplicationSolar::drawStars() const {
  std::cout<<"drawStars called\n";
  glUseProgram(m_shaders.at("star").handle);

  // bind the VAO to draw
  glBindVertexArray(star_object.vertex_AO);

  glPointSize (2.0f);
    // draw bound vertex array using bound shader
  glDrawArrays(star_object.draw_mode, 0, (int)stars_.size());

}
/*
void ApplicationSolar::drawPlanets() const {
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
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
}*/

//assignment 2
void ApplicationSolar::initializeStars(){
    std::cout<<"initialize Stars\n";
    for(int i=0; i< 1000*6;++i){
      float j = (float)((rand()%300)-250);
      stars_.push_back(j);
    }
    //here same as in initializeGeometry
    glGenVertexArrays(1, &star_object.vertex_AO);
    glBindVertexArray(star_object.vertex_AO);

    glGenBuffers(1, &star_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (float)stars_.size(), stars_.data(), GL_STATIC_DRAW);
    std::cout<<"initialize Stars-Buffer\n";

      // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with stride of 6 floats(floats till next attribute)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0); //could also be 3*sizeof float
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats; same as above 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)3); //(sizeof(float)*3)
  std::cout<<"initialize Stars-vertex\n";

  // generate generic buffer
  //glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
  // configure currently bound array buffer
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(stars_.size()); //schmaybe durch 6 teilen
  std::cout<<"came so far\n";
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));


  std::cout<<"did star view upload\n";
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection)); 
  std::cout<<"greeetings from uploadProjection\n";

  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection)); 
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  std::cout<<"greetings from uploadUniforms\n"; 
  // bind shader to which to upload unforms
  
  //glUseProgram(m_shaders.at("planet").handle);
  //glUseProgram(m_shaders.at("star").handle);
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

  //shader for stars
   // store shader program objects in container
  std::cout<<"greetings from initializeShader\n";
  
 m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  std::cout<<"emplace-planet\n";
  // request uniform locations for shader program  
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;
  std::cout<<"hello there general kenobi\n";
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
  /*
  m_view_transform = glm::rotate(m_view_transform, float(pos_x)/100, glm::fvec3{0.0f, -1.0f, 0.0f});
  m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, float(pos_y)/100, 0.0f});
  uploadView();*/
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