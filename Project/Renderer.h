#pragma once
#include "../nclgl/OGLRenderer.h"

#include "../nclgl/Mesh.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"

#include "../nclgl/SceneNode.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Frustum.h"

#include "../nclgl/Camera.h"
#include "../nclgl/Light.h"

#include "../nclgl/HeightMap.h"

#include "../nclgl/Shader.h"
#include <algorithm>

class Camera;
class Shader;
class HeightMap;
class Mesh;
class MeshAnimation;
class MeshMaterial;
class SceneNode;

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

protected:
	//Different Draw Methods
	void DrawScene();
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawNAObjects();
	void DrawSpotObjects();
	void DrawAniObjects();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void DrawSceneGraph();

	//Get Keyboard Inputs
	void GetKeyboardInputs();

	//Methods involving Scene Graph implementation
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	
	//Shaders
	Shader* terrainShader;
	Shader* waterShader;
	Shader* skyboxShader;
	Shader* NAObjShader;
	Shader* sceneShader;
	Shader* SpotObjectShader;
	Shader* AniObjShader;

	//Objects
	HeightMap* heightMap;
	Mesh* multiquad;
	Mesh* sceneCube;
	Mesh* shieldQuad;

	//Vector storages for group of Objects & Materials
	vector<Mesh*> NAObj;
	vector<Mesh*> SpotObject;
	vector<MeshMaterial*> NAMaterial;
	vector<MeshMaterial*> SpotObjectMaterial;
	vector<GLuint> NAMatTextures;
	vector<GLuint> SpotObjMatTextures;
	vector<vector<GLuint>> NAMatTexturesColl;
	vector<vector<GLuint>> SpotObjMatTexturesColl;
	
	//vector<Mesh*> AniObj;
	//vector<MeshAnimation*> AniObjectAnimation;
	//vector<MeshMaterial*> AniObjectMaterial;
	//vector<GLuint> AniObjMatTextures;
	//vector<vector<GLuint>> AniObjMatTexturesColl;

	Mesh* AniObject;
	MeshAnimation* AniObjectAnimation;
	MeshMaterial* AniObjMaterial;
	vector<GLuint> AniObjMatTextures;
	
	//Different class handles
	Light* light;
	Camera* camera;
	SceneNode* root;
	Frustum frustumFrame;

	//GL texture variables
	GLuint cubeMap;
	GLuint waterTex;
	GLuint sandTex;
	GLuint sandBump;
	GLuint grassTex;
	GLuint grassBump;
	GLuint rocksTex;
	GLuint rocksBump;
	GLuint mossTex;
	GLuint mossBump;
	GLuint blendMap;
	GLuint sceneTex;
	GLuint SpotObjectTex;

	//Crucial varibales playing important role
	float waterRotate;
	float waterCycle;
	float frameTime;
	int currentFrame;
	vector<float>frameTimeColl;
	vector<int>currentFrameColl;
	int randomRotationY;
	int Pos2;
	bool automattionSwitch = true;
	bool orientationSwitch = false;
	bool DirecLightSwitch = true;
	bool SpotLightSwitch = true;
	bool terrainLightSwitch = true;
	bool isWaterOn = true;
	Vector3 htmpSize;
	vector<Vector3> OnGroundPos;
	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};