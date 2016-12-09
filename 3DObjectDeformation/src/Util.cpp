#include "Util.h"



Util::Util()
{
}


Util::~Util()
{
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
