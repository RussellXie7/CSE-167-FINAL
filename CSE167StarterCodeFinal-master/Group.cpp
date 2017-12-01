#include "Group.h"

using namespace std;

Group::Group() {
	M = glm::mat4(1.0f);

}


Group::~Group(){}




void Group::draw(glm::mat4 C){
	glm::mat4 new_matrix = C*M;

	for each (Node* cp in child_nodes)
	{
		//cout << "draw call detected, and the transform name is " << this->name << endl;
		cp->draw(new_matrix);
	}
}




bool Group::addChild(Node* child) {
	child_nodes.push_back(child);
	return true;
}

bool Group::removeChild(Node* child) {
	child_nodes.remove(child);

	return true;
}


void Group::Scale(glm::vec3 scaleFactor) {
	this->M = glm::scale(this->M, scaleFactor);
}

void Group::update() {
	// everyone update
	for each (Node* cp in child_nodes)
	{
		//cout << "draw call detected, and the transform name is " << this->name << endl;
		cp->update();
	}
}

// Translate this object by moveVector amoung
void Group::Translate(glm::vec3 moveVector) {

	this->M = glm::translate(glm::mat4(1.0f), moveVector) *this->M;
}


void Group::RotateAllAxis(float degree, glm::vec3 axis) {
	this->M = this->M*glm::rotate(glm::mat4(1.0f), degree / 180.0f * glm::pi<float>(), axis);
}


