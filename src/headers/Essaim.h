/**
 * \file Essamin.h
 * \brief Header de la classe essaim permettant d'initialiser un essaim avec les classes BaseEssaim et essaim.
 * \author Matthieu Sauboua-Beneluz, Cecile Riquart, Sébastien Griolet
 * \version 0.1
 * \param _essaim vector contenant toutes les particules
 * \param _meilleure_postion_e vector contenant la meilleures position de chaque particule
 */

#ifndef SRC_HEADERS_ESSAIM_H_
#define SRC_HEADERS_ESSAIM_H_

#include "mpi-bind.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include <time.h>
#include <memory>
#include <utility>

#include "Particule.h"
#include "Objectif.h"
#include "Sobol.h"

//################ Constantes utilisées pour l'application de l'algorithme
//Condition : C1 + C2 <= 4
constexpr auto C1 = 1.0; //Indépence
constexpr auto C2 = 3.0; //Panurgisme

constexpr auto NB_VOISINS = 3;
constexpr auto chemin =  "./joe-kuo-other-4.5600.txt";

//################

template <typename TypeVector, unsigned dimension>
class Particule;

template <typename TypeVector, unsigned dimension>
class Objectif;

template <typename TypeVector, unsigned dimension>
class BaseEssaim {
protected:
	std::unique_ptr<std::vector<Particule<TypeVector, dimension>>> _essaim{new std::vector<Particule<TypeVector, dimension>>{}};
	std::unique_ptr<std::vector<TypeVector>> _meilleure_postion_e{new std::vector<TypeVector>{}};

};

template <typename TypeVector, unsigned dimension>
class Essaim : public BaseEssaim<TypeVector,dimension> {
private:
	std::vector<Particule<TypeVector, dimension>>& essaim() { return *(BaseEssaim<TypeVector,dimension>::_essaim); }
	const std::vector<Particule<TypeVector, dimension>>& essaim() const { return *(BaseEssaim<TypeVector,dimension>::_essaim); }

	void initEssaim(const unsigned nb_particules, const std::vector<TypeVector> min_espace, const std::vector<TypeVector> max_espace, Objectif<TypeVector, dimension>& obj);
	std::unique_ptr<std::vector<unsigned>> recupVoisins(const unsigned taille, const unsigned pos, const unsigned nb_part) const;
	void majVitesse(Particule<TypeVector, dimension> &p, unsigned max_it, unsigned i);
	void deplacementParticule(Particule<TypeVector, dimension> &p, Objectif<TypeVector, dimension>& obj);
	void fonctionDePositionActuelle(Particule<TypeVector, dimension> &p, Objectif<TypeVector, dimension>& obj, TypeVector f_position_actuelle);
	void communicationVoisins(Particule<TypeVector, dimension>& p, std::vector<unsigned> &vect_voisins);
	void majMeilleurObjectif(Particule<TypeVector, dimension>& p, TypeVector f_meilleure_pos_e, Objectif<TypeVector, dimension>& obj);

public:
	Essaim();
	Essaim(const unsigned nb_particules, const std::vector<TypeVector> &min_espace, const std::vector<TypeVector> &max_espace, Objectif<TypeVector, dimension>& obj);
	Essaim (const Essaim &autre);

	std::vector<TypeVector> &meilleure_postion_e() { return *(BaseEssaim<TypeVector,dimension>::_meilleure_postion_e); }
	const std::vector<TypeVector> &meilleure_postion_e() const { return *(BaseEssaim<TypeVector,dimension>::_meilleure_postion_e); } //meilleure position de la meilleure particule

	unsigned taille() const;
	void majMeilleuresPositions(Objectif<TypeVector, dimension>& obj);
	void deplacements(unsigned max_it, Objectif<TypeVector, dimension>& obj);

	std::unique_ptr<std::vector<TypeVector>> algoEssaim(Objectif<TypeVector, dimension>& obj, unsigned max_it);

	//######## ACCESSEURS
	const std::vector<Particule<TypeVector, dimension> >& getEssaim() const {
		return essaim();
	}
};

//###### Constructeurs
template <typename TypeVector, unsigned dimension>
Essaim<TypeVector,dimension>::Essaim() : BaseEssaim<TypeVector,dimension>() {

}

