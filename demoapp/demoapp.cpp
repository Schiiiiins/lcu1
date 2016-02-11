#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <directfb.h>
#include <string>

/* Alle Pixelangaben beziehen sich auf die Displaygr��e von 320 x 240 */

/* Schrifth�he f�r Textausgabe in Pixeln */
#define TEXT_HEIGHT 15

/* Formatstring, um mit sprintf() Dateinamen der Ger�tedateien f�r die Digitaleing�nge zu erzeugen */
#define INPUT_FILE_FORMAT "/dev/inout/in%d"

/* Formatstring, um mit sprintf() Dateinamen der Ger�tedateien f�r die Digitalausg�nge zu erzeugen */
#define OUTPUT_FILE_FORMAT "/dev/inout/out%d"

/* Formatstring, um mit sprintf() Dateinamen der Dateien f�r die Analogeing�nge zu erzeugen */
#define ADC_FILE_FORMAT "/sys/class/hwmon/hwmon0/device/in%d_input"

/* Pfad zur zu verwendenden True-Type-Schrift */
#define FONT_FILE "/usr/share/fonts/decker.ttf"

/* Anzahl der Digitaleing�nge */
#define NUM_INPUTS 4

/* Anzahl der Digitalausg�nge */
#define NUM_OUTPUTS 5

/* Anzahl der Analogeing�nge */
#define NUM_ADCS 2

/* Gr��e der Quadrate f�r die Digitaleing�nge und -ausg�nge in Pixeln */
#define RECT_SIZE 40
/* Rand um die Quadrate f�r die Digitaleing�nge und -ausg�nge in Pixeln */
#define RECT_MARGIN 6
/* Y-Position der Oberkante der Quadrate f�r die Digitalausg�nge in Pixeln */
#define OUTPUT_RECTS_TOP 120
/* Y-Position der Oberkante der Quadrate f�r die Digitaleing�nge in Pixeln */
#define INPUT_RECTS_TOP 30

/* Y-Position der Oberkante der Rechtecke f�r die Analogeing�nge in Pixeln */
#define ADC_TOP 210
/* Breite der Rechtecke f�r die Analogeing�nge in Pixeln */
#define ADC_RECT_WIDTH 150
/* H�he der Rechtecke f�r die Analogeing�nge in Pixeln */
#define ADC_RECT_HEIGHT 25
/* Rand um die Rechtecke f�r die Analogeing�nge in Pixeln */
#define ADC_RECT_MARGIN 6
/* Innerer Rand um den Wertebalken f�r die Analogeing�nge in Pixeln */
#define ADC_RECT_INTERNAL_MARGIN 2
/* Maximaler ADC-Wert der Analogeing�nge. Einheit mV */
#define ADC_MAX_VALUE 30360


/* Interfacepointer f�r DirectFB */
static IDirectFB *_dfb;
static IDirectFBSurface *_primarySurface;
static IDirectFBEventBuffer *_eventBuffer;
static IDirectFBFont* _font;

/* Dateidescriptor f�r DirectFB-Events */
static int _eventFd;
/* Bitfeld f�r Touchscreen-Ereignisse */
static int _eventFlags = 0;
/* Letzte vom Touchscreen empfangene X-Koordinate */
static int _penX;
/* Letzte vom Touchscreen empfangene Y-Koordinate */
static int _penY;

/* Bitflags f�r die Verarbeitung von Touchscreen-Ereignissen */
/* Ein "Klick" wird erkannt, wenn nach einem Tastendruckereignis noch eine X-Koordinate und eine Y-Koordinate empfangen wurden */
#define EVT_FLAG_X_RECEIVED 1 /* X-Koordinate empfangen */
#define EVT_FLAG_Y_RECEIVED 2 /* Y-Koordinate empfangen */
#define EVT_FLAG_PENDOWN_WAIT 4 /* Tastendruckereignis empfangen */
#define EVT_FLAG_PENDOWN 8 /* Tastendruckereignis und Koordinaten empfangen */
#define EVT_FLAG_COMPLETE (EVT_FLAG_X_RECEIVED|EVT_FLAG_Y_RECEIVED|EVT_FLAG_PENDOWN_WAIT)

