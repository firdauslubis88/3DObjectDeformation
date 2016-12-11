#include "Util.h"



Util::Util()
{
}


Util::~Util()
{
}

vector<ofVec3f> Util::readPartCageVerticesFromFile(string fileName)
{
	vector<string>lines = ofSplitString(ofBufferFromFile(fileName).getText(), "\n");
	vector<ofVec3f> partVertices;

	for (size_t i = 0; i < (lines.size() - 1); i++)
	{
		float x, y, z;
		auto values = ofSplitString(lines[i], " ");
		istringstream(values[0]) >> x;
		istringstream(values[1]) >> y;
		istringstream(values[2]) >> z;
		ofVec3f vertex = ofVec3f(x, y, z);
		partVertices.push_back(vertex);
	}

	return partVertices;
}

bool Util::upperLowerPlaneTest(ofVec3f testPoint, ofVec3f planeNormal, ofVec3f planePoint)
{
	ofVec3f a = testPoint-planePoint;
	float dotResult = a.dot(planeNormal);
	if (dotResult > 0)
	{
		return true;
	}
	else return false;
}

ofVec3f Util::calculateNormal(ofVec3f* inputVector)
{
	ofVec3f a = inputVector[0];
	ofVec3f b = inputVector[1];
	ofVec3f c = inputVector[2];
	ofVec3f ba = b - a;
	ofVec3f ca = c - a;
	ofVec3f d = ba.getCrossed(ca);
	return d.normalize();
}
