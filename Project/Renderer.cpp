#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	//Depth Testing, Culling, Alpha Blending and CubeMap Seamless features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//Maps
	heightMap = new HeightMap(TEXTUREDIR"HeightMap01.png");

	//Shapes
	multiquad = Mesh::GenerateQuad();
	shieldQuad = Mesh::GenerateQuad();

	//Meshes & its Materials
	sceneCube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	NAObj.emplace_back(Mesh::LoadFromMeshFile("Palm_Tree.msh"));
	NAMaterial.emplace_back(new MeshMaterial("Palm_Tree_Copy.mat")); //Palm_Tree.mat
	NAObj.emplace_back(Mesh::LoadFromMeshFile("Oak_Tree.msh"));
	NAMaterial.emplace_back(new MeshMaterial("Oak_Tree_Copy.mat")); //Oak_Tree.mat

	//S_Objects & its Materials
	SpotObject.emplace_back(Mesh::LoadFromMeshFile("rpgpp_lt_bird_house_01.msh"));
	SpotObjectMaterial.emplace_back(new MeshMaterial("rpgpp_lt_bird_house_01.mat"));

	//Animated Objects & its Materials
	AniObject = Mesh::LoadFromMeshFile("Role_T.msh");
	AniObjectAnimation = new MeshAnimation("Role_T.anm");
	AniObjMaterial = new MeshMaterial("Role_T.mat");


	//Non-Animated Object's SubMeshes & its Materials
	for (int j = 0; j < NAObj.size(); j++)
	{
		for (int i = 0; i < NAObj[j]->GetSubMeshCount(); ++i)
		{
			const MeshMaterialEntry* matEntry = NAMaterial[j]->GetMaterialForLayer(i);
			const string* filename = nullptr;
			matEntry->GetEntry("Diffuse", &filename);
			string path = TEXTUREDIR + *filename;
			GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
			NAMatTextures.emplace_back(texID);
		}
		NAMatTexturesColl.emplace_back(NAMatTextures);
		NAMatTextures.clear();
	}

	//S_Objects
	for (int j = 0; j < SpotObject.size(); j++)
	{
		for (int i = 0; i < SpotObject[j]->GetSubMeshCount(); ++i)
		{
			const MeshMaterialEntry* matEntry = SpotObjectMaterial[j]->GetMaterialForLayer(i);
			const string* filename = nullptr;
			matEntry->GetEntry("Diffuse", &filename);
			string path = TEXTUREDIR + *filename;
			GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
			SpotObjMatTextures.emplace_back(texID);
		}
		SpotObjMatTexturesColl.emplace_back(SpotObjMatTextures);
		SpotObjMatTextures.clear();
	}

	//Animated Objects
	for (int i = 0; i < AniObject->GetSubMeshCount(); ++i)
	{
		const MeshMaterialEntry* matEntry = AniObjMaterial->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		AniObjMatTextures.emplace_back(texID);
	}

	//Textures
	sceneTex = SOIL_load_OGL_texture(TEXTUREDIR "stained.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandTex = SOIL_load_OGL_texture(TEXTUREDIR "sand01.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandBump = SOIL_load_OGL_texture(TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassTex = SOIL_load_OGL_texture(TEXTUREDIR "grass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassBump = SOIL_load_OGL_texture(TEXTUREDIR "grass_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	rocksTex = SOIL_load_OGL_texture(TEXTUREDIR "rocks.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	rocksBump = SOIL_load_OGL_texture(TEXTUREDIR "rocks_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	mossTex = SOIL_load_OGL_texture(TEXTUREDIR "moss.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	mossBump = SOIL_load_OGL_texture(TEXTUREDIR "moss_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	blendMap = SOIL_load_OGL_texture(TEXTUREDIR "BlendMap01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg", 
									TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg", 
									SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	//SceneNode Section
	root = new SceneNode();

	for (int i = 0; i < 5; ++i)
	{
		SceneNode* s = new SceneNode();

		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));

		s->SetTransform(Matrix4::Translation(Vector3(0, 200.0f, -300.0f + 100.0f + 100 * i)));

		s->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));

		s->SetBoundingRadius(100.0f);

		s->SetMesh(shieldQuad);

		s->SetTexture(sceneTex);

		root->AddChild(s);
	}
	root->AddChild(new CubeRobot(sceneCube));

	//Texture load check
	if (!sandTex || !sandBump || !grassTex || !grassBump || !rocksTex || !rocksBump || !mossTex || !mossBump || !blendMap || !cubeMap || !waterTex || !sceneTex)
	{
		return;
	}

	//Texture Repeat
	SetTextureRepeating(sandTex, true);
	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(rocksTex, true);
	SetTextureRepeating(mossTex, true);

	//Shaders
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	terrainShader = new Shader("terrainVertex.glsl", "terrainFragment.glsl");
	waterShader = new Shader("waterVertex.glsl", "waterFragment.glsl");
	NAObjShader = new Shader("NAObjVertex.glsl", "NAObjFragment.glsl");
	SpotObjectShader = new Shader("SpotLightObjectVertex.glsl","SpotLightObjectFragment.glsl");
	AniObjShader = new Shader("AnimSkinningVertex.glsl", "AnimSkinningFragment.glsl");
	sceneShader = new Shader("sceneVertex.glsl", "sceneFragment.glsl");

	//Shader load check
	if (!waterShader->LoadSuccess() || !skyboxShader->LoadSuccess() || !terrainShader->LoadSuccess() || !NAObjShader->LoadSuccess() || !sceneShader->LoadSuccess() || !SpotObjectShader->LoadSuccess() || !AniObjShader->LoadSuccess())
	{
		return;
	}

	//HeightMap Section
	htmpSize = heightMap->GetHeightmapSize();

	//Objects Random Rotation & placement
	OnGroundPos = heightMap->GetGroundPos();
	randomRotationY = rand() % 360;

	//Projection View Matrix
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	//Camera & Light Positioning
	//camera = new Camera(htmpSize.x * 0.0f, htmpSize.z * -0.5f, htmpSize * Vector3(0.1f, -5.0f, 0.1f));
	camera = new Camera();
	camera->SetPosition(htmpSize / Vector3(14.114124f, 0.405953f, 0.924434f));
	camera->SetPitch(-10.570014f); camera->SetYaw(0.058960f);

	//Crucial variables
	currentFrame = 0;
	frameTime = 0.0f;
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	init = true;
}

Renderer::~Renderer(void)
{
	delete camera;
	delete light;

	delete heightMap;
	delete root;
	delete shieldQuad;
	delete sceneCube;
	delete multiquad;

	delete waterShader;
	delete skyboxShader;
	delete terrainShader;
	delete NAObjShader;
	delete sceneShader;
	delete SpotObjectShader;
	delete AniObjShader;
	delete AniObject;
	delete AniObjectAnimation;
	delete AniObjMaterial;

	for (auto& i : NAObj)
	{
		delete i;
	}
	for (auto& i : NAMaterial)
	{
		delete i;
	}

	for (auto& i : SpotObject)
	{
		delete i;
	}
	for (auto& i : SpotObjectMaterial)
	{
		delete i;
	}
}

void Renderer::UpdateScene(float dt)
{
//********************CAMERA************************
	camera->UpdateCamera(automattionSwitch, htmpSize, dt);
	viewMatrix = camera->BuildViewMatrix();

	if(automattionSwitch == false) {
		if (orientationSwitch == true) {
			projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
		}
		else{
			projMatrix = Matrix4::Perspective(1.0f, 25000.0f, (float)width / (float)height, 45.0f);
		}
	}
	else {
		projMatrix = Matrix4::Perspective(1.0f, 25000.0f, (float)width / (float)height, 45.0f);
	}

	frustumFrame.FromMatrix(projMatrix * viewMatrix);
//**************************************************

//**************ANIMATION*****************
	frameTime -= dt;
	while (frameTime < 0.0f)
	{
		currentFrame = (currentFrame + 1) % AniObjectAnimation->GetFrameCount();
		frameTime += 1.0f / AniObjectAnimation->GetFrameRate();
	}
//****************************************
	
	waterRotate += dt * 2.0f; // 2 degrees a second
	waterCycle += dt * 0.25f; //10 units a second

	root->Update(dt);
	GetKeyboardInputs();

	//Camera position tracker
	/*std::cout << "Position: X: " + std::to_string(htmpSize.x / camera->GetPosition().x) + " Y: " + std::to_string(htmpSize.y / camera->GetPosition().y) + " Z: " + std::to_string(htmpSize.z / camera->GetPosition().z) << std::endl;
	std::cout << "Pitch(x): " + std::to_string(camera->GetPitch()) + " Yaw(y): " + std::to_string(camera->GetYaw()) << std::endl;*/
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawScene();
}

void Renderer::GetKeyboardInputs() {

	//Toggle Water
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_B))
	{
		isWaterOn = !isWaterOn;
	}

	//Toggle Point Light - Lits the terrain
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L))
	{
		terrainLightSwitch = !terrainLightSwitch;
	}

	//Toggle Directional Light - Lits trees
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		DirecLightSwitch = !DirecLightSwitch;
	}

	//Toggle Spot Light - Programmed to work on the Well!
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_K))
	{
		SpotLightSwitch = !SpotLightSwitch;
	}

	//Toggle Camera Automation and set its position to default.
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_U))
	{
		automattionSwitch = !automattionSwitch;
		if (automattionSwitch) {
			camera->SetPosition(htmpSize / Vector3(14.114124f, 0.405953f, 0.924434f));
			camera->SetPitch(-10.570014f); camera->SetYaw(0.058960f);
		}
		else {
			camera->SetPosition(htmpSize / Vector3(1.105201f, 0.405953f, 0.925173f));
			camera->SetPitch(-10.570014f); camera->SetYaw(0.058960f);
		}
	}

	//Toggle Orientation - Orthographic / Perspective
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
	{
		orientationSwitch = !orientationSwitch;
	}
}

