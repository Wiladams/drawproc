#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"

class GMesh
{
public:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;

	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;

	void load_texture(std::string filename, const char *suffix, TGAImage &img) {
		std::string texfile(filename);
		size_t dot = texfile.find_last_of(".");
		if (dot != std::string::npos) {
			texfile = texfile.substr(0, dot) + std::string(suffix);
			std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
			img.flip_vertically();
		}
	}

public:
	GMesh()
		: verts_(), 
		faces_(), 
		norms_(), 
		uv_(), diffusemap_(), normalmap_(), specularmap_()
	{}

	void addVert(Vec3f &v) {
		verts_.push_back(v);
	}

	void addFace(std::vector<Vec3i> &fv) {
		faces_.push_back(fv);
	}

	void addNormal(Vec3f &v) {
		norms_.push_back(v);
	}

	void addUV(Vec2f &uv) {
		uv_.push_back(uv);
	}

	size_t nverts() {return verts_.size();}
	size_t nfaces() {return faces_.size();}
	size_t nnormals() { return norms_.size(); }
	size_t nuvs() { return uv_.size(); }

	// Retrieve a face at the specified index
	std::vector<int> face(int idx) {
		std::vector<int> vface;
		for (int i = 0; i<(int)faces_[idx].size(); i++) 
			vface.push_back(faces_[idx][i][0]);

		return vface;
	}

	// Get the normal for a specific face and vertex
	Vec3f normal(int iface, int nthvert) 
	{
		int idx = faces_[iface][nthvert][2];
		return norms_[idx].normalize();
	}

	Vec3f vert(int i) {
		return verts_[i];
	}

	Vec3f vert(int iface, int nthvert) {
		return verts_[faces_[iface][nthvert][0]];
	}

	Vec2f uv(int iface, int nthvert) {
		return uv_[faces_[iface][nthvert][1]];
	}

	TGAColor diffuse(Vec2f uvf) {
		Vec2i uv(uvf[0] * diffusemap_.get_width(), uvf[1] * diffusemap_.get_height());
		return diffusemap_.get(uv[0], uv[1]);
	}

	Vec3f normal(Vec2f uvf) {
		Vec2i uv(uvf[0] * normalmap_.get_width(), uvf[1] * normalmap_.get_height());
		TGAColor c = normalmap_.get(uv[0], uv[1]);
		Vec3f res;
		for (int i = 0; i<3; i++)
			res[2 - i] = (float)c[i] / 255.f*2.f - 1.f;
		return res;
	}


	float specular(Vec2f uvf) {
		Vec2i uv(uvf[0] * specularmap_.get_width(), uvf[1] * specularmap_.get_height());
		return specularmap_.get(uv[0], uv[1])[0] / 1.f;
	}

};

