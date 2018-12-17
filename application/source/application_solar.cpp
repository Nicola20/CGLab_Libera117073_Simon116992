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
 ,rb_object{}//renderbuffer
 ,fb_object{}//framebuffer
 ,quad_tex_object{}//quad
 ,quad_object{}//quad
 ,shader_Mode{1} //default mode is 1 or blinn phong
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
<<<<<<< HEAD
  initializeFramebuffer();//buffer
  initializeScreenQuad();//quad
=======
  initializeFramebuffer();
>>>>>>> 64df44b684606d0ef6c7c94002bfceafb6e474ae
  loadTextures();
  initializeTextures();
  initializeSkyBox();//skybox

  //initialize star geometry
  initializeStars();
  //set scenefraph with planets
  solar_ = initializeSceneGraph();
  initializeShaderPrograms();
  //set pointlight for the scene
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

    //skybox
  glDeleteTextures(1, &skybox_tex_obj.handle);
}

/*--------------------------initializes scenegraph with all the components to compute with ---------------------------------------*/
SceneGraph ApplicationSolar::initializeSceneGraph() const {

  //initializes solarsystemgraph
  Node root{"root", 1};

  glm::mat4 local= glm::mat4{};
  GeometryNode sun {"sun", 0, 1.0f, 0.0f, 0.0f, 0.0f};
  sun.setLocalTransform(local);
  sun.setPlanetColor(glm::vec3{1.0, 0.5, 0.2});
  sun.setPlanetTex(texObj_[0]);

  GeometryNode mercury {"mercury", 1, 0.383f, 3.012f, 2.0f, 1.0f};
  mercury.setLocalTransform(local);
  mercury.setPlanetColor(glm::vec3{0.4, 0.5, 0.5});
  mercury.setPlanetTex(texObj_[1]); 

  GeometryNode venus{"venus",1, 0.950f, 1.177f, 3.723f, 1.0f};
  venus.setLocalTransform(local);
  venus.setPlanetColor(glm::vec3{0.5, 0.8, 0.0});
  venus.setPlanetTex(texObj_[2]);

  GeometryNode earth{"earth", 1, 1.0f, 1.0f, 5.0f, 1.0f};
  earth.setLocalTransform(local); 
  earth.setPlanetColor(glm::vec3{0.0, 0.8, 0.3});
  earth.setPlanetTex(texObj_[3]);

  GeometryNode moon{"moon", 2, 0.003f, 1.0f, 7.01f, 1.0f};
  moon.setLocalTransform(local);
  moon.setPlanetColor(glm::vec3{0.8, 0.8, 0.8});
  moon.setPlanetTex(texObj_[4]);

  GeometryNode mars{"mars",1, 0.583f, 0.53f, 9.524f,1.0f};
  mars.setLocalTransform(local);
  mars.setPlanetColor(glm::vec3{0.8, 0.0, 0.0});
  mars.setPlanetTex(texObj_[5]);

  GeometryNode jupiter{"jupiter", 1, 10.97f, 0.084f, 11.2f, 1.0};
  jupiter.setLocalTransform(local);
  jupiter.setPlanetColor(glm::vec3{0.7, 0.5, 0.1});
  jupiter.setPlanetTex(texObj_[6]);

  GeometryNode saturn{"saturn", 1, 9.14f, 0.0339f, 13.54f, 1.0};
  saturn.setLocalTransform(local);
  saturn.setPlanetColor(glm::vec3{0.9, 0.8, 0.5});
  saturn.setPlanetTex(texObj_[7]);

  GeometryNode uranus{"uranus", 1, 3.98f, 0.0119f, 15.19f, 1.0};
  uranus.setLocalTransform(local);
  uranus.setPlanetColor(glm::vec3{0.7, 0.8, 1.0});
  uranus.setPlanetTex(texObj_[8]);

  GeometryNode neptune{"neptune",1, 3.87f, 0.6f, 17.1f, 1.0};
  neptune.setLocalTransform(local);
  neptune.setPlanetColor(glm::vec3{0.0, 0.2, 1.0});
  neptune.setPlanetTex(texObj_[9]);

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

//main method with is doing all the drawing
void ApplicationSolar::render() const {
  //eigene Funktion besser?
  //define renderbuffer
  glBindFramebuffer(GL_FRAMEBUFFER, fb_object.handle);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  //draw skybox
  drawSkybox();

  //draws planets
  planetRendering();

  //draws srars
  drawStars();

  glBindFramebuffer(GL_FRAMEBUFFER, 0); //buffer

  //make quad
  makeQuad();

  

}

//assignment 1 draw planets
void ApplicationSolar::planetRendering() const {
    //set model for planets
    model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
    // bind shader to upload uniforms
   glUseProgram(m_shaders.at("planet").handle);

  //go through all the children of the rootnode
  for (auto& i: solar_.getRoot()->getListOfChildren()) {
    int count = 0;

        //we have to handle the sun a bit different since its not moving
        if (i->getName() == "sun") {
          glm::fmat4 model_matrix = i->getLocalTransform();
          //we have to do the translation first and then the rotation otherwise sun would be moving
          model_matrix = glm::translate (model_matrix, glm::vec3{0.0f, 0.0f, -1.0f*i->getDistance()});
          model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*(i->getSelfRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});
          
          //sending planetcolor at the shader since uniform is only accepting floats we have to split the color vec3
          glm::vec3 planetcol = i->getPlanetColor();
          glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), planetcol.x, planetcol.y, planetcol.z);

          // activate the proper Texture Unit
          glActiveTexture(GL_TEXTURE0);
          // bind the proper texture object
          glBindTexture(GL_TEXTURE_2D, i->getPlanetTex().handle);
          std::cout<<"sun texture \n";
          // location of the sampler uniform for shader
          int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "Texture");
          // upload index of unit to sampler
          glUseProgram(m_shaders.at("planet").handle);
          glUniform1i(sampler_location, 0);

          //ShaderMode
          glUniform1i(m_shaders.at("planet").u_locs.at("ShaderMode"), shader_Mode);

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
        count +=1;

        } else  {

        glm::fmat4 model_matrix = glm::fmat4{};
        model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*(i->getRotation())), glm::fvec3{0.0f, 1.0f, 0.0f});
         model_matrix = glm::translate (model_matrix, glm::vec3{0.0f, 0.0f, -1.0f*i->getDistance()});

         i->setLocalTransform(model_matrix);

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
          // activate the proper Texture Unit
          glActiveTexture(GL_TEXTURE0);
          // bind the proper texture object
          glBindTexture(GL_TEXTURE_2D, i->getPlanetTex().handle);
          // location of the sampler uniform for shader
          int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "Texture");
          // upload index of unit to sampler
          glUniform1i(sampler_location, 0);

        //Shader Mode
         glUniform1i(m_shaders.at("planet").u_locs.at("ShaderMode"), shader_Mode);

        /* set lighting for planet */
        glm::vec3 pointlightcol = light_.getColor();
        glUniform3f(m_shaders.at("planet").u_locs.at("LightColor"), pointlightcol.x, pointlightcol.y, pointlightcol.z);
        glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"), light_.getLightIntensity());

          // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
          count +=1;


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

                  // activate the proper Texture Unit
                  glActiveTexture(GL_TEXTURE0);
                  // bind the proper texture object
                  glBindTexture(GL_TEXTURE_2D, i->getPlanetTex().handle);
                  // location of the sampler uniform for shader
                  int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "Texture");
                  // upload index of unit to sampler
                  glUniform1i(sampler_location, 0);
                          
                  //ShaderMode
                  glUniform1i(m_shaders.at("planet").u_locs.at("ShaderMode"), shader_Mode);

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
                  count +=1;
            }
          }
        }
        
  }
}

