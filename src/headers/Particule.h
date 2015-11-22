/*
 * Particule.h
 *
 *  Created on: Mar 11, 2015
 *      Author: eisti
 */

#ifndef SRC_HEADERS_PARTICULE_H_
#define SRC_HEADERS_PARTICULE_H_

#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>

#include "Essaim.h"
#include "Objectif.h"


template <typename TypeVector, unsigned dimension>
class Particule;

template <typename TypeVector, unsigned dimension>
class Objectif;

//template <typename TypeVector, unsigned dimension>
//class Essaim;

template <typename TypeVector, unsigned dimension>
bool operator<(const Particule<TypeVector,dimension>& paticuleGauche, const Particule<TypeVector,dimension>& paticuleDroite);

template <typename TypeVector, unsigned dimension>
bool operator==(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite);

template <typename TypeVector, unsigned dimension>
bool operator!=(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite);

template <typename TypeVector, unsigned dimension>
std::ostream& operator<<(std::ostream &out, const Particule<TypeVector, dimension> &p);

template <typename TypeVector, unsigned dimension>
class BaseParticule {
protected:
	std::unique_ptr<std::vector<TypeVector>> _position_actuelle{new std::vector<TypeVector>{}};
	std::unique_ptr<std::vector<TypeVector>> _meilleure_position_voisinage{new std::vector<TypeVector>{}};
	std::unique_ptr<std::vector<TypeVector>> _meilleure_position{new std::vector<TypeVector>{}};
	std::unique_ptr<std::vector<TypeVector>> _vitesse_actuelle{new std::vector<TypeVector>{}};
};

template <typename TypeVector, unsigned dimension>
class Particule : public BaseParticule<TypeVector,dimension> {
private:
	TypeVector valeur_meilleure_position;
	void initParticule(const std::vector<TypeVector> &min_espace, const std::vector<TypeVector> &max_espace, Objectif<TypeVector, dimension>& obj,const std::vector<TypeVector>& valeur);


public:
	//##### CONSTRUCTEURS
	Particule();
	Particule(const std::vector<TypeVector> &min_espace, const std::vector<TypeVector> &max_espace, Objectif<TypeVector, dimension>& obj,const std::vector<TypeVector>& valeur);
	Particule (const Particule &autre);
	Particule(const std::vector<TypeVector> &position_actuelle);

	//##### Méthodes
	TypeVector calculNormeEuclidienne(const std::vector<TypeVector> &vecteur) const;//WARNING Mettre en privé si nécessaire
	//	void majMeilleurePositionParticule(Essaim<TypeVector, dimension>& e, Objectif<TypeVector, dimension> obj);
	//	void majMeilleurePositionVoisinageParticule(Essaim<TypeVector, dimension>& e, Objectif<TypeVector, dimension> obj);

	//##### GETTERS
	const std::vector<TypeVector>& getMeilleurePosition() const;
	const std::vector<TypeVector>& getPositionActuelle() const;
	const std::vector<TypeVector>& getVitesseActuelle() const;
	const std::vector<TypeVector>& getMeilleurePositionVoisinage() const;
	const TypeVector& getValeurMeilleurePosition() const;

	//######### SETTERS
//	void setMeilleurePosition(const std::vector<TypeVector>& meilleurePosition);
//	void setMeilleurePositionVoisinage(const std::vector<TypeVector>& meilleurePositionVoisinage);
//	void setPositionActuelle(const std::vector<TypeVector>& positionActuelle);
//	void setVitesseActuelle(const std::vector<TypeVector>& vitesseActuelle);
	void setValeurMeilleurePosition(const TypeVector valeur_meilleure_position);

	std::vector<TypeVector> &position_actuelle() { return *(BaseParticule<TypeVector,dimension>::_position_actuelle); }
	const std::vector<TypeVector> &position_actuelle() const { return *(BaseParticule<TypeVector,dimension>::_position_actuelle); }

	std::vector<TypeVector> &meilleure_position_voisinage() { return *(BaseParticule<TypeVector,dimension>::_meilleure_position_voisinage); }
	const std::vector<TypeVector> &meilleure_position_voisinage() const { return *(BaseParticule<TypeVector,dimension>::_meilleure_position_voisinage); }

	std::vector<TypeVector> &meilleure_position() { return *(BaseParticule<TypeVector,dimension>::_meilleure_position); }
	const std::vector<TypeVector> &meilleure_position() const { return *(BaseParticule<TypeVector,dimension>::_meilleure_position); }

