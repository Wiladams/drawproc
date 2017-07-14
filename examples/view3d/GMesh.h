#pragma once

#include <vector>
#include "geometry.h"

class GMesh
{
public:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;

public:
	GMesh()
		: verts_(), 
		faces_(), 
		norms_(), 
		uv_()
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

};