/* Dateidescriptoren f�r die Ger�tedateien der Digitaleing�nge */
static int _inputFds[NUM_INPUTS];
/* Dateidescriptoren f�r die Ger�tedateien der Digitalausg�nge */
static int _outputFds[NUM_OUTPUTS];
/* Dateidescriptoren f�r die Ger�tedateien der Analogeing�nge */
static int _adcFds[NUM_ADCS];

/* Aktuelle Zust�nde der Digitaleing�nge */
static bool _inputStates[NUM_INPUTS];
/* Aktuelle Zust�nde der Digitalausg�nge */
static bool _outputStates[NUM_OUTPUTS];
/* Aktuelle Werte der Analogeing�nge */
static int _adcValues[NUM_ADCS];


static void initDfb( int argc, char **argv );
static void renderScreen();
static void processInputEvent();
static void openFiles();
static void mainLoop();

int main (int argc, char **argv)
{
    initDfb( argc, argv );
    
    openFiles();
    
    mainLoop();
    
}

/*
 * Initialisierung der DirectFB-Datenstrukturen
 */
static void initDfb( int argc, char **argv )
{
   
    DirectFBInit (&argc, &argv);
    
	DFBResult err;
	DirectFBCreate (&_dfb);
	_dfb->SetCooperativeLevel (_dfb, DFSCL_FULLSCREEN);

	DFBSurfaceDescription dsc;
	dsc.flags = DSDESC_CAPS;
	dsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);
	err = _dfb->CreateSurface( _dfb, &dsc, &_primarySurface );
	if( err != DFB_OK )
	{
		printf("DFB error %d", err );
        exit(-1);
	}
    
	DFBFontDescription fontDsc;
	fontDsc.flags = DFDESC_HEIGHT;
	fontDsc.height = TEXT_HEIGHT;

	err = _dfb->CreateFont (_dfb, FONT_FILE, &fontDsc, &_font);
	if( err != DFB_OK )
	{
		printf( "DFB error %d creating font", err );
        exit(-1);
	}
    
	_dfb->CreateInputEventBuffer( _dfb, DICAPS_ALL, DFB_FALSE, &_eventBuffer );
    
    /* Auf den Eventbuffer f�r Input-Ereignisse wird �ber einen Dateidescriptor zugegriffen, um die Verarbeitung �ber select() mit der 
       Verarbeitung der Eingangssignale multiplexen zu k�nnen */
    
	if( (err=_eventBuffer->CreateFileDescriptor(_eventBuffer, &_eventFd)) != DFB_OK )
	{
		printf( "DFB error %d creating event file descriptor", err );
        exit(-1);
	}
	fcntl(_eventFd, F_SETFL, O_NONBLOCK);
}

/*
 * Hauptschleife. Wartet auf ein Ereignis entweder von den Digitaleing�ngen oder vom Touchscreen. Danach wird die
 * Grafikausgabe neu erzeugt. Nach jeweils zwei Sekunden wird auf jeden Fall die Grafikausgabe neu erzeugt, um die
 * Darstellung der Analogeing�nge aktuell zu halten.
 */
