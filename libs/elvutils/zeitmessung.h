/***********************************************************************
 * Autor: Harald Badberg
 * Version: 0.0.1
 * Datum: 23.10.2006
 * Datei: zeitmessung.h
 * SVN: https://svn.elv.de/svn/entwicklung/home/hss/central/firmware/trunk/base/libs/elvutils/zeitmessung.h
 ***********************************************************************
 */

#ifndef __ZEITMESSUNG__
#define __ZEITMESSUNG__

#include <string>
#include <stdio.h>
#include <time.h>
#include <ostream>
#include <sstream>

using namespace std;

class Zeitmessung {
public:
     Zeitmessung (string bezeichnung);
    ~Zeitmessung ();

    //Generiert Zwischenmeldungen ohne Unterbrechung der Messung
    bool Zeitmarke(string kennung);

    //Unterbricht die laufende Messung
    bool Suspend(string kennung);

    //Setzt die Messung fort
    bool Resume(string kennung);

    //Hält die Messung endgültig an
    bool Stop(string kennung);

	//Gibt die Summe der gemessene Zeiten zurück
	//Rückgabe wie clock() aus time.h
	double TotalTime() const;

	//Gibt das Protokoll der Zeitmessung als String zurück
	string Str() const;

    //Ermöglicht das Schreiben des Zeitprotokolls auf eine Ausgabeeinheit
    friend ostream& operator<< (ostream& os, const Zeitmessung& z);
	
private:
    clock_t cl;
    double elapsed;
    ostringstream oss;
    enum {run, suspend, stop} status;

};//endclass
//----------------------------------------------------------------------
#endif
