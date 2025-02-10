#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Light
{
public:
	Light() {} //Default constructor.
	Light(const Vector3& position, const Vector4& colour, float radius)
	{
		this->position = position;
		this->colour = colour;
		this->radius = radius;
	}

	~Light(void) {};

	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& val) { position = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(const Vector4& val) { colour = val; }

	//For Directional Light
	Vector3 GetDirection() const { return Direction; }
	void SetDirection(const Vector3& val) { Direction = val; }

	Vector3 GetAmbient() const { return Ambient; }
	void SetAmbient(const Vector3& val) { Ambient = val; }

	Vector3 GetDiffuse() const { return Diffuse; }
	void SetDiffuse(const Vector3& val) { Diffuse = val; }

	//For SpotLight
	Vector3 Get_SpotLightPosition() const { return SpotLightPosition; }
	void Set_SpotLightPosition(const Vector3& val) { SpotLightPosition = val; }

	Vector3 Get_SpotLightDirection() const { return SpotLightDirection; }
	void Set_SpotLightDirection(const Vector3& val) { SpotLightDirection = val; }

	Vector3 Get_SpotLightAmbient() const { return SpotLightAmbient; }
	void Set_SpotLightAmbient(const Vector3& val) { SpotLightAmbient = val; }

	Vector3 Get_SpotLightDiffuse() const { return SpotLightDiffuse; }
	void Set_SpotLightDiffuse(const Vector3& val) { SpotLightDiffuse = val; }

	float Get_SpotLightCutOff() const { return SpotLightCutOff; }
	void Set_SpotLightCutOff(const float& val) { SpotLightCutOff = val; }

	float Get_SpotLightOuterCutOff() const { return SpotLightOuterCutOff; }
	void Set_SpotLightOuterCutOff(const float& val) { SpotLightOuterCutOff = val; }

	float Get_SpotLightConstant() const { return SpotLightConstant; }
	void Set_SpotLightConstant(const float& val) { SpotLightConstant = val; }

	float Get_SpotLightLinear() const { return SpotLightLinear; }
	void Set_SpotLightLinear(const float& val) { SpotLightLinear = val; }

	float Get_SpotLightQuadratic() const { return SpotLightQuadratic; }
	void Set_SpotLightQuadratic(const float& val) { SpotLightQuadratic = val; }



protected:
	float radius = 0.0f;
	Vector4 colour = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 Direction = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 Ambient = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 Diffuse = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 SpotLightPosition = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 SpotLightDirection = Vector3(0.0f, 0.0f, 0.0f);
	float SpotLightCutOff = 0.0f;
	float SpotLightOuterCutOff = 0.0f;
	Vector3 SpotLightAmbient = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 SpotLightDiffuse = Vector3(0.0f, 0.0f, 0.0f);
	float SpotLightConstant = 0.0f;
	float SpotLightLinear = 0.0f;
	float SpotLightQuadratic = 0.0f;
};