static void mainLoop()
{
    while(true)
    {        
    	/* Erzeugen der Sets von Dateidescriptoren f�r select() */
	    fd_set inFd, outFd, excFd;
	    FD_ZERO(&inFd);
	    FD_ZERO(&outFd);
	    FD_ZERO(&excFd);

        /* Hinzuf�gen des Dateidescriptors f�r den Touchscreen */
	    FD_SET(_eventFd, &inFd);
	    int maxFd = _eventFd;
        
        /* Hinzuf�gen der Dateidescriptoren f�r die Digitaleing�nge */
        for( int i=0; i<NUM_INPUTS; i++ )
        {
    	    FD_SET(_inputFds[i], &inFd);
            if( _inputFds[i] > maxFd ) maxFd = _inputFds[i];
        }

	    
	    int nEvents;
        /* Timeout f�r Warten auf Ereignisse 2 Sekunden */
	    struct timeval tv;
	    tv.tv_sec = 2;
	    tv.tv_usec = 0;
        /* Aufruf von select(). Anzahl der lesbaren Descriptoren in nEvents */
	    nEvents = select(maxFd+1, &inFd, &outFd, &excFd, &tv);
    
	    if( nEvents < 0 )
	    {
            perror("select");
            exit( -1 );
	    }
        
        /* Pr�fen, ob einer der Descriptoren f�r die Digitaleing�nge lesbar ist */
        for( int i=0; i<NUM_INPUTS; i++ )
        {
    	    if( FD_ISSET(_inputFds[i], &inFd) )
            {
                /* OK, Daten zum Lesen stehen bereit.
                 * Es gibt meistens nur ein Zeichen zu lesen, daher lesen wir auch nur ein Zeichen. Im schlimmsten Fall landen wir in mehreren
                 * aufeinanderfolgenden Schleifendurchl�ufen hier
                 */                
                char c;
                if( read( _inputFds[i], &c, 1 ) != 1 )
                {
                    perror("read");
                    exit( -1 );
                }
                /* Das niederwertigste Bit gibt den Eingangszustand an. Wir speichern diesen in _inputStates[i] */
                _inputStates[i] = ((c&0x01) != 0);
            }
        }
        
        /* Pr�fen, ob der Descriptor f�r den Touchscreen lesbar ist */
    	if( FD_ISSET(_eventFd, &inFd) )
	    {
            /* Verarbeitung des Touchscreen-Ereignisses */
            processInputEvent();
	    }
        /* Grafikausgabe neu erzeugen */
        renderScreen();
    }
}

/*
 * Liest ein Touchscreen-Ereignis und verarbeitet es
 */
static void processInputEvent()
{
	DFBEvent evt;
	int count = read(_eventFd, &evt, sizeof(evt));
	if( count != sizeof(evt) )
	{
		printf( "Expected %d bytes on read from event fd but received %d bytes", sizeof(evt), count );
        exit( -1 );
	}
    /* Wir sind nur an Input-Ereignissen interessiert */
	if( evt.clazz == DFEC_INPUT )
	{
        /* Extrahieren des Input-Events aus der universellen Struktur DFBEvent */
        DFBInputEvent inputEvent;
    	memcpy( &inputEvent, &evt.input, sizeof(DFBInputEvent) );
    	if( inputEvent.type == DIET_BUTTONPRESS )
	    {
            /* Tastendruck */
            if( !(_eventFlags & EVT_FLAG_PENDOWN) )
            {
                /* We set the flags here to wait for subsequent X and Y messages */
		        _eventFlags = EVT_FLAG_PENDOWN_WAIT;
            }
	    }else if( inputEvent.type == DIET_BUTTONRELEASE )
        {
            /* Zur�cksetzen der Flags, um f�r den n�chsten Tastendruck bereit zu sein */
            _eventFlags = 0;
	    }else if( inputEvent.type == DIET_AXISMOTION )
	    {
		    switch( inputEvent.axis )
		    {
		    case DIAI_X:
			    _penX = inputEvent.axisabs;
                _eventFlags |= EVT_FLAG_X_RECEIVED;
			break;
    		case DIAI_Y:
	    		_penY = inputEvent.axisabs;
                _eventFlags |= EVT_FLAG_Y_RECEIVED;
			break;
		    default:
			break;
		    }
            /* Pr�fen, ob alle f�r die Verarbeitung eines Touch-Ereignisses n�tigen Informationen vorliegen */
		    if( (_eventFlags & EVT_FLAG_COMPLETE) == EVT_FLAG_COMPLETE )
		    {
                _eventFlags = EVT_FLAG_PENDOWN;
                for( int i=0; i<NUM_OUTPUTS; i++ )
                {
                    /* Pr�fung, ob die Koordinaten innerhalb eines der Rechtecke f�r die Digitalausg�nge liegen */
                    int rectangleX = 2 * RECT_MARGIN + i*(RECT_SIZE + 2*RECT_MARGIN);
                    int rectangleY = OUTPUT_RECTS_TOP + RECT_MARGIN;
                    
                    if( (_penX - rectangleX < RECT_SIZE) && (_penX - rectangleX > 0) && (_penY - rectangleY < RECT_SIZE) && (_penY - rectangleY > 0) )
                    {
                        /* Toggeln des Ausganges */
                        _outputStates[i] = !_outputStates[i];
                        char c = (char)_outputStates[i];
                        write( _outputFds[i], &c, 1 );
                    }
                }
    		}
	    }
        
	}
}