void Renderer::DrawScene()
{
	DrawSkybox();
	DrawHeightmap();
	DrawNAObjects();
	//DrawSceneGraph();
	DrawSpotObjects();
	DrawAniObjects();
	if (isWaterOn == true) DrawWater();
}

// Draw SkyBox
void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();

	multiquad->Draw();

	glDepthMask(GL_TRUE);
}

// Draw HeightMap
void Renderer::DrawHeightmap()
{
	if (terrainLightSwitch == true)
	{
		light = new Light(htmpSize * Vector3(0.70f, 2.0f, 0.80f), Vector4(1, 1, 1, 1), htmpSize.x); //pointlight
	}
	else
	{
		light = new Light(Vector3(NULL, NULL, NULL), Vector4(NULL, NULL, NULL, NULL), NULL);
	}

	BindShader(terrainShader);
	SetShaderLight(*light);
	
	glUniform3fv(glGetUniformLocation(terrainShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	
	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "diffuseTex1"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sandTex);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "bumpTex1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sandBump);
	
	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "diffuseTex2"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grassTex);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "bumpTex2"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassBump);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "diffuseTex3"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rocksTex);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "bumpTex3"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rocksBump);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "diffuseTex4"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, mossTex);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "bumpTex4"), 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, mossBump);

	glUniform1i(glGetUniformLocation(terrainShader->GetProgram(), "blendMap"), 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, blendMap);

	//modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f));
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	heightMap->Draw();
}

