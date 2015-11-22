#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <memory>
#include <cstdlib>
#include <iostream>

#include "../headers/Particule.h"
#include "../headers/Objectif.h"
#include "../headers/SampleOfFunctions.h"
#include "../headers/Sobol.h"


using namespace std;

//##################" TESTS SOBOL
void test_throw_sobol() {
	unique_ptr<Objectif<long double, 3>> _obj(new Objectif<long double,3>());
	Objectif<long double, 3>& obj = *_obj;

	//############ Chemin fichier incorrect
	auto chemin = "./mauvais-chemin";
	long double tmp = 0.0;
	std::unique_ptr<std::vector<std::vector<long double>>> _valeur_sobol{sobol_points(1, 3, chemin,tmp)};
}

void tests_sobol() {
	ASSERTM("------- Tests sur Sobol -------",true);

	//############Test fichier incorrect
	ASSERT_THROWS(test_throw_sobol(),const char*);

	//############Test dimension > 1
	unique_ptr<Objectif<long double, 3>> _obj(new Objectif<long double,3>());
	Objectif<long double, 3>& obj = *_obj;
	unsigned nb_particules = 10;

	auto chemin = "./joe-kuo-other-4.5600.txt";
	long double tmp = 0.0;
	std::unique_ptr<std::vector<std::vector<long double>>> _valeur_sobol{sobol_points(nb_particules, 3, chemin,tmp)};
	std::vector<std::vector<long double>> &valeur_sobol {*_valeur_sobol};

	for (unsigned i = 0 ; i < nb_particules ; ++i) {
		for (unsigned j = 0 ; j < 3 ; ++j) {
			ASSERT_EQUAL(true,((valeur_sobol[i][j] > obj.getMinEspace()[j]) && (valeur_sobol[i][j] < obj.getMaxEspace()[j])));
		}
	}

//	_valeur_sobol.release();
}

//################## TESTS SAMPLEOFFUNCTIONS
void test_sample_of_functions() {
	ASSERTM("------- Tests sur SampleOfFunctions -------",true);

	//########### TESTS dimension 3
	SampleOfFunctions<long double, 3> sof{};

	unique_ptr<std::vector<long double>> _vecteur_test1(new vector<long double> { });
	std::vector<long double>& vecteur_test1= *_vecteur_test1;

	for (unsigned i = 0 ; i < 3 ; ++i) {
		vecteur_test1.push_back(0.0);
	}

	ASSERT_EQUAL(0.0, sof.sphere(vecteur_test1));
	ASSERT_EQUAL(0.0, sof.sumSquares(vecteur_test1));
	ASSERT_EQUAL(0.0, sof.rastrigin(vecteur_test1));

	for (unsigned i = 0 ; i < 3 ; ++i) {
		vecteur_test1[i] = 1.0;
	}

	ASSERT_EQUAL(0.0, sof.rosenbrock(vecteur_test1));

	//########### TESTS dimension 6
	SampleOfFunctions<long double, 6> sof2{};

	unique_ptr<std::vector<long double>> _vecteur_test2(new vector<long double> { });
	std::vector<long double>& vecteur_test2= *_vecteur_test2;

	for (unsigned i = 0 ; i < 6 ; ++i) {
		vecteur_test2.push_back(1.0);
	}

	ASSERT_EQUAL(-5.0, sof2.trid(vecteur_test2));


	//########### TESTS dimension 10
	SampleOfFunctions<long double, 10> sof3{};

	unique_ptr<std::vector<long double>> _vecteur_test3(new vector<long double> { });
	std::vector<long double>& vecteur_test3= *_vecteur_test3;

	for (unsigned i = 0 ; i < 10 ; ++i) {
		vecteur_test3.push_back(1.0);
	}

	ASSERT_EQUAL(-9.0, sof3.trid(vecteur_test3));
}

//################## TESTS OBJECTIF
void test_objectif() {
	ASSERTM("------- Tests sur Objectif -------",true);

	//############### Tests pour sphère
	//######### Constructeur classique
	unique_ptr<Objectif<long double, 3>> _obj(new Objectif<long double,3>());
	Objectif<long double, 3>& obj = *_obj;

	//######## Constructeur avec params
	unique_ptr<std::vector<long double>> _min_espace(new vector<long double>());
	std::vector<long double>& min_espace= *_min_espace;
	unique_ptr<std::vector<long double>> _max_espace(new vector<long double>());
	std::vector<long double>& max_espace= *_max_espace;

	for (unsigned i = 0 ; i < 3 ; ++i) {
		min_espace.push_back(-2.048);
		max_espace.push_back(2.048);
	}

	unique_ptr<Objectif<long double, 3>> _obj_params(new Objectif<long double,3>(min_espace,max_espace));
	Objectif<long double, 3>& obj_params = *_obj_params;

	for (unsigned i = 0 ; i < 3 ;++i) {
		ASSERT_EQUAL(-2.048, obj.getMinEspace()[i]);//Constructeur sans params
		ASSERT_EQUAL(2.048, obj.getMaxEspace()[i]);

		ASSERT_EQUAL(-2.048,obj_params.getMinEspace()[i]);//Constructeur avec params
		ASSERT_EQUAL(2.048, obj_params.getMaxEspace()[i]);
	}

	//######## Appel fonction val
	unique_ptr<std::vector<long double>> _vecteur_0(new vector<long double>());
	std::vector<long double>& vecteur_0= *_vecteur_0;
	for (unsigned i = 0 ; i < 3 ; ++i) {
		vecteur_0.push_back(1.0);
	}

	ASSERT_EQUAL(0.0, obj(vecteur_0));
}

