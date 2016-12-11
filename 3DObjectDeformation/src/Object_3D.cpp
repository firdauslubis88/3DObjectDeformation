#include "Object_3D.h"



Object_3D::Object_3D()
{
}


Object_3D::~Object_3D()
{
}

int* Object_3D::addModelVertices(ofVec3f* inputVertices, int inputVerticesNum)
{
	int* detectedModelVertexIndex = new int[inputVerticesNum];
//	cout << sizeof(inputVertices) << endl;
	for (size_t i = 0; i < inputVerticesNum; i++)
	{
//		cout << i << endl;
		ofVec3f inputVertex = inputVertices[i];
		detectedModelVertexIndex[i] = internalModelVertexInspection(inputVertex);
//		cout << detectedModelVertexIndex[i] << endl;
		if (detectedModelVertexIndex[i] < 0)
		{
			detectedModelVertexIndex[i] = this->vModelVertices.size();
			this->vModelVertices.push_back(inputVertex);
		}
	}
	return detectedModelVertexIndex;
}

int* Object_3D::addCageVertices(ofVec3f* inputVertices, int inputVerticesNum)
{
	int* detectedCageVertexIndex = new int[inputVerticesNum];
	for (size_t i = 0; i < inputVerticesNum; i++)
	{
		ofVec3f inputVertex = inputVertices[i];
		detectedCageVertexIndex[i] = internalCageVertexInspection(inputVertex);
		if (detectedCageVertexIndex[i] < 0)
		{
			detectedCageVertexIndex[i] = this->vCageVertices.size();
			this->vCageVertices.push_back(inputVertex);
		}
	}
	return detectedCageVertexIndex;
}

int* Object_3D::addCageFacesByMeshFaces(ofMeshFace* meshFaces, int inputMeshFacesNum)
{
	int* cageFacesIndices = new int[inputMeshFacesNum];

	for (size_t i = 0; i < inputMeshFacesNum; i++)
	{
		ofMeshFace meshFace = meshFaces[i];
		ofVec3f vertices[3];
		vertices[0] = meshFace.getVertex(0);
		vertices[1] = meshFace.getVertex(1);
		vertices[2] = meshFace.getVertex(2);
		cageFacesIndices[i] = addCageFacesByVertices(vertices,3);
	}
	return cageFacesIndices;
}

int Object_3D::addCageFacesByVertices(ofVec3f* inputFaceVertices, int inputFaceVerticesNum)
{
	if (inputFaceVerticesNum != 3)
	{
		ofLogError() << "You have to input 3 vertices to form one new cage face!!";
		return -1;
	}

	int* cageVerticesIndices =  addCageVertices(inputFaceVertices,3);

	int internalCageFacesNumber = this->vCageFaces.size();
	CageFaces tempCageFaces;
	tempCageFaces.id = internalCageFacesNumber;
	tempCageFaces.verticesId[0] = cageVerticesIndices[0];
	tempCageFaces.verticesId[1] = cageVerticesIndices[1];
	tempCageFaces.verticesId[2] = cageVerticesIndices[2];
	this->vCageFaces.push_back(tempCageFaces);
	return internalCageFacesNumber;
}

int Object_3D::addPartsName(string partName)
{
	for (size_t i = 0; i < this->vPartsName.size(); i++)
	{
		string partsNameInspected = this->vPartsName[i];
		if (partName == partsNameInspected)
		{
			return i;
		}
	}
	int detectedPartsNameIndex = this->vPartsName.size();
	this->vPartsName.push_back(partName);
	return detectedPartsNameIndex;
}



void Object_3D::createModelAndCage(ofVec3f * inputModelVertices, ofVec3f * inputCageVertices, ofMeshFace* inputCageFaces, int inputModelVertices_size, int inputCageVertices_size, int inputCageFaces_size)
{
	addModelVertices(inputModelVertices, inputModelVertices_size);
	addCageVertices(inputCageVertices, inputCageVertices_size);
	addCageFacesByMeshFaces(inputCageFaces, inputCageFaces_size);
	initObject();
}

