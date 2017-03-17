#include "renderer.h"
#include "drawproc.h"

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) 
{
	Vec3f s[2];
	for (int i = 2; i--; ) 
	{
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2) {
		// dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}
	
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void ogl_triangle(Matrix &Viewport, mat<4, 3, float> &clipc, IShader &shader, pb_rgba &image, float *zbuffer) 
{
	mat<3, 4, float> pts = (Viewport*clipc).transpose(); // transposed to ease access to each of the points
	mat<3, 2, float> pts2;
	for (int i = 0; i<3; i++) pts2[i] = proj<2>(pts[i] / pts[i][3]);

	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.frame.width - 1, image.frame.height - 1);
	
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<2; j++) {
			bboxmin[j] = MAX(0.f, MIN(bboxmin[j], pts2[i][j]));
			bboxmax[j] = MIN(clamp[j], MAX(bboxmax[j], pts2[i][j]));
		}
	}

	Vec2i P;
	TGAColor color;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3f bc_screen = barycentric(pts2[0], pts2[1], pts2[2], P);
			Vec3f bc_clip = Vec3f(bc_screen.x / pts[0][3], bc_screen.y / pts[1][3], bc_screen.z / pts[2][3]);
			bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
			float frag_depth = clipc[2] * bc_clip;
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0 || zbuffer[P.x + P.y*image.frame.width]>frag_depth) 
				continue;
			
			bool discard = shader.fragment(bc_clip, color);
			
			if (!discard) {
				zbuffer[P.x + P.y*image.frame.width] = frag_depth;
				//image.set(P.x, P.y, color);
				//pb_rgba_set_pixel(&image, P.x, P.y, RGBA(color.bgra[0], color.bgra[1], color.bgra[2], color.bgra[3]));
				pb_rgba_set_pixel(&image, P.x, P.y, color.bgraInt);
				//pb_rgba_set_pixel(&image, P.x, P.y, RGBA(127, 127, 127, 255));
			}
		}
	}
}

