// GraphicObject_Color
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Color
#define _GraphicObject_Color

#include "GraphicObject_Base.h"
#include "Math/Vect.h"
#include "ShaderWireframe.h"
#include "ShaderWireframe.h"

class GraphicObject_Wireframe : public GraphicObject_Base
{
friend class ShaderWireframe;

public:
	GraphicObject_Wireframe(const GraphicObject_Wireframe&) = delete;				 // Copy constructor
	GraphicObject_Wireframe(GraphicObject_Wireframe&&) = default;                    // Move constructor
	GraphicObject_Wireframe& operator=(const GraphicObject_Wireframe&) & = default;  // Copy assignment operator
	GraphicObject_Wireframe& operator=(GraphicObject_Wireframe&&) & = default;       // Move assignment operator
	~GraphicObject_Wireframe();		  											 // Destructor

	GraphicObject_Wireframe() = delete;

	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_Wireframe(ShaderWireframe* shader, int meshCount, Model* mod, Vect& color);
	GraphicObject_Wireframe(Model* mod, ShaderBase* shader, Vect& color);

private:
	ShaderWireframe*					pShader;
	Vect							Color;
	Matrix							World;
};

#endif _GraphicObject_Color
