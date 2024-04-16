#include<iostream>
#include<string>
#include<map>
#include<cstdlib>
#include<random>
#include<glad/include/glad/gl.h> 
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"OwnHeaderFiles/ShaderProgram/shaderClass.h"
#include"OwnHeaderFiles/model/Model.h"
#include"OwnHeaderFiles/camera/camera.h"
#include<ft2build.h>
#include FT_FREETYPE_H 


GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;

	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;

		switch (errorCode)
		{
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;

		}
		std::cout << "ERROR CODE: " << errorCode << std::endl;
		std::cout << error << " | " << file << "(" << line << ")" << std::endl;
	}

	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__,__LINE__);

//This struct it's for save the id's of a gBuffer generated and do deferred-shading
struct gBuffer {
	unsigned int gbuffer;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedoSpecular;
};


void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei lenght,
	const char* message,
	const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "_______________" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
		case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System" << std::endl;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler" << std::endl;
		case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party" << std::endl;
		case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application" << std::endl;
		case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other" << std::endl;
	}
	std::cout << std::endl;

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break;
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: hight"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;

	std::cout << std::endl;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void activeBlitFramebuffer(unsigned int fboDraw, unsigned int fboRead);
void renderScene(const Shader& shader);
void renderCube();
void renderQuad();
void setDepthMap(const unsigned int depthMap, const unsigned int depthMapFBO);
void attachCubeDepthTex(unsigned int depthCubeMap, unsigned int depthMapFBO);
void renderSimpleWallScene(const Shader& shader);
void renderSimpleWall();
void renderPlane();
void renderTunnelScene(const Shader& shader);
void renderSceneWall(const Shader& shader);
void renderCubesColor(const Shader& shader, std::vector<glm::vec3> lightColor, std::vector<glm::vec3> lightPos);
void renderPlaneScene(const Shader& shader, unsigned int textureID);
void colorsAttachment();
void setSSAOcolorBuffer(unsigned int ssaoColorBuffer);
void renderSphere();
float lerp(float a, float b, float f);
unsigned int genCubeMap(unsigned int internalformat = GL_RGB16F, unsigned int scr_width = 512,
	unsigned int scr_height = 512);
unsigned int genCubeMapMipmap();
unsigned int genDepthMapFBO();
unsigned int genTextureDepthMap();
unsigned int genTextureColorBuffer(unsigned int wrap);
unsigned int loadTexture(const char* path, bool gamaCorrection = false);
unsigned int loadHDRTexture(const char* path);
unsigned int loadCubeMapTex(std::vector<std::string> textures_faces);
unsigned int genFramebufferObject();
unsigned int genRenderBufferObjectMultSampled(unsigned int samples);
unsigned int genRenderBufferObject();
unsigned int genRenderBufferDepthObject();
unsigned int genRenderBuffercubeMap(int scr_width = 512, int scr_height = 512);
unsigned int genMultiSampleTexture(unsigned int samples);
unsigned int genGenericTex();
unsigned int genTexFacesDepthCubeMap(unsigned int depthCubeMap);
unsigned int genNoiseTexture(std::vector<glm::vec3>* ssaoNoise);
unsigned int genTextureColorBuffer16F(unsigned int wrap);
unsigned int genLUTTexture();
unsigned int* genTextureColorBuffers16F(unsigned int amount);
unsigned int* genPingpongFBO();
unsigned int* genPingpongColorBuffer16F(unsigned int* pingpongFBO);
gBuffer genGbuffer();
gBuffer genGbufferSSAO(); 
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

bool firstMouse = true;
bool gamma = false;
bool gammaPressed = false;
bool hdr = false;
bool hdrPressed = false;
bool spacePressed = true;
bool shadowOn = false;
bool change = false;
bool changePressed = false;
bool bloom = true;
bool bloomPressed = false;

//Initial settup of deltaTime and lastFrame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float exposure = 1.0f;

