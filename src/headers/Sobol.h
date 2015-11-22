/*
 * sobol.h
 *
 *  Created on: 30 mars 2015
 *      Author: sebastien
 */

#ifndef SRC_HEADERS_SOBOL_H_
#define SRC_HEADERS_SOBOL_H_
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <vector>


/**
 * \fn std::unique_ptr<std::vector<std::vector<TypeVector>>> sobol_points(const unsigned N, unsigned D,const char *dir_file,TypeVector)
 * \brief calcule une sequence de sobol pour un nombre de point ainsi qu'une dimension donnée
 *
 * \param N nombre de points à générer
 * \param D dimension des points
 * \param dir_file chemin du fichier à ouvrir contenant des direction numbers
 * \param TypeVector permet simplement de faire passer le type des coordonnées à générer
 */
template <typename TypeVector>
std::unique_ptr<std::vector<std::vector<TypeVector>>> sobol_points(const unsigned N, unsigned D,const char *dir_file,TypeVector){

	std::string chemin{dir_file};
	std::ifstream infile(chemin.c_str(),std::ios::in);
	if (!infile) {
		throw "Sobol : Input file containing direction numbers cannot be found!";
	}
	char buffer[1000]{};
	infile.getline(buffer,1000,'\n');

	// L = max number of bits needed
	unsigned L {(unsigned)(ceil(log((double) (N)) / log(2.0)))};

	// C[i] = index from the right of the first zero bit of i
	std::unique_ptr<unsigned> _C(new unsigned[N]);

//	unsigned *C = new unsigned[N]{};
	_C.get()[0] = 1;
	for (unsigned i=1;i<=N-1;i++) {
		_C.get()[i] = 1;
		unsigned value {i};
		while (value & 1) {
			value >>= 1;
			_C.get()[i]++;
		}
	}

	// POINTS[i][j] = the jth component of the ith point
	//                with i indexed from 0 to N-1 and j indexed from 0 to D-1
	std::unique_ptr<std::vector<std::vector<TypeVector>>> _POINTS(new std::vector<std::vector<TypeVector>>{});
	std::vector<std::vector<TypeVector>> &POINTS{*_POINTS};
	POINTS.resize(N);
	for (unsigned i=0;i<=N-1;++i){
		//POINTS = _POINTS.get()[i];
		for(unsigned j = 0;j<=D-1;++j){
			POINTS[i].push_back(0.0);
		}
	}
	// ----- Compute the first dimension -----

	// Compute direction numbers V[1] to V[L], scaled by pow(2,32)
	std::unique_ptr<unsigned> _V(new unsigned[L + 1]);
	//unsigned *V = new unsigned[L + 1]{};
	for (unsigned i=1;i<=L;i++) _V.get()[i] = 1 << (32-i); // all m's = 1

	// Evalulate X[0] to X[N-1], scaled by pow(2,32)
	std::unique_ptr<unsigned> _X(new unsigned[N]);
//	unsigned *X = new unsigned[N]{};
	_X.get()[0] = 0;
	for (unsigned i=1;i<=N-1;i++) {
		_X.get()[i] = _X.get()[i-1] ^ _V.get()[_C.get()[i-1]];
		POINTS[i][0] = (long double)_X.get()[i]/pow(2.0,32); // *** the actual points
		//        ^ 0 for first dimension
	}

	// Clean up
//	delete [] V;
//	delete [] X;

	// ----- Compute the remaining dimensions -----
	for (unsigned j=1;j<=D-1;j++) {
		// Read in parameters from file
		unsigned d, s;
		unsigned a;
		infile >> d >> s >> a;

		std::unique_ptr<unsigned> _m(new unsigned[s + 1]);
//		unsigned *m = new unsigned[s + 1]{};

		for (unsigned i=1;i<=s;i++) infile >> _m.get()[i];

		// Compute direction numbers V[1] to V[L], scaled by pow(2,32)
		std::unique_ptr<unsigned> _V(new unsigned[L + 1]);
//		unsigned *V = new unsigned[L + 1]{};
		if (L <= s) {
			for (unsigned i=1;i<=L;i++) _V.get()[i] = _m.get()[i] << (32-i);
		}
		else {
			for (unsigned i=1;i<=s;i++) _V.get()[i] = _m.get()[i] << (32-i);
			for (unsigned i=s+1;i<=L;i++) {
				_V.get()[i] = _V.get()[i-s] ^ (_V.get()[i-s] >> s);
				for (unsigned k=1;k<=s-1;k++)
					_V.get()[i] ^= (((a >> (s-1-k)) & 1) * _V.get()[i-k]);
			}
		}
		// Evalulate X[0] to X[N-1], scaled by pow(2,32)
		std::unique_ptr<unsigned> _X(new unsigned[N]);
//		unsigned *X = new unsigned[N]{};
		_X.get()[0] = 0;
		for (unsigned i=1;i<=N-1;i++) {
			_X.get()[i] = _X.get()[i-1] ^ _V.get()[_C.get()[i-1]];
			POINTS[i][j] = (TypeVector)_X.get()[i]/pow(2.0,32); // *** the actual points
			//        ^ j for dimension (j+1)
		}
		// Clean up
		//delete [] m;
//		delete [] V;
//		delete [] X;
	}
//	delete [] C;

	return std::move(_POINTS);
}


#endif /* SRC_HEADERS_SOBOL_H_ */