//does the actual drawing of the stars
void ApplicationSolar::drawStars() const {
  std::cout<<"drawStars called \n";
  glUseProgram(m_shaders.at("star").handle);

  // bind the VAO to draw
  glBindVertexArray(star_object.vertex_AO);

  glPointSize (2.0f);
    // draw bound vertex array using bound shader
  glDrawArrays(star_object.draw_mode, 0, (int)stars_.size());

}

// draw Skybox
void ApplicationSolar::drawSkybox() const {
  glDepthMask(GL_FALSE);
  glUseProgram(m_shaders.at("skybox").handle);
  glActiveTexture(GL_TEXTURE0);//specifies which texture unit to make active - default initialisation
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex_obj.handle);//binds texture to target
  glBindVertexArray(skybox_object.vertex_AO);// bind the VAO to draw
  glDrawElements(skybox_object.draw_mode, skybox_object.num_elements, model::INDEX.type, NULL);// draw bound vertex array using bound shader
  glDepthMask(GL_TRUE);
}

//make Quad
void ApplicationSolar::makeQuad() const {
  glUseProgram(m_shaders.at("quad").handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fb_tex_object.handle); //bind texture to target
  //Shadermodi
  glUniform1i(m_shaders.at("quad").u_locs.at("FramebufferTex"), 0);
  glUniform1i(m_shaders.at("quad").u_locs.at("HorizontalMirroringMode"), horizontal_Mode);
  glUniform1i(m_shaders.at("quad").u_locs.at("GreyScaleMode"), greyscale_Mode);
  glUniform1i(m_shaders.at("quad").u_locs.at("VerticalMirroringMode"), vertical_Mode);
  glUniform1i(m_shaders.at("quad").u_locs.at("BlurMode"), blur_Mode);
  

  glBindVertexArray(quad_object.vertex_AO);
  glDrawArrays(quad_object.draw_mode, 0, quad_object.num_elements);


}


