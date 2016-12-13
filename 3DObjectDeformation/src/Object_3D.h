#pragma once

#include "ofMain.h"
#include "GreenCoordinate.h"
#include "Util.h"

typedef struct CageFaces {
	int id;
	int verticesId[3];
	int normalSign;
} CageFaces;

typedef struct PartModelVertices {
	int id;
	string partName;
	int* modelVerticesId;
	int partModelVerticesNum;
} PartModelVertices;

typedef struct PartCageVertices {
	int id;
	string partName;
	int* cageVerticesId;
	int partCageVerticesNum;
} PartCageVertices;

typedef struct PartCageFaces {
	int id;
	string partName;
	int* cageFacesId;
	int partCageFacesNum;
} PartCageFaces;

typedef struct BarycentricCoord {
	int modelVertexId;
	string partName;
	pair<int, double>* cageVerticesId;
	pair<int, double>* cageFacesId;
} BarycentricCoord;

class Object_3D
{
public:
	Object_3D();
	~Object_3D();
	int* addModelVertices(ofVec3f* vertices, int vertices_size);
	int* addCageVertices(ofVec3f* vertices, int vertices_size);
	int* addCageFacesByMeshFaces(ofMeshFace* meshFace, int meshFace_size);
	int addCageFacesByVertices(ofVec3f* faceVertices, int faceVertices_size);
	int addPartsName(string partName);
	void createModelAndCage(ofVec3f * inputModelVertices, ofVec3f * inputCageVertices, ofMeshFace* inputCageFaces, int inputModelVertices_size, int inputCageVertices_size, int inputCageFaces_size);
	void createModelAndCage(vector<ofVec3f> modelVertices, vector<ofVec3f> cageVertices, vector<ofMeshFace> inputVectorCageFaces);
	void createPartsFromModel(string partName, vector<ofVec3f> inputPartCageVertices);
	void createPartsFromScratch(string partName, ofVec3f* modelVertices, ofMeshFace* cageMeshFaces, int modelVertices_size, int cageMeshFaces_size);
	bool initBarycentricCoordinate();
	int internalModelVertexInspection(ofVec3f inputModelVertex);
	int internalCageVertexInspection(ofVec3f inputCageVertex);
	int internalCageFaceInspection(ofMeshFace inputCageFace);
	vector<int> internalCageFaceFromVerticesInspection(vector<ofVec3f> inputCageVertices);

	void initObject();
	shared_ptr<ofVec3f> getModelVertices();
	shared_ptr<ofVec3f> getPartModelVertices(string partName);
	shared_ptr<ofVec3f> getCageVertices();
	shared_ptr<ofVec3f> getPartCageVertices(string partName);
	shared_ptr<ofMeshFace> getCageFaces();
	shared_ptr<ofMeshFace> getPartCageFaces(string partName);
	int getModelVerticesNum();
	int getCageVerticesNum();
	int getCageFacesNum();
	int getPartModelVerticesNum(string partName);
	int getPartCageVerticesNum(string partName);
	int getPartCageFacesNum(string partName);
	void updateModelVertices();
	void updateCageVertices();

private:
	vector<ofVec3f> vModelVertices;
	vector<ofVec3f> vCageVertices;
	vector<CageFaces> vCageFaces;
	vector<PartModelVertices> vPartModelVertices;
	vector<PartCageVertices> vPartCageVertices;
	vector<PartCageFaces> vPartCageFaces;
	vector<BarycentricCoord> vGreenCoord;
	vector<string> vPartsName;

	ofVec3f* modelVertices;
	ofVec3f* cageVertices;
	string* partsName;
	CageFaces* cageFaces;
	PartModelVertices* partModelVertices;
	PartCageVertices* partCageVertices;
	PartCageFaces* partCageFaces;
//	BarycentricCoord greenCoord;

	float foundThreshold = 0.00001;
};

