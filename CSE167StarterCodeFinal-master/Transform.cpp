#include "Transform.h"

using namespace std;

float vibrationCount = 0;
bool isVibUp = true;

Transform::Transform(std::string name){

	M = glm::mat4(1.0f);
	this->name = name;
}

Transform::~Transform(){
	
}

void Transform::draw(glm::mat4 C){
	glm::mat4 new_matrix = C*M;


	for each (Node* cp in child_nodes)
	{
		//cout << "draw call detected, and the transform name is " << this->name << endl;
		cp -> draw(new_matrix);
	}
}

bool Transform::addChild(Node* child) {
	child_nodes.push_back(child);

	return true;
}

bool Transform::removeChild(Node* child) {
	child_nodes.remove(child);
	
	return true;
}


void Transform::update() {

	// everyone update
	for each (Node* cp in child_nodes)
	{
		//cout << "draw call detected, and the transform name is " << this->name << endl;
		cp->update();
	}

}

void Transform::update(vec3 point) {
	this->M[3] = vec4(point, 1);
}



void Transform::RotateAlongX(float degree) {
	this->M = this->M * glm::rotate(glm::mat4(1.0f), degree / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
}

// Translate this object by moveVector amoung
void Transform::Translate(glm::vec3 moveVector) {

	this->M = glm::translate(glm::mat4(1.0f), moveVector) *this->M;
}

// Scale this object depends on the Sign of scaleInc, probably need to be changed
void Transform::Scale(glm::vec3 scaleFactor) {
	this->M = glm::scale(this->M, scaleFactor);
}

void Transform::ResetPosition() {
	this->M[3][0] = 0;
	this->M[3][1] = 0;
	this->M[3][2] = 0;
}

void Transform::ResetOrientationAndScale() {
	this->M[0][0] = 1;
	this->M[0][1] = 0;
	this->M[0][2] = 0;
	this->M[1][1] = 1;
	this->M[1][0] = 0;
	this->M[1][2] = 0;
	this->M[2][2] = 1;
	this->M[2][1] = 0;
	this->M[2][0] = 0;
}
