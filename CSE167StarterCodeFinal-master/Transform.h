#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Node.h"

class Transform : public Node{

public:
	Transform(std::string);
	~Transform();

	glm::mat4 M;
	std::list<Node*> child_nodes;

	bool addChild(Node*);
	bool removeChild(Node*);

	void draw(glm::mat4 C);
	void update();
	void update(vec3);

	void Translate(glm::vec3 moveVector);
	void Scale(glm::vec3 scaleFactor);
	void ResetPosition();
	void ResetOrientationAndScale();

	void RotateAlongX(float degree);

	std::string name;

};

#endif