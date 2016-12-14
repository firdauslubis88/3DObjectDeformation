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
	for (size_t i = 0; i < inputVerticesNum; i++)
	{
		ofVec3f inputVertex = inputVertices[i];
		detectedModelVertexIndex[i] = internalModelVertexInspection(inputVertex);
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
	tempCageFaces.normalSign = 1;
	this->vCageFaces.push_back(tempCageFaces);
	delete[] cageVerticesIndices;
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
	delete[] inputModelVertices;
	delete[] inputCageVertices;
	delete[] inputCageFaces;

	//Place here (temporarily)
	string partName = "finger";
	vector<ofVec3f> cageVertices = Util::readPartCageVerticesFromFile(partName);
	createPartsFromModel(partName, cageVertices);
	initBarycentricCoordinate();
	initObject();
}

void Object_3D::createPartsFromModel(string inputPartName, vector<ofVec3f> inputPartCageVertices)
{
	int partIndex = addPartsName(inputPartName);
	vector<int> cageFacesIndesDetected = internalCageFaceFromVerticesInspection(inputPartCageVertices);

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
	delete[] tempAddPartCageVertices;
	PartCageVertices tempPartCageVertices;
	tempPartCageVertices.id = partIndex;
	tempPartCageVertices.partName = inputPartName;
	tempPartCageVertices.cageVerticesId = detectedInternalCageVerticesIndices;
	tempPartCageVertices.partCageVerticesNum = inputPartCageVertices.size();
	this->vPartCageVertices.push_back(tempPartCageVertices);

	PartCageFaces tempPartCageFace;
	tempPartCageFace.id = partIndex;
	tempPartCageFace.partName = inputPartName;
	tempPartCageFace.cageFacesId = detectedInternalCageFacesIndex;
	tempPartCageFace.partCageFacesNum = cageFacesIndesDetected.size();
	this->vPartCageFaces.push_back(tempPartCageFace);

	vector<ofVec3f> tempVPartModelVertices;
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
			decision = !Util::upperLowerPlaneTest(testPoint, faceNormal, facePoint);

			if (!decision)
			{
				break;
			}
		}
		if (decision)
		{
			tempVPartModelVertices.push_back(testPoint);
		}
	}

	ofVec3f* tempAddPartModelVertices = new ofVec3f[tempVPartModelVertices.size()];
	for (size_t i = 0; i < tempVPartModelVertices.size(); i++)
	{
		tempAddPartModelVertices[i] = tempVPartModelVertices[i];
	}

	int* detectedInternalModelVerticesIndices = addModelVertices(tempAddPartModelVertices, tempVPartModelVertices.size());
	delete[] tempAddPartModelVertices;
	PartModelVertices tempPartModelVertices;
	tempPartModelVertices.id = partIndex;
	tempPartModelVertices.partName = inputPartName;
	tempPartModelVertices.modelVerticesId = detectedInternalModelVerticesIndices;
	tempPartModelVertices.partModelVerticesNum = tempVPartModelVertices.size();
	this->vPartModelVertices.push_back(tempPartModelVertices);
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
	for (size_t i = 0; i < vPartsName.size(); i++)
	{
		string inspectedPartName = this->vPartsName[i];
		PartModelVertices inspectedPartModelVertex;
		PartCageVertices inspectedPartCageVertex;
		PartCageFaces inspectedPartCageFace;
		for (size_t j = 0; j < this->vPartModelVertices.size(); j++)
		{
			inspectedPartModelVertex = this->vPartModelVertices[j];
			if (inspectedPartModelVertex.partName == inspectedPartName)
			{
				break;
			}
			return false;
		}
		for (size_t j = 0; j < this->vPartCageVertices.size(); j++)
		{
			inspectedPartCageVertex = this->vPartCageVertices[j];
			if (inspectedPartCageVertex.partName == inspectedPartName)
			{
				break;
			}
			return false;
		}
		for (size_t j = 0; j < this->vPartCageFaces.size(); j++)
		{
			inspectedPartCageFace = this->vPartCageFaces[j];
			if (inspectedPartCageFace.partName == inspectedPartName)
			{
				break;
			}
			return false;
		}

		ofVec3f* passCageVertices = new ofVec3f[inspectedPartCageVertex.partCageVerticesNum];
		int* cageVerticesIdList = new int[inspectedPartCageVertex.partCageVerticesNum];
		for (size_t j = 0; j < inspectedPartCageVertex.partCageVerticesNum; j++)
		{
			passCageVertices[j] = this->vCageVertices[inspectedPartCageVertex.cageVerticesId[j]];
			cageVerticesIdList[j] = inspectedPartCageVertex.cageVerticesId[j];
		}

		ofMeshFace* passCageFaces = new ofMeshFace[inspectedPartCageFace.partCageFacesNum];
		int* cageFacesIdList = new int[inspectedPartCageFace.partCageFacesNum];
		for (size_t j = 0; j < inspectedPartCageFace.partCageFacesNum; j++)
		{
			CageFaces inspectedCageFace = this->vCageFaces[inspectedPartCageFace.cageFacesId[j]];
			ofVec3f v0 = this->vCageVertices[inspectedCageFace.verticesId[0]];
			ofVec3f v1 = this->vCageVertices[inspectedCageFace.verticesId[1]];
			ofVec3f v2 = this->vCageVertices[inspectedCageFace.verticesId[2]];
			ofVec3f ba = v1 - v0;
			ofVec3f ca = v2 - v0;
			ofVec3f d = ba.getCrossed(ca);
			ofVec3f n = d.normalize();

			passCageFaces[j].setVertex(0, v0);
			passCageFaces[j].setVertex(1, v1);
			passCageFaces[j].setVertex(2, v2);
			passCageFaces[j].setNormal(3, n); //could it work??

			cageFacesIdList[j] = inspectedPartCageFace.cageFacesId[j];
		}

		int* passModelVerticesId = inspectedPartModelVertex.modelVerticesId;
		int* modelGreenCoordsDetected = new int[inspectedPartModelVertex.partModelVerticesNum];
		for (size_t j = 0; j < inspectedPartModelVertex.partModelVerticesNum; j++)
		{
			double* psi = new double[inspectedPartCageFace.partCageFacesNum];
			for (size_t k = 0; k < inspectedPartCageFace.partCageFacesNum; k++)
			{
				psi[k] = 0.0;
			}

			double* phi = new double[inspectedPartCageVertex.partCageVerticesNum];
			for (size_t k = 0; k < inspectedPartCageVertex.partCageVerticesNum; k++)
			{
				phi[k] = 0.0;
			}

			double* s = new double[inspectedPartCageFace.partCageFacesNum];
			for (size_t k = 0; k < inspectedPartCageVertex.partCageVerticesNum; k++)
			{
				s[k] = 1.0;
			}

			ofVec3f passModelVertex = this->vModelVertices[passModelVerticesId[j]];
			GreenCoordinate::computeGreenCoordinate(passModelVertex, passCageFaces, passCageVertices, psi, phi, s, inspectedPartCageFace.partCageFacesNum, inspectedPartCageVertex.partCageVerticesNum);

			ModelBarycentricCoords tempModelGreenCoords;
			tempModelGreenCoords.modelVertexId = passModelVerticesId[j];
			tempModelGreenCoords.cageVerticesId = cageVerticesIdList;
			tempModelGreenCoords.cageFacesId = cageFacesIdList;
			tempModelGreenCoords.phi = phi;
			tempModelGreenCoords.psi = psi;
			tempModelGreenCoords.s = s;
			tempModelGreenCoords.cageVerticesNum = inspectedPartCageVertex.partCageVerticesNum;
			tempModelGreenCoords.cageFacesNum = inspectedPartCageFace.partCageFacesNum;

			modelGreenCoordsDetected[j] = i*inspectedPartModelVertex.partModelVerticesNum + j; //temporary solution T T
			this->vModelGreenCoords.push_back(tempModelGreenCoords);
		}
		delete[] passCageVertices;
		delete[] passCageFaces;

		PartBarycentricCoords tempPartGreenCoords;
		tempPartGreenCoords.partId = i;
		tempPartGreenCoords.partName = inspectedPartName;
		tempPartGreenCoords.modelBarycentricCoordId = modelGreenCoordsDetected;
		tempPartGreenCoords.partBarycentricCoordNum = inspectedPartModelVertex.partModelVerticesNum;
		this->vPartGreenCoords.push_back(tempPartGreenCoords);
	}
	return true;
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
	return -1;
}

