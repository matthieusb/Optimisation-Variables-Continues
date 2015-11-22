/*
 * FonctionObjectif.h
 *
 *  Created on: Mar 11, 2015
 *      Author: eisti
 */

#ifndef SRC_HEADERS_OBJECTIF_H_
#define SRC_HEADERS_OBJECTIF_H_

#include <memory>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cmath>
#include <typeinfo>
#include <math.h>

#include "SampleOfFunctions.h"

template <typename TypeVector, unsigned dimension>
class BaseObjectif {
protected:
	std::unique_ptr<std::vector<TypeVector>> _min_espace{new std::vector<TypeVector>{}};
	std::unique_ptr<std::vector<TypeVector>> _max_espace{new std::vector<TypeVector>{}};
};

template <typename TypeVector, unsigned dimension>
class Objectif : public BaseObjectif<TypeVector,dimension> {
private:
	std::vector<TypeVector>& min_espace() { return *(BaseObjectif<TypeVector,dimension>::_min_espace); }
	const std::vector<TypeVector>& min_espace() const { return *(BaseObjectif<TypeVector,dimension>::_min_espace); }

	std::vector<TypeVector>& max_espace() { return *(BaseObjectif<TypeVector,dimension>::_max_espace); }
	const std::vector<TypeVector>& max_espace() const { return *(BaseObjectif<TypeVector,dimension>::_max_espace); }

public:
	Objectif();
	Objectif(const std::vector<TypeVector> &min_espace, const std::vector<TypeVector> &max_espace);
	Objectif(Objectif& autre);
	TypeVector operator() (const std::vector<TypeVector> &val) const;

	const std::vector<TypeVector>& getMaxEspace() const {
		return max_espace();
	}

	const std::vector<TypeVector>& getMinEspace() const {
		return min_espace();
	}
};

template<typename TypeVector, unsigned dimension>
Objectif<TypeVector, dimension>::Objectif() : BaseObjectif<TypeVector,dimension>() {
	//#### FONCTION SPHERE ET RASTRIGRIN BORNE ESPACE DE RECHERCHE #####
//	for (unsigned i=0; i< dimension; ++i) {
//		min_espace().push_back(-5.12);
//		max_espace().push_back(5.12);
//	}

	//#### FONCTION SUM SQUARES BORNE ESPACE DE RECHERCHE #####
//	for (unsigned i=0; i< dimension; ++i) {
//		min_espace().push_back(-10.0);
//		max_espace().push_back(10.0);
//	}

	//#### FONCTION ROSENBROCK BORNE ESPACE DE RECHERCHE #####
//	for (unsigned i=0; i< dimension; ++i) {
//		min_espace().push_back(-2.048);
//		max_espace().push_back(2.048);
//	}

	//#### FONCTION TRID BORNE ESPACE DE RECHERCHE #####
	for (unsigned i=0; i< dimension; ++i) {
		min_espace().push_back(-(pow(dimension,2)));
		max_espace().push_back(pow(dimension,2));
	}
}

template<typename TypeVector, unsigned dimension>
Objectif<TypeVector, dimension>::Objectif(const std::vector<TypeVector> &_min_espace, const std::vector<TypeVector> &_max_espace) : BaseObjectif<TypeVector,dimension>() {
	min_espace()=_min_espace;
	max_espace()=_max_espace;
}

template<typename TypeVector, unsigned dimension>
TypeVector Objectif<TypeVector, dimension>::operator ()(const std::vector<TypeVector> &val) const {
	SampleOfFunctions<TypeVector, dimension> sample {};

//	return sample.sphere(val);
//	return sample.rastrigin(val);
//	return sample.sumSquares(val);
//	return sample.rosenbrock(val);
	return sample.trid(val);
}
#endif /* SRC_HEADERS_OBJECTIF_H_ */