void ApplicationSolar::loadTextures() {

  pixel_data sun = texture_loader::file(m_resource_path + "textures/sunmap.png");
  pixel_data mercury = texture_loader::file(m_resource_path + "textures/mercurymap.png");
  pixel_data venus = texture_loader::file(m_resource_path + "textures/venusmap.png");
  pixel_data earth = texture_loader::file(m_resource_path + "textures/earthmap1k.png");
  pixel_data moon = texture_loader::file(m_resource_path + "textures/moonmap1k.png");
  pixel_data mars = texture_loader::file(m_resource_path + "textures/marsmap1k.png");
  pixel_data jupiter = texture_loader::file(m_resource_path + "textures/jupitermap.png");
  pixel_data saturn = texture_loader::file(m_resource_path + "textures/saturnmap.png");
  pixel_data uranus = texture_loader::file(m_resource_path + "textures/uranusmap.png");
  pixel_data neptune = texture_loader::file(m_resource_path + "textures/neptunemap.png");

   //skybox textures
  pixel_data skybox_right = texture_loader::file(m_resource_path + "textures/skybox_right.png");
  pixel_data skybox_left = texture_loader::file(m_resource_path + "textures/skybox_left.png");
  pixel_data skybox_top = texture_loader::file(m_resource_path + "textures/skybox_top.png");
  pixel_data skybox_bottom = texture_loader::file(m_resource_path + "textures/skybox_down.png");
  pixel_data skybox_back = texture_loader::file(m_resource_path + "textures/skybox_back.png");
  pixel_data skybox_front = texture_loader::file(m_resource_path + "textures/skybox_front.png");


  texContainer_.push_back(sun);
  std::cout<<"loaded sun texture \n";
  texContainer_.push_back(mercury);
  std::cout<<"loaded mercury texture \n";
  texContainer_.push_back(venus);
  std::cout<<"loaded venus texture \n";
  texContainer_.push_back(earth);
  std::cout<<"loaded earth texture \n";
  texContainer_.push_back(moon);
  std::cout<<"loaded moon texture \n";
  texContainer_.push_back(mars);
  std::cout<<"loaded mars texture \n";
  texContainer_.push_back(jupiter);
  std::cout<<"loaded jupiter texture \n";
  texContainer_.push_back(saturn);
  std::cout<<"loaded saturn texture \n";
  texContainer_.push_back(uranus);
  std::cout<<"loaded uranus texture \n";
  texContainer_.push_back(neptune);
  std::cout<<"loaded neptune texture \n";

  skybox_container.push_back(skybox_right);
  skybox_container.push_back(skybox_left);
  skybox_container.push_back(skybox_top);
  skybox_container.push_back(skybox_bottom);
  skybox_container.push_back(skybox_back);
  skybox_container.push_back(skybox_front);
}

