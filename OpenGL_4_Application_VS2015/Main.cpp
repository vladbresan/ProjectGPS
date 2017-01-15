//
//  main.cpp
//  OpenGL_Shader_Example_step1
//
//  Created by CGIS on 02/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC

#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#define TINYOBJLOADER_IMPLEMENTATION

#include "Model3D.hpp"
#include "Mesh.hpp"

int glWindowWidth = 1366;
int glWindowHeight = 768;
int retina_width, retina_height;
float delta = 0;
float delta1 = 0;
float delta2 = 0;
float movementSpeed = 1.2;
float movementSpeed1 = 1.6;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = glWindowWidth / 2.0;
float lastY = glWindowHeight / 2.0;
GLFWwindow* glWindow = NULL;
std::vector<const GLchar*> faces;


glm::mat4 model,auxModel;
GLint modelLoc,auxLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;

glm::vec3 lightDir[2];
GLuint lightDirLoc;
GLuint lightDirLoc1;
glm::vec3 lightPos[2];
GLuint lightPosLoc;
glm::vec3 lightPos1;
GLuint lightPosLoc1;
glm::vec3 lightColor[2];
GLuint lightColorLoc;
GLuint lightColorLoc1;

glm::vec3 texture;
GLuint textureLoc;
glm::vec3 fragTexCoord;
GLuint fragTexCoordLoc;

gps::Camera myCamera(glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 2.0f, -10.0f));
float cameraSpeed = 1.0f;

bool pressedKeys[1024];
float angle = 0.0f;
float angle1 = 0.f;
float angle2 = 0.0f;

GLuint verticesVBO;
GLuint verticesEBO;
GLuint objectVAO;

double lastTimeStamp = glfwGetTime();

bool firstMouse = true;

gps::Model3D myModel, myEarth, myShip, mySun,myHand,myPluto,yingBall,yangBall;
gps::Shader myCustomShader;

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

float countUpDown = 0.0f;
float countLeftRight = 0.0f;
float count;


bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);


	return true;
}
void processMovement()
{

	if (pressedKeys[GLFW_KEY_T]) {
		delta1 += 0.004;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		angle += 0.000002;
		myCamera.rotate(angle, 0);
		lastX += angle;
		lastY += angle;
	}

	if (pressedKeys[GLFW_KEY_U]) {
		angle += 0.000002;
		myCamera.rotate(-angle, 0);
		lastX += angle;
		lastY += angle;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle1 += 0.000002;
		myCamera.rotate(0, angle1);
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		angle1 += 0.000002;
		myCamera.rotate(0, -angle1);

	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_UP]) {
		countUpDown = countUpDown + 0.00001;
		lightPos[1] = glm::vec3(0, countUpDown, 0);
		glUniform3fv(lightPosLoc1, 1, glm::value_ptr(lightPos[1]));
		glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightPos[1]));
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {
		countUpDown = countUpDown - 0.00001;
		lightPos[1] = glm::vec3(0, countUpDown, 0);
		glUniform3fv(lightPosLoc1, 1, glm::value_ptr(lightPos[1]));
		glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightPos[1]));
	}
	if (pressedKeys[GLFW_KEY_LEFT]) {
		countLeftRight = countLeftRight + 0.00001;
		lightPos[1] = glm::vec3(countLeftRight,0, 0);
		glUniform3fv(lightPosLoc1, 1, glm::value_ptr(lightPos[1]));
		glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightPos[1]));
	}
	if (pressedKeys[GLFW_KEY_RIGHT]) {
		countLeftRight = countLeftRight - 0.00001;
		lightPos[1] = glm::vec3(-countLeftRight, 0, 0);
		glUniform3fv(lightPosLoc1, 1, glm::value_ptr(lightPos[1]));
		glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightPos[1]));
	}
	if (pressedKeys[GLFW_KEY_C]) {
		if(count<1)
		count += 0.02;
		lightColor[1] = glm::vec3(0,0, count);
		glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor[1]));
	}
	if (pressedKeys[GLFW_KEY_V]) {
		if(count>0)
		count -= 0.02;
		lightColor[1] = glm::vec3(0, 0, count);
		glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor[1]));
	}
	if (pressedKeys[GLFW_KEY_R]) {
		lightColor[1] = glm::vec3(1.0f, 0, 0);
		glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor[1]));
	}
	if (pressedKeys[GLFW_KEY_G]) {
		lightColor[1] = glm::vec3(0, 1.0f, 0);
		glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor[1]));
	}
	if (pressedKeys[GLFW_KEY_B]) {
		lightColor[1] = glm::vec3(0, 0, 1.0f);
		glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor[1]));
	}
}

