

#include "model.h"


GMesh * Model::loadModel(const char *filename) 
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) 
			return nullptr;

	std::string line;
	GMesh *amesh = new GMesh();

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i<3; i++) 
				iss >> v[i];
			amesh->addVert(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			Vec3f n;
			for (int i = 0; i<3; i++) 
				iss >> n[i];
			amesh->addNormal(n);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec2f uv;
			for (int i = 0; i<2; i++) iss >> uv[i];
			amesh->addUV(uv);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<Vec3i> f;
			Vec3i tmp;
			iss >> trash;
			while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
				for (int i = 0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
				f.push_back(tmp);
			}
			amesh->addFace(f);
		}
	}
	//std::cerr << "# v# " << nverts() << " f# " << nfaces() << " vt# " << nuvs() << " vn# " << nnormals() << std::endl;
	amesh->load_texture(filename, "_diffuse.tga", amesh->diffusemap_);
	amesh->load_texture(filename, "_nm_tangent.tga", amesh->normalmap_);
	amesh->load_texture(filename, "_spec.tga", amesh->specularmap_);

	return amesh;
}