//firsts lasts pos of mouse in x and y
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT/2.0f;

//camera initial settup
Camera camera(glm::vec3(0.0f, 0.0f, 9.0f));



//unsigned int planeVAO, planeVBO;

int main() {
	//Initiate glfw to use a window to set opengl 

	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// initialize debug output
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	
	}
	
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LEQUAL);
	
	FT_Library ft;

	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERRROR::FREETYPE: could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;

	if (FT_New_Face(ft, "fonts/Roboto-Regular.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: failed to load font" << std::endl;
		return -1;
	}

	Shader shader("shaders/PBR/directLighting/shaderVertex.glsl",
		"shaders/PBR/directLighting/shaderFragment.glsl");
	Shader shaderLight("shaders/PBR/renderSphereLight/shaderVertex.glsl",
		"shaders/PBR/renderSphereLight/shaderFragment.glsl");
	Shader shaderEquirectTocubeMap("shaders/PBR/EquirectToCubemap/shaderVertex.glsl",
		"shaders/PBR/EquirectToCubemap/shaderFragment.glsl");
	Shader shaderBackgroundSkybox("shaders/PBR/shaderCubemap/shaderVertex.glsl",
		"shaders/PBR/shaderCubemap/shaderFragment.glsl");
	Shader shaderIrradianceMap("shaders/PBR/cubeMapToIrradiance/shaderVertex.glsl",
		"shaders/PBR/cubeMapToIrradiance/shaderFragment.glsl");
	Shader shaderSpecularMap("shaders/PBR/cubeMapToSpecular/shaderVertex.glsl",
		"shaders/PBR/cubeMapToSpecular/shaderFragment.glsl");
	Shader shaderBRDF("shaders/PBR/brdfShader/shaderVertex.glsl",
		"shaders/PBR/brdfShader/shaderFragment.glsl");
	Shader RenderQuad("shaders/PBR/renderQuad/shaderVertex.glsl",
		"shaders/PBR/renderQuad/shaderFragment.glsl");
	
	unsigned int captureFBO = genFramebufferObject();
	unsigned int captureRBO = genRenderBuffercubeMap();
	
	unsigned int envCubemap = genCubeMap();
	unsigned int irradianceCubemap = genCubeMap(GL_RGBA16F, 32, 32);
	//Cubemap that stores different levels of cubemaps increasing roughness levels
	//this cubemap stores the reflection of surfaces with differents rough values
	unsigned int specularCubemap = genCubeMapMipmap();
	unsigned int brdfLUTTexture = genLUTTexture();
	
	//This mat4 is for capture a scene, a face of a scene, this gives of how the 
	//scene is projected to the screen 
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	//This is the directions of where the view matrix is looking in the scene
	glm::mat4 captureViews[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//the hdr texture helps for better sense of lighting and it's in a format that allows
	//values of colors that are not clamped to 1.0 value, that's why it's called hdr
	//texture
	//This is considered a more precise form of ambient lighting
	unsigned int hdrTexture = loadHDRTexture("textures/PBR/cubeMap_enviroment/christmas_photo_studio_07_4k.hdr");
	
	//This textures gives us the values of the surfaces to reflect better the light 
	//and give more details to the surface
	unsigned int rustyMetalAlbedo = loadTexture("textures/PBR/rusty-metal-bl/rusty-metal_albedo.png");
	unsigned int rustyMetalNormalMap = loadTexture("textures/PBR/rusty-metal-bl/rusty-metal_normal-ogl.png");
	unsigned int rustyMetalMetallicMap = loadTexture("textures/PBR/rusty-metal-bl/rusty-metal_metallic.png");
	unsigned int rustyMetalRoughnessMap = loadTexture("textures/PBR/rusty-metal-bl/rusty-metal_roughness.png");
	unsigned int rustyMetalAOMap = loadTexture("textures/PBR/rusty-metal-bl/rusty-metal_ao.png");
	
	unsigned int speckledRustAlbedo = loadTexture("textures/PBR/speckled-rust-bl/speckled-rust_albedo.png");
	unsigned int speckledRustNormalMap = loadTexture("textures/PBR/speckled-rust-bl/speckled-rust_normal-ogl.png");
	unsigned int speckledRustMetallicMap = loadTexture("textures/PBR/speckled-rust-bl/speckled-rust_metallic.png");
	unsigned int speckledRustRoughnessMap = loadTexture("textures/PBR/speckled-rust-bl/speckled-rust_roughness.png");
	unsigned int speckledRustAOMap = loadTexture("textures/PBR/speckled-rust-bl/speckled-rust_ao.png");

	unsigned int lightGoldAlbedo = loadTexture("textures/PBR/light-gold-bl/lightgold_albedo.png");
	unsigned int lightGoldNormalMap = loadTexture("textures/PBR/light-gold-bl/lightgold_normal-ogl.png");
	unsigned int lightGoldMetallicMap = loadTexture("textures/PBR/light-gold-bl/lightgold_metallic.png");
	unsigned int lightGoldRoughnessMap = loadTexture("textures/PBR/light-gold-bl/lightgold_roughness.png");

	//convert equirectangularmap to cubeMap
	shaderEquirectTocubeMap.use();
	shaderEquirectTocubeMap.setInt("equirectangularMap", 0);
	shaderEquirectTocubeMap.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512); //this is the size of the capture dimensions
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	for (unsigned int i = 0; i < 6; i++) {
		//render each face of the cube map and giving the directions to see the scene
		//and capturing the rendered scene into the cubemap
		shaderEquirectTocubeMap.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


	
	// as is not hightly detailed the texture we set the resolution to 32x32
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	//Now is time for the irradiance map, that gives us the global lighting ambience
	shaderIrradianceMap.use();
	shaderIrradianceMap.setInt("environmentMap", 0);
	shaderIrradianceMap.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);


	//do the irradiance calc for each face and store it in the irradiance cubemap
	// this is the diffuse part of the ibl tecnique
	for (unsigned int i = 0; i < 6; i++) {
		shaderIrradianceMap.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
	}

	// this is the specular part and reflective part of the obl tecnique
	shaderSpecularMap.use();
	shaderSpecularMap.setInt("enviromentMap", 0);
	shaderSpecularMap.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	// this cube map it's gonna have mip levels depending of the roughness
	// this is for storing differents reflections depending of the roughness
	// with less rough value a better look like a mirror surface
	// in this cubemap we gonna store the reflective part of the specular part of ibl
	for (unsigned int mip = 0; mip < maxMipLevels; mip++) {
		// each mipmap levels we reduce the resolution of the reflection
		// because more roughness less detail we need to store
		unsigned int mipWidth = 256 * std::pow(0.5, mip);
		unsigned int mipHeight = 256 * std::pow(0.5, mip);

		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		shaderSpecularMap.setFloat("roughness", roughness);

		for (unsigned int i = 0; i < 6; i++) {
			shaderSpecularMap.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, specularCubemap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}


	// this part it's for precomputate the the brdf part of the ibl of the scene
	// this generate a texture for every combination of normal and light direction on 
	// varying roughness values
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	shaderBRDF.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//setting the values of the textures
	shader.use();
	shader.setInt("irradianceMap", 0);
	shader.setInt("prefilterMap", 1);
	shader.setInt("brdfLUT", 2);
	shader.setInt("albedoMap", 3);
	shader.setInt("metallicMap", 4);
	shader.setInt("normalMap", 5);
	shader.setInt("roughnessMap", 6);
	shader.setInt("aoMap", 7);
	//shader.setVec3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	//shader.setFloat("ao", 1.0f);


	glm::vec3 lightPositions[] = {
		glm::vec3(0.0f, -10.0f, 10.0f), 
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f),
		
	};

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5f;

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader.use();
	shader.setMat4("projection", projection);
	shaderBackgroundSkybox.use();
	shaderBackgroundSkybox.setInt("enviromentMap", 0);
	shaderBackgroundSkybox.setMat4("projection", projection);
	

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use(); 
		// This bool uniform is to let know to the shader if we are using a texture for ao
		// or not, if we not i set to 1.0 to not change a thing 
		shader.setBool("aoUse", true);
		// setting ibl textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubemap);  
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, specularCubemap);  
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);  

		// rusty iron textures
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, rustyMetalAlbedo);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, rustyMetalMetallicMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, rustyMetalNormalMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, rustyMetalRoughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, rustyMetalAOMap); 
		
		glm::mat4 view(camera.GetViewMatrix());
		shader.setMat4("view", view);
		shader.setVec3("camPos", camera.Position);

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));

		shader.setMat4("model", model);
		shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		renderSphere();

		//speckled rust textures
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, speckledRustAlbedo);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, speckledRustMetallicMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, speckledRustNormalMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, speckledRustRoughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, speckledRustAOMap);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		shader.setMat4("model", model);
		shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		renderSphere();

		//light gold
		shader.setBool("aoUse", false);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, lightGoldAlbedo);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, lightGoldMetallicMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, lightGoldNormalMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, lightGoldRoughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, speckledRustAOMap);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		renderSphere();


		//scene with differents spheres with differents metallic values and roughnes values
		/*for (int row = 0; row < nrRows; ++row) {
			shader.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col) {
				
				shader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				shader.setMat4("model", model);
				shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				renderSphere();
			}
		}*/
		//commented just for use of the indirect light from the hdr texture
		/*shaderLight.use();
		shaderLight.setMat4("projection", projection);
		shaderLight.setMat4("view", camera.GetViewMatrix());
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			shaderLight.use();
			shaderLight.setMat4("model", model);
			shaderLight.setVec3("lightColor", lightColors[i]);
			renderSphere();
			shader.use();
			shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
		}*/

		shaderBackgroundSkybox.use();
		shaderBackgroundSkybox.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		renderCube();

		/*RenderQuad.use();
		RenderQuad.setInt("LUTTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		renderQuad();*/

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	
	glfwTerminate();

	
	return 0;


}


unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere() {
	if (sphereVAO == 0) {
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;

		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}
		bool oddRow = false;

		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;

		for (unsigned int i = 0; i < positions.size(); ++i) {
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);

			if (normals.size()) {
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size()) {
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	}
	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void renderPlaneScene(const Shader& shader, unsigned int textureID) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f));
	shader.setMat4("model", model);
	renderPlane();

	glActiveTexture(textureID);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.50f));
	shader.setMat4("model", model);
	renderCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.50f));
	shader.setMat4("model", model);
	renderCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	model = glm::scale(model, glm::vec3(0.50f));
	shader.setMat4("model", model);
	renderCube();
}

void renderCubesColor(const Shader& shader,std::vector<glm::vec3> lightColor, std::vector<glm::vec3> lightPos) {
	glm::mat4 model(1.0f);
	
	for (int i = 0; i < lightColor.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos[i]);
		model = glm::scale(model, glm::vec3(0.25f));
		shader.setMat4("model", model);
		shader.setVec3("lightColor", lightColor[i]);
		renderCube();
	}
}

void renderSimpleWallScene(const Shader& shader) {

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
	//model = glm::scale(model, glm::vec3(0.75f));
	//model = glm::rotate(model, glm::radians(30.0f), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE);
	renderSimpleWall();
	//glEnable(GL_CULL_FACE);
}

