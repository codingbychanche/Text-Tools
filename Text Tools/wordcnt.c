/*                                                                                           
 * wordcnt.c
 *
 * Filtert Wörter aus einer Textdatei und zählt, wie oft diese darin vorkommen.
 * Ausgabe ist eine Liste bestehend aus den gefunden Wörtern und deren Häufigkeit
 *
 * Wörter sind: Alle aus Groß- und Kleinbuchstaben bestehenden Zeichenketten die entweder
 * durch ein Leerzeichen, ein gültiges Satzzeichen (, . ; ) ein 'LF'= Line Feed, oder 
 * ein nicht gültiges Zeichen abgeschlossen werden.
 *                                                                                          
 * Version:
 *
 * V1.2 Beta B.F. 1.9.2014   
 *
 * Puplished:
 *
 * - 1.9.2014   V1.2 Beta
 *              Auf GitHub veröfentlicht
 *
 * - 23.5.2014  V1.1 Beta
 *              Einige Rechtschreibefehler in der Benutzerführung geändert
 *
 * - 3.5.2014   V1.0 Beta
 *              On my own Mac: /bin/wordcnt
 *              Kann jetzt von der Shell aus als Komando genutzt werden :-)
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX 255 // Maximale Länge eines Wortes

FILE *f;        // Eingabe- Datei


/*
 * Die Folgende Struktur enthält die gefundenen Wörter und
 * einen Zähler der angibt, wie oft diese in der Datei enthlten sind.
 *
 * Der Zeiger '*next' zeigt auf den Speicherplatz, unter dem die Folge-
 * struktur abgelegt ist. 
 */

struct tags 
{
    char tag [MAX];
    int count;
    struct tags *next;
};

struct tags *anfang=NULL;
struct tags *zeiger;

/*
 * Main
 */

int  main(int argc, const char * argv[])
{
    
    int i;              // Allgemeines
    int tags=0;         // Wort- Zähler
    
    char zeichen;       // Ein einzelnes Zeichen
    char tag[255];      // Ein gültiges Wort
    int tag_total;      // Soviele Wörter haben wir insgesammt gefunden
    
    if (argc<=1)
    {
        printf ("wordcnt V1.2 Beta B.F. 1.9.2014\n");
        printf ("Listet die Häufigkeit von Wörtern\n\n");
        printf ("Usage: wordcnt [Pfad]\n");
        return(0);
    }
    
    if ((f=fopen (argv[1],"r"))==NULL)
    {
        printf ("Fehler %s nicht gefunden\n",argv[1]);
        return (0);
    }
    
    printf ("Prüfe Datei: %s\n",argv[1]);
    
    
    /*
     * Hauptschleife
     *
     * Datei, zeichenweise lesen bis EOF
     */
    
    i=0;
    
    do
    { //0
        
        if (feof(f)) break;
        
        zeichen=fgetc (f);
        
        /*
         * Wenn das eben gelesene Zeichen ein gültiges Zeichen ist,
         * dann lese die Zeichenkette, da es sich dann möglicherweise
         * um ein Wort handelt :-)
         *
         * Nun lesen wir ab dem ersten Zeichen nach dem ersten gültigen Zeichen
         * bis ein Leerzeichen gefunden wurde, oder eine neue Zeile beginnt
         */
        
        if ((zeichen >='A') && (zeichen<='Z') ||
            (zeichen >='a') && (zeichen<='z'))   // Gültiges Zeichen
        { //1
            
            i=0;
            
            /*
             * Nun, so lange keines der unten gelisteten Zeichen auftaucht,
             * handelt es sich um ein gültiges Wort in einer beliebigen Sprache
             */
            
            while ((zeichen!=' ')   && 
                   (zeichen!='\n')  && 
                   (zeichen!='.')   &&
                   (zeichen!=',')   &&
                   (zeichen!=';')   &&
                   (zeichen!=':')   &&
                   (zeichen!='-')   &&
                   (zeichen!='(')   &&
                   (zeichen!=')')   &&
                   (zeichen!='=')   &&
                   (zeichen!='\t'))
                
            { //2
                
                /*
                 * Wenn das erste Zeichen ein Großbuchstaben ist, dann 
                 * wandeln wir den in einen Kleinbuchstaben
                 * Damit wäre dann die Vergleichbarkeit hergestellt
                 */
                
                if ((zeichen>='A') && (zeichen <='Z')) zeichen=zeichen+32;
                
                // Wort bilden
                
                tag[i]=zeichen;
                zeichen=fgetc(f);
                i++;
                
            } //2
            
            /*
             * Wir schließen den String ab, damit wir später
             * einige Funktionen aus der C- Standart Bibliothek
             * darauf anwenden können
             */
            
            tag[i]='\0';
            
            /*
             * Gibt es es das gefundene Wort schon in der Liste?
             * Falls ja, brauchen wir den da nicht mehr!
             */
            
            if ((unique(tag))!=1)
                
            { //3
                
                /* Nein!
                 * Das haben wir noch nicht, ab in die Liste!
                 */
                
                add(tag,1);
                tags++;
                
            } //3
            
            tag_total++;            // Eines gültiges Wort mehr!
            
        } //1
        
    } //0
    
    /*
     * So lange, bis EOF
     */
    
    while (1);
    
    /*
     * Und das Ergebnis ist....
     */
    
    fclose (f);
    printf ("%d unterschiedliche Wörter von insgesammt %d gefunden\n",tags,tag_total);
    
    ausgabe();
    
    return (0);
}

