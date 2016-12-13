#pragma once

#include "ofMain.h"

template< typename T >
struct array_deleter
{
	void operator ()(T const * p)
	{
		delete[] p;
	}
};

class Util
{
public:
	Util();
	~Util();

	static vector<ofVec3f> readPartCageVerticesFromFile(string fileName);
	static bool upperLowerPlaneTest(ofVec3f testPoint, ofVec3f planeNormal, ofVec3f planePoint);
	static ofVec3f calculateNormal(ofVec3f * inputVector);
};

