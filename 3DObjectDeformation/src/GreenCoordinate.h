#pragma once
#include "ofMain.h"
#include "Object_3D.h"
#include "Util.h"

class GreenCoordinate
{
public:
	GreenCoordinate();
	~GreenCoordinate();

	void initCage(ofMesh mesh);
	void cagePartsInit(string partName);
	void initModel(ofMesh mesh);
	void computeGreenCoordinate(ofVec3f modelVertex, ofMeshFace* cageFaces, ofVec3f* cageVertices, double* psi, double* phi, double* s, int cageFacesNum, int cageVerticesNum);
	static double* computeGreenCoordinate(ofVec3f inputModelVertex, ofVec3f* inputCageVertices, ofMeshFace* inputCageFaces);
	double GCTriInt(ofVec3f p, ofVec3f v1, ofVec3f v2, ofVec3f e);
	void initVerticesPhiMap();
	void initFacesPsiMap();
	void deform();

	vector<ofMeshFace> meshFaces;
	vector<ofPoint> meshFacesNormal;
	vector<std::pair<ofMeshFace, ofPoint>> meshFacesAndNormalPair;
	int numMeshFaces;
	vector<string> partsName;
	std::map<string,vector<std::pair<ofMeshFace, ofPoint>>> cagePartsVerticesMap;
	std::map<string, vector<std::pair<ofMeshFace, ofPoint>>> originalCagePartsVerticesMap;
	std::map<string, vector<ofPoint>> modelPartsVerticesMap;
	std::map<string, std::vector<std::vector<pair<ofVec3f, double>>>> cagePartsVectorVerticesPhiMap;
	std::map<string, std::vector<std::vector<pair<ofMeshFace, double>>>> cagePartsVectorFacesPsiMap;
	float s[3] = { 1,1,1 };
};

