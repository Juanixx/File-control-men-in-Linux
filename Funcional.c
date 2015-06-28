#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include <fts.h>
#include <sys/types.h>

WINDOW *win;

void IniVideo(){
win=initscr();
clear();
refresh();
cbreak();
keypad(win, TRUE);
}

void Exit(){
refresh();
endwin();
system("clear");
exit(1);
}

void Menu(){
printw("\n");
printw("\n1.- Mostrar árbol de directorio\n");
printw("2.- Agregar directorio\n");
printw("3.- Modificar directorio\n");
printw("4.- Eliminar directorio\n");
printw("5.- Agregar archivo\n");
printw("6.- Modificar archivo\n");
printw("7.- Eliminar archivo\n");
printw("8.- Salir\n");
}

int ERecursivo(const char *dir)
{
    int ret = 0;
    FTS *ftsp = NULL;
    FTSENT *curr;

    char *files[] = { (char *) dir, NULL };
    ftsp = fts_open(files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
    if (!ftsp) {
        //fprintf(stderr, "%s: fts_open failed: %s\n", dir, strerror(errno));
        ret = -1;
        goto finish;
    }

    while ((curr = fts_read(ftsp))) {
        switch (curr->fts_info) {
        case FTS_NS:
        case FTS_DNR:
        case FTS_ERR:
            //fprintf(stderr, "%s: fts_read error: %s\n",
                    //curr->fts_accpath, strerror(curr->fts_errno));
            break;

        case FTS_DC:
        case FTS_DOT:
        case FTS_NSOK:
            break;

        case FTS_D:
            // Do nothing. Need depth-first search, so directories are deleted
            // in FTS_DP
            break;

        case FTS_DP:
        case FTS_F:
        case FTS_SL:
        case FTS_SLNONE:
        case FTS_DEFAULT:
            if (remove(curr->fts_accpath) < 0) {
               // fprintf(stderr, "%s: Failed to remove: %s\n",
               //         curr->fts_path, strerror(errno));
                ret = -1;
            }
            break;
        }
    }

finish:
    if (ftsp) {
        fts_close(ftsp);
    }

    return ret;
}
main(){

int ch;


IniVideo();

for(;;){
char dir[256];
char mod[256];
char sis[256]="mv ";
char rem[256]="rm -r ";
char toch[256]="touch ";
char remarch[256]="rm ";
char vacio[256]="rmdir ";
char crea[256]="mkdir ";
char a[256]="/root/Escritorio/Juanix";
char b[256]="/root/Escritorio/Juanix/";

Menu();
ch=getch();
IniVideo();
switch(ch){

case 49:
endwin();
int temp=0;
if(temp!=10){
system("clear");
printf("El árbol de la carpeta conteniendo todos los archivos es: \n");
system("tree\n");
temp=getch();
}
break;

case 50:
initscr();
printw("Escriba el nombre del directorio a agregar\n");
refresh();
scanw("%s",&dir);
strcat(crea,dir);
system(crea);
break;

case 51:
printw("Escriba el nombre del directorio a modificar y presione enter\n");
refresh();
scanw("%s",&dir);
if(strcmp(dir,a)==0||strcmp(dir,b)==0)
printw("No se puede modificar el directorio raíz\n");
else{

printw("Escriba el nuevo nombre del directorio y presione enter\n");
refresh();
scanw("%s",&mod);
strcat(dir," ");
strcat(dir,mod);
strcat(sis,dir);
if(system(sis)==0)
printw("Ha cambiado el nombre a la carpeta");
else{
clear();
printw("No se cambio el nombre debido a un error de sintaxis");
}
}

break;

case 52:
endwin();
printw("Escriba el nombre del directorio a eliminar\n");
refresh();
scanw("%s",&dir);
if(strcmp(dir,a)==0||strcmp(dir,b)==0)
printw("No se puede eliminar el directorio raíz\n");
else{

strcat(vacio,dir);
if(system(vacio)==0){
system("clear");
printw("El directorio se ha eliminado ya que estaba vacío");
}
else{
system("clear");
printw("El directorio no esta vacio\n");
printw("\n¿Esta seguro que desea eliminarlo?\n");
printw("1.- Si\n");
printw("2.- No\n");
int opc=getch();

if(opc==49){
clear();
strcat(rem,dir);
ERecursivo(dir);
printw("Se ha eliminado el directorio");
refresh();}
else
if(opc==50){
clear();
printw("El directorio no se ha eliminado");
refresh();

}
}}

break;

case 53:
printw("Escriba el nombre del archivo a agregar junto con su extensión si lo desea\n");
refresh();
scanw("%s",&dir);
strcat(toch,dir);
system(toch);
break;

case 54:
printw("Escriba el nombre del archivo a modificar y presione enter\n");
refresh();
scanw("%s",&dir);
if(strcmp(dir,a)==0||strcmp(dir,b)==0)
printw("No se puede eliminar\n");
else{
printw("\nEscriba el nuevo nombre del archivo y presione enter\n");
refresh();
scanw("%s",&mod);
strcat(dir," ");
strcat(dir,mod);
strcat(sis,dir);
system(sis);
}
break;

case 55:
printw("Escriba el nombre del archivo a eliminar\n");
refresh();
scanw("%s",&dir);
strcat(remarch,dir);
if(system(remarch)!=0){
clear();
printw("No se puede eliminar ya que es un directorio");}
else
printw("El archivo se ha eliminado");
break;

case 56:
Exit();
break;



}

}
}