GLuint ReadTextureFromFile(const char* file_name) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
			);
	}

	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB, //GL_SRGB,//GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
		);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void updateDelta(double elapsedSeconds) {
	delta = delta + movementSpeed*elapsedSeconds;
	delta1 = delta1 + 1.2*movementSpeed*elapsedSeconds;
	delta2 = delta2 + movementSpeed1*elapsedSeconds;
}

void initUniforms()
{
	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 100000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightPos[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	lightPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos[0]");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos[0]));


	lightDir[0] = lightPos[0];
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir[0]");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir[0]));

	//set light color
	lightColor[0] = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor[0]");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor[0]));

	lightPos[1] = glm::vec3(0.0f, 0.0f, 30.0f);
	lightPosLoc1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos[1]");
	glUniform3fv(lightPosLoc1, 1, glm::value_ptr(lightPos[1]));

	lightDir[1] = lightPos[1];
	lightDirLoc1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir[1]");
	glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightDir[1]));

	lightColor[1] = glm::vec3(0.0f, 0.0f, 0.5f); //blue light
	lightColorLoc1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor[1]");
	glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor[1]));

	//diffuse texture
	texture = glm::vec3(1.0f, 1.0f, 1.0f);
	textureLoc = glGetUniformLocation(myCustomShader.shaderProgram, "diffuseTexture");
	glUniform3fv(textureLoc, 1, glm::value_ptr(texture));

	//fragment texture coords
	fragTexCoord = glm::vec3(1.0f, 1.0f, 1.0f);
	fragTexCoordLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fragTexCoord");
	glUniform3fv(fragTexCoordLoc, 1, glm::value_ptr(fragTexCoord));
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	//initialize the model matrix
	
	processMovement();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");

	myCustomShader.useShaderProgram();

	//initialize the view matrix
	glm::mat4 view = myCamera.getViewMatrix();
	//send matrix data to shader
	GLint viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	model = glm::translate(model,glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	mySun.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, delta, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0, 0, -50));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, 0.25f, glm::vec3(0, 0, 1));
	model = glm::rotate(model, delta2, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	myEarth.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, delta, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0, 0, -50));
	model = glm::rotate(model, delta, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0, 0, -12.5));
	model = glm::scale(model, glm::vec3(0.027, 0.027, 0.027));
	double currentTimeStamp = glfwGetTime();
	updateDelta(currentTimeStamp - lastTimeStamp);
	lastTimeStamp = currentTimeStamp;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	myModel.Draw(myCustomShader);
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 20));
	model = glm::scale(model, glm::vec3(0.0025f, 0.0025f, 0.0025f));
	model = glm::translate(model, glm::vec3(0,0, -countUpDown));
	model = glm::rotate(model, countLeftRight, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(countLeftRight, 0, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	myShip.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(delta1, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0, 0, -100));
	model = glm::scale(model, glm::vec3(0.035, 0.035, 0.035));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	myPluto.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(delta1, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(15, 15, 15));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	yingBall.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(delta1, glm::vec3(0, 1, 1));
	model = glm::translate(model, glm::vec3(15, 15, 15));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverse(glm::transpose(view*model)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	yangBall.Draw(myCustomShader);

	glBindVertexArray(objectVAO);


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	myCamera.rotateMouse(pitch, yaw);
}

void initOpenGLState() {
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initObjects() {
	myModel = gps::Model3D("objects/moon.obj", "objects/");
	myEarth = gps::Model3D("objects/earth.obj", "objects/");
	myShip = gps::Model3D("objects/Republic Assault Ship.obj", "objects/");
	mySun = gps::Model3D("objects/Sun.obj", "objects/");
	myPluto = gps::Model3D("objects/pluto.obj", "objects/");
	yingBall = gps::Model3D("objects/yingBall.obj", "objects/");
	yangBall = gps::Model3D("objects/yangBall.obj", "objects/");
}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
}

int main(int argc, const char * argv[]) {

	initOpenGLWindow();
	initOpenGLState();
	initObjects();
	initShaders();

	initUniforms();

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);

	for (GLuint i = 0;i<)

	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();
		glfwSetCursorPosCallback(glWindow, mouse_callback);
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	glfwTerminate();

	return 0;
}
