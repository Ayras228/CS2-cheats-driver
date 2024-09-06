#pragma once
#include"../math/Vector3.h"
#include<vector>
struct BoneJointData
{
	Vector3 Pos;
	char pad[0x14];
};

struct BoneMatrix
{
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
};

enum BoneIds : DWORD
{
	Waist = 0,
	Neck = 5,
	Head = 6,
	ShoulderLeft = 8,
	ForeLeft = 9,
	HandLeft = 11,
	ShoulderRight = 13,
	ForeRight = 14,
	HandRight = 16,
	KneeLeft = 23,
	FeetLeft = 24,
	KneeRight = 26,
	FeetRight = 27
};

inline std::vector<DWORD> BoneIdsVec = {
	Waist ,
	Neck ,
	Head ,
	ShoulderLeft ,
	ForeLeft ,
	HandLeft ,
	ShoulderRight ,
	ForeRight ,
	HandRight ,
	KneeLeft ,
	FeetLeft ,
	KneeRight ,
	FeetRight
};

