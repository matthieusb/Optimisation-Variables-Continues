/*
 * mpi-bind.h
 *
 *  Created on: 19 mars 2015
 *      Author: Rémi Vernay
 */

#ifndef MPI_BIND_H_
#define MPI_BIND_H_

#include <mpi.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>
#include <iterator>

#include "Particule.h"

class BaseMpiBind {
private:
	MPI::Comm *_comm;
protected:
	BaseMpiBind(int argc, char** argv);
	BaseMpiBind(MPI::Comm &comm);
public:
	virtual ~BaseMpiBind();

	MPI::Comm& comm();
	const MPI::Comm& comm() const;
};

class MpiBind : public BaseMpiBind{
private:
	void send(const void *data, unsigned count, const MPI::Datatype &dataType, unsigned dest) const;
	void recv(void *data, unsigned count, const MPI::Datatype &dataType, unsigned dest) const;

	void send(const void *data, const MPI::Datatype &dataType, unsigned dest) const;
	void recv(void *data, const MPI::Datatype &dataType, unsigned dest) const;

	template <typename T>
	const MPI::Datatype& typeOf(const T&) const;
public:
	MpiBind(int argc, char** argv);
	MpiBind(MPI::Comm &comm);
	virtual ~MpiBind();

	unsigned getRank() const;
	unsigned getSize() const;

	template <typename T>
	void send(const T& data, unsigned dest) const;
	template <typename T>
	void recv(T& data, unsigned dest) const;

	template <typename TypeContenu>
	void send(const std::vector<TypeContenu> &tab, unsigned dest) const;
	template <typename TypeContenu>
	void recv(std::vector<TypeContenu> &tab, unsigned exp) const;
};

template <typename T>
void MpiBind::send(const T& val, unsigned dest) const {
	send(&val,typeOf(val),dest);
}

template <typename T>
void MpiBind::recv(T& val, unsigned exp) const {
	recv(&val,typeOf(val),exp);
}

template <typename T>
const MPI::Datatype& MpiBind::typeOf(const T&) const {
	throw "not yet implemented";
}

template <typename TypeContenu>
void MpiBind::send(const std::vector<TypeContenu>& tab, unsigned dest) const {
	unsigned taille=tab.size();
	send(taille,dest);
//	std::cout<<"----- ENVOI";

	for (auto it = tab.begin() ; it!=tab.end() ; ++it) {
//		std::cout<<" -> "<<*it;
		send(*it,dest);
	}
}

template <typename TypeContenu>
void MpiBind::recv(std::vector<TypeContenu>& tab, unsigned exp) const {
	unsigned taille;
	recv(taille,exp);

	tab.clear();
	tab.resize(taille);

	for (unsigned i=0;i<taille;++i) {
		recv(tab[i],exp);
	}
}

#endif /* MPI_BIND_H_ */