void Object_3D::createModelAndCage(vector<ofVec3f> inputVectorModelVertices, vector<ofVec3f> inputVectorCageVertices, vector<ofMeshFace> inputVectorCageFaces)
{
	ofVec3f* inputModelVertices = new ofVec3f[inputVectorModelVertices.size()];
	for (size_t i = 0; i < inputVectorModelVertices.size(); i++)
	{
		inputModelVertices[i] = inputVectorModelVertices[i];
	}

	ofVec3f* inputCageVertices = new ofVec3f[inputVectorCageVertices.size()];
	for (size_t i = 0; i < inputVectorCageVertices.size(); i++)
	{
		inputCageVertices[i] = inputVectorCageVertices[i];
	}

	ofMeshFace* inputCageFaces = new ofMeshFace[inputVectorCageFaces.size()];
	for (size_t i = 0; i < inputVectorCageFaces.size(); i++)
	{
		inputCageFaces[i] = inputVectorCageFaces[i];
	}

	createModelAndCage(inputModelVertices, inputCageVertices, inputCageFaces, inputVectorModelVertices.size(), inputVectorCageVertices.size(), inputVectorCageFaces.size());

	//Place here (temporarily)
	string partName = "finger";
	vector<ofVec3f> cageVertices = Util::readPartCageVerticesFromFile(partName);
	createPartsFromModel(partName, cageVertices);
}

void Object_3D::createPartsFromModel(string inputPartName, vector<ofVec3f> inputPartCageVertices)
{
	int partIndex = addPartsName(inputPartName);
	vector<int> cageFacesIndesDetected = internalCageFaceInspection(inputPartCageVertices);

	int* detectedInternalCageFacesIndex = new int[cageFacesIndesDetected.size()];
	for (size_t i = 0; i < cageFacesIndesDetected.size(); i++)
	{
		detectedInternalCageFacesIndex[i] = cageFacesIndesDetected[i];
	}

	ofVec3f* tempAddPartCageVertices = new ofVec3f[inputPartCageVertices.size()];
	for (size_t i = 0; i < inputPartCageVertices.size(); i++)
	{
		tempAddPartCageVertices[i] = inputPartCageVertices[i];
	}

	int* detectedInternalCageVerticesIndices = addCageVertices(tempAddPartCageVertices, inputPartCageVertices.size());
	PartCageVertices tempPartCageVertices;
	tempPartCageVertices.id = partIndex;
	tempPartCageVertices.partName = inputPartName;
	tempPartCageVertices.cageVerticesId = detectedInternalCageVerticesIndices;
	this->vPartCageVertices.push_back(tempPartCageVertices);

	PartCageFaces tempPartCageFace;
	tempPartCageFace.id = partIndex;
	tempPartCageFace.partName = inputPartName;
	tempPartCageFace.cageFacesId = detectedInternalCageFacesIndex;
	this->vPartCageFaces.push_back(tempPartCageFace);

	vector<ofVec3f> tempModelVertices;
	for (size_t j = 0; j < vModelVertices.size(); j++)
	{
		ofVec3f testPoint = vModelVertices[j];
		bool decision = false;
		int partCageFacesSize = cageFacesIndesDetected.size();
		for (size_t k = 0; k < partCageFacesSize; k++)
		{
			ofVec3f facePoint = this->vCageVertices[this->vCageFaces[detectedInternalCageFacesIndex[k]].verticesId[0]];
			ofVec3f a = this->vCageVertices[this->vCageFaces[detectedInternalCageFacesIndex[k]].verticesId[0]];
			ofVec3f b = this->vCageVertices[this->vCageFaces[detectedInternalCageFacesIndex[k]].verticesId[1]];
			ofVec3f c = this->vCageVertices[this->vCageFaces[detectedInternalCageFacesIndex[k]].verticesId[2]];
			ofVec3f ba = b - a;
			ofVec3f ca = c - a;
			ofVec3f d = ba.getCrossed(ca);
			ofVec3f faceNormal = d.normalize();
			if (k == 11)
			{
				faceNormal = -faceNormal;
			}
			decision = !Util::upperLowerPlaneTest(testPoint, faceNormal, facePoint);

			if (!decision)
			{
				break;
			}
		}
		if (decision)
		{
			tempModelVertices.push_back(testPoint);
		}
	}
}