template <typename TypeVector, unsigned dimension>
Essaim<TypeVector, dimension>::Essaim(const unsigned nb_particules, const std::vector<TypeVector> &min_espace, const std::vector<TypeVector> &max_espace, Objectif<TypeVector, dimension>& obj) : Essaim<TypeVector, dimension>() {
	initEssaim(nb_particules, min_espace, max_espace, obj);
}

template <typename TypeVector, unsigned dimension>
Essaim<TypeVector, dimension>::Essaim (const Essaim &autre) {
	(*this) = autre;
}

template <typename TypeVector, unsigned dimension>
unsigned Essaim<TypeVector, dimension>::taille() const {
	return essaim().size();
}

//###### Methodes

/**
 * \fn void Essaim<TypeVector, dimension>::initEssaim(const unsigned nb_particules, const std::vector<TypeVector> min_espace, const std::vector<TypeVector> max_espace, Objectif<TypeVector, dimension>& obj)
 * \brief Initialise les particules de l'essaim avec une sequence de sobol
 *
 * \param nb_particules nombre de particules à insérer dans l'essaim
 * \param min_espace vector contenant les bornes inférieures de l'espace de recherche
 * \param max_espace vector contenant les bornes supérieures de l'espace de recherche
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 * \return void
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::initEssaim(const unsigned nb_particules, const std::vector<TypeVector> min_espace, const std::vector<TypeVector> max_espace, Objectif<TypeVector, dimension>& obj) {
	srand(time(NULL));

	TypeVector f_meilleure_pos_e {};

	std::unique_ptr<std::vector<std::vector<TypeVector>>> _valeur_sobol{sobol_points(nb_particules, dimension, chemin,f_meilleure_pos_e)};
	std::vector<std::vector<TypeVector>> &valeur_sobol {*_valeur_sobol};

	Particule<TypeVector, dimension> p(min_espace, max_espace, obj, valeur_sobol[0]);
	essaim().push_back(p);
	meilleure_postion_e() = p.meilleure_position();
	for (unsigned i = 1 ; i < nb_particules ; ++i) {
		Particule<TypeVector, dimension> p(min_espace, max_espace, obj, valeur_sobol[i]);
		essaim().push_back(p);
	}
	//_valeur_sobol.release();
}


/**
 * \fn std::unique_ptr<std::vector<unsigned>> Essaim<TypeVector, dimension>::recupVoisins(const unsigned taille, const unsigned pos, const unsigned nb_part) const
 * \brief Recupere l'indice des voisins
 *
 * \param taille correspond a la taille de l'essaim
 * \param pos position dans le vector de particule
 * \param nb_part nombre de voisin
 * \return vector contenant les indices des voisins correspond a une particule
 */
template<typename TypeVector, unsigned dimension>
std::unique_ptr<std::vector<unsigned>> Essaim<TypeVector, dimension>::recupVoisins(const unsigned taille, const unsigned pos, const unsigned nb_part) const {
	std::unique_ptr<std::vector<unsigned>> _vect_voisins {new std::vector<unsigned>{} };
	std::vector<unsigned> &vect_voisins{*_vect_voisins};

	if  ((nb_part > 0) &&(taille > nb_part)) {
		if (pos + nb_part < taille) {
			for (unsigned i = 1 ; i < nb_part ; ++i) {
				vect_voisins.push_back(pos+i);
			}
		} else {
			for (unsigned i = pos ; i < taille ; ++i) {
				vect_voisins.push_back(i);
			}
			for (unsigned i = 0 ; i < (taille - pos) ; ++i) {
				vect_voisins.push_back(i);
			}
		}
	} else {
		throw "Mauvais paramètres : Nombre de voisins doivent être >0 et nombre de particules > nombre de voisins";
	}
	return std::move(_vect_voisins);
}

