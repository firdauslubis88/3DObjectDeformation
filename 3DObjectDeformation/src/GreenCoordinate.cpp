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
	ofVec3f d = ba.getCrossed(ca);
	tempFaceNormal1 = d.normalize();
	ba = tempAdd3 - tempAdd2;
	ca = tempAdd4 - tempAdd2;
	d = ba.getCrossed(ca);
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
		originalCagePartsVerticesMap.insert(std::pair<std::string, vector<pair<ofMeshFace, ofPoint>>>(partName, tempMeshFacesAndNormal));
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
		initVerticesPhiMap();
		initFacesPsiMap();
//		cout << "Model Vertex Number inside hand cage:\t" << tempModelVertices.size() << endl;
	}
}

void GreenCoordinate::initVerticesPhiMap()
{
	std::vector<pair<ofVec3f, double>> VerticesPhiMap;
	std::vector<std::vector<pair<ofVec3f, double>>> vectorVerticesPhiMap;
	for (size_t part = 0; part < partsName.size(); part++)
	{
		vector<pair<ofMeshFace, ofPoint>>meshFacesAndNormalPair = cagePartsVerticesMap.find(partsName[part])->second;
		for (size_t i = 0; i < meshFacesAndNormalPair.size(); i++)
		{
			ofMeshFace meshFace = meshFacesAndNormalPair[i].first;
			for (size_t j = 0; j < 3; j++)
			{
				ofVec3f meshVertex = meshFace.getVertex(j);
				bool included = true;
				for (auto it = 0; it < VerticesPhiMap.size(); it++)
				{
					ofVec3f mapVertex = VerticesPhiMap[it].first;
					if (mapVertex.distance(meshVertex) < 0.00001)
					{
						included = false;
						break;
					}
				}
				if (included)
				{
					VerticesPhiMap.push_back(pair<ofVec3f, double>(meshVertex, 0));
				}
			}
		}

		vector<ofPoint>modelVertices = modelPartsVerticesMap.find(partsName[0])->second;
		for (size_t i = 0; i < modelVertices.size(); i++)
		{
			vectorVerticesPhiMap.push_back(VerticesPhiMap);
		}

		cagePartsVectorVerticesPhiMap.insert(pair < string, std::vector < std::vector<pair<ofVec3f, double>>>>(partsName[part], vectorVerticesPhiMap));
	}
}

void GreenCoordinate::initFacesPsiMap()
{
	std::vector<pair<ofMeshFace, double>> FacesPsiMap;
	std::vector<std::vector<pair<ofMeshFace, double>>> vectorFacesPsiMap;
	for (size_t part = 0; part < partsName.size(); part++)
	{
		vector<pair<ofMeshFace, ofPoint>>meshFacesAndNormalPair = cagePartsVerticesMap.find(partsName[part])->second;
		for (size_t i = 0; i < meshFacesAndNormalPair.size(); i++)
		{
			ofMeshFace meshFace = meshFacesAndNormalPair[i].first;
			FacesPsiMap.push_back(pair<ofMeshFace, double>(meshFace, 0.0));
		}

		vector<ofPoint>modelVertices = modelPartsVerticesMap.find(partsName[0])->second;
		for (size_t i = 0; i < modelVertices.size(); i++)
		{
			vectorFacesPsiMap.push_back(FacesPsiMap);
		}

		cagePartsVectorFacesPsiMap.insert(pair < string, std::vector < std::vector<pair<ofMeshFace, double>>>>(partsName[part], vectorFacesPsiMap));
	}
}

