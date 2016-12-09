#include "GreenCoordinate.h"



GreenCoordinate::GreenCoordinate()
{
	partsName.push_back("cage finger");
}


GreenCoordinate::~GreenCoordinate()
{
}

void GreenCoordinate::initCage(ofMesh cageMesh)
{
	meshFaces = cageMesh.getUniqueFaces();
	vector<ofPoint> tempFaceNormals = cageMesh.getFaceNormals();
	for (size_t i = 0, j = 0; i < 3*meshFaces.size(); i+=3, j++)
	{
		meshFacesNormal.push_back(tempFaceNormals[i]);
		meshFacesAndNormalPair.push_back(pair<ofMeshFace, ofPoint>(meshFaces[j], tempFaceNormals[i]));
	}

	vector<ofVec3f> additionalVertices;
	ofVec3f tempAdd1 = ofVec3f(-4.5592, 10.4961, 0.7687);
	ofVec3f tempAdd2 = ofVec3f(-4.5458, 10.495, -1.0324);
	ofVec3f tempAdd3 = ofVec3f(-5.8635, 10.5888, 0.6961);
	ofVec3f tempAdd4 = ofVec3f(-5.9954, 10.5877, -1.3406);
	ofMeshFace tempMeshFace1 = ofMeshFace();
	tempMeshFace1.setVertex(0, tempAdd1);
	tempMeshFace1.setVertex(1, tempAdd2);
	tempMeshFace1.setVertex(2, tempAdd3);
	ofMeshFace tempMeshFace2 = ofMeshFace();
	tempMeshFace2.setVertex(0, tempAdd2);
	tempMeshFace2.setVertex(1, tempAdd3);
	tempMeshFace2.setVertex(2, tempAdd4);
	ofPoint tempFaceNormal1, tempFaceNormal2;
	ofVec3f ba = tempAdd2 - tempAdd1;
	ofVec3f ca = tempAdd3 - tempAdd1;
	ofVec3f d = ba.cross(ca);
	tempFaceNormal1 = d.normalize();
	ba = tempAdd3 - tempAdd2;
	ca = tempAdd4 - tempAdd2;
	d = ba.cross(ca);
	tempFaceNormal2 = -d.normalize();
//	cout << meshFacesAndNormalPair.size() << endl;
	meshFaces.push_back(tempMeshFace1);
	meshFaces.push_back(tempMeshFace2);
	meshFacesNormal.push_back(tempFaceNormal1);
	meshFacesNormal.push_back(tempFaceNormal2);
	meshFacesAndNormalPair.push_back(pair<ofMeshFace, ofPoint>(tempMeshFace1, tempFaceNormal1));
	meshFacesAndNormalPair.push_back(pair<ofMeshFace, ofPoint>(tempMeshFace2, tempFaceNormal2));
//	cout << meshFacesAndNormalPair.size() << endl;

	numMeshFaces = meshFacesAndNormalPair.size();

	for (size_t i = 0; i < partsName.size(); i++)
	{
		cagePartsInit(partsName[i]);
	}
}

void GreenCoordinate::cagePartsInit(string partName)
{
	vector<string>lines = ofSplitString(ofBufferFromFile(partName).getText(), "\n");
	vector<ofVec3f> partVertices;
	vector<pair<ofMeshFace,ofPoint>> tempMeshFacesAndNormal;

	for (size_t i = 0; i < (lines.size()-1); i++)
	{
		float x, y, z;
		auto values = ofSplitString(lines[i], " ");
		istringstream(values[0]) >> x;
		istringstream(values[1]) >> y;
		istringstream(values[2]) >> z;
		ofVec3f vertex = ofVec3f(x, y, z);
		partVertices.push_back(vertex);
//		cout << vertex << endl;
	}



	int count = 0;
	for (size_t i = 0; i < numMeshFaces; i++)
	{
		ofMeshFace meshFace = meshFacesAndNormalPair[i].first;//meshFaces[i];
		bool included = false;
		for (size_t j = 0; j < 3; j++)
		{
			ofVec3f meshFaceVertex = meshFace.getVertex(j);
//			cout << "MeshFaces-" << i << "\tVertex-" << j << ":\t" << meshFaceVertex << endl;
			for (size_t k = 0; k < partVertices.size(); k++)
			{
				ofVec3f partVertex = partVertices[k];
				if (meshFaceVertex.distance(partVertex) < 0.00001)
				{
					included = true;
					break;
				}
			}
			if (!included)
			{
				break;
			}
		}
		if (included)
		{
//			cout << meshFacesNormal[i] << endl;
			count++;
			tempMeshFacesAndNormal.push_back(pair<ofMeshFace, ofPoint>(meshFace,meshFacesNormal[i]));
//			meshF
		}
	}
	if (tempMeshFacesAndNormal.size() != 0)
	{
		cagePartsVerticesMap.insert(std::pair<std::string, vector<pair<ofMeshFace, ofPoint>>>(partName, tempMeshFacesAndNormal));
	}
//	cout << tempMeshFacesAndNormal.size() << endl;
//	cout << count << endl;
}

void GreenCoordinate::initModel(ofMesh modelMesh)
{
	for (size_t i = 0; i < partsName.size(); i++)
	{
		vector<pair<ofMeshFace, ofPoint>>meshFacesAndNormalPair = cagePartsVerticesMap.find(partsName[i])->second;
		vector<ofVec3f> vertices = modelMesh.getVertices();
		vector<ofPoint> tempModelVertices;
		for (size_t j = 0; j < vertices.size(); j++)
		{
			ofVec3f testPoint = vertices[j];
			bool decision = false;
			for (size_t k = 0; k < meshFacesAndNormalPair.size(); k++)
			{
				ofVec3f facePoint = meshFacesAndNormalPair[k].first.getVertex(0);
				ofVec3f faceNormal = meshFacesAndNormalPair[k].second;
				decision = !Util::upperLowerPlaneTest(testPoint, faceNormal, facePoint);
				if (!decision)
				{
					break;
				}
			}
			if (decision)
			{
				tempModelVertices.push_back(testPoint);
//				cout << testPoint << endl;
			}
		}
		modelPartsVerticesMap.insert(pair<string, vector<ofPoint>>(partsName[i], tempModelVertices));
//		cout << "Model Vertex Number inside hand cage:\t" << tempModelVertices.size() << endl;
	}
}