	std::vector<TypeVector> &vitesse_actuelle() { return *(BaseParticule<TypeVector,dimension>::_vitesse_actuelle); }
	const std::vector<TypeVector> &vitesse_actuelle() const { return *(BaseParticule<TypeVector,dimension>::_vitesse_actuelle); }

	//##### Operateurs
	Particule<TypeVector, dimension>& operator=(const Particule<TypeVector, dimension>& autre);//OK


	template<typename U>
	friend bool operator<(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite);

	template<typename U>
	friend bool operator==(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite);

	template<typename U>
	friend bool operator!=(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite);

	template<typename U>
	friend std::ostream& operator<<(std::ostream &out, const Particule<TypeVector, dimension> &p);

};

//###### CONSTRUCTEURS
template<typename TypeVector, unsigned dimension>
Particule<TypeVector, dimension>::Particule() : BaseParticule<TypeVector, dimension>(), valeur_meilleure_position{0.0}  {

}

template<typename TypeVector, unsigned dimension>
Particule<TypeVector, dimension>::Particule(const std::vector<TypeVector> &min_espace, const std::vector<TypeVector> &max_espace, Objectif<TypeVector, dimension>& obj,const std::vector<TypeVector>& valeur) : Particule<TypeVector, dimension>()  {
	initParticule(min_espace, max_espace, obj,valeur);
}

template<typename TypeVector, unsigned dimension>
Particule<TypeVector, dimension>::Particule(const Particule& autre) : Particule() {
	(*this) = autre;
}

template<typename TypeVector, unsigned dimension>
Particule<TypeVector, dimension>::Particule(const std::vector<TypeVector> &_position_actuelle) : Particule() {
	if (_position_actuelle.size()==dimension) {
		position_actuelle() = _position_actuelle;
	} else {
		throw "Dimension du vecteur incorrecte !";
	}
}


//##### Méthodes
template<typename TypeVector, unsigned dimension>
TypeVector Particule<TypeVector, dimension>::calculNormeEuclidienne(const std::vector<TypeVector> &vecteur) const {
	TypeVector res = 0.0;

	for (unsigned i = 0 ; i < dimension ; ++i) {
		res += pow(vecteur[i],2);
	}

	res = sqrt(res);
	return res;
}



template<typename TypeVector, unsigned dimension>
void Particule<TypeVector, dimension>::initParticule(const std::vector<TypeVector> &min_espace,
		const std::vector<TypeVector> &max_espace, Objectif<TypeVector, dimension>& obj,const std::vector<TypeVector>& valeur) {

	for (unsigned i=0; i<dimension; ++i) {
		TypeVector stock = abs(max_espace[i] - min_espace[i]);
		TypeVector valTmp =(((1-min_espace[i])*valeur[i]) + (max_espace[i] * valeur[i]));
		//(1-a)t+bt

		position_actuelle().push_back(valTmp);

		//position_actuelle().push_back(min_espace[i] + (TypeVector(rand()) / TypeVector(RAND_MAX)) * (max_espace[i] - min_espace[i]));
		vitesse_actuelle().push_back((-1.0*stock) + (TypeVector(rand()) / TypeVector(RAND_MAX)) * (stock - (-1.0*stock)));
		meilleure_position().push_back(position_actuelle()[i]);
		meilleure_position_voisinage().push_back(position_actuelle()[i]);
	}
		valeur_meilleure_position = obj(meilleure_position());
}

//template<typename TypeVector, unsigned dimension>
//void Particule<TypeVector, dimension>::majMeilleurePositionParticule(Essaim<TypeVector, dimension>& e, Objectif<TypeVector, dimension> obj) {
//	auto it = e.essaim.begin();
//	std::vector<TypeVector> meilleure_position_essaim = *it;
//	++it;
//
//	for (; it!= e.essaim.end(); ++it) {
//		if (obj(*it)<obj(meilleure_position_essaim)) {
//			meilleure_position_essaim = *it;
//		}
//	}
//
//	position_actuelle = meilleure_position_essaim;
//}
//
//template<typename TypeVector, unsigned dimension>
//void Particule<TypeVector, dimension>::majMeilleurePositionVoisinageParticule(Essaim<TypeVector, dimension>& e, Objectif<TypeVector, dimension> obj) {
//
//}


//##### GETTERS
template<typename TypeVector, unsigned dimension>
const std::vector<TypeVector>& Particule<TypeVector, dimension>::getMeilleurePosition() const {
	return meilleure_position();
}