void GreenCoordinate::computeGreenCoordinate()
{
	int testI = 0;
	for (size_t i = 0; i < partsName.size(); i++)
	{
		vector<pair<ofMeshFace, ofPoint>>meshFacesAndNormalPair = cagePartsVerticesMap.find(partsName[i])->second;
		vector<ofPoint>modelVertices = modelPartsVerticesMap.find(partsName[i])->second;
		std::vector<std::vector<pair<ofVec3f, double>>>& vectorVerticesPhiMap = cagePartsVectorVerticesPhiMap.find(partsName[i])->second;
		std::vector<std::vector<pair<ofMeshFace, double>>>& vectorFacesPsiMap = cagePartsVectorFacesPsiMap.find(partsName[i])->second;
//		cout << modelVertices.size() << endl;
		for (size_t e = 0; e < modelVertices.size(); e++)
		{
			ofVec3f modelVertex = modelVertices[e]; //meshFacesAndNormalPair[0].first.getVertex(0);//
			ofVec3f estimate = ofVec3f(0, 0, 0);
			double* phi[3];
			vector<pair<ofVec3f, double>>& tempVerticesPhiMap = vectorVerticesPhiMap[e];
			vector<pair<ofMeshFace, double>>& tempFacesPsiMap = vectorFacesPsiMap[e];
			for (size_t j = 0; j < meshFacesAndNormalPair.size(); j++)
			{
				double* psi;
				ofMeshFace meshFace = meshFacesAndNormalPair[j].first;
				ofVec3f n = meshFacesAndNormalPair[j].second;
				//Preparing phi
				for (size_t l = 0; l < 3; l++)
				{
					ofVec3f cageVertex = meshFace.getVertex(l);
					for (size_t it = 0; it < tempVerticesPhiMap.size(); it++)
					{
						ofVec3f mapVertex = tempVerticesPhiMap[it].first;
						if (mapVertex.distance(cageVertex) < 0.00001)
						{
							phi[l] = &tempVerticesPhiMap[it].second;
//							cout << cageVertex << endl;
//							cout << *phi[l] << endl;
							break;
						}
					}
				}
				//Preparing psi
				for (size_t it = 0; it < tempFacesPsiMap.size(); it++)
				{
					ofMeshFace mapFace = tempFacesPsiMap[it].first;
					if (mapFace.getVertex(0).distance(meshFace.getVertex(0)) < 0.00001 && mapFace.getVertex(1).distance(meshFace.getVertex(1)) < 0.00001 && mapFace.getVertex(2).distance(meshFace.getVertex(2)) < 0.00001)
					{
						psi = &tempFacesPsiMap[it].second;
						break;
					}
				}

				ofVec3f v[3];
				for (size_t l = 0; l < 3; l++)
				{
					v[l] = meshFace.getVertex(l) - modelVertex;
//					cout << "j = "<< j << ":\t" << v[l] << endl;
				}
//				cout << "Volume at face " << j << ":\t" << v[0].dot(v[2].getCrossed(v[1])) << endl;
				/*
				cout << v[2] << endl;
				cout << "j = " << j << ":\t" << (v[2].getCrossed(v[1])) << endl;
				cout << v[2] << endl;
				*/
				ofVec3f p = v[0].dot(n)*n;
//				cout << n.length() << endl;
				double s[3], I[3], II[3];
				ofVec3f q[3], N[3];
				for (size_t l = 0; l < 3; l++)
				{
					int currentl = l;
					int nextl = (l + 1) % 3;
					s[l] = ((v[currentl] - p).getCrossed(v[nextl] - p)).dot(n) > 0 ? 1 : -1;
					I[l] = GCTriInt(p, v[currentl], v[nextl], ofVec3f(0.0, 0.0, 0.0));
					II[l] = GCTriInt(ofVec3f(0.0, 0.0, 0.0), v[nextl], v[currentl], ofVec3f(0.0, 0.0, 0.0));
					q[l] = v[nextl].getCrossed(v[currentl]);
					if (q[l].length() < 0.00001)
					{
						II[l] = 0.0;
					}
					else
					{
						N[l] = q[l].normalize();
					}
//					cout << N[l].length() << endl;
				}
				double I_t = -abs(s[0] * I[0] + s[1] * I[1] + s[2] * I[2]);
				*psi = -I_t;
//				cout << psi << endl;
				ofVec3f w = n*I_t + N[0] * II[0] + N[1] * II[1] + N[2] * II[2];
//				cout << "w at face " << j << ":\t" << w.length() << endl;
				if (w.length() < 0.00001)
				{
					if (s[0] > 0 && s[1] > 0 && s[2] > 0)
					{

					}
				}
				else
				{
					//					cout << "IN" << endl;
					for (size_t l = 0; l < 3; l++)
					{
						int currentl = l;
						int nextl = (l + 1) % 3;
						*phi[currentl] += (N[nextl].dot(w) / N[nextl].dot(v[currentl]));
						//					cout << "N:\t" << N[nextl] << endl;
						//					cout << "v:\t" << v[currentl] << endl;
						//					cout << q[nextl].dot(v[currentl]) << endl;
						//					cout << "phi[l]:\t" << phi[currentl] << endl;
					}
				}
//				cout << psi << endl;
//				estimate += -1/2.5*(psi*n);// +phi[0] * meshFace.getVertex(0) + phi[1] * meshFace.getVertex(1) + phi[2] * meshFace.getVertex(2));
//				checkPhi += phi[0] + phi[1] + phi[2];// += (psi*n + phi[0] * meshFace.getVertex(0) + phi[1] * meshFace.getVertex(1) + phi[2] * meshFace.getVertex(2));
			}
//			if (e == testI)
//			{
			double checkPhi = 0;
			for (size_t ver = 0; ver < tempVerticesPhiMap.size(); ver++)
			{
//					cout << VerticesPhiMap[ver].second << endl;
				checkPhi += tempVerticesPhiMap[ver].second;// VerticesPhiMap[ver].second*VerticesPhiMap[ver].first;
//					cout << ver << endl;
			}
			for (size_t ver = 0; ver < tempVerticesPhiMap.size(); ver++)
			{
				estimate += tempVerticesPhiMap[ver].second*tempVerticesPhiMap[ver].first;
			}
			for (size_t face = 0; face < tempFacesPsiMap.size(); face++)
			{
				ofMeshFace meshFace = tempFacesPsiMap[face].first;
				ofVec3f a = meshFace.getVertex(0);
				ofVec3f b = meshFace.getVertex(1);
				ofVec3f c = meshFace.getVertex(2);
				ofVec3f ba = b - a;
				ofVec3f ca = c - a;
				ofVec3f d = ba.getCrossed(ca);
				ofVec3f n = d.normalize();
				estimate += tempFacesPsiMap[face].second*n;
			}
			estimate /= checkPhi;
			cout << "model vertex-" << e << ":\t" << modelVertex << endl;
			cout << "estimate vertex-" << e << ":\t" << estimate << endl;
//				cout << checkPhi << endl;
//				cout << e << ":\t" << modelVertex.distance(estimate) << endl;
//			}
		}
	}
}

