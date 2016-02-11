/***********************************************************************
 * Modul Zeitmessung: Durchführung von Laufzeitmessungen in Sekunden-
 *                    genauigkeit mit Protokollierung
 *
 * Umsetzung mit Bibliotheksfunktion clock().
 * clock() liefert die vom Programm bisher benutzte Prozessorzeit   
 * der Teiler CLOCKS_PER_SEC ist maschinenabhängig und spezifiziert,
 * wieviel Uhr-Ticks auf der jeweiligen Maschine pro Sekunde vergehen.
 * CLOCKS_PER_SEC definiert also die clock_t-Einheiten pro Sekunde.
 * Bei Linux (i[345..]86 glibc): #define CLOCKS_PER_SEC  1000000 
 ***********************************************************************
 *
 * Autor: Harald Badberg
 * Version: 0.1.0
 * Datum: 25.10.2006
 * Datei: zeitmessung.cpp
 * SVN: https://svn.elv.de/svn/entwicklung/home/hss/central/firmware/trunk/base/libs/elvutils/zeitmessung.cpp
 * 
 * vgl. Aufgabe 5.5-1, S. 249 aus
 *      "Das C++ Kompendium", Gilbert Brands, 
 *      Springer-Verlag Berlin Heidelberg 2005
 ***********************************************************************
 */

#include "zeitmessung.h"

Zeitmessung::Zeitmessung(string bezeichnung)
{
    cl      = clock();
    elapsed = 0;
    status  = run;

    //-------------------------------------
    const size_t      SIZE = 20;
    char buffer[SIZE];
    time_t curtime;
    struct tm *loctime;

    /* Get the current time.  */
    curtime = time (NULL);

    /* Convert it to local time representation.  */
    loctime = localtime (&curtime);

    /* Print it out in a nice format.  */
    strftime (buffer, SIZE, "%d.%m.%Y %H:%M:%S", loctime);
    //-------------------------------------

    oss << "<Zeitmessung>Start  (" << bezeichnung << "): " << buffer << endl;
}//endfunction
//----------------------------------------------------------------------

Zeitmessung::~Zeitmessung ()
{
                                
}//endfunction
//----------------------------------------------------------------------

bool Zeitmessung::Zeitmarke(string kennung)
{
    bool ret = false;

    if (status == run) 
    {
        elapsed += ((double) (clock() - cl)) / (double) CLOCKS_PER_SEC;    
        ret = true;
        oss << "<Zeitmessung>Marke(" << kennung << "): " << elapsed << endl;
    }//endif

    return ret;
}//endfunction
//----------------------------------------------------------------------

bool Zeitmessung::Suspend(string kennung)
{
    bool ret = false;

    if (status == run)
    {
        elapsed += ((double) (clock() - cl)) / (double) CLOCKS_PER_SEC;
        status   = suspend;
        ret      = true;

        oss << "<Zeitmessung>Suspend(" << kennung << "): " << elapsed << endl;
    }//endif

    return ret;
}//endfunction
//----------------------------------------------------------------------

bool Zeitmessung::Resume(string kennung)
{
    bool ret = false;

    if (status == suspend)
    {
        cl     = clock();
        status = run;
        ret    = true;

        oss << "<Zeitmessung>Resume (" << kennung << "): " << elapsed << endl;
    }//endif

    return ret;
}//endfunction
//----------------------------------------------------------------------

bool Zeitmessung::Stop(string kennung)
{
    bool ret = false;

    if (status == run)
    {
        elapsed += ((double) (clock() - cl)) / (double) CLOCKS_PER_SEC;
        status   = stop;
        ret      = true;

        oss << "<Zeitmessung>Stop   (" << kennung << "): " << elapsed << endl;
    }//endif

    return ret;
}//endfunction
//----------------------------------------------------------------------

double Zeitmessung::TotalTime() const
{
    return elapsed;                                
}//endfunction
//----------------------------------------------------------------------

string Zeitmessung::Str() const
{
	return oss.str();
}//endfunction
//----------------------------------------------------------------------
	
/*friend*/ ostream& operator<< (ostream& os, const Zeitmessung& z)
{
    os << z.oss.str();

    return os;
}//endfunction
//----------------------------------------------------------------------
