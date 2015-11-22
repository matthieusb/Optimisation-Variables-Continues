/*
 * ManipFichiers.h
 *
 *  Created on: Mar 31, 2015
 *      Author: eisti
 */

#ifndef SRC_HEADERS_MANIPFICHIERS_H_
#define SRC_HEADERS_MANIPFICHIERS_H_

#include <cstdlib>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

template <typename TypeVector>
void stockageDonneesDansFichier(std::vector<TypeVector> it_et_eval_stats, const std::string nom_fichier) {
	std::ofstream fichier { };
	long unsigned taille {it_et_eval_stats.size()};

	fichier.open(nom_fichier);

	if (fichier.is_open()) {
		fichier<<"ITERATION; F(MEILLEURE PARTICULE)"<<std::endl;

		for (unsigned i =0; i < taille-1; i+=2) {
			fichier<<it_et_eval_stats[i]<<";"<<it_et_eval_stats[i+1]<<std::endl;
		}

	} else {
		std::cerr<<"ERREUR lors de l'ouverture du fichier"<<std::endl;
	}



	fichier.close();
}



#endif /* SRC_HEADERS_MANIPFICHIERS_H_ */
