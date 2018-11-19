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
  
  PointLightNode sunlight{40.0f, glm::vec3{0.4, 0.5, 0.5}};
  light_ = sunlight;
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
  Node root{"root", 1};

  glm::mat4 local= glm::mat4{};
  GeometryNode sun {"sun", 0, 1.0f, 0.0f, 0.0f, 0.0f};
  sun.setLocalTransform(local);
  sun.setPlanetColor(glm::vec3{1.0, 0.5, 0.2});

  GeometryNode mercury {"mercury", 1, 0.383f, 3.012f, 2.0f, 1.0f};
  mercury.setLocalTransform(local);
  mercury.setPlanetColor(glm::vec3{0.4, 0.5, 0.5}); 

  GeometryNode venus{"venus",1, 0.950f, 1.177f, 3.723f, 1.0f};
  venus.setLocalTransform(local);
  venus.setPlanetColor(glm::vec3{0.5, 0.8, 0.0});

  GeometryNode earth{"earth", 1, 1.0f, 1.0f, 5.0f, 1.0f};
  earth.setLocalTransform(local); 
  earth.setPlanetColor(glm::vec3{0.0, 0.8, 0.3});

  GeometryNode moon{"moon", 2, 0.003f, 1.0f, 7.01f, 1.0f};
  moon.setLocalTransform(local);
  moon.setPlanetColor(glm::vec3{0.8, 0.8, 0.8});

  GeometryNode mars{"mars",1, 0.583f, 0.53f, 9.524f,1.0f};
  mars.setLocalTransform(local);
  mars.setPlanetColor(glm::vec3{0.8, 0.0, 0.0});

  GeometryNode jupiter{"jupiter", 1, 10.97f, 0.084f, 11.2f, 1.0};
  jupiter.setLocalTransform(local);
  jupiter.setPlanetColor(glm::vec3{0.7, 0.5, 0.1});

  GeometryNode saturn{"saturn", 1, 9.14f, 0.0339f, 13.54f, 1.0};
  saturn.setLocalTransform(local);
  saturn.setPlanetColor(glm::vec3{0.9, 0.8, 0.5});

  GeometryNode uranus{"uranus", 1, 3.98f, 0.0119f, 15.19f, 1.0};
  uranus.setLocalTransform(local);
  uranus.setPlanetColor(glm::vec3{0.7, 0.8, 1.0});

  GeometryNode neptune{"neptune",1, 3.87f, 0.6f, 17.1f, 1.0};
  neptune.setLocalTransform(local);
  neptune.setPlanetColor(glm::vec3{0.0, 0.2, 1.0});

  //PointLightNode light{10.0f, glm::vec3{1.0f, 1.0f, 1.0f}};

  root.addChildren(std::make_shared<GeometryNode>(sun));
  root.addChildren(std::make_shared<GeometryNode>(mercury));
  root.addChildren(std::make_shared<GeometryNode>(venus));
  root.addChildren(std::make_shared<GeometryNode>(earth));

  earth.addChildren(std::make_shared<GeometryNode>(moon));

  root.addChildren(std::make_shared<GeometryNode>(mars));
  root.addChildren(std::make_shared<GeometryNode>(jupiter));
  root.addChildren(std::make_shared<GeometryNode>(saturn));
  root.addChildren(std::make_shared<GeometryNode>(uranus));
  root.addChildren(std::make_shared<GeometryNode>(neptune));

  //root.addChildren(std::make_shared<PointLightNode>(light));
  

  SceneGraph solarsystem {"solarsystem", std::make_shared<Node>(root)};
  return solarsystem;
}


void ApplicationSolar::render() const {

  //for now its just traversing through a list not the actual tree
/*
  for(auto i : solar_.getRoot()->getListOfChildren()) {
      planetRendering(i);
  }*/

  planetRendering();

  //draws srars
  drawStars();

}

