/*                                                                                           
 * Erkennt mit '#' markierte Wörter oder einzelne Zeichen und zählt diese                    
 *                                                                                           
 * BF 1.5.2014                                                                               
 */

#include <stdio.h>
#include <stdlib.h>

int i,j;      // Allgemeines
int tags=0;   // Tag Zähler
FILE *f;      // Datei


/*
 * Strukturen
 */

struct tags 
{
    char tag [255];
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
    char zeichen;
    char tag[255];
    
    if (argc<=1)
    {
        printf ("Datei?\n");
        return(0);
    }
    
    if ((f=fopen (argv[1],"r"))==NULL)
    {
        printf ("Fehler %s nicht gefunden\n",argv[1]);
        return (0);
    }
    
    printf ("Prüfe das %s\n",argv[1]);
    
    /*
     * Hauptschleife
     *
     * Datei, zeichenweise lesen bis EOF
     */
    
    do
    { //0
        
        if (feof(f)) break;
        
        zeichen=fgetc (f);
        
        /*
         * Das #- Zeichen kennzeichnet, es handelt sich um einen Tag.
         *
         * Nun lesen wir ab dem ersten Zeichen nach dem '#' bis ein
         * Leerzeichen gefunden wurde, oder eine neue Zeile beginnt
         */
        
        if (zeichen=='#')
        { //1
            
            while ((zeichen!=' ') && (zeichen!='\n'))
            { //2
                zeichen=fgetc(f);
                tag[i]=zeichen;
                i++;
            } //2
            
            /*
             * Wir schließen den String ab, damit wir später
             * einige Funktionen aus der C- Standart Bibliothek
             * darauf anwenden können
             */
            
            i=i-1;
            tag[i]='\0';
            
            /*
             * Gibt es es gefundenen Hashtag schon in der Liste?
             * Falls ja, brauchen wir den da nicht mehr!
             */
            
            if ((unique(tag))!=1)
            { //3
                
                /* Nein!
                 * Den haben wir noch nicht, ab in die Liste!
                 */
                
                add(tag,1);
                tags++;
            } //3
            
            i=0;
            
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
    printf ("%d unique Hashtags gefunden\n",tags);
    
    ausgabe();
    
    return (0);
}

/*
 * Hänge an
 *
 * Wir arbeiten mit dynamischen Datenstrukturen und verketteten Listen Oo
 *
 */

add(taged[],cnt)
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
            printf ("Kein Speicher");
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
        printf ("%s, %x\n",zeiger->tag,zeiger->count);
        zeiger=zeiger->next;
    }
    return ;
}