template<typename TypeVector, unsigned dimension>
const std::vector<TypeVector>& Particule<TypeVector, dimension>::getMeilleurePositionVoisinage() const {
	return meilleure_position_voisinage();
}


template<typename TypeVector, unsigned dimension>
const std::vector<TypeVector>& Particule<TypeVector, dimension>::getPositionActuelle() const {
	return position_actuelle();
}

template<typename TypeVector, unsigned dimension>
const std::vector<TypeVector>& Particule<TypeVector, dimension>::getVitesseActuelle() const {
	return vitesse_actuelle();
}

//#####SETTERS

//template<typename TypeVector, unsigned dimension>
//void Particule<TypeVector, dimension>::setMeilleurePosition(const std::vector<TypeVector>& meilleurePosition) {
//	meilleure_position() = meilleurePosition;
//}
//
//template<typename TypeVector, unsigned dimension>
//void Particule<TypeVector, dimension>::setMeilleurePositionVoisinage(const std::vector<TypeVector>& meilleurePositionVoisinage) {
//	meilleure_position_voisinage() = meilleurePositionVoisinage;
//}
//
//template<typename TypeVector, unsigned dimension>
//void Particule<TypeVector, dimension>::setPositionActuelle(const std::vector<TypeVector>& positionActuelle) {
//	position_actuelle() = positionActuelle;
//}
//
//template<typename TypeVector, unsigned dimension>
//void Particule<TypeVector, dimension>::setVitesseActuelle(const std::vector<TypeVector>& vitesseActuelle) {
//	vitesse_actuelle() = vitesseActuelle;
//}

template<typename TypeVector, unsigned dimension>
void Particule<TypeVector, dimension>::setValeurMeilleurePosition(const TypeVector _valeur_meilleure_position) {
	valeur_meilleure_position = _valeur_meilleure_position;
}

template<typename TypeVector, unsigned dimension>
const TypeVector& Particule<TypeVector, dimension>::getValeurMeilleurePosition() const {
	return valeur_meilleure_position;
}

//##### OPERATEURS
template<typename TypeVector, unsigned dimension>
Particule<TypeVector,dimension>& Particule<TypeVector, dimension>::operator =(const Particule<TypeVector, dimension>& autre) {
	position_actuelle() = autre.getPositionActuelle();
	meilleure_position_voisinage() = autre.getMeilleurePositionVoisinage();
	meilleure_position() = autre.getMeilleurePosition();
	vitesse_actuelle() = autre.getVitesseActuelle();
	valeur_meilleure_position = autre.getValeurMeilleurePosition();

	return (*this);
}

template<typename TypeVector, unsigned dimension>
bool operator<(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite) {
	TypeVector normeG {particuleGauche.calculNormeEuclidienne(particuleGauche.getVitesseActuelle())};
	TypeVector normeD {particuleDroite.calculNormeEuclidienne(particuleDroite.getVitesseActuelle())};

	return normeG < normeD;
}

template<typename TypeVector, unsigned dimension>
bool operator==(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite) {
	return ((particuleGauche.getPositionActuelle() == particuleDroite.getPositionActuelle())
			&& (particuleGauche.getMeilleurePosition() == particuleDroite.getMeilleurePosition())
			&& (particuleGauche.getVitesseActuelle() == particuleDroite.getVitesseActuelle())
			&& (particuleGauche.getValeurMeilleurePosition() == particuleDroite.getValeurMeilleurePosition()));
}

template<typename TypeVector, unsigned dimension>
bool operator!=(const Particule<TypeVector,dimension>& particuleGauche, const Particule<TypeVector,dimension>& particuleDroite) {
	return (!(particuleGauche == particuleDroite));
}

template<typename TypeVector, unsigned dimension>
std::ostream& operator<<(std::ostream &out, const Particule<TypeVector, dimension> &p) {
	out<<"------------"<<std::endl;
	out<<"Position actuelle : ";
	for (TypeVector tmp : p.getPositionActuelle()) {
		out<<" "<<tmp;
	}
	out<<std::endl;
	out<<"Vitesse actuelle: ";
	for (TypeVector tmp : p.getVitesseActuelle()) {
		out<<" "<<tmp;
	}
	out<<std::endl;
	out<<"Meilleure position : ";
	for (TypeVector tmp : p.getMeilleurePosition()) {
		out<<" "<<tmp;
	}
	out<<std::endl;
	out<<"Valeur meilleure position : "<<p.getValeurMeilleurePosition()<<std::endl;
	out<<"------------"<<std::endl;

	return out;
}



#endif /* SRC_HEADERS_PARTICULE_H_ */
