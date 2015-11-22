/*
 * SampleOfFunctions.h
 *
 *  Created on: Mar 14, 2015
 *      Author: eisti
 */

#ifndef SRC_HEADERS_SAMPLEOFFUNCTIONS_H_
#define SRC_HEADERS_SAMPLEOFFUNCTIONS_H_

#include <math.h>
#include <vector>
#include <iostream>

template <typename TypeVector, unsigned dimension>
class SampleOfFunctions {
private : //Pas d'attributs

public :
	SampleOfFunctions();
	TypeVector sumSquares(const std::vector<TypeVector> &val);
	TypeVector rastrigin(const std::vector<TypeVector> &val);
	TypeVector sphere(const std::vector<TypeVector> &val);
	TypeVector rosenbrock(const std::vector<TypeVector> &val);
	TypeVector trid(const std::vector<TypeVector> &val);
};

template<typename TypeVector, unsigned dimension>
SampleOfFunctions<TypeVector, dimension>::SampleOfFunctions() {
}

template<typename TypeVector, unsigned dimension>
TypeVector SampleOfFunctions<TypeVector, dimension>::sumSquares(const std::vector<TypeVector> &val) {
	TypeVector res=0.0;

	for (unsigned i=0; i< dimension; ++i) {
		res += i*pow(val[i],2);
	}
	return res;
}

template<typename TypeVector, unsigned dimension>
TypeVector SampleOfFunctions<TypeVector, dimension>::rastrigin(const std::vector<TypeVector> &val) {
	TypeVector res=10.0*dimension;

	for (unsigned i=0; i< dimension; ++i) {
		res += pow(val[i],2)-10*cos(2*M_PI*val[i]);
	}
	return res;

}

template<typename TypeVector, unsigned dimension>
TypeVector SampleOfFunctions<TypeVector, dimension>::sphere(const std::vector<TypeVector> &val) {
	TypeVector res=0.0;

	for (unsigned i=0; i< dimension; ++i) {
		res += pow(val[i],2);
	}
	return res;
}

template<typename TypeVector, unsigned dimension>
TypeVector SampleOfFunctions<TypeVector, dimension>::rosenbrock(const std::vector<TypeVector>& val) {
	TypeVector res = 0.0;

	for (unsigned i = 0 ; i < dimension-1 ; ++i) {
		res += 100.0*pow((val[i+1] - pow(val[i],2)),2) + pow(val[i] - 1, 2);
	}

	return res;
}

template<typename TypeVector, unsigned dimension>
TypeVector SampleOfFunctions<TypeVector, dimension>::trid(const std::vector<TypeVector>& val) {
	TypeVector res1 = 0.0;
	TypeVector res2 = 0.0;


	for (unsigned i = 0 ; i < dimension ; ++i) {
		res1 += pow(val[i] - 1,2);
	}

	for (unsigned i = 1 ; i < dimension ; ++i) {
		res2 += val[i] * val[i  - 1];
	}
	return (res1 - res2);
}

#endif /* SRC_HEADERS_SAMPLEOFFUNCTIONS_H_ */