void renderTunnelScene(const Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5, 2.5f, 27.5f));
	
	shader.setMat4("model", model);
	shader.setInt("inverse_normals", true);
	renderCube();
	
}

void renderSceneWall(const Shader& shader) {

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
	model = glm::rotate(model, glm::radians(50.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
	shader.setMat4("model", model);
	renderSimpleWall();
}

//render the actual scene, passing the shader need it for that
void renderScene(const Shader& shader) {



	glm::mat4 model = glm::mat4(1.0f);
	/*model = glm::scale(model, glm::vec3(6.0f));
	shader.setMat4("model", model);
	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);
	*/
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0)); 
	model = glm::scale(model, glm::vec3(0.75f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.setMat4("model", model);
	renderCube();


}

unsigned int wallVAO = 0;
unsigned int wallVBO = 0;
void renderSimpleWall() {

	if (wallVAO == 0) {
		//Coords of triangles in 3D space
		glm::vec3 pos1(-1.0, 1.0, 0.0);
		glm::vec3 pos2(-1.0, -1.0, 0.0);
		glm::vec3 pos3(1.0, -1.0, 0.0);
		glm::vec3 pos4(1.0, 1.0, 0.0);

		//Coords of textures
		glm::vec2 uv1(0.0, 1.0);
		glm::vec2 uv2(0.0, 0.0);
		glm::vec2 uv3(1.0, 0.0);
		glm::vec2 uv4(1.0, 1.0);

		//Normal of the tangent space looking to z
		glm::vec3 nm(0.0, 0.0, 1.0);

		
		//Triangle 1
		//Getting the edges to calc the tangent and bitangent of triangle 1
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;

		//calc the tangent and bitangent of triangle1
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		//Triangle 2 
		//Getting the edges to calc the tangent and bitangent of triangle 1
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		//calc the tangent and bitangent of triangle2
		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		//setting the data to make the TBN matrix and calc better ilumination based of a normal map
		float vertices[]{
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		glGenVertexArrays(1,&wallVAO);
		glBindVertexArray(wallVAO);
		glGenBuffers(1, &wallVBO);

		glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(wallVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(wallVAO);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube() {

	if (cubeVAO == 0) {
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};

		//setting all the data if it's need it
		glGenVertexArrays(1,&cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	//draw the cube and unbind the vertex array obj
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int planeVAO = 0, planeVBO = 0;
void renderPlane() {
	if (planeVAO == 0) {
		float vertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};

		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);

		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glBindVertexArray(planeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

//This is for rendering the depth text to see how it looks like, for debug porpuse
unsigned int quadVAO = 0, quadVBO;
void renderQuad() {
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll((float)yoffset);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

	float xpos = (float)xposIn;
	float ypos = (float)yposIn;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;


	camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {

	const float cameraSpeed = 5.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyBoard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyBoard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyBoard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyBoard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyBoard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyBoard(DOWN, deltaTime);
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
		shadowOn = !shadowOn;
		spacePressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		spacePressed = false;
	//Active hdr
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !bloomPressed) {
		bloom = !bloom;
		bloomPressed = true;
		std::cout << "bloom: " << (bloom ? "true" : "false") << std::endl;
		
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE)
		bloomPressed = false;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (exposure < 5.0f)
			exposure += 0.001f;
		else
			exposure = 5.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (exposure > 0.0f)
			exposure -= 0.001f;
		else
			exposure = 0.0f;
	}

	
		

}

void colorsAttachment(){
	//Tell explicity to openGL that the curren fbo have to color attacments when we 
	//render a scene
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	//Bind to the default fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int loadTexture(const char* path, bool gamaCorrection) {
	
	unsigned int textureID;

	glGenTextures(1, &textureID);

	
	int width, height, nrComponents;

	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data) {
		GLenum internalFormat;
		GLenum dataFormat;

		if (nrComponents == 1)
			internalFormat = dataFormat= GL_RED;
		else if (nrComponents == 3) {

			
			internalFormat = gamaCorrection ? GL_SRGB : GL_RGB;
			
			
			dataFormat = GL_RGB;
		
		}
		else if (nrComponents == 4) {
			internalFormat = gamaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		
		
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadHDRTexture(const char* path) {
	unsigned int hdrTexture;
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &hdrTexture);

	int width, height, nrComponents;

	float* data = stbi_loadf(path, &width, &height, &nrComponents, 0);

	if (data) {
		GLenum internalFormat;
		GLenum dataFormat;
		std::string dataFormatS;

		if (nrComponents == 3) {
			internalFormat = GL_RGB16F;
			dataFormat = GL_RGB;
			
		}
		else if (nrComponents == 4) {
			internalFormat = GL_RGBA16F;
			dataFormat = GL_RGBA;
			
		}
		
		
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load HDR image" << std::endl;
	}
	return hdrTexture;
}

unsigned int genFramebufferObject() {

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	return framebuffer;
}

unsigned int genTextureColorBuffer(unsigned int wrap = GL_LINEAR) {
	unsigned int textureColorBuffer;

	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, wrap);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
	
	return textureColorBuffer;
}

unsigned int genLUTTexture() {
	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return brdfLUTTexture;
}

unsigned int genTextureColorBuffer16F(unsigned int wrap = GL_LINEAR) {
	unsigned int textureColorBuffer16F;

	glGenTextures(1, &textureColorBuffer16F);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer16F);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, wrap);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer16F, 0);
	
	return textureColorBuffer16F;
}

unsigned int* genTextureColorBuffers16F(unsigned int amount = 2) {
	unsigned int* colorBuffers = new unsigned int[amount];

	glGenTextures(amount, colorBuffers);

	for (unsigned int i = 0; i < amount; i++) {
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}
	
	return colorBuffers;
}

unsigned int* genPingpongFBO() {
	static unsigned int pingpongFBO[2];
	

	glGenFramebuffers(2, pingpongFBO);

	return pingpongFBO;
}


unsigned int* genPingpongColorBuffer16F(unsigned int *pingpongFBO) {
	
	//getting the amount of elements of the dynamic array
	unsigned int amount = sizeof(pingpongFBO) / sizeof(unsigned int);
	unsigned int* pingpongBuffer = new unsigned int[amount];

	
	glGenTextures(amount, pingpongBuffer);

	for (unsigned int i = 0; i < amount; i++) {

		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);

		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	
	
	return pingpongBuffer;
}

//This texture is just for depth test
unsigned int genTextureDepthMap() {
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// we set this for the parts of the depthMap those are outside of the projection
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
	//setting the color to be white, meaning this parts are gonna be in light
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//To fix the parts that are far away of the z component of the far plane of the projection
	//we need to set the shadow to 0.0 if we want to be in light, because those fragments
	//would be more than 1.0 in the z component
	//Thats something to change in the shader where the shadow and lighting calculations are 
	//happening
	return depthMap;
}

unsigned int genRenderBufferObject(){
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	return rbo;
}

unsigned int genRenderBufferDepthObject() {
	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
	
	return depthRBO;
}

unsigned int genRenderBuffercubeMap(int scr_width, int scr_height) {
	unsigned int captureRBO;
	glGenRenderbuffers(1, &captureRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, scr_width, scr_height); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	
	return captureRBO;
}

unsigned int genDepthMapFBO() {
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	
	return depthMapFBO;
}
//This is for setting and bind the depthMap tex and the depthMapFBO for render the deothMap
void setDepthMap(const unsigned int depthMap, const unsigned int depthMapFBO) {
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// We only need the depth info, no need to render colors
	// This tell that this FBO it's not gonna render color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

unsigned int genRenderBufferObjectMultSampled(unsigned int samples) {
	unsigned int rboMS;

	glGenRenderbuffers(1, &rboMS);
	glBindRenderbuffer(GL_RENDERBUFFER, rboMS);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMS);
	
	return rboMS;
}

void activeBlitFramebuffer(unsigned int fboDraw , unsigned int fboRead) {
	//writing data to other framebuffer for resolving the MSAA and then painting it to a tex 
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboRead);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboDraw);
	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

}

unsigned int genMultiSampleTexture(unsigned int samples) {
	
	unsigned int textureID;

	glGenTextures(1, &textureID);

	
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureID, 0);
	
	return textureID;
}

unsigned int loadCubeMapTex(std::vector<std::string> textures_faces){

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	int width, height, nrChannels;
	unsigned char *data;

	for (unsigned int i = 0; i < textures_faces.size(); i++) {
		data = stbi_load(("textures/" + textures_faces[i]).c_str(), &width, &height, &nrChannels, 0);
		
		if (data) {
			GLenum format;

			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3) {

				format = GL_RGB;

			}
			else if (nrChannels == 4) {
				format = GL_RGBA;

			}

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap text failed to load at path: textures/" << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}
		
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int genCubeMap(unsigned int internalformat, unsigned int scr_width,
	unsigned int scr_height) {
	
	unsigned int cubeMap;
	unsigned int format;

	if (internalformat == GL_RGB16F)
		format = GL_RGB;
	else if (internalformat == GL_RGBA16F)
		format = GL_RGBA;
	else if (internalformat == GL_RED)
		format = GL_RED;

	
	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat,
			scr_width, scr_height, 0, format, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return cubeMap;
}

unsigned int genCubeMapMipmap() {
	//This generates a cubemap with mipmap levels for store different pre-filtered
	//reflections maps
	//we use GL_LINEAR_MIPMAP_LINEAR to enable trilinear filtering for better looking
	//and use a resolution of 128x128 at base resolution, and because the smoother 
	//surfaces are small 
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);

	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 256, 256, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//This allows us to allocate memory for the mipmas
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return prefilterMap;
}

unsigned int genGenericTex() {
	unsigned int genericTex;
	glGenTextures(1, &genericTex);
	
	return genericTex;
}

void setSSAOcolorBuffer(unsigned int ssaoColorBuffer) {
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete" << std::endl;
}

unsigned int genNoiseTexture(std::vector<glm::vec3>* ssaoNoise) {
	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return noiseTexture;
}

//This for the cubeMap for depth test
unsigned int genTexFacesDepthCubeMap(unsigned int depthCubeMap) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	return depthCubeMap;
}
//attaching the actual textureCubeMap with a FBO to do the depthMap
void attachCubeDepthTex(unsigned int depthCubeMap, unsigned int depthMapFBO) {
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

gBuffer genGbufferSSAO() {
	//This generate a gBuffer for screen-space ambien occlusion (SSAO)
	//This is for better lightin and give more realistic values to the 
	//ambient component of lighting calc
	unsigned int gBuffergen;
	glGenFramebuffers(1, &gBuffergen);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffergen);
	unsigned int gPosition, gNormal, gAlbedo;

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// setting render buffer as depth buffer
	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	//Bind to the default fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	gBuffer GbufferSSAO{ gBuffergen, gPosition, gNormal, gAlbedo };

	return GbufferSSAO;

}

gBuffer genGbuffer() {
	// This function gen a struct with the data of a gBuffer for make the deferred-shading
	// attach 3 texs which are gonna store the color, pos and normals for lighting
	// the rendered scenes in the frags are gonna store the info in vec3 and the rgb value
	// is the actual info used in the light scene, where it should be, wich color and the normals
	// each texture store there respective values 
	// gPosition store in the fragment the value where it should be the fragment
	// gNormal the normal for that fragment
	// gAlbedoSpec the color and the specular value in a vec4 value, rgb for the color
	// and the alpha the spec 
	unsigned int gBuffergen;
	glGenFramebuffers(1, &gBuffergen);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffergen);
	unsigned int gPosition, gNormal, gAlbedoSpecular;

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpecular);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// setting render buffer as depth buffer
	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	//Bind to the default fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	gBuffer Gbuffer{ gBuffergen, gPosition, gNormal, gAlbedoSpecular };

	return Gbuffer;
}

float lerp(float a, float b, float f) {
	
	return a + f * (b - a);
}