vector<int> Object_3D::internalCageFaceFromVerticesInspection(vector<ofVec3f> inputCageVertices)
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
	this->modelVertices = new ofVec3f[this->vModelVertices.size()];
	for (size_t i = 0; i < this->vModelVertices.size(); i++)
	{
		this->modelVertices[i] = this->vModelVertices[i];
	}

	this->cageVertices = new ofVec3f[this->vCageVertices.size()];
	for (size_t i = 0; i < this->vCageVertices.size(); i++)
	{
		this->cageVertices[i] = this->vCageVertices[i];
	}

	this->cageFaces = new CageFaces[this->vCageFaces.size()];
	for (size_t i = 0; i < this->vCageFaces.size(); i++)
	{
		this->cageFaces[i] = this->vCageFaces[i];
	}

	this->modelGreenCoords = new ModelBarycentricCoords[this->vModelGreenCoords.size()];
	for (size_t i = 0; i < this->vModelGreenCoords.size(); i++)
	{
		this->modelGreenCoords[i] = this->vModelGreenCoords[i];
	}

	this->partModelVertices = new PartModelVertices[this->vPartModelVertices.size()];
	for (size_t i = 0; i < this->vPartModelVertices.size(); i++)
	{
		this->partModelVertices[i] = this->vPartModelVertices[i];
	}

	this->partCageVertices = new PartCageVertices[this->vPartCageVertices.size()];
	for (size_t i = 0; i < this->vPartCageVertices.size(); i++)
	{
		this->partCageVertices[i] = this->vPartCageVertices[i];
	}

	this->partCageFaces = new PartCageFaces[this->vPartCageFaces.size()];
	for (size_t i = 0; i < this->vPartCageFaces.size(); i++)
	{
		this->partCageFaces[i] = this->vPartCageFaces[i];
	}

	this->partGreenCoords = new PartBarycentricCoords[this->vPartGreenCoords.size()];
	for (size_t i = 0; i < this->vPartGreenCoords.size(); i++)
	{
		this->partGreenCoords[i] = this->vPartGreenCoords[i];
	}

	this->partsName = new string[this->vPartsName.size()];
	for (size_t i = 0; i < this->vPartsName.size(); i++)
	{
		this->partsName[i] = this->vPartsName[i];
	}
}

