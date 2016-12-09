#pragma once
#include "ofMain.h"
#include "Util.h"

class GreenCoordinate
{
public:
	GreenCoordinate();
	~GreenCoordinate();

	void initCage(ofMesh mesh);
	void cagePartsInit(string partName);
	void initModel(ofMesh mesh);

	vector<ofMeshFace> meshFaces;
	vector<ofPoint> meshFacesNormal;
	vector<std::pair<ofMeshFace, ofPoint>> meshFacesAndNormalPair;
	int numMeshFaces;
	vector<string> partsName;
	std::map<string,vector<std::pair<ofMeshFace, ofPoint>>> cagePartsVerticesMap;
	std::map<string, vector<ofPoint>> modelPartsVerticesMap;
};

