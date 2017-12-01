#ifndef GROUP_H
#define GROUP_H


#include "Transform.h"
#include "glm/ext.hpp" 

class Group : public Node{
public:
	Group();

	~Group();
	
	glm::mat4 M;
	// child_nodes should all be Transforms
	std::list<Node*> child_nodes;

	void draw(glm::mat4 C);
	void update();

	bool addChild(Node*);
	bool removeChild(Node*);

	void Translate(glm::vec3 moveVector);
	void Scale(glm::vec3);
	void RotateAllAxis(float degree, glm::vec3 axis);



};
#endif
