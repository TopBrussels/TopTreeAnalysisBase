#include "../interface/Container.h"

ClassImp(Container);
	
Container::Container(const Container &d){
	int_ = d.int_;
	float_ = d.float_;
	double_ = d.double_;
	string_ = d.string_;
}

void Container::Clear(){
	int_.clear();
	float_.clear();
	double_.clear();
	string_.clear();
}