double GreenCoordinate::GCTriInt(ofVec3f p, ofVec3f v1, ofVec3f v2, ofVec3f etha)
{
	double temp = (v2 - v1).dot(p - v1) / ((v2 - v1).length()*(p - v1).length());
//	cout << temp << endl;
	if (abs(temp) > (1.0 - 0.0000001))
	{
		return 0.0;
	}
	double alpha = acos(temp);
	if (abs(alpha - PI) < 0.00000000001 || abs(alpha) < 0.00000000001)
		return 0.0;

	temp = (v1 - p).dot(v2 - p) / ((v1 - p).length()*(v2 - p).length());
	if (abs(temp) > (1.0 - 0.0000001))
	{
		return 0.0;
	}
	double beta = acos(temp);
//	cout << "alpha:\t" << alpha << endl;
//	cout << "beta:\t" << beta << endl;
	double sigma = (p - v1).lengthSquared()*pow(sin(alpha), 2);
	double c = (p - etha).lengthSquared();
	double theta[2] = { PI - alpha, PI - alpha - beta };
	double I[2];
	for (size_t i = 0; i < 2; i++)
	{
		double S = sin(theta[i]);
//		cout << S << endl;
		double C = cos(theta[i]);
		double threshold = abs(0.001);
		double M = -(S > threshold ? 1 : -threshold > S ? -1 : 0) / 2;
		double N = 2 * sqrt(c)*atan(sqrt(c)*C / sqrt(sigma + pow(S, 2)*c));
		double O = sqrt(sigma);
		double P = 2 * sqrt(sigma)*pow(S, 2) / pow(1 - C, 2);
		double denom = c*(1 + C) + sigma + sqrt(pow(sigma, 2) + sigma*c*pow(S, 2));
		double Q = 2*c*C / denom;
		double R = 1.0 - Q;

		I[i] = M*(N + (O * log(P*R)));
//		cout << C << endl;
	}
//	cout << -abs(I[0] - I[1] - sqrt(c)*beta) / (4 * PI) << endl;
	return -abs(I[0] - I[1] - sqrt(c)*beta)/(4*PI);
}