/**
 * \fn void Essaim<TypeVector, dimension>::fonctionDePositionActuelle(Particule<TypeVector, dimension>& p, Objectif<TypeVector, dimension>& obj, TypeVector f_position_actuelle)
 * \brief applique la fonction objectif sur la position actuelle
 *
 * \param p particule dont on doit déterminer la position
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 * \param f_position_actuelle valeur de la position actuelle
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::fonctionDePositionActuelle(Particule<TypeVector, dimension>& p, Objectif<TypeVector, dimension>& obj
		, TypeVector f_position_actuelle) {

	f_position_actuelle = obj(p.position_actuelle()); //Calcul fonction objectif pour la position actuelle

	if (f_position_actuelle < p.getValeurMeilleurePosition()) {
		p.setValeurMeilleurePosition(f_position_actuelle);
		p.meilleure_position() = p.position_actuelle();
	}
}


/**
 * \fn void Essaim<TypeVector, dimension>::communicationVoisins(Particule<TypeVector, dimension>& p, std::vector<unsigned>& vect_voisins)
 * \brief permet a une particule de communiquer avec ses voisin
 *
 * \param p particule que l'on étudie
 * \param vect_voisin vector contenant l'indice des voisin de la particule
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::communicationVoisins(Particule<TypeVector, dimension>& p, std::vector<unsigned>& vect_voisins) {
	if (vect_voisins.size()>1) {
		unsigned nb_voisins = vect_voisins.size();
		for (unsigned j = 0 ; j < nb_voisins - 1 ; ++j) {
			if (essaim()[(vect_voisins[j])].getValeurMeilleurePosition() < essaim()[(vect_voisins[j+1])].getValeurMeilleurePosition()) {
				p.meilleure_position_voisinage() = essaim()[(vect_voisins[j])].meilleure_position();
			} else {
				p.meilleure_position_voisinage() = essaim()[(vect_voisins[j+1])].meilleure_position();
			}
		}
	}
}
/**
 * \fn void Essaim<TypeVector, dimension>::majMeilleurObjectif(Particule<TypeVector, dimension>& p, TypeVector f_meilleure_pos_e, Objectif<TypeVector, dimension>& obj)
 * \brief met a jour la meilleure position de la particule
 *
 * \param p particule que l'on étudie
 * \param f_meilleure_pos_e meilleure position
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::majMeilleurObjectif(Particule<TypeVector, dimension>& p, TypeVector f_meilleure_pos_e, Objectif<TypeVector, dimension>& obj) {
	if (p.getValeurMeilleurePosition() < f_meilleure_pos_e) {
		meilleure_postion_e() = p.meilleure_position();
		f_meilleure_pos_e = obj(meilleure_postion_e());
	}
}
/**
 * \fn void Essaim<TypeVector, dimension>::majMeilleurObjectif(Particule<TypeVector, dimension>& p, TypeVector f_meilleure_pos_e, Objectif<TypeVector, dimension>& obj)
 * \brief met a jour la meilleure position de l'essaim
 *
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 */
template<typename TypeVector, unsigned dimension> //Vers la meilleure particule de l'essaim
void Essaim<TypeVector, dimension>::majMeilleuresPositions(Objectif<TypeVector, dimension>& obj) {
	unsigned taille_e = essaim().size();
	TypeVector f_meilleure_pos_e { };
	TypeVector f_position_actuelle { };

	f_meilleure_pos_e = obj(meilleure_postion_e());//Calcul de la fonction objectif pour la meilleure position actuelle de l'essaim

	for (unsigned i = 0; i < taille_e; ++i) {//Parcours de chaque particule
		fonctionDePositionActuelle(essaim()[i], obj, f_position_actuelle);

		//Maj meilleure position de voisins de particule (Topologie en anneau)
		std::unique_ptr<std::vector<unsigned>> _vect_voisins {recupVoisins(taille_e, i,NB_VOISINS)};
		std::vector<unsigned> &vect_voisins{*_vect_voisins};

		communicationVoisins(essaim()[i], vect_voisins); //la particule en cours ecoute ses voisins pour savoir dans quelle direction se deplacer par la suite

		majMeilleurObjectif(essaim()[i], f_meilleure_pos_e, obj);
	}
}
/**
 * \fn void Essaim<TypeVector, dimension>::majVitesse(Particule<TypeVector, dimension> &p, unsigned max_it, unsigned i_enCours)
 * \brief met a jour la vitesse d'une particule
 *
 * \param p particule que l'on étudie
 * \param max_it nombre maximum d'itération
 * \param i_enCours iteration en cours de parcours
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::majVitesse(Particule<TypeVector, dimension> &p, unsigned max_it, unsigned i_enCours) {
	std::unique_ptr<std::vector<TypeVector>> _nouv_vitesse {new std::vector<TypeVector>{} };
	std::vector<TypeVector> &nouv_vitesse{*_nouv_vitesse};

	TypeVector PSI = 0.9 - (i_enCours/max_it);

	for (unsigned i = 0 ; i < dimension; ++i) {
		TypeVector r1 = TypeVector(rand()) / TypeVector(RAND_MAX); TypeVector r2 = TypeVector(rand()) / TypeVector(RAND_MAX);
		TypeVector rho1 = C1 * r1; TypeVector rho2 = C2 * r2;
		p.vitesse_actuelle()[i] = PSI * (p.vitesse_actuelle()[i]) + rho1 *(p.meilleure_position()[i] - p.position_actuelle()[i]) +rho2*(p.meilleure_position_voisinage()[i] - p.position_actuelle()[i]);
	}
}


/**
 * \fn void Essaim<TypeVector, dimension>::deplacementParticule(Particule<TypeVector, dimension> &p, Objectif<TypeVector, dimension>& obj)
 * \brief effectue le déplacement d'une particule
 *
 * \param p particule a déplacer
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::deplacementParticule(Particule<TypeVector, dimension> &p, Objectif<TypeVector, dimension>& obj) {
	std::unique_ptr<std::vector<TypeVector>> _recentre {new std::vector<TypeVector>{} };
	std::vector<TypeVector> &recentre{*_recentre};

	for (unsigned i = 0 ; i < dimension ; ++i) {
		recentre.push_back((abs(obj.getMinEspace()[i]) + abs(obj.getMaxEspace()[i])));
	}

	for (unsigned i = 0; i < dimension; ++i) {
		p.position_actuelle()[i] += p.vitesse_actuelle()[i] / ((TypeVector) recentre[i]);
	}
}


/**
 * \fn void Essaim<TypeVector, dimension>::deplacementParticule(Particule<TypeVector, dimension> &p, Objectif<TypeVector, dimension>& obj)
 * \brief effectue le déplacement des particules
 *
 * \param max_it nombre maximum d'itération
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 */
template<typename TypeVector, unsigned dimension>
void Essaim<TypeVector, dimension>::deplacements(unsigned max_it, Objectif<TypeVector, dimension>& obj) {
	unsigned taille_e = essaim().size();

	for (unsigned i = 0; i< taille_e; ++i) {
		majVitesse(essaim()[i], max_it, i); //maj de la vitesse de la particule
		deplacementParticule(essaim()[i], obj); //Deplacement de la particule selon cette nouvelle vitesse
	}
}