void ApplicationSolar::initializeTextures() {
  //initialize every texture in the texture conatainer
  for (int i = 0; i<texContainer_.size(); ++i){
    //make a texture object to handle 
    texture_object texture;

    /*initialize texture*/
    //specifies which texture unit to make active - default initialisation
    glActiveTexture(GL_TEXTURE0);
    //specifies num of textures to be generated and which
    glGenTextures(1, &texture.handle);
    //binds texture to target - in this case 2D because we use an image as texture
    glBindTexture(GL_TEXTURE_2D, texture.handle);

    /*define texture sampling parameters*/
    //texture filtering set to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    /*define texture data and format*/
    glTexImage2D(GL_TEXTURE_2D, 0, texContainer_[i].channels, (GLsizei)texContainer_[i].width, (GLsizei)texContainer_[i].height,
                0,texContainer_[i].channels,texContainer_[i].channel_type, texContainer_[i].ptr());

    texObj_.push_back(texture);
    std::cout<<"initialised texture\n";
  }
  
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

  glUseProgram(m_shaders.at("skybox").handle);
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ViewMatrix"),
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

  glUseProgram(m_shaders.at("skybox").handle);
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ProjectionMatrix"),
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
  //quad
  // store shader program objects in container
  m_shaders.emplace("quad", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/quad.vert"},
                                           {GL_FRAGMENT_SHADER,m_resource_path + "shaders/quad.frag"}}});

  // request uniform locations for shader program
  m_shaders.at("quad").u_locs["FramebufferTex"] = -1;
  m_shaders.at("quad").u_locs["HorizontalMirroringMode"] = -1;
  m_shaders.at("quad").u_locs["GreyScaleMode"] = -1;
  m_shaders.at("quad").u_locs["VerticalMirroringMode"] = -1;
  m_shaders.at("quad").u_locs["BlurMode"] = -1;

  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});

  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet").u_locs["ShaderMode"] = -1;

  //request uniform location for shader which is the color of the planet
  m_shaders.at("planet").u_locs["PlanetColor"] = -1;
  m_shaders.at("planet").u_locs["LightColor"] = -1;
  m_shaders.at("planet").u_locs["LightIntensity"] = -1;

  m_shaders.at("planet").u_locs["Texture"] = -1;

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

    //shader for skybox
  m_shaders.emplace("skybox", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/skybox.vert"},
                                              {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("skybox").u_locs["ViewMatrix"] = -1;
  m_shaders.at("skybox").u_locs["ProjectionMatrix"] = -1;
  
}

// load models -> is know more like initializePlanets
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);
    model skybox_model = model_loader::obj(m_resource_path + "models/skybox.obj");//load skybox model

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);std::cout<<"hello there general kenobi\n";
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

	// Add texture coordinates as attribute _ ass4
	// activate third attribute on gpu
	glEnableVertexAttribArray(2);
	// third attribute is 3 floats with no offset & stride 
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]); 
   
   
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


   // cube for skybox
  // generate vertex array object
  glGenVertexArrays(1, &skybox_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(skybox_object.vertex_AO);
  // generate generic buffer
  glGenBuffers(1, &skybox_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, skybox_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skybox_model.data.size(), skybox_model.data.data(), GL_STATIC_DRAW);
  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, skybox_model.vertex_bytes, skybox_model.offsets[model::POSITION]);
  // generate generic buffer
  glGenBuffers(1, &skybox_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * skybox_model.indices.size(), skybox_model.indices.data(), GL_STATIC_DRAW);
  // store type of primitive to draw
  skybox_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  skybox_object.num_elements = GLsizei(skybox_model.indices.size());
}

//assignment 2 initialize the stars
void ApplicationSolar::initializeStars(){
    std::cout<<"initialize Stars\n";

    //initialize with rgb rand and pos rand seperatly
    //fill star verctor with random numbers
    for(int i=0; i< 3000*6;++i){
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

//Assignement 4 initialize skybox
// load Skybox
void ApplicationSolar::initializeSkyBox(){
      glActiveTexture(GL_TEXTURE0);//specifies which texture unit to make active - default initialisatio
      glGenTextures(1, &skybox_tex_obj.handle);//specifies num of textures to be generated and which
      glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex_obj.handle);//binds texture to target
                    

      // set the wrap parameter for texture coordinate S,T and R
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      // define interpolation type when fragment does not exactly cover one texel
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // define interpolation type when fragment covers multiple texels (texture pixels)
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      /*define texture data and format*/
      for(unsigned int idx = 0; idx < skybox_container.size(); ++idx){ 
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, skybox_container[idx].channels, (GLsizei)skybox_container[idx].width,
                    (GLsizei)skybox_container[idx].height, 0, skybox_container[idx].channels,
                    skybox_container[idx].channel_type, skybox_container[idx].ptr());
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
}

void ApplicationSolar::initializeFramebuffer() {

  //create renderbuffer
  //1. generate renderbuffer obj
  glGenRenderbuffer(1, &ren_obj.handle);
  //2. bind it for formatting
  glBindRenderbuffer(GL_RENDERBUFFER, ren_obj.handle);
  //3. specify properties
  //establishes data storage, format and dimension of a renderbuffer objects image
  //internalformat​ specifies the internal format to be used for the renderbuffer object's storage and must be a color-renderable, depth-renderable, or stencil-renderable format
  glRenderbufferStorage(GL_RENDERBUFFER,);//have to fill this out

  //define the framebuffer
  glGenFramebuffers(1, &fbo_obj.handle);
  glBindFramebuffers(GL_FRAMEBUFFER, fbo_obj.handle);

  //bind texture for texture attachement
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &fbo_tex.handle);
  glBindTexture(GL_TEXTURE_2D, fbo_tex.handle);

   //800 and 600 are the screen height and width - not sure wich value to set
   //only allocating memory for the texture but not really filling it yet - this will happen in the renderer 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

  /*define attachments - we have to make on call for each attachement*/
  //define texture/color attachement
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ren_obj.handle);

  //define which buffers to write
  GLenum draw_buffers[n] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(n, draw_buffers);
  glDrawBuffers(1, GL_DEPTH_ATTACHEMENT);

  //check that the framebuffer can be written
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout<<"Framebuffer is not complete\n";
  } else {
    std::cout<<"victory dance\n";
  }

}


