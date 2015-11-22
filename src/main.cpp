/*!
 * \file main.cpp
 * \author Sebastien Griolet, Cecile Riquart, Matthieu Sauboua-Beneluz
 * \date 31 mars 2015
 * \version 0.1
 * \brief Optimisation par essaims particluaires
 * \remarks aucune
 */

#include "headers/mpi-bind.h"
#include "headers/Essaim.h"
#include "headers/EssaimOMP.h"
#include "headers/EssaimMPI.h"
#include "headers/Objectif.h"
#include "headers/ManipFichiers.h"
#include <fstream>
#include <unistd.h>

/*! \namespace std
 * espace de nommage regroupant la bibliotheque standard C++
 */
using namespace std;

void affichageVecteur(vector<long double>& vecteur, long double res_obj) {
	unsigned taille = vecteur.size();

	cout<<"---------- Coordonnees du point objectif obtenu -----------"<<endl;
	for (unsigned i = 1; i <= taille; ++i) {
		cout<<"X"+to_string(i)<<" : "<<vecteur[i-1]<<endl;
	}
	cout<<"---------- Valeur de la meilleure position ----------------"<<endl;
	cout<<res_obj<<endl;
}

/**
 *
 */
void runEssaimSequentiel() {
	string nom_fichier = "donneesResultats";
	unsigned nb_echantillon_de_test { 1 }; //Execution de l'algoritme nb_echantillon_de_test fois pour pouvoir faire des statistiques derriere
	unique_ptr<Objectif<long double, 3>> obj_ptr(new Objectif<long double, 3> { });
	Objectif<long double, 3>& obj = *obj_ptr;

	for (unsigned i =1; i <= nb_echantillon_de_test; ++i) {
		unique_ptr<Essaim<long double,3>> e_ptr(new Essaim<long double,3>(30,obj.getMinEspace(), obj.getMaxEspace(), obj));
		Essaim<long double,3>& e = *e_ptr;

		cout<<"############ Itération n°"<<i<<" ############"<<endl;
		unique_ptr<vector<long double>> _it_et_eval_stats {e.algoEssaim(obj, 50000)};
		vector<long double> &it_et_eval_stats{*_it_et_eval_stats};
		affichageVecteur(e.meilleure_postion_e(), obj(e.meilleure_postion_e()));

		//############ Decommenter pour écrire les résultats dans des fichiers CSV
		//		stockageDonneesDansFichier(it_et_eval_stats, nom_fichier+to_string(i)+".csv");
	}
}

/**
 *
 */
void runEssaimParallele() {
	string nom_fichier = "donneesResultats";
	unsigned nb_echantillon_de_test { 1 }; //Execution de l'algoritme nb_echantillon_de_test fois pour pouvoir faire des statistiques derriere
	unique_ptr<Objectif<long double, 4>> obj_ptr(new Objectif<long double, 4> { });
	Objectif<long double, 4>& obj = *obj_ptr;



	for (unsigned i =1; i <= nb_echantillon_de_test; ++i) {
		unique_ptr<EssaimOMP<long double,4>> e_ptr(new EssaimOMP<long double,4>(100,obj.getMinEspace(), obj.getMaxEspace(), obj));
		EssaimOMP<long double,4>& e = *e_ptr;

		cout<<"############ Itération n°"<<i<<" ############"<<endl;
		unique_ptr<vector<long double>> _it_et_eval_stats {e.algoEssaim(obj, 10000)};
		vector<long double> &it_et_eval_stats{*_it_et_eval_stats};
		affichageVecteur(e.meilleure_postion_e(), obj(e.meilleure_postion_e()));

		//############ Decommenter pour écrire les résultats dans des fichiers CSV
		//		stockageDonneesDansFichier(it_et_eval_stats, nom_fichier+to_string(i)+".csv");
	}
}


///**
// *
// */
void runEssaimParalleleMPI(int argc, char** argv) {
	string nom_fichier = "donneesResultats";

	unique_ptr<Objectif<long double, 3>> obj_ptr(new Objectif<long double, 3> { });
	Objectif<long double, 3>& obj = *obj_ptr;

	MpiBind mpi(argc,argv);

	if (mpi.getSize() == 1) {
		unique_ptr<EssaimMPI<long double,3>> e_ptr(new EssaimMPI<long double,3>(160,obj.getMinEspace(), obj.getMaxEspace(), obj));
		EssaimMPI<long double,3>& e { *e_ptr};
		e.algoEssaim(obj, 50000,0);

		affichageVecteur(e.meilleure_postion_e(), obj(e.meilleure_postion_e()));
	} else {
		if (mpi.getRank() == 0) {//Processeur principal, envoi d'un essaim fraichement intialisé à chaque processeur
			unique_ptr<vector<vector<long double>>> _all_tests {new vector<vector<long double>>{}};
			vector<vector<long double>>& all_vects {*_all_tests};

			for (unsigned i = 1 ; i < mpi.getSize() ; ++i) {
				vector<long double>vect_res {};
				mpi.recv(vect_res,i);
				all_vects.push_back(vect_res);
			}

			for (unsigned i = 0 ; i < mpi.getSize() - 1 ; ++i) {
				cout<<"####################### Résulat processeur n° "<<i+1<<"#######################"<<endl;
				affichageVecteur(all_vects[i], obj(all_vects[i]));
			}
		} else {//Reception, calcul et renvoi
//			usleep(1000000*((double)mpi.getRank()));//permet d'empêcher les même résultats

			unique_ptr<EssaimMPI<long double,3>> e_ptr(new EssaimMPI<long double,3>(30,obj.getMinEspace(), obj.getMaxEspace(), obj));
			EssaimMPI<long double,3>& e { *e_ptr};

			e.algoEssaim(obj, 50000, mpi.getRank());//ON envoi la seed en même temps

			vector<long double> vect_envoi {e.meilleure_postion_e()};
			mpi.send(vect_envoi, 0);
		}
	}
}

/**
 *
 * @param
 * @param
 * @return
 */
int main(int argc, char** argv){
	try {
		cout.precision(30);//Changer la précision d'affichage
		runEssaimSequentiel();
//		runEssaimParallele();
//		runEssaimParalleleMPI(argc, argv);
	} catch (std::bad_alloc& ba) {
		std::cerr << "bad_alloc caught: " << ba.what() << endl;
	} catch (const char* &e) {
		std::cerr << "Exception caught: " << e << endl;
	}
	return 0;
}

