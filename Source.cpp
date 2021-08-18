#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"




struct Vector3d
{
	float x, y, z;
};

struct triangle
{
	Vector3d position[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

struct matrix4x4
{
	float m[4][4] = { 0 };
};

class coreEngine : public olc::PixelGameEngine
{
public:
	coreEngine()
	{
		sAppName = "coreEngine";

	}
private:
	mesh meshCube;
	matrix4x4 matProj;
	float fTheta;

	void multiplyMatrixVector(Vector3d& i, Vector3d& o, matrix4x4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; 
			o.y /= w; 
			o.z /= w;
		}
	};
public:
	bool OnUserCreate() override
	{

		meshCube.tris = {

			//SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			//EAST
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		//PROJECTION MATRIX
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);


		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
		
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		matrix4x4 matrotationZ, matrotationX;
		fTheta += 1.0f * fElapsedTime;

		matrotationZ.m[0][0] = cosf(fTheta);
		matrotationZ.m[0][1] = sinf(fTheta);
		matrotationZ.m[1][0] = -sinf(fTheta);
		matrotationZ.m[1][1] = cosf(fTheta);
		matrotationZ.m[2][2] = 1;
		matrotationZ.m[3][3] = 1;

		matrotationX.m[0][0] = 1;
		matrotationX.m[1][1] = cosf(fTheta * 0.5f);
		matrotationX.m[1][2] = sinf(fTheta * 0.5f);
		matrotationX.m[2][1] = -sinf(fTheta * 0.5f);
		matrotationX.m[2][2] = cosf(fTheta * 0.5f);
		matrotationX.m[3][3] = 1;
	
		
		//triangle
		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			//rotate in z axis
			multiplyMatrixVector(tri.position[0], triRotatedZ.position[0], matrotationZ);
			multiplyMatrixVector(tri.position[1], triRotatedZ.position[1], matrotationZ);
			multiplyMatrixVector(tri.position[2], triRotatedZ.position[2], matrotationZ);


			//rotate in x axis
			multiplyMatrixVector(triRotatedZ.position[0], triRotatedZX.position[0], matrotationX);
			multiplyMatrixVector(triRotatedZ.position[1], triRotatedZX.position[1], matrotationX);
			multiplyMatrixVector(triRotatedZ.position[2], triRotatedZX.position[2], matrotationX);

			//offset
			triTranslated = triRotatedZX;
			triTranslated.position[0].z = triRotatedZX.position[0].z + 3.0f;
			triTranslated.position[1].z = triRotatedZX.position[1].z + 3.0f;
			triTranslated.position[2].z = triRotatedZX.position[2].z + 3.0f;

			multiplyMatrixVector(triTranslated.position[0], triProjected.position[0], matProj);
			multiplyMatrixVector(triTranslated.position[1], triProjected.position[1], matProj);
			multiplyMatrixVector(triTranslated.position[2], triProjected.position[2], matProj);

			//multiply matrix vector
			triProjected.position[0].x += 1.0f; triProjected.position[0].y += 1.0f;
			triProjected.position[1].x += 1.0f; triProjected.position[1].y += 1.0f;
			triProjected.position[2].x += 1.0f; triProjected.position[2].y += 1.0f;


			triProjected.position[0].x *= 0.5f * (float)ScreenWidth();
			triProjected.position[0].y *= 0.5f * (float)ScreenHeight();
			triProjected.position[1].x *= 0.5f * (float)ScreenWidth();
			triProjected.position[1].y *= 0.5f * (float)ScreenHeight();
			triProjected.position[2].x *= 0.5f * (float)ScreenWidth();
			triProjected.position[2].y *= 0.5f * (float)ScreenHeight();

			DrawTriangle(triProjected.position[0].x, triProjected.position[0].y,
				triProjected.position[1].x, triProjected.position[1].y, triProjected.position[2].x, triProjected.position[2].y, olc::WHITE);
		}
		return true;

	}

};

int main(int argc(), char* argv[])
{
	coreEngine demo;
	if (demo.Construct(256, 240, 4, 4))
	{
		demo.Start();
	}
	return 0;
}