/*
 * Hänge an
 *
 * Wir arbeiten mit dynamischen Datenstrukturen und verketteten Listen Oo
 *
 */

add(char taged[],cnt)
{
    
    /*
     * Wenn Anfang leer, also NULL, dann lege das erste Elemnet der
     * Liste an
     *
     * Wenn malloc erfolgreich war, dann zeigt 'anfang' auf die Adresse
     * an der wir die erste Struktur der Liste finden.
     *
     * 'anfang' ist dann das erste mal initialisiert, wir haben genau ein
     * Element (Struktur des Typs 'tags') in der Liste.
     *
     */
    
    if (anfang==NULL)
    {
        if ((anfang=malloc(sizeof(struct tags)))==NULL)
        {
            printf ("Kein Speicher\n");
            return;
        }
        else 
        {
            anfang->count=cnt;
            strcpy (anfang->tag,taged);
            anfang->next=NULL;
        }
    }
    
    else
        
    /*
     * Es gibt mindestens ein Element in der Liste,
     * Füge also das neue Element an die besthende Liste am Ende
     * dazu
     *
     * Das funtioniert so: Da wir den Zeiger 'anfang' nicht verändern wollen,
     * wir möchten schließch immer wissen wo unsere Liste beginnt, retten
     * wir den zuerst einmal in einen zweiten zeiger, den wir 'zeiger' getauft haben.
     */
        
    {
        /*
         * Das letzte Element in der Liste suchen
         */
        
        zeiger=anfang;
        while (zeiger->next!=NULL) zeiger=zeiger->next;
        
        if ((zeiger->next=malloc(sizeof(struct tags)))==NULL) 
        {
            printf ("Kein Speicher");
            return;
        }
        
        /*
         * malloc war erfolgreich. Damit zeigt 'next' nun auf
         * das Folgende Elemnt der Liste
         */
        
        else
        {
            zeiger=zeiger->next;            // Zeiger auf den neuen Speicherplatz
            
            strcpy(zeiger->tag,taged);
            zeiger->count=cnt;
            zeiger->next=NULL;
        }
    }
    return;
}

/*
 * Prüfe ob ein Element bereits in der Liste 
 * enthalten ist
 *
 * Liefert eine 1 zurück, wenn der übergebene String in der Liste enthalten ist.
 * Ansonsten wir eine 0 geliefert.
 */

unique(char taged [])
{
    zeiger=anfang;
    
    /*
     * Wenn die Liste leer ist, dann mache nix
     */
    
    if (zeiger==NULL) return (0);
    
    /*
     * Die Liste enthält mindestens ein Element
     * dann prüfe das
     */
    
    if (zeiger->next==NULL)
    {
        if (strcmp (taged,zeiger->tag)==0) 
        {
            zeiger->count++;
            return (1);
        }
        else return(0);
    }
    
    /*
     * Es gibt mehr als ein Element in der Liste, also
     * durchsuche alle Elemente der Liste
     */
    
    while (zeiger!=NULL)
    {
        if (strcmp (taged,zeiger->tag)==0) 
        {
            zeiger->count++;
            return (1);
        }
        zeiger=zeiger->next;
    }
    
    return (0);
}

/*
 * Ausgabe
 */

ausgabe()
{
    zeiger=anfang;
    
    if (anfang== NULL)
    {
        printf ("Liste leer\n");
        return;
    }
    
    while (zeiger!=NULL)
    {
        printf ("%d %s\n",zeiger->count,zeiger->tag);
        zeiger=zeiger->next;
    }
    return ;
}



