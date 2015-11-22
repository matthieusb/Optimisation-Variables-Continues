# README PROJET GSI

## Attention :
	La fichier "joe-kuo-other-4.5600.txt" fourni à la racine du projet, doit IMPÉRATIVEMENT être mis dans le même dossier que l'éxécutable, autrement l'exécution ne fonctionnera pas.  

## Dossiers :
	CahierDesCharges -> Dossier contenant le cahier des charges  
	ProjetGSI -> Dossier contenant le code  
	JeuDeDonnees-Stats -> Dossier contenant les résultats obtenus sous forme de fichiers csv et xls  
	Executables -> Contient des executables tests (Dimension 3 pour le séquentiel, dimension 4 pour la parallèle)

## Paramétrage Projet Séquentiel :
Pour compiler le projet normalement, il n'y a rien à faire de particulier. Vous pouvez faire des modifs dans le main.cpp selon le type d'éxécution (Parallèle OMP, MPI, ou séquentiel) que vous souhaitez.  

	Pour faire des tests classiques (CUTE sans couverture de code) :  
	Commenter le main situé dans le fichier "main.cpp"  
	Décommenter le main situé dans "Tests.cpp"  

	Pour faire les tests de couverture avec gcov, il faut ajouter les options suivantes :  
	Dans "GCC C++ Compiler/Debugging" :  ajouter "-coverage" et cocher "Generate gcov information"  
	Dans "GCC C++ Linker" : ajouter la librairie "gcov"  

## Paramétrage Projet Parallèle OMP :
	Attention, pour faire tourner le projet en parallèle, il faut :  
	Enlever tous les paramètres relatifs aux tests de couverture  
	"GCC C++ Compiler/Optimization", ajouter O2 dans la barre de défilement
	
## Paramétrage Projet Parallèle MPI :
Attention, pour faire tourner le projet en parallèle avec OMP, il faut :  

	Enlever tous les paramètres relatifs aux tests de couverture  
	"GCC C++ Compiler/Optimization", mettre O0 dans la barre de défilement
	Dans "GCC C++ Compiler/Includes", ajouter "/usr/lib/openmpi/include"
	Dans "GCC C++ Compiler/Miscellaneous" ajouter "-Wno-long-long", et enlever "-Weffc++"
	Dans "GCC C++ Linker", changer "Command" à "mpiCC" au lieu de "g++" et l'option à "-fopenmp"
	

## Personnalisation des paramètres d'éxécution :
Pour changer les différents coefficients, rendez vous dans "Essaim.h" (Ils seront aussi changés pour EssaimOMP.h et EssaimMPI.h). Ceux-ci permettent un calcul de la vitesse plus efficace selon les cas.  

Pour une fonction avec beaucoup de minima locaux, nous conseillons :  

	C1=1.0 et C2=2.0 OU d''une façon générale, un C2 plus important que C1  
	Dans un cas avec des minima plus globaux :  
	C1=2.0 et C2=2.0  

Dans le main.cpp vous pouvez modifier :  

	Le nombre particules lors de la création de l'essaim  
	Le nombre de dimensions lors de la création de l'essaim  
	Le nombre d'itérations lors de l'appel de algoEssaim  

Dans Objectif.h  

	Vous pouvez choisir quelle fonction éxécuter dans l'opérateur "()"  
	ATTENTION : vous devez choisir l'espace de recherche correspondant à la fonction choisie au niveau du constructeur d'Objectif  
	
## Options de compilations :
Voici toutes les options de compilation que nous utilisons pour notre projet (Si vous rencontrez des problèmes), attention certains peuvent entrer en conflit si vous travaillez avec MPI :  

	-std=c++1y -D__cplusplus=201303L -I"/home/eisti/Documents/ING2/Informatique/ProjetGSI/Source/git/projetgsi/ProjetGSI/cute" -O0 -g3 -coverage -ftest-coverage -fprofile-arcs -pedantic -Wall -Wextra -c -fmessage-length=0 -Weffc++ -fopenmp -fPIC  

