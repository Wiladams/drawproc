#include "arcball.h"
#include "drawproc.h"


// onDisplay()
// Handle arcball 
/*
if (cur_mx != last_mx || cur_my != last_my) {
	glm::vec3 va = get_arcball_vector(last_mx, last_my);
	glm::vec3 vb = get_arcball_vector(cur_mx, cur_my);
	float angle = acos(min(1.0f, glm::dot(va, vb)));
	glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
	glm::mat3 camera2object = glm::inverse(glm::mat3(transforms[MODE_CAMERA]) * glm::mat3(main_object.object2world));
	glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
	main_object.object2world = glm::rotate(main_object.object2world, angle, axis_in_object_coord);
	last_mx = cur_mx;
	last_my = cur_my;
}
*/

/*
void ArcBall::onMouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		arcball_on = true;
		last_mx = cur_mx = x;
		last_my = cur_my = y;
	}
	else {
		arcball_on = false;
	}
}
*/

void ArcBall::onMotion(int x, int y) 
{
	if (arcball_on) {  // if left button is pressed
		cur_mx = x;
		cur_my = y;
	}
}

/*
void ArcBall::onIdle()
{
	// onIdle()
	if (cur_mx != last_mx || cur_my != last_my) {
		Vec3f va = get_arcball_vector(last_mx, last_my);
		Vec3f vb = get_arcball_vector(cur_mx, cur_my);
		float angle = acos(MIN(1.0f, dot(va, vb)));
		Vec3f axis_in_camera_coord = cross(va, vb);
		glm::mat3 camera2object = glm::inverse(glm::mat3(transforms[MODE_CAMERA]) * glm::mat3(mesh.object2world));
		Vec3f axis_in_object_coord = camera2object * axis_in_camera_coord;
		mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);
		last_mx = cur_mx;
		last_my = cur_my;
	}
}
*/

/**
* Get a normalized vector from the center of the virtual ball O to a
* point P on the virtual ball surface, such that P is aligned on
* screen's (X,Y) coordinates.  If (X,Y) is too far away from the
* sphere, return the nearest point on the virtual ball surface.
*/
Vec3f ArcBall::get_arcball_vector(int x, int y) 
{
	Vec3f P = Vec3f(1.0*x / width * 2 - 1.0,
		1.0*y / height * 2 - 1.0,
		0);
	P.y = -P.y;
	float OP_squared = P.x * P.x + P.y * P.y;
	if (OP_squared <= 1 * 1)
		P.z = sqrt(1 * 1 - OP_squared);  // Pythagore
	else
		P.normalize();

	return P;
}
