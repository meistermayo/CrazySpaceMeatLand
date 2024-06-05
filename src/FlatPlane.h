// FlatPlane.h
// Berthiaume Jan 2017

#ifndef _FlatPlane
#define _FlatPlane

class Model;

class FlatPlane 
{
private:
	Model* pPlane;

public:
	FlatPlane(const FlatPlane&) = delete;				// Copy constructor
	FlatPlane(Model&&) = delete;                    // Move constructor
	FlatPlane& operator=(const FlatPlane&) & = delete;  // Copy assignment operator
	FlatPlane& operator=(FlatPlane&&) & = delete;       // Move assignment operator
	~FlatPlane();

	FlatPlane(float size, float hrep, float vrep);

	void Render();

};

#endif _FlatPlane