shared_ptr<ofVec3f> Object_3D::getModelVertices()
{
	shared_ptr<ofVec3f> outputModelVertices(new ofVec3f[this->vModelVertices.size()], array_deleter<ofVec3f>());
	for (size_t i = 0; i < this->vModelVertices.size(); i++)
	{
		outputModelVertices.get()[i] = this->modelVertices[i];
	}
	return outputModelVertices;
}

shared_ptr<ofVec3f> Object_3D::getPartModelVertices(string partName)
{
	PartModelVertices inspectedPartModelVertex;
	bool found = false;
	for (size_t i = 0; i < vPartModelVertices.size(); i++)
	{
		inspectedPartModelVertex = partModelVertices[i];
		string inspectedName = inspectedPartModelVertex.partName;
		if (inspectedName == partName)
		{
			int partModelVerticesNum = inspectedPartModelVertex.partModelVerticesNum;
			shared_ptr<ofVec3f> outputPartModelVertices(new ofVec3f[partModelVerticesNum], array_deleter<ofVec3f>());
			for (size_t j = 0; j < partModelVerticesNum; j++)
			{
				outputPartModelVertices.get()[j] = this->modelVertices[inspectedPartModelVertex.modelVerticesId[j]];
			}
			return outputPartModelVertices;
		}
	}
	if (!found)
	{
		return nullptr;
	}
}