/*
 * �ffnen der Ger�tedateien f�r die Ein- und Ausg�nge
 */
static void openFiles()
{
    char buffer[64];
    /* Digitaleing�nge */
    for( int i=0; i<NUM_INPUTS; i++ )
    {
        sprintf( buffer, INPUT_FILE_FORMAT, i );
        int fd = open( buffer, O_RDONLY );
        if( fd <  0 )
        {
            printf("Error opening input device %s\n", buffer);
            exit(-1);
        }
        _inputFds[i] = fd;
    }
    
    /* Digitalausg�nge */
    for( int i=0; i<NUM_OUTPUTS; i++ )
    {
        sprintf( buffer, OUTPUT_FILE_FORMAT, i );
        int fd = open( buffer, O_RDWR );
        if( fd <  0 )
        {
            printf("Error opening output device %s\n", buffer);
            exit(-1);
        }
        _outputFds[i] = fd;
    }
    
    /* Analogeing�nge */
    for( int i=0; i<NUM_ADCS; i++ )
    {
        sprintf( buffer, ADC_FILE_FORMAT, i );
        int fd = open( buffer, O_RDONLY );
        if( fd <  0 )
        {
            printf("Error opening adc device %s\n", buffer);
            exit(-1);
        }
        _adcFds[i] = fd;
    }
    
}

/*
 * Lesen der aktuellen Werte von den Analogeing�ngen
 */
static void readAdcValues()
{
    for( int i=0; i<NUM_ADCS; i++ )
    {
        char buffer[32];
        int count;
        /* Dateizeiger auf Dateianfang setzen */
        lseek( _adcFds[i], 0, SEEK_SET);
        if( (count = read( _adcFds[i], buffer, sizeof(buffer) )) < 1 )
        {
            perror("read");
            exit( -1 );
        }
        if( !count )
        {
            printf("ADC %d read error\n", i);
        }
        buffer[count] = 0;
        _adcValues[i] = strtol( buffer, NULL, 10 );
    }
}

/*
 * Ausgabe der Grafikdarstellung
 */