// Draw Water
void Renderer::DrawWater()
{
	BindShader(waterShader);
	
	glUniform3fv(glGetUniformLocation(waterShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	
	modelMatrix = Matrix4::Translation(Vector3(htmpSize.x * 0.5f, htmpSize.y * 0.2f, htmpSize.z * 0.5f)) 
				* Matrix4::Scale(Vector3(htmpSize.x * 0.5f, htmpSize.y * 2.0f, htmpSize.z * 0.5f)) //I don't understand why scale is here because the water object is a flat quad.
				* Matrix4::Rotation(90, Vector3(1, 0, 0));
	textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) 
				* Matrix4::Scale(Vector3(10, 10, 10)) 
				* Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	UpdateShaderMatrices();
	SetShaderLight(*light);

	multiquad->Draw();
}

//Draw Non-Animated Objects
void Renderer::DrawNAObjects()
{
	if (DirecLightSwitch == true)
	{
		//Enable Directional light Values
		light->SetDirection(Vector3(htmpSize * Vector3(0.1f, 2.5f, 0.3f)));
		light->SetAmbient(Vector3(0.5f, 0.5f, 0.5f));
		light->SetDiffuse(Vector3(1.0f, 1.0f, 1.0f));
	}
	else
	{
		//Disable Directional light Values
		light->SetDirection(Vector3(NULL, NULL, NULL));
		light->SetAmbient(Vector3(NULL, NULL, NULL));
		light->SetDiffuse(Vector3(NULL, NULL, NULL));
	}

	BindShader(NAObjShader);
	glUniform1i(glGetUniformLocation(NAObjShader->GetProgram(), "matrl.diffuseTex"), 0);
	glUniform3fv(glGetUniformLocation(NAObjShader->GetProgram(), "lit.direction"), 1, (float*)&light->GetDirection());
	glUniform3fv(glGetUniformLocation(NAObjShader->GetProgram(), "lit.ambient"), 1, (float*)&light->GetAmbient());
	glUniform3fv(glGetUniformLocation(NAObjShader->GetProgram(), "lit.diffuse"), 1, (float*)&light->GetDiffuse());

	int pos1 = 0;

	for  (int k = 0; k < NAObj.size(); k++)
	{
		for (int v = 0; v < 10; v++)
		{
			modelMatrix = Matrix4::Translation(Vector3(OnGroundPos[pos1].x, OnGroundPos[pos1].y, OnGroundPos[pos1].z))
						* Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) 
						* Matrix4::Rotation(randomRotationY, Vector3(0, 1, 0));
			UpdateShaderMatrices();

			for (int j = 0; j < NAObj[k]->GetSubMeshCount(); ++j) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, NAMatTexturesColl[k][j]);
				NAObj[k]->DrawSubMesh(j);
			}
			pos1 += 5;
		}
	}

	Pos2 = pos1;
}