/**
 * \fn void Essaim<TypeVector, dimension>::deplacementParticule(Particule<TypeVector, dimension> &p, Objectif<TypeVector, dimension>& obj)
 * \brief fonction effectuant l'algorithme des essaims particulaires
 *
 * \param max_it nombre maximum d'itération
 * \param obj variable de type objectif permettant d'utiliser la fonction objectif fourni par l'utilisateur
 */
template<typename TypeVector, unsigned dimension>
std::unique_ptr<std::vector<TypeVector>> Essaim<TypeVector,dimension>::algoEssaim(Objectif<TypeVector, dimension>& obj, unsigned max_it) {
	unsigned i {0};
	unsigned j {0};

	TypeVector eval_avant {0.0};
	TypeVector eval_apres {0.0};
	TypeVector limite = pow(10,-50);

	std::unique_ptr<std::vector<TypeVector>> _it_et_eval_stats {new std::vector<TypeVector>{} };
	std::vector<TypeVector> &it_et_eval_stats{*_it_et_eval_stats};

	do {
		eval_avant = obj(meilleure_postion_e());
		majMeilleuresPositions(obj);
		eval_apres = obj(meilleure_postion_e());

		//Ecrire le num d'iteration dans le fichier des statistiques
		it_et_eval_stats.push_back(i);
		it_et_eval_stats.push_back(eval_apres);

		deplacements(max_it,obj);

		if ((eval_avant - eval_apres) < limite) {
			++j;
		} else {
			j = 0;
		}

//		std::cout<<"------------- VAL TMP (itération n°"<<i<<") = "<<obj(meilleure_postion_e())<<"----------------"<<std::endl;

		++i;
	} while ((i < max_it) && (j < 10000));

	return (std::move(_it_et_eval_stats));
}

#endif /* SRC_HEADERS_ESSAIM_H_ */