shared_ptr<ofVec3f> Object_3D::getCageVertices()
{
	shared_ptr<ofVec3f> outputCageVertices(new ofVec3f[this->vCageVertices.size()]);
	for (size_t i = 0; i < this->vCageVertices.size(); i++)
	{
		outputCageVertices.get()[i] = this->cageVertices[i];
	}
	return outputCageVertices;
}

shared_ptr<ofVec3f> Object_3D::getPartCageVertices(string partName)
{
	PartCageVertices inspectedPartCageVertex;
	bool found = false;
	for (size_t i = 0; i < vPartModelVertices.size(); i++)
	{
		inspectedPartCageVertex = partCageVertices[i];
		string inspectedName = inspectedPartCageVertex.partName;
		if (inspectedName == partName)
		{
			int partCageVerticesNum = inspectedPartCageVertex.partCageVerticesNum;
			shared_ptr<ofVec3f> outputPartCageVertices(new ofVec3f[partCageVerticesNum], array_deleter<ofVec3f>());
			for (size_t j = 0; j < partCageVerticesNum; j++)
			{
				outputPartCageVertices.get()[j] = this->modelVertices[inspectedPartCageVertex.cageVerticesId[j]];
			}
			return outputPartCageVertices;
		}
	}
	if (!found)
	{
		return nullptr;
	}
}

shared_ptr<ofMeshFace> Object_3D::getCageFaces()
{
	shared_ptr<ofMeshFace> outputCageFaces(new ofMeshFace[this->vCageFaces.size()],array_deleter<ofMeshFace>());
	for (size_t i = 0; i < this->vCageFaces.size(); i++)
	{
		outputCageFaces.get()[i].setVertex(0, modelVertices[cageFaces[i].verticesId[0]]);
		outputCageFaces.get()[i].setVertex(1, modelVertices[cageFaces[i].verticesId[1]]);
		outputCageFaces.get()[i].setVertex(2, modelVertices[cageFaces[i].verticesId[2]]);
	}
	return outputCageFaces;
}

shared_ptr<ofMeshFace> Object_3D::getPartCageFaces(string partName)
{
	bool found = false;
	PartCageFaces detectedPartCageFaces;
	for (size_t i = 0; i < vPartCageFaces.size(); i++)
	{
		PartCageFaces inspectedPartCageFaces = partCageFaces[i];
		if (inspectedPartCageFaces.partName == partName)
		{
			detectedPartCageFaces = inspectedPartCageFaces;
			found = true;
			break;
		}
	}
	if (!found)
	{
		return nullptr;
	}
	shared_ptr<ofMeshFace> returnMeshFaces(new ofMeshFace[detectedPartCageFaces.partCageFacesNum]);
	for (size_t i = 0; i < detectedPartCageFaces.partCageFacesNum; i++)
	{
		ofVec3f v0 = this->cageVertices[this->cageFaces[detectedPartCageFaces.cageFacesId[i]].verticesId[0]];
		ofVec3f v1 = this->cageVertices[this->cageFaces[detectedPartCageFaces.cageFacesId[i]].verticesId[1]];
		ofVec3f v2 = this->cageVertices[this->cageFaces[detectedPartCageFaces.cageFacesId[i]].verticesId[2]];

		returnMeshFaces.get()[i].setVertex(0, v0);
		returnMeshFaces.get()[i].setVertex(1, v1);
		returnMeshFaces.get()[i].setVertex(2, v2);
	}

	return returnMeshFaces;
}