//Abgabe 5 framebuffer
void ApplicationSolar::initializeFramebuffer() {
  // generate Renderbuffer Object
  glGenRenderbuffers(1, &rb_object.handle);
  // bind RBO for formatting
  glBindRenderbuffer(GL_RENDERBUFFER, rb_object.handle);
  // specify RBO properties
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 768);

  // generate Frame Buffer Object
  glGenFramebuffers(1, &fb_object.handle);
  //bind FBO for configuration
  glBindFramebuffer(GL_FRAMEBUFFER, fb_object.handle);

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &fb_tex_object.handle);
  glBindTexture(GL_TEXTURE_2D, fb_tex_object.handle);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // specify Texture Object attachments
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb_tex_object.handle, 0);
  //specify Renderbuffer Object attachments
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_object.handle);

  //create array containing enums representing color attachments
  draw_buffers[0] = {GL_COLOR_ATTACHMENT0};
  //set these color attachments to receive fragments
  glDrawBuffers(1, draw_buffers);
  //get the FBO status
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  //compare return value with the valid status value
  if(status != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "FRAMEBUFFER not Complete" << std::endl;
  }
}

//initialize quad
void ApplicationSolar::initializeScreenQuad() {
  model quad_model = model_loader::obj(m_resource_path + "models/quad.obj", model::TEXCOORD);

  glGenVertexArrays(1, &quad_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(quad_object.vertex_AO);
  // generate generic buffer
  glGenBuffers(1, &quad_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, quad_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quad_model.data.size(), quad_model.data.data(), GL_STATIC_DRAW);
  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, quad_model.vertex_bytes, quad_model.offsets[model::POSITION]);

  glEnableVertexAttribArray(1);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, quad_model.vertex_bytes, quad_model.offsets[model::TEXCOORD]);

  quad_object.draw_mode = GL_TRIANGLE_STRIP;
  quad_object.num_elements = GLsizei(quad_model.indices.size());

  glBindVertexArray(0);
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
  //ShaderMode = 1
  else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    shader_Mode = 1;
    std::cout<<"ShaderMode = 1\n";
  }
  //ShaderMode = 2
  else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
    if(shader_Mode == 2) {
      shader_Mode = 1;
      std::cout<<"ShaderMode = 1\n";
    } else {
      shader_Mode = 2;
      std::cout<<"ShaderMode = 2\n";
    }
  }
  //Post-processing effects 
  //Luminance Preserving Grayscale
  else if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
    if(greyscale_Mode) {
      greyscale_Mode = false;
    } else {
      greyscale_Mode = true;
    }
  }

  //Horizontal Mirroring
  else if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
    if(horizontal_Mode) {
      horizontal_Mode = false;
    } else {
      horizontal_Mode = true;
    }
  }

  //Vertical Mirroring
  else if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
    if(vertical_Mode) {
      vertical_Mode = false;
    } else {
      vertical_Mode = true;
    }
  }

  //Blur with 3x3 Gaussian Kernel
  else if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
    if(blur_Mode) {
      blur_Mode = false;
    } else {
      blur_Mode = true;
    }
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