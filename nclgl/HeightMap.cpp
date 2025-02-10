#include "HeightMap.h"
#include <iostream>

HeightMap::HeightMap(const std::string& name) {
	int iWidth, iHeight, iChans;
	unsigned char* data = SOIL_load_image(name.c_str(), &iWidth, &iHeight, &iChans, 1);

	if (!data) {
		std::cout << " Heightmap can't load file !\n";
		return;
	}

	numVertices = iWidth * iHeight;
	numIndices = (iWidth - 1) * (iHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	Vector3 vertexScale = Vector3(1.0f, 0.05f, 1.0f);
	Vector2 textureScale = Vector2(1/512.0f, 1/512.0f);

	for (int z = 0; z < iHeight; ++z)
	{
		for (int x = 0; x < iWidth; ++x)
		{
			int offset = (z * iWidth) + x;
			vertices[offset] = Vector3(x, data[offset], z) * vertexScale;
			textureCoords[offset] = Vector2(x, z) * textureScale;
		}
	}
	SOIL_free_image_data(data);

	int i = 0;
	for (int z = 0; z < iHeight - 1; ++z) {
		for (int x = 0; x < iWidth - 1; ++x) {
			int a = (z * (iWidth)) + x;
			int b = (z * (iWidth)) + (x + 1);
			int c = ((z + 1) * (iWidth)) + (x + 1);
			int d = ((z + 1) * (iWidth)) + x;
			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;
			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}

	GenerateNormals();
	GenerateTangents();
	BufferData();

	heightmapSize.x = vertexScale.x * (iWidth - 1);
	heightmapSize.y = vertexScale.y * 255.0f;// each height is a byte ! //255.0f //This value represents the colour extracted from heightMap, 255.0f is full White. The heightest point in a heightMap. This doesn't have to do anything with the vertex scale.
	heightmapSize.z = vertexScale.z * (iHeight - 1);

	for (int i = 0; i < numVertices; i++)
	{
		if (vertices[i].y >= heightmapSize.y)
		{
			NosOfGroundPos.emplace_back(i);
		}
		else
		{
			NosOfUnderGroundPos.emplace_back(i);
		}
	}
}

vector<int> HeightMap::GetNosOfGround()
{
	return NosOfGroundPos;
}

vector<int> HeightMap::GetNosOfUnderGround()
{
	return NosOfUnderGroundPos;
}

vector<Vector3> HeightMap::GetGroundPos()
{
	srand((unsigned int)time(0));

	vector<int> randomPosition;
	vector<int> temporaryPosition;

	for (int i = 0; i < 5000; i++)
	{
		bool isAdded = false;

		while (isAdded == false)
		{
			if (randomPosition.empty() && temporaryPosition.empty())
			{
				int randtemp = (rand() + rand()) % NosOfGroundPos.size(); //Randtemp = (rand() + rand()) % numVertices -> gives negative positions as well. Need to check this online to make sure.
				randomPosition.emplace_back(NosOfGroundPos[randtemp]);
				temporaryPosition.emplace_back(randomPosition[i]);
				isAdded = true;
			}
			else if (!randomPosition.empty() && !temporaryPosition.empty())
			{
				int samePos = 0;
				int randtemp = rand() % NosOfGroundPos.size();
				randomPosition.emplace_back(NosOfGroundPos[randtemp]);

				for (int j = 0; j < temporaryPosition.size(); j++)
				{
					if (randomPosition[i] == temporaryPosition[j])
					{
						samePos++;
					}
					else
					{
						//Nothing happens here!
					}
				}
				if (samePos == 0)
				{
					temporaryPosition.emplace_back(randomPosition[i]);
					isAdded = true;
				}
				else if(samePos > 0)
				{
					randomPosition.pop_back();
				}
			}
		}
	}
	
	vector<Vector3> GeneratedRandPositions;

	for (int i = 0; i < randomPosition.size(); i++)
	{
		GeneratedRandPositions.emplace_back(vertices[randomPosition[i]]);
	}

	return GeneratedRandPositions;

}