shared_ptr<ofVec3f> Object_3D::getPartVerticesATGreen(string partName)
{
	bool found = false;
	PartBarycentricCoords detectedPartGreenCoords;
	for (size_t i = 0; i < vPartGreenCoords.size(); i++)
	{
		PartBarycentricCoords inspectedPartGreenCoords = partGreenCoords[i];
		if (inspectedPartGreenCoords.partName == partName)
		{
			detectedPartGreenCoords = inspectedPartGreenCoords;
			found = true;
			break;
		}
	}
	if (!found)
	{
		return nullptr;
	}
	shared_ptr<ofVec3f> returnModelVertices(new ofVec3f[detectedPartGreenCoords.partBarycentricCoordNum]);
	for (size_t i = 0; i < detectedPartGreenCoords.partBarycentricCoordNum; i++)
	{
		ModelBarycentricCoords inspectedModelGreenCoord = this->modelGreenCoords[detectedPartGreenCoords.modelBarycentricCoordId[i]];
		ofVec3f inspectedModelVertex = this->modelVertices[inspectedModelGreenCoord.modelVertexId];
		ofVec3f* inspectedCageVertices = new ofVec3f[inspectedModelGreenCoord.cageVerticesNum];
		ofVec3f* inspectedCageFaces = new ofVec3f[inspectedModelGreenCoord.cageFacesNum];

		returnModelVertices.get()[i] = ofVec3f(0,0,0);
		double checkPhi = 0;
		for (size_t j = 0; j < inspectedModelGreenCoord.cageVerticesNum; j++)
		{
			returnModelVertices.get()[i] += inspectedModelGreenCoord.phi[j]* this->cageVertices[inspectedModelGreenCoord.cageVerticesId[j]];
			checkPhi += inspectedModelGreenCoord.phi[j];
		}
		for (size_t j = 0; j < inspectedModelGreenCoord.cageFacesNum; j++)
		{
			CageFaces currentCageFace = this->cageFaces[inspectedModelGreenCoord.cageFacesId[j]];
			ofVec3f v0 = this->cageVertices[currentCageFace.verticesId[0]];
			ofVec3f v1 = this->cageVertices[currentCageFace.verticesId[1]];
			ofVec3f v2 = this->cageVertices[currentCageFace.verticesId[2]];
			ofVec3f ba = v1 - v0;
			ofVec3f ca = v2 - v0;
			ofVec3f d = ba.getCrossed(ca);
			ofVec3f n = d.normalize();

			returnModelVertices.get()[i] += inspectedModelGreenCoord.psi[j] * n;
		}
		returnModelVertices.get()[i] /= checkPhi; // <- Normalization of phi so that sum of phi is equal to 1
	}
	return returnModelVertices;
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

int Object_3D::getPartModelVerticesNum(string partName)
{
	PartModelVertices inspectedPartModelVertices;
	for (size_t i = 0; i < vPartModelVertices.size(); i++)
	{
		inspectedPartModelVertices = this->partModelVertices[i];
		string inspectedName = inspectedPartModelVertices.partName;
		if (inspectedName == partName)
		{
			return inspectedPartModelVertices.partModelVerticesNum;
		}
	}
		return 0;
}

int Object_3D::getPartCageVerticesNum(string partName)
{
	PartCageVertices inspectedPartCageVertices;
	for (size_t i = 0; i < vPartCageVertices.size(); i++)
	{
		inspectedPartCageVertices = this->partCageVertices[i];
		string inspectedName = inspectedPartCageVertices.partName;
		if (inspectedName == partName)
		{
			return inspectedPartCageVertices.partCageVerticesNum;
		}
	}
	return 0;
}

int Object_3D::getPartCageFacesNum(string partName)
{
	PartCageFaces inspectedPartCageFaces;
	for (size_t i = 0; i < vPartCageFaces.size(); i++)
	{
		inspectedPartCageFaces = this->partCageFaces[i];
		string inspectedName = inspectedPartCageFaces.partName;
		if (inspectedName == partName)
		{
			return inspectedPartCageFaces.partCageFacesNum;
		}
	}
	return 0;
}

void Object_3D::update()
{
	updateCageVertices();
	updateModelVertices();
}

void Object_3D::updateCageVertices()
{
	for (size_t h = 0; h < vPartsName.size(); h++)
	{
		string partName = this->partsName[h];
		PartCageVertices detectedPartCageVertices;
		for (size_t i = 0; i < vPartCageVertices.size(); i++)
		{
			PartCageVertices inspectedPartCageVertex = this->partCageVertices[i];
			if (inspectedPartCageVertex.partName == partName)
			{
				detectedPartCageVertices = inspectedPartCageVertex;
				break;
			}
		}
		for (size_t i = 0; i < detectedPartCageVertices.partCageVerticesNum; i++)
		{
			if (this->cageVertices[detectedPartCageVertices.cageVerticesId[i]].y < 10.00)
			{
				this->cageVertices[detectedPartCageVertices.cageVerticesId[i]] += ofVec3f(handAddition, 0.0, 0.0);
			}
		}
	}
	handAddition = counter%800 < 400 ? 0.001 : -0.001;
	counter++;
}

void Object_3D::updateModelVertices()
{
	for (size_t h = 0; h < vPartsName.size(); h++)
	{
		string partName = this->partsName[h];
		PartBarycentricCoords detectedPartGreenCoords;
		for (size_t i = 0; i < vPartGreenCoords.size(); i++)
		{
			PartBarycentricCoords inspectedPartGreenCoords = partGreenCoords[i];
			if (inspectedPartGreenCoords.partName == partName)
			{
				detectedPartGreenCoords = inspectedPartGreenCoords;
				break;
			}
		}
		shared_ptr<ofVec3f> updateModelVertices(new ofVec3f[detectedPartGreenCoords.partBarycentricCoordNum]);
		for (size_t i = 0; i < detectedPartGreenCoords.partBarycentricCoordNum; i++)
		{
			ModelBarycentricCoords inspectedModelGreenCoord = this->modelGreenCoords[detectedPartGreenCoords.modelBarycentricCoordId[i]];

			updateModelVertices.get()[i] = ofVec3f(0, 0, 0);
			double checkPhi = 0;
			for (size_t j = 0; j < inspectedModelGreenCoord.cageVerticesNum; j++)
			{
				updateModelVertices.get()[i] += inspectedModelGreenCoord.phi[j] * this->cageVertices[inspectedModelGreenCoord.cageVerticesId[j]];
				checkPhi += inspectedModelGreenCoord.phi[j];
			}
			for (size_t j = 0; j < inspectedModelGreenCoord.cageFacesNum; j++)
			{
				CageFaces currentCageFace = this->cageFaces[inspectedModelGreenCoord.cageFacesId[j]];
				ofVec3f v0 = this->cageVertices[currentCageFace.verticesId[0]];
				ofVec3f v1 = this->cageVertices[currentCageFace.verticesId[1]];
				ofVec3f v2 = this->cageVertices[currentCageFace.verticesId[2]];
				ofVec3f ba = v1 - v0;
				ofVec3f ca = v2 - v0;
				ofVec3f d = ba.getCrossed(ca);
				ofVec3f n = d.normalize();

				updateModelVertices.get()[i] += inspectedModelGreenCoord.psi[j] * n;
			}
			updateModelVertices.get()[i] /= checkPhi; // <- Normalization of phi so that sum of phi is equal to 1
			this->modelVertices[inspectedModelGreenCoord.modelVertexId] = updateModelVertices.get()[i];
		}
	}
}

