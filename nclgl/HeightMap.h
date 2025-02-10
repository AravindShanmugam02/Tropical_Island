#pragma once
#include <string>
#include "Mesh.h"
#include <time.h>

class HeightMap : public Mesh {
public:
	HeightMap(const std::string &name);
	~HeightMap(void) {};

	Vector3 GetHeightmapSize() const { return heightmapSize; }

	vector<Vector3> GetGroundPos();
	vector<int> GetNosOfGround();
	vector<int> GetNosOfUnderGround();

protected:
	vector<int> NosOfGroundPos;
	vector<int> NosOfUnderGroundPos;
	Vector3 heightmapSize;
};