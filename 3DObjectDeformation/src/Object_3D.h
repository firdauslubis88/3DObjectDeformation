#pragma once

#include "ofMain.h"
#include "GreenCoordinate.h"
#include "Util.h"

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
	vector<int> internalCageFaceInspection(vector<ofVec3f> inputCageVertices);

	void initObject();
	ofVec3f* getModelVertices();
	ofVec3f* getPartModelVertices(string partName);
	ofVec3f* getCageVertices();
	ofVec3f* getPartCageVertices(string partName);
	ofMeshFace* getCageFaces();
	ofMeshFace* getPartCageFaces();
	int getModelVerticesNum();
	int getCageVerticesNum();
	int getCageFacesNum();
	void updateModelVertices();
	void updateCageVertices();

private:
	typedef struct CageFaces {
		int id;
		int verticesId[3];
	} CageFaces;

	typedef struct PartModelVertices {
		int id;
		string partName;
		int* modelVerticesId;
	} PartModelVertices;

	typedef struct PartCageVertices {
		int id;
		string partName;
		int* cageVerticesId;
	} PartCageVertices;

	typedef struct PartCageFaces {
		int id;
		string partName;
		int* cageFacesId;
	} PartCageFaces;

	typedef struct BarycentricCoord {
		int modelVertexId;
		string partName;
		pair<int, double>* cageVerticesId;
		pair<int, double>* cageFacesId;
	} BarycentricCoord;

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