//Draw Spotlight_Object
void Renderer::DrawSpotObjects()
{
	if (SpotLightSwitch == true)
	{
		//Enable Spotlight Values
		light->Set_SpotLightPosition(Vector3(camera->GetPosition()));
		light->Set_SpotLightDirection(Vector3(0.0f, 0.0f, -1.0f));
		light->Set_SpotLightCutOff(12.5f);
		light->Set_SpotLightOuterCutOff(17.5f);
		light->Set_SpotLightAmbient(Vector3(0.5f, 0.5f, 0.5f));
		light->Set_SpotLightDiffuse(Vector3(1.0f, 1.0f, 1.0f));
		light->Set_SpotLightConstant(1.0f);
		light->Set_SpotLightLinear(0.045f);
		light->Set_SpotLightQuadratic(0.0075f);
	}
	else
	{
		//Enable Spotlight Values
		light->Set_SpotLightPosition(Vector3(NULL, NULL, NULL));
		light->Set_SpotLightDirection(Vector3(NULL, NULL, NULL));
		light->Set_SpotLightCutOff(NULL);
		light->Set_SpotLightOuterCutOff(NULL);
		light->Set_SpotLightAmbient(Vector3(NULL, NULL, NULL));
		light->Set_SpotLightDiffuse(Vector3(NULL, NULL, NULL));
		light->Set_SpotLightConstant(NULL);
		light->Set_SpotLightLinear(NULL);
		light->Set_SpotLightQuadratic(NULL);
	}

	BindShader(SpotObjectShader);
	Pos2 += 15;

	//Setting lighting properties for chest (Spotlight)
	glUniform1i(glGetUniformLocation(SpotObjectShader->GetProgram(), "matrl.diffuseTex"), 0);
	glUniform3fv(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.position"), 1, (float*)&light->Get_SpotLightPosition());
	glUniform3fv(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.direction"), 1, (float*)&light->Get_SpotLightDirection());
	glUniform1f(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.cutOff"), light->Get_SpotLightCutOff());
	glUniform1f(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.outerCutOff"), light->Get_SpotLightOuterCutOff());
	glUniform3fv(glGetUniformLocation(SpotObjectShader->GetProgram(), "viewPos"), 1, (float*)&light->Get_SpotLightPosition());

	glUniform3fv(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.ambient"), 1, (float*)&light->Get_SpotLightAmbient());
	glUniform3fv(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.diffuse"), 1, (float*)&light->Get_SpotLightDiffuse());
	glUniform1f(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.constant"), light->Get_SpotLightConstant());
	glUniform1f(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.linear"), light->Get_SpotLightLinear());
	glUniform1f(glGetUniformLocation(SpotObjectShader->GetProgram(), "lit.quadratic"), light->Get_SpotLightQuadratic());

	for (int k = 0; k < SpotObject.size(); k++)
	{
		for (int v = 0; v < 1; v++)
		{
			modelMatrix = Matrix4::Translation(Vector3(OnGroundPos[Pos2].x, OnGroundPos[Pos2].y, OnGroundPos[Pos2].z))
						* Matrix4::Scale(Vector3(20.0f, 20.0f, 20.0f));

			UpdateShaderMatrices();

			for (int j = 0; j < SpotObject[k]->GetSubMeshCount(); ++j) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, SpotObjMatTexturesColl[k][j]);
				SpotObject[k]->DrawSubMesh(j);
			}
			Pos2 += 5;
		}
	}
}

//Draw Animated Objects
void Renderer::DrawAniObjects()
{
	int pos3 = Pos2;
	
	BindShader(AniObjShader);
	glUniform1i(glGetUniformLocation(AniObjShader->GetProgram(), "matrl.diffuseTex"), 0);
	glUniform3fv(glGetUniformLocation(AniObjShader->GetProgram(), "lit.direction"), 1, (float*)&light->GetDirection());
	glUniform3fv(glGetUniformLocation(AniObjShader->GetProgram(), "lit.ambient"), 1, (float*)&light->GetAmbient());
	glUniform3fv(glGetUniformLocation(AniObjShader->GetProgram(), "lit.diffuse"), 1, (float*)&light->GetDiffuse());

	pos3 += 15;
	modelMatrix = Matrix4::Translation(Vector3(OnGroundPos[pos3].x, OnGroundPos[pos3].y, OnGroundPos[pos3].z))
				* Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) 
				* Matrix4::Rotation(randomRotationY, Vector3(0, 1, 0));
	UpdateShaderMatrices();
	vector<Matrix4> frameMatrices;

	const Matrix4* invBindPose = AniObject->GetInverseBindPose();
	const Matrix4* frameData = AniObjectAnimation->GetJointData(currentFrame);

	for (unsigned int i = 0; i < AniObject->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}

	int j = glGetUniformLocation(AniObjShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

	for (int l = 0; l < AniObject->GetSubMeshCount(); ++l) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, AniObjMatTextures[l]);
		AniObject->DrawSubMesh(l);
	}
}

//Draw SceneGraph
void Renderer::DrawSceneGraph()
{
	BuildNodeLists(root);
	SortNodeLists();

	BindShader(sceneShader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);

	DrawNodes();

	ClearNodeLists();
}

//Methods involving Scene Graph - Node List Building, Node List Sorting, Node List Clearning & Drawing Nodes
//--------------------------------------------------------------------------------------------------------
void Renderer::BuildNodeLists(SceneNode* from)
{
	if (frustumFrame.InsideFrustum(*from))
	{
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
		{
			transparentNodeList.push_back(from);
		}
		else
		{
			nodeList.push_back(from);
		}
	}

	for (vector < SceneNode* >::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.rbegin(), // note the r!
		transparentNodeList.rend(), // note the r!
		SceneNode::CompareByCameraDistance);

	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (const auto& i : nodeList)
	{
		DrawNode(i);
	}

	for (const auto& i : transparentNodeList)
	{
		DrawNode(i);
	}
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(sceneShader->GetProgram(), "modelMatrix"), 1, false, model.values);

		glUniform4fv(glGetUniformLocation(sceneShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		sceneTex = n->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sceneTex);

		glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "useTexture"), sceneTex);

		n->Draw(*this);
	}
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}
//--------------------------------------------------------------------------------------------------------