/*
 *  Mouse Scroll Class
 */

#ifndef Controller_H
#define Controller_H

#include "d3dApp.h"
#include "Camera.h"

#include <WinDef.h>
#include <WinUser.h>

class Controller : D3DApp
{

public: 

	Controller(HINSTANCE hInstance);

	bool Init();
	void UpdateScene(float dt);
	void DrawScene(); 

	/*
	 * Accessor Functions
	 */
	float	get_Speed();
	void	set_Speed(float s);

	Camera& get_Camera();

	float		get_earthRadius();
	float		get_skyAltitude();
	XMFLOAT3	get_earthPosW();

	float	get_earthAngle();
	void	set_earthAngle(float a);

	float	get_farClipPlaneDist();
	void	set_farClipPlaneDist(float f);
	float	get_nearClipPlaneDist();
	void	set_nearClipPlaneDist(float n);

	POINT	get_mLastMousePos();

	/*
	 *  Mouse Control
	 */
	void OnMouseWheel(WPARAM wheelState, int delta);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private: 
	float speed;
	float minSpeed, maxSpeed;

	Camera mCam;

	POINT mLastMousePos;

	float earthRadius;
	float skyAltitude;

	XMFLOAT3 earthPosW;
	float earthAngle;

	float farClipPlaneDist;
	float nearClipPlaneDist;
};

#endif