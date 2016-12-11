#pragma once

#include "ofMain.h"

class Object_3D
{
public:
	Object_3D();
	~Object_3D();
	void addVertices(ofVec3f vertices);
	void addCageVertices(ofVec3f vertices);
	void addCageFaces(ofMeshFace meshFaces);

	int verticesNumber;
	int cageVerticesNumber;
	ofVec3f* vertices;
	ofVec3f* cageVertices;
	typedef struct CageFaces {
		int id;
		ofVec3f* vertex[3];
		ofVec3f* normal;
	} CageFaces;
	CageFaces cageFaces;
};

