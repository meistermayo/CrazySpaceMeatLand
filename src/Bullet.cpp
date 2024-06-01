#include "Bullet.h"
/*
void BulletManager::UpdateBullets(float elapsedMilliseconds)
{
	//for (std::vector<Bullet*>::iterator iter = BulletList.begin(); iter != BulletList.end ; ++iter)
	{
		// iter->Update();
	}
}

void BulletManager::SpawnBullet(const Vect& speed, const Matrix& world, Model* model = 0)
{
	//std::vector<Bullet*>::iterator iter = BulletList.begin();

}*/

Bullet::Bullet()
{
	Speed = Vect();
	World = Matrix();
	pGO = 0;
}

Bullet::Bullet(GraphicObject_TextureLight* GO)
{
	Speed = Vect();
	World = Matrix();
	pGO = GO;
}

Bullet::~Bullet()
{
	delete pGO;
}

Bullet::Bullet(const Bullet&tmp)
{
	this->Speed = tmp.Speed;
	this->pGO = tmp.pGO;
	this->World = tmp.World;
}

Bullet & Bullet:: operator = (const Bullet&tmp)
{
	this->Speed = tmp.Speed;
	this->pGO = tmp.pGO;
	this->World = tmp.World;
	return * this;
}

void Bullet::Activate(const Vect& speed, const Matrix& world)
{
	active = true;
	timer = 0;
	Speed = speed;
	pGO->SetWorld(world);
}

void Bullet::Deactivate()
{
	active = false;
}

bool Bullet::isActive()
{
	return active;
}

void Bullet::Update(float elapsedMilliseconds)
{
	timer += elapsedMilliseconds;
	if (timer > lifeTime)
	{
		Deactivate();
	}
	else
		pGO->SetWorld(pGO->GetWorld() * Matrix(TRANS, Speed));
}

void Bullet::Render()
{
	if (active)
	{
		if (pGO != 0)
			pGO->Render();
	}
}

const Matrix& Bullet::GetWorld()
{
	return World;
}