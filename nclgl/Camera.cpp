#include "Camera.h"
#include "Window.h"
#include <algorithm>

void Camera::UpdateCamera(bool automation, Vector3 htmpSize, float dt)
{
	camAutomation = automation;

	if ((htmpSize.x / position.x) > 14.114124f) {
		goRight = true;
	}
	else if ((htmpSize.x / position.x) < 1.105201f) {
		goRight = false;
	}

	GetCameraInputs(dt);

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0.0f)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}

	rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	forward = rotation * Vector3(0, 0, -1);
	right = rotation * Vector3(1, 0, 0);
}

Matrix4 Camera::BuildViewMatrix()
{
	return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) * Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) * Matrix4::Translation(-position);
}

Matrix4 Camera::BuildViewMatrix(float pitch, float yaw)
{
	return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) * Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) * Matrix4::Translation(-position);
}

void Camera::GetCameraInputs(float dt) {

	float speed = 15.0f;

	if(camAutomation == false) {
		pitch -= (Window::GetMouse()->GetRelativePosition().y);
		yaw -= (Window::GetMouse()->GetRelativePosition().x);

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_CONTROL)) {
			speed = 50.0f;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
		{
			position += forward * speed * dt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
		{
			position -= forward * speed * dt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
		{
			position -= right * speed * dt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))
		{
			position += right * speed * dt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))
		{
			position.y += speed * dt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))
		{
			position.y -= speed * dt;
		}
	}
	else {
		goRight == true ? position += right * speed * dt : position -= right * speed * dt;
		/*position += (forward * 1.5f) * cos(time(0)) * dt;
		position -= (right * -1.5f) * sin(time(0)) * dt;*/
	}
}