//assignment 1 draw planets
void ApplicationSolar::planetRendering() const {

    model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
    // bind shader to upload uniforms
   glUseProgram(m_shaders.at("planet").handle);

  for (auto& i: solar_.getRoot()->getListOfChildren()) {

        if (i->getName() == "sun") {
          glm::fmat4 model_matrix = i->getLocalTransform();
          //no transformations since sun isn't moving or rotating
          
          //sending planetcolor at the shader since uniform is only accepting floats we have to split the color vec3
          glm::vec3 planetcol = i->getPlanetColor();
          glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), planetcol.x, planetcol.y, planetcol.z);
            /*
           //sending lightsettings at the shader
          glm::vec3 pointlightcol = light_.getColor();
          glUniform3f(m_shaders.at("planet").u_locs.at("LightColor"), pointlightcol.x, pointlightcol.y, pointlightcol.z);
          glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"), light_.getLightIntensity());*/


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

        } else if(i->getDepth() == 1)  {

        //glm::fmat4 model_matrix = glm::fmat4{};
        glm::fmat4 model_matrix = glm::rotate(i->getLocalTransform(), float(glfwGetTime()*(i->getRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});
         model_matrix = glm::translate (model_matrix, glm::vec3{0.0f, 0.0f, -1.0f*i->getDistance()});
        model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*(i->getSelfRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});

         //scale the planet according to their diameter
         glm::vec3 planetSize {i->getDiameter(), i->getDiameter(), i->getDiameter()};
         //model_matrix = glm::scale(model_matrix, planetSize);

        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

        // extra matrix for normal transformation to keep them orthogonal to surface
        glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

         //gives planet specific color - first parameter is location in fragment shader second to forth are the color values
         glm::vec3 planetcol = i->getPlanetColor();
          glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), planetcol.x, planetcol.y, planetcol.z);

        /* set lighting for planet */
        glm::vec3 pointlightcol = light_.getColor();
        glUniform3f(m_shaders.at("planet").u_locs.at("LightColor"), pointlightcol.x, pointlightcol.y, pointlightcol.z);
        glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"), light_.getLightIntensity());

          // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);



          //check if the current planet has moons surounding it
          if(i->getListOfChildren().size() > 0) {
            for (auto& moon: i->getListOfChildren()) {

                  glm::fmat4 moon_matrix = glm::rotate(moon->getParent()->getLocalTransform(), float(glfwGetTime())*(moon->getParent()->getRotation()), glm::fvec3{0.0f, 1.0f, 0.0f});
                  moon_matrix = glm::translate (moon_matrix, glm::vec3{0.0f, 0.0f, -1.0f*moon->getParent()->getDistance()});

                  moon_matrix = glm::rotate(moon_matrix*moon->getLocalTransform(), float(glfwGetTime()*(moon->getRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});
                  moon_matrix = glm::translate (moon_matrix, glm::vec3{0.0f, 0.0f, -1.0f*moon->getDistance()});
                  //model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*(moon->getSelfRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});

                  //scale the planet according to their diameter
                  glm::vec3 planetSize {moon->getDiameter(), moon->getDiameter(), moon->getDiameter()};
                  //model_matrix = glm::scale(model_matrix, planetSize);

                  //gives planet specific color - first parameter is location in fragment shader second to forth are the color values
                  glm::vec3 mooncol = moon->getPlanetColor();
                    glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), mooncol.x, mooncol.y, mooncol.z);

                  /* set lighting for planet */
                 glm::vec3 moonpointlightcol = light_.getColor();
                  glUniform3f(m_shaders.at("planet").u_locs.at("LightColor"), moonpointlightcol.x, moonpointlightcol.y, moonpointlightcol.z);
                  glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"), light_.getLightIntensity());
                  

                  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                              1, GL_FALSE, glm::value_ptr(moon_matrix));

                  // extra matrix for normal transformation to keep them orthogonal to surface
                  glm::fmat4 moon_normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * moon_matrix);
                  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                              1, GL_FALSE, glm::value_ptr(moon_normal_matrix));

                    // bind the VAO to draw
                  glBindVertexArray(planet_object.vertex_AO);

                  // draw bound vertex array using bound shader
                  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
            }
          }
        }
  }
}

//does the actual drawing of the stars
void ApplicationSolar::drawStars() const {
  std::cout<<"drawStars called\n";
  glUseProgram(m_shaders.at("star").handle);

  // bind the VAO to draw
  glBindVertexArray(star_object.vertex_AO);

  glPointSize (2.0f);
    // draw bound vertex array using bound shader
  glDrawArrays(star_object.draw_mode, 0, (int)stars_.size());

}


//is called each time the camera changes position
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
  //you cant't put multiple glUsePrograms in here so they have to be moved to the different functions
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

  //request uniform location for shader which is the color of the planet
  m_shaders.at("planet").u_locs["PlanetColor"] = -1;
  m_shaders.at("planet").u_locs["LightColor"] = -1;
  m_shaders.at("planet").u_locs["LightIntensity"] = -1;

  //shader for stars
   // store shader program objects in container
  std::cout<<"greetings from initializeShader\n";
  
 m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  std::cout<<"emplace-planet\n";
  // request uniform locations for shader program 
  //you dont't need a normal or modelmatrix since they are only drawn as points 
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;
  std::cout<<"hello there general kenobi\n";
}

// load models -> is know more like initializePlanets
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

  //AttributePointer sets color stuff

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

//assignment 2 initialize the stars
void ApplicationSolar::initializeStars(){
    std::cout<<"initialize Stars\n";

    //initialize with rgb rand and pos rand seperatly
    //fill star verctor with random numbers
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),(void*)0); //could also be 3*sizeof float
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats; same as above 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)3); //(sizeof(float)*3)
  std::cout<<"initialize Stars-vertex\n";

    // store type of primitive to draw stars are points
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(stars_.size()/6); 
  std::cout<<"came so far\n";
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