//##################" TESTS PARTICULE
void test_throw_constructeur_particule() {
	unique_ptr<std::vector<long double>> _vecteur1(new vector<long double>());
	std::vector<long double>& vecteur1= *_vecteur1;

	for (unsigned i = 0 ; i < 4 ; ++i) {
		vecteur1.push_back(1.0);
	}

	Particule<long double, 5> p1(vecteur1);}


void test_init_particule() {

	ASSERTM("------- Tests sur Init Particule -------",true);

	//############### Tests sur les constructeurs
	unique_ptr<Objectif<long double, 3>> _obj(new Objectif<long double,3>());
	Objectif<long double, 3>& obj = *_obj;

	auto chemin = "./joe-kuo-other-4.5600.txt";

	long double tmp {0.0};
	std::unique_ptr<std::vector<std::vector<long double>>> _valeur_sobol{sobol_points(1, 3, chemin,tmp)};
	std::vector<std::vector<long double>> valeur_sobol {*_valeur_sobol};

	Particule<long double, 3> p1(obj.getMinEspace(), obj.getMaxEspace(), obj, valeur_sobol[0]);

	for (unsigned i = 0 ; i < 3 ; ++i) {
		ASSERT_EQUAL(true,((p1.getPositionActuelle()[i] > obj.getMinEspace()[i]) && (p1.getPositionActuelle()[i] < obj.getMaxEspace()[i])));
	}

//	_valeur_sobol.release();

	//####Constructeur par recopie et opérateurs == et !=
	Particule<long double, 3> p2(p1);
	ASSERT_EQUAL(true, (p1==p2));
	ASSERT_EQUAL(false, (p1!=p2));


	//############### Tests sur norme euclidienne
	ASSERTM("------- Tests sur norme euclidienne -------",true);
	unique_ptr<std::vector<long double>> _vecteur(new vector<long double>());
	std::vector<long double>& vecteur= *_vecteur;

	for (unsigned i = 0 ; i < 4 ; ++i) {
		vecteur.push_back(1.0);
	}

	Particule<long double, 4> pnorm(vecteur);

	ASSERT_EQUAL(2.0, pnorm.calculNormeEuclidienne(pnorm.position_actuelle()));
	ASSERT_THROWS(test_throw_constructeur_particule(), const char*);

	//### Tests opérateur <
	unique_ptr<std::vector<long double>> _vecteur2(new vector<long double>());
	std::vector<long double>& vecteur2= *_vecteur2;
	for (unsigned i = 0 ; i < 4 ; ++i) {
		vecteur2.push_back(0.5);
	}

	Particule<long double, 4> pnorm2(vecteur2);

	for (unsigned i = 0 ; i < 4 ; ++i) {//Set de la vitesse
		pnorm.vitesse_actuelle().push_back(0.5);
		pnorm2.vitesse_actuelle().push_back(1.0);
	}

	ASSERT_EQUAL(true,(pnorm < pnorm2));

}

//################ TESTS ESSAIM
void test_essaim() {
	unique_ptr<Objectif<long double, 6>> obj_ptr(new Objectif<long double, 6> { });
	Objectif<long double, 6>& obj = *obj_ptr;
	unique_ptr<Essaim<long double,6>> e_ptr(new Essaim<long double,6>(60,obj.getMinEspace(), obj.getMaxEspace(), obj));
	Essaim<long double,6>& e = *e_ptr;

	e.algoEssaim(obj, 50000);

	long double res = obj(e.meilleure_postion_e());

	//Pour sphere
	ASSERT_EQUAL(true, ((res > -0.1) && (res < 0.1)));
}

void runAllTests(int argc, char const *argv[]){
	cute::suite s{};

	//################### Ajouts des fonctions à tester
	s.push_back(CUTE(tests_sobol));
	s.push_back(CUTE(test_sample_of_functions));
	s.push_back(CUTE(test_objectif));
	s.push_back(CUTE(test_init_particule));
	s.push_back(CUTE(test_essaim));

	//##################

	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}


//int main(int argc, char const *argv[]){
//	runAllTests(argc,argv);
//	return 0;
//}




