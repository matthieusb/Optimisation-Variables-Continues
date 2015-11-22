/*
 * mpi-bind.cc
 *
 *  Created on: 19 mars 2015
 *      Author: Rémi Vernay
 */

#include "../headers/mpi-bind.h"


BaseMpiBind::BaseMpiBind(int argc, char** argv) : _comm(nullptr) {
	if (!(MPI::Is_initialized())) {
//		MPI::Init_thread(argc,argv,MPI::THREAD_FUNNELED);
		MPI::Init(argc,argv);
	}
	_comm=&MPI::COMM_WORLD;
}

BaseMpiBind::~BaseMpiBind() {
	if (MPI::Is_initialized() && !(MPI::Is_finalized())) {
		MPI::Finalize();
	}
}

MPI::Comm& BaseMpiBind::comm() {
	return *_comm;
}

BaseMpiBind::BaseMpiBind(MPI::Comm& comm) : _comm(nullptr) {
	if (!(MPI::Is_initialized())) {
		throw "MPI not yet initialized";
	}
	else {
		_comm=&comm;
	}
}

const MPI::Comm& BaseMpiBind::comm() const {
	return *_comm;
}

MpiBind::MpiBind(int argc, char** argv) : BaseMpiBind(argc,argv) {}

MpiBind::MpiBind(MPI::Comm& comm) : BaseMpiBind(comm) {}

MpiBind::~MpiBind() {
}

void MpiBind::send(const void* data, unsigned count, const MPI::Datatype& dataType, unsigned dest) const {
	comm().Send(data,count,dataType,dest,42);
}

void MpiBind::recv(void* data, unsigned count, const MPI::Datatype& dataType, unsigned dest) const {
	comm().Recv(data,count,dataType,dest,42);
}

void MpiBind::send(const void* data, const MPI::Datatype& dataType, unsigned dest) const {
	send(data,1,dataType,dest);
}

void MpiBind::recv(void* data, const MPI::Datatype& dataType, unsigned dest) const {
	recv(data,1,dataType,dest);
}

unsigned MpiBind::getRank() const {
	return comm().Get_rank();
}

unsigned MpiBind::getSize() const {
	return comm().Get_size();
}

template <>
const MPI::Datatype& MpiBind::typeOf<short>(const short&) const {
	return MPI::SHORT;
}

template <>
const MPI::Datatype& MpiBind::typeOf<int>(const int&) const {
	return MPI::INT;
}

template <>
const MPI::Datatype& MpiBind::typeOf<long>(const long&) const {
	return MPI::LONG;
}

template <>
const MPI::Datatype& MpiBind::typeOf<long long>(const long long&) const {
	return MPI::LONG_LONG;
}

template <>
const MPI::Datatype& MpiBind::typeOf<unsigned short>(const unsigned short&) const {
	return MPI::UNSIGNED_SHORT;
}

template <>
const MPI::Datatype& MpiBind::typeOf<unsigned int>(const unsigned int&) const {
	return MPI::UNSIGNED;
}

template <>
const MPI::Datatype& MpiBind::typeOf<unsigned long>(const unsigned long&) const {
	return MPI::UNSIGNED_LONG;
}

template <>
const MPI::Datatype& MpiBind::typeOf<unsigned long long>(const unsigned long long&) const {
	return MPI::UNSIGNED_LONG_LONG;
}

template <>
const MPI::Datatype& MpiBind::typeOf<float>(const float&) const {
	return MPI::FLOAT;
}

template <>
const MPI::Datatype& MpiBind::typeOf<double>(const double&) const {
	return MPI::DOUBLE;
}

template <>
const MPI::Datatype& MpiBind::typeOf<long double>(const long double&) const {
	return MPI::LONG_DOUBLE;
}

template <>
const MPI::Datatype& MpiBind::typeOf<char>(const char&) const {
	return MPI::CHAR;
}

template <>
const MPI::Datatype& MpiBind::typeOf<bool>(const bool&) const {
	return MPI::BOOL;
}

template <>
void MpiBind::send(const std::string& chaine, unsigned dest) const {
	unsigned taille=chaine.size();
	send(taille,dest);

	for (auto it=chaine.begin();it!=chaine.end();++it) {
		send(*it,dest);
	}
}

template <>
void MpiBind::recv(std::string& tab, unsigned exp) const {
	unsigned taille;
	char lettre;
	recv(taille,exp);

	tab="";

	typename std::ostringstream oss{};

	for (unsigned i=0;i<taille;++i) {
		recv(lettre,exp);
		oss<<lettre;
	}

	tab=oss.str();
}

//#########MATRICE
//template<>
//void MpiBind::send<Matrice> (const Matrice &mat, unsigned dest) const {
//	this->send((unsigned) mat.size(),dest);//Envoi de la taille
//	for (unsigned i = 0 ; i < mat.size() ; ++i) {
//		this->send(mat[i],dest);
//	}
//}
//
//template<>
//void MpiBind::recv<Matrice> (Matrice &mat, unsigned exp) const {
//	mat.clear();
//
//	unsigned nbLignes;
//	this->recv(nbLignes,exp);
//	std::vector<int> ligne{};
//
//	for (unsigned i = 0 ; i < nbLignes ; ++i) {
//		this->recv(ligne,exp);
//		mat.push_back(ligne);
//	}
//}



