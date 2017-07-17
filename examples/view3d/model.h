#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>

#include "GMesh.h"
#include "tgaimage.h"


class Model : public GMesh {
private:

public:
	Model(const char *filename);
	~Model();
};
#endif //__MODEL_H__