static void renderScreen()
{
	_primarySurface->SetFont( _primarySurface, _font );
    
    /* Schriftfarbe setzen */
	_primarySurface->SetColor( _primarySurface, 0xff, 0xff, 0xff, 0xff );

    /* �berschriftstexte ausgeben */
	_primarySurface->DrawString( _primarySurface, "Inputs", -1, 10, INPUT_RECTS_TOP - TEXT_HEIGHT - RECT_MARGIN, DSTF_TOPLEFT );
	_primarySurface->DrawString( _primarySurface, "Outputs", -1, 10, OUTPUT_RECTS_TOP - TEXT_HEIGHT - RECT_MARGIN, DSTF_TOPLEFT );
	_primarySurface->DrawString( _primarySurface, "ADCs", -1, 10, ADC_TOP - TEXT_HEIGHT - ADC_RECT_MARGIN, DSTF_TOPLEFT );

    /* Rechtecke zeichen f�r die Digitaleing�nge */    
    for( int i=0; i<NUM_INPUTS; i++ )
    {
        /* Farbauswahl, gr�n f�r aktiv, rot f�r nicht aktiv */
        if( _inputStates[i] )
        {
        	_primarySurface->SetColor( _primarySurface, 0x00, 0xff, 0x00, 0xff );
        }else{
        	_primarySurface->SetColor( _primarySurface, 0xff, 0x00, 0x00, 0xff );
        }
        /* Zeichnen eines gef�llten Rechtecks */
        _primarySurface->FillRectangle( _primarySurface, RECT_MARGIN + i*(RECT_SIZE + 2*RECT_MARGIN), INPUT_RECTS_TOP + RECT_MARGIN, RECT_SIZE, RECT_SIZE );
        /* Textausgabe der Nummer des Einganges */
        char buffer[4];
        sprintf(buffer, "%d", i);
    	_primarySurface->SetColor( _primarySurface, 0x00, 0x00, 0x00, 0xff );
    	_primarySurface->DrawString( _primarySurface, buffer, -1, RECT_MARGIN + RECT_SIZE/2 + i*(RECT_SIZE + 2*RECT_MARGIN), INPUT_RECTS_TOP + RECT_MARGIN + RECT_SIZE/2 - TEXT_HEIGHT/2, DSTF_TOPCENTER );
    }
    
    /* Rechtecke zeichen f�r die Digitalausg�nge */    
    for( int i=0; i<NUM_OUTPUTS; i++ )
    {
        /* Farbauswahl, gr�n f�r aktiv, rot f�r nicht aktiv */
        if( _outputStates[i] )
        {
        	_primarySurface->SetColor( _primarySurface, 0x00, 0xff, 0x00, 0xff );
        }else{
        	_primarySurface->SetColor( _primarySurface, 0xff, 0x00, 0x00, 0xff );
        }
        /* Zeichnen eines gef�llten Rechtecks */
        _primarySurface->FillRectangle( _primarySurface, RECT_MARGIN + i*(RECT_SIZE + 2*RECT_MARGIN), OUTPUT_RECTS_TOP + RECT_MARGIN, RECT_SIZE, RECT_SIZE );
        /* Textausgabe der Nummer des Ausganges, f�r den Piezo wird der Text "buzz" ausgegeben */
        char buffer[8];
        if( i<4 )
            sprintf(buffer, "%d", i);
        else
            strcpy( buffer, "buzz" );
    	_primarySurface->SetColor( _primarySurface, 0x00, 0x00, 0x00, 0xff );
    	_primarySurface->DrawString( _primarySurface, buffer, -1, RECT_MARGIN + RECT_SIZE/2 + i*(RECT_SIZE + 2*RECT_MARGIN), OUTPUT_RECTS_TOP + RECT_MARGIN + RECT_SIZE/2 - TEXT_HEIGHT/2, DSTF_TOPCENTER );
    }

    /* Aktuelle Werte der Analogeing�nge lesen */
    readAdcValues();

    /* Balken zeichen f�r die Analogeing�nge */    
    for( int i=0; i<NUM_ADCS; i++ )
    {
        /* Gr�nes Rechteck als Hintergrund */
       	_primarySurface->SetColor( _primarySurface, 0x00, 0xff, 0x00, 0xff );
        _primarySurface->FillRectangle( _primarySurface, ADC_RECT_MARGIN + i*(ADC_RECT_WIDTH + 2*ADC_RECT_MARGIN), ADC_TOP + ADC_RECT_MARGIN, ADC_RECT_WIDTH, ADC_RECT_HEIGHT );

        /* Darin rotes Rechteck mit Breite proportional zum Spannungswert */
       	_primarySurface->SetColor( _primarySurface, 0xff, 0x00, 0x00, 0xff );
        int width = (ADC_RECT_WIDTH - 2*ADC_RECT_INTERNAL_MARGIN) * _adcValues[i] / ADC_MAX_VALUE;
        _primarySurface->FillRectangle( _primarySurface, ADC_RECT_MARGIN + ADC_RECT_INTERNAL_MARGIN + i*(ADC_RECT_WIDTH + 2*ADC_RECT_MARGIN), ADC_TOP + ADC_RECT_MARGIN + ADC_RECT_INTERNAL_MARGIN, width, ADC_RECT_HEIGHT- 2*ADC_RECT_INTERNAL_MARGIN );
        
        /* Gemessene Spannung als Text mittig im Balkendiagramm ausgeben */
        char buffer[8];
        sprintf(buffer, "%.1f", double(_adcValues[i])/1000.);
    	_primarySurface->SetColor( _primarySurface, 0x00, 0x00, 0x00, 0xff );
    	_primarySurface->DrawString( _primarySurface, buffer, -1, ADC_RECT_MARGIN + ADC_RECT_WIDTH/2 + i*(ADC_RECT_WIDTH + 2*ADC_RECT_MARGIN), ADC_TOP + ADC_RECT_MARGIN + ADC_RECT_HEIGHT/2 - TEXT_HEIGHT/2, DSTF_TOPCENTER );
    }
    
    /* Die neue Grafikausgabe wurde in den nicht sichtbaren Hintergrundpuffer geschrieben. Hier werden die Puffer ausgetauscht */
	_primarySurface->Flip (_primarySurface, NULL, DSFLIP_WAITFORSYNC);
    
}
