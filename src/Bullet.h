#pragma once
#include <vector>
#include "Math/Matrix.h"
#include "Math/Vect.h"
#include <Model.h>
#include <d3d11.h>
#include <GraphicObject_TextureL.h>
class Bullet;
/*
class BulletManager {
public:
	static BulletManager& getInstance()
	{
		static BulletManager B;
		return B;
	}

private: 
	BulletManager() {}; // Hide these
	BulletManager(const BulletManager &tmp);
	void operator = (const BulletManager &tmp);
	std::vector<Bullet*> BulletList;
	int currentBullet = 0;

public:
	BulletManager(const BulletManager &tmp) = delete; // For Descriptive Errors.
	void operator = (const BulletManager &tmp) = delete;
	
	void UpdateBullets(float elapsedMilliseconds);
	void SpawnBullet(const Vect& speed, const Matrix& world, Model* model = 0);
};
*/
class Bullet : public Align16
{
public:
	Bullet();
	Bullet(GraphicObject_TextureLight* GO = 0);
	~Bullet();
	Bullet(const Bullet& tmp);
	Bullet & operator = (const Bullet& tmp);
	void Update(float elapsedMilliseconds);
	void Activate(const Vect& speed, const Matrix& world);
	void Deactivate();
	void Render();
	bool isActive();
	const Matrix& GetWorld();
private:
	bool active = false;

	float lifeTime = 3;
	float timer = 0;

	Matrix World;
	GraphicObject_TextureLight* pGO;
	Vect Speed;
};