void Object_3D::createPartsFromScratch(string inputPartName, ofVec3f* inputPartModelVertices, ofMeshFace* inputPartCageMeshFaces, int inputPartModelVertices_size, int inputPartCageMeshFaces_size)
{
	int partIndex = addPartsName(inputPartName);
	int* detectedInternalModelVerticesIndices = addModelVertices(inputPartModelVertices, inputPartModelVertices_size);
	int* detectedInternalCageFacesIndex = addCageFacesByMeshFaces(inputPartCageMeshFaces, inputPartCageMeshFaces_size);

	PartModelVertices tempPartModelVertices;
	tempPartModelVertices.id = partIndex;
	tempPartModelVertices.partName = inputPartName;
	tempPartModelVertices.modelVerticesId = detectedInternalModelVerticesIndices;
	this->vPartModelVertices.push_back(tempPartModelVertices);

	PartCageFaces tempPartCageFace;
	tempPartCageFace.id = partIndex;
	tempPartCageFace.partName = inputPartName;
	tempPartCageFace.cageFacesId = detectedInternalCageFacesIndex;
	this->vPartCageFaces.push_back(tempPartCageFace);
}

bool Object_3D::initBarycentricCoordinate()
{
	return false;
}

int Object_3D::internalModelVertexInspection(ofVec3f inputModelVertex)
{
	ofVec3f inputModelVertexInspected = inputModelVertex;
	for (size_t j = 0; j < this->vModelVertices.size(); j++)
	{
		ofVec3f modelVerticesInspected = this->vModelVertices[j];
		if (modelVerticesInspected.distance(inputModelVertexInspected) < foundThreshold)
		{
			return j;
		}
	}
	return -1;
}

int Object_3D::internalCageVertexInspection(ofVec3f inputCageVertex)
{
	ofVec3f inputCageVertexInspected = inputCageVertex;
	for (size_t j = 0; j < this->vCageVertices.size(); j++)
	{
		ofVec3f cageVerticesInspected = this->vCageVertices[j];
		if (cageVerticesInspected.distance(inputCageVertexInspected) < foundThreshold)
		{
			return j;
		}
	}
	return -1;
}

int Object_3D::internalCageFaceInspection(ofMeshFace inputCageFace)
{
	bool indicator;
	ofMeshFace inputCageFaceInspected = inputCageFace;
	for (size_t j = 0; j < this->vCageFaces.size(); j++)
	{
		CageFaces cageFacesInspected = this->vCageFaces[j];
		for (size_t k = 0; k < 3; k++)
		{
			indicator = false;
			ofVec3f inputCageFaceVertexInspected = inputCageFaceInspected.getVertex(k);
			for (size_t l = 0; l < 3; l++)
			{
				if (inputCageFaceVertexInspected.distance(this->vCageVertices[cageFacesInspected.verticesId[l]]) < foundThreshold)
				{
					indicator = true;
					break;
				}
			}
			if (!indicator)
			{
				break;
			}
		}
		if (indicator)
		{
			return j;
		}
	}
}

vector<int> Object_3D::internalCageFaceInspection(vector<ofVec3f> inputCageVertices)
{
	bool indicator;
	vector<int> vIndexDetected;
	int count = 0;
	for (size_t j = 0; j < this->vCageFaces.size(); j++)
	{
		CageFaces cageFacesInspected = this->vCageFaces[j];
		for (size_t k = 0; k < 3; k++)
		{
			indicator = false;
			ofVec3f cageFaceVertexInspected = this->vCageVertices[cageFacesInspected.verticesId[k]];
			for (size_t l = 0; l < inputCageVertices.size(); l++)
			{
				if (cageFaceVertexInspected.distance(inputCageVertices[l]) < foundThreshold)
				{
					indicator = true;
					break;
				}
			}
			if (!indicator)
			{
				break;
			}
		}
		if (indicator)
		{
			vIndexDetected.push_back(j);
		}
	}
	return vIndexDetected;
}

void Object_3D::initObject()
{
	modelVertices = new ofVec3f[vModelVertices.size()];
	for (size_t i = 0; i < vModelVertices.size(); i++)
	{
		modelVertices[i] = vModelVertices[i];
	}

	cageVertices = new ofVec3f[vCageVertices.size()];
	for (size_t i = 0; i < vCageVertices.size(); i++)
	{
		cageVertices[i] = vCageVertices[i];
	}

	cageFaces = new CageFaces[vCageFaces.size()];
	for (size_t i = 0; i < vCageFaces.size(); i++)
	{
		cageFaces[i] = vCageFaces[i];
	}

	partModelVertices = new PartModelVertices[vPartModelVertices.size()];
	for (size_t i = 0; i < vPartModelVertices.size(); i++)
	{
		partModelVertices[i] = vPartModelVertices[i];
	}

	partCageVertices = new PartCageVertices[vPartCageVertices.size()];
	for (size_t i = 0; i < vPartCageVertices.size(); i++)
	{
		partCageVertices[i] = vPartCageVertices[i];
	}

	partCageFaces = new PartCageFaces[vPartCageFaces.size()];
	for (size_t i = 0; i < vPartCageFaces.size(); i++)
	{
		partCageFaces[i] = vPartCageFaces[i];
	}
}

