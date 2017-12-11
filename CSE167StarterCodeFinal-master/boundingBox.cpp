#include "boundingBox.h"
#include "TerrainGen.h"

boundingBox::boundingBox(glm::vec3 center, float width, float height, float depth)
{
	this->depth = depth;
	this->width = width;
	this->height = height;
	this->center = center;
	this->minX = center.x - width / 2.0f;
	this->maxX = center.x + width / 2.0f;
	this->minY = center.y - height / 2.0f;
	this->maxY = center.y + height / 2.0f;
	this->minZ = center.z - depth / 2.0f;
	this->maxZ = center.z + depth / 2.0f;
}

boundingBox::boundingBox() 
{
	this->depth = 0.0f;
	this->width = 0.0f;
	this->height = 0.0f;
	this->center = glm::vec3(0.0f, 0.0f, 0.0f);
	this->minX = 0.0f;
	this->maxX = 0.0f;
	this->minY = 0.0f;
	this->maxY = 0.0f;
	this->minZ = 0.0f;
	this->maxZ = 0.0f;
}

void boundingBox::drawBbox() 
{

}

int boundingBox::detectCollision(boundingBox * another) 
{
	bool check1 = (bool)(fabs(this->center.x - another->center.x) > this->width / 2.0f + another->width / 2.0f);
	if (check1 == false) 
	{
		return 0;
	}
	bool check2 = (bool)(fabs(this->center.y - another->center.y) > this->height / 2.0f + another->height / 2.0f);
	if (check2 == false)
	{
		return 0;
	}
	bool check3 = (bool)(fabs(this->center.z - another->center.z) > this->depth / 2.0f + another->depth / 2.0f);
	if (check3 == false)
	{
		return 0;
	}

	return 1;
}

int boundingBox::detectCollisionHeight()
{
	float radius = 29.5f;
	glm::vec3 terrain1 = TerrainGen::getHeight(maxX, minZ, radius);
	glm::vec3 terrain2 = TerrainGen::getHeight(minX, maxZ, radius);
	glm::vec3 terrain3 = TerrainGen::getHeight(minX, minZ, radius);
	glm::vec3 terrain4 = TerrainGen::getHeight(maxX, maxZ, radius);
	float height1 = terrain1.y;
	float height2 = terrain2.y;
	float height3 = terrain3.y;
	float height4 = terrain4.y;
	if (minX > 0.0f && maxZ < 0.0f) 
	{
		float offset = 2.5f;
		height1 += offset;
		height2 += offset;
		height3 += offset;
		height4 += offset;
	}
	else if (minX > 0.0f && minZ > 0.0f)
	{
		float offset = 3.0f;
		height1 += offset;
		height2 += offset;
		height3 += offset;
		height4 += offset;
	}
	else if (maxX < 0.0f && maxZ < 0.0f)
	{
		float offset = 2.0f;
		height1 -= offset;
		height2 -= offset;
		height3 -= offset;
		height4 -= offset;
	}
	minY = 0.0f;
	if (minY <= height3 || minY <= height2) 
	{
		std::cout << "left" << std::endl;
		return 1;
	}
	else if (minY <= height2 || minY <= height4)
	{
		std::cout << "down" << std::endl;
		return 1;
	}
	else if (minY <= height4 || minY <= height1)
	{
		std::cout << "right" << std::endl;
		return 1;
	}
	else if (minY <= height3 || minY <= height1)
	{
		std::cout << "up" << std::endl;
		return 1;
	}
	/**if(minY > terrain1.y && minY > terrain2.y ) 
	{
		return 0;
	}*/
	return 0;
}

void boundingBox::setCenter(glm::vec3 center)
{
	this->center = center;
	minX = center.x - width / 2.0f;
	maxX = center.x + width / 2.0f;
	minY = center.y - height / 2.0f;
	maxY = center.y + height / 2.0f;
	minZ = center.z - depth / 2.0f;
	maxZ = center.z + depth / 2.0f;
}

void boundingBox::setParameter(float width, float height, float depth)
{
	this->depth = depth;
	this->width = width;
	this->height = height;	
	minX = center.x - width / 2.0f;
	maxX = center.x + width / 2.0f;
	minY = center.y - height / 2.0f;
	maxY = center.y + height / 2.0f;
	minZ = center.z - depth / 2.0f;
	maxZ = center.z + depth / 2.0f;
}