void GreenCoordinate::deform()
{
	for (size_t i = 0; i < partsName.size(); i++)
	{
		vector<pair<ofMeshFace, ofPoint>>meshFacesAndNormalPair = cagePartsVerticesMap.find(partsName[i])->second;
		vector<ofPoint>modelVertices = modelPartsVerticesMap.find(partsName[i])->second;
		std::vector<std::vector<pair<ofVec3f, double>>>& vectorVerticesPhiMap = cagePartsVectorVerticesPhiMap.find(partsName[i])->second;
		std::vector<std::vector<pair<ofMeshFace, double>>>& vectorFacesPsiMap = cagePartsVectorFacesPsiMap.find(partsName[i])->second;
		//		cout << modelVertices.size() << endl;
		for (size_t e = 0; e < modelVertices.size(); e++)
		{
			ofVec3f modelVertex = modelVertices[e]; //meshFacesAndNormalPair[0].first.getVertex(0);//
			ofVec3f estimate = ofVec3f(0, 0, 0);
			double* phi[3];
			vector<pair<ofVec3f, double>>& tempVerticesPhiMap = vectorVerticesPhiMap[e];
			vector<pair<ofMeshFace, double>>& tempFacesPsiMap = vectorFacesPsiMap[e];
			for (size_t j = 0; j < meshFacesAndNormalPair.size(); j++)
			{
				double* psi;
				ofMeshFace meshFace = meshFacesAndNormalPair[j].first;
				ofVec3f n = meshFacesAndNormalPair[j].second;
				//Preparing phi
				for (size_t l = 0; l < 3; l++)
				{
					ofVec3f cageVertex = meshFace.getVertex(l);
					for (size_t it = 0; it < tempVerticesPhiMap.size(); it++)
					{
						ofVec3f mapVertex = tempVerticesPhiMap[it].first;
						if (mapVertex.distance(cageVertex) < 0.00001)
						{
							phi[l] = &tempVerticesPhiMap[it].second;
							//							cout << cageVertex << endl;
							//							cout << *phi[l] << endl;
							break;
						}
					}
				}
				//Preparing psi
				for (size_t it = 0; it < tempFacesPsiMap.size(); it++)
				{
					ofMeshFace mapFace = tempFacesPsiMap[it].first;
					if (mapFace.getVertex(0).distance(meshFace.getVertex(0)) < 0.00001 && mapFace.getVertex(1).distance(meshFace.getVertex(1)) < 0.00001 && mapFace.getVertex(2).distance(meshFace.getVertex(2)) < 0.00001)
					{
						psi = &tempFacesPsiMap[it].second;
						break;
					}
				}

				ofVec3f v[3];
				for (size_t l = 0; l < 3; l++)
				{
					v[l] = meshFace.getVertex(l) - modelVertex;
					//					cout << "j = "<< j << ":\t" << v[l] << endl;
				}
				//				cout << "Volume at face " << j << ":\t" << v[0].dot(v[2].getCrossed(v[1])) << endl;
				/*
				cout << v[2] << endl;
				cout << "j = " << j << ":\t" << (v[2].getCrossed(v[1])) << endl;
				cout << v[2] << endl;
				*/
				ofVec3f p = v[0].dot(n)*n;
				//				cout << n.length() << endl;
				double s[3], I[3], II[3];
				ofVec3f q[3], N[3];
				for (size_t l = 0; l < 3; l++)
				{
					int currentl = l;
					int nextl = (l + 1) % 3;
					s[l] = ((v[currentl] - p).getCrossed(v[nextl] - p)).dot(n) > 0 ? 1 : -1;
					I[l] = GCTriInt(p, v[currentl], v[nextl], ofVec3f(0.0, 0.0, 0.0));
					II[l] = GCTriInt(ofVec3f(0.0, 0.0, 0.0), v[nextl], v[currentl], ofVec3f(0.0, 0.0, 0.0));
					q[l] = v[nextl].getCrossed(v[currentl]);
					if (q[l].length() < 0.00001)
					{
						II[l] = 0.0;
					}
					else
					{
						N[l] = q[l].normalize();
					}
					//					cout << N[l].length() << endl;
				}
				double I_t = -abs(s[0] * I[0] + s[1] * I[1] + s[2] * I[2]);
				*psi = -I_t;
				//				cout << psi << endl;
				ofVec3f w = n*I_t + N[0] * II[0] + N[1] * II[1] + N[2] * II[2];
				//				cout << "w at face " << j << ":\t" << w.length() << endl;
				if (w.length() < 0.00001)
				{
					if (s[0] > 0 && s[1] > 0 && s[2] > 0)
					{

					}
				}
				else
				{
					//					cout << "IN" << endl;
					for (size_t l = 0; l < 3; l++)
					{
						int currentl = l;
						int nextl = (l + 1) % 3;
						*phi[currentl] += (N[nextl].dot(w) / N[nextl].dot(v[currentl]));
						//					cout << "N:\t" << N[nextl] << endl;
						//					cout << "v:\t" << v[currentl] << endl;
						//					cout << q[nextl].dot(v[currentl]) << endl;
						//					cout << "phi[l]:\t" << phi[currentl] << endl;
					}
				}
				//				cout << psi << endl;
				//				estimate += -1/2.5*(psi*n);// +phi[0] * meshFace.getVertex(0) + phi[1] * meshFace.getVertex(1) + phi[2] * meshFace.getVertex(2));
				//				checkPhi += phi[0] + phi[1] + phi[2];// += (psi*n + phi[0] * meshFace.getVertex(0) + phi[1] * meshFace.getVertex(1) + phi[2] * meshFace.getVertex(2));
			}
			//			if (e == testI)
			//			{
			double checkPhi = 0;
			for (size_t ver = 0; ver < tempVerticesPhiMap.size(); ver++)
			{
				//					cout << VerticesPhiMap[ver].second << endl;
				checkPhi += tempVerticesPhiMap[ver].second;// VerticesPhiMap[ver].second*VerticesPhiMap[ver].first;
														   //					cout << ver << endl;
			}
			for (size_t ver = 0; ver < tempVerticesPhiMap.size(); ver++)
			{
				estimate += tempVerticesPhiMap[ver].second*tempVerticesPhiMap[ver].first;
			}
			for (size_t face = 0; face < tempFacesPsiMap.size(); face++)
			{
				ofMeshFace meshFace = tempFacesPsiMap[face].first;
				ofVec3f a = meshFace.getVertex(0);
				ofVec3f b = meshFace.getVertex(1);
				ofVec3f c = meshFace.getVertex(2);
				ofVec3f ba = b - a;
				ofVec3f ca = c - a;
				ofVec3f d = ba.getCrossed(ca);
				ofVec3f n = d.normalize();
				estimate += tempFacesPsiMap[face].second*n;
			}
			estimate /= checkPhi;
			cout << "model vertex-" << e << ":\t" << modelVertex << endl;
			cout << "estimate vertex-" << e << ":\t" << estimate << endl;
			//				cout << checkPhi << endl;
			//				cout << e << ":\t" << modelVertex.distance(estimate) << endl;
			//			}
		}
	}
}