ofVec3f * Object_3D::getModelVertices()
{
	ofVec3f* outputModelVertices = new ofVec3f[this->vModelVertices.size()];
//	cout << vModelVertices.size() << endl;
	for (size_t i = 0; i < this->vModelVertices.size(); i++)
	{
		outputModelVertices[i] = modelVertices[i];
//		cout << vModelVertices[i] << endl;
	}
	return outputModelVertices;
}

ofVec3f * Object_3D::getPartModelVertices(string partName)
{
	ofVec3f* outputPartModelVertices = new ofVec3f[vPartModelVertices.size()];
	PartModelVertices inspectedPartModelVertex;
	bool found = false;
	for (size_t i = 0; i < vPartModelVertices.size(); i++)
	{
		inspectedPartModelVertex = partModelVertices[i];
		string inspectedName = inspectedPartModelVertex.partName;
		if (inspectedName == partName)
		{
			int partModelVerticesNum = sizeof(inspectedPartModelVertex.modelVerticesId) / sizeof(*inspectedPartModelVertex.modelVerticesId);
			for (size_t j = 0; j < partModelVerticesNum; j++)
			{
				outputPartModelVertices[j] = modelVertices[inspectedPartModelVertex.modelVerticesId[j]];
			}
			return outputPartModelVertices;
		}
	}
	if (!found)
	{
		return nullptr;
	}
}

ofVec3f * Object_3D::getCageVertices()
{
	ofVec3f* outputCageVertices = new ofVec3f[this->vCageVertices.size()];
	for (size_t i = 0; i < this->vCageVertices.size(); i++)
	{
		outputCageVertices[i] = cageVertices[i];
	}
	return outputCageVertices;
}

ofVec3f * Object_3D::getPartCageVertices(string partName)
{
	ofVec3f* outputPartCageVertices = new ofVec3f[vPartCageVertices.size()];
	PartCageVertices inspectedPartCageVertex;
	bool found = false;
	for (size_t i = 0; i < vPartModelVertices.size(); i++)
	{
		inspectedPartCageVertex = partCageVertices[i];
		string inspectedName = inspectedPartCageVertex.partName;
		if (inspectedName == partName)
		{
			int partModelVerticesNum = sizeof(inspectedPartCageVertex.cageVerticesId) / sizeof(*inspectedPartCageVertex.cageVerticesId);
			for (size_t j = 0; j < partModelVerticesNum; j++)
			{
				outputPartCageVertices[j] = modelVertices[inspectedPartCageVertex.cageVerticesId[j]];
			}
			return outputPartCageVertices;
		}
	}
	if (!found)
	{
		return nullptr;
	}
}

ofMeshFace * Object_3D::getCageFaces()
{
	ofMeshFace* outputCageFaces = new ofMeshFace[this->vCageFaces.size()];
	for (size_t i = 0; i < this->vCageFaces.size(); i++)
	{
		outputCageFaces[i].setVertex(0, modelVertices[cageFaces[i].verticesId[0]]);
		outputCageFaces[i].setVertex(1, modelVertices[cageFaces[i].verticesId[1]]);
		outputCageFaces[i].setVertex(2, modelVertices[cageFaces[i].verticesId[2]]);
	}
	return outputCageFaces;
}

ofMeshFace * Object_3D::getPartCageFaces()
{
	return nullptr;
}

int Object_3D::getModelVerticesNum()
{
	return this->vModelVertices.size();
}

int Object_3D::getCageVerticesNum()
{
	return this->vCageVertices.size();
}

int Object_3D::getCageFacesNum()
{
	return this->vCageFaces.size();
}

void Object_3D::updateModelVertices()
{
}

void Object_3D::updateCageVertices()
{
}

