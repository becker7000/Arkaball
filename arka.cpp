#include<allegro.h>
#include<string>
#include<conio.h>
#include<sstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<winalleg.h>
#include<fstream>

using namespace std;

#define ancho 1024
#define alto 740

// Definiciones de funetes:


// Prototipos de las funciones.
int inicializo();
void inicializo_pantalla();
void inicializo_sonidos();
void armo_pantalla();
void jugar();

void inicializo_nivel();
void chequeo_base(); // Checa la base
void muestro_ladrillos();
void muevo_bola();
void chequeo_teclas_sonido();
void retoma_juego();

void configura_nivel();

// Objetos de Allegro necesarios para el juego:

// Allegro soportar archivos de audio .wav
// Objetos para música y efectos de sonido.
MIDI *musica_Inicio;
MIDI *musica_Juego;
SAMPLE *sonido_InicioJuego;
SAMPLE *sonido_InicioNivel;
SAMPLE *sonido_LadrilloRoto;
SAMPLE *sonido_RebotePelota; // Pendiente.
SAMPLE *sonido_Revivir;
SAMPLE *sonido_VidaExtra; // Para cuando queremos premiar al jugador. (No se usará)
SAMPLE *sonido_RebotaParedes;
SAMPLE *sonido_RebotaBase; // Para cuando rebota en la base.
SAMPLE *sonido_VidaPerdida;
SAMPLE *sonido_GameOver;

// Objetos para imagenes:
BITMAP *buffer;
BITMAP *logo;
BITMAP *panel;
BITMAP *recuadro;
BITMAP *fondo1;
BITMAP *fondo2;
BITMAP *fondo3;
BITMAP *fondo4;
BITMAP *fondo5;
BITMAP *gameover;
BITMAP *lad1;
BITMAP *lad2;
BITMAP *lad3;
BITMAP *lad4;
BITMAP *lad5;
BITMAP *lad6;
BITMAP *lad7;
BITMAP *ladd; // Ladrillo especial
BITMAP *base;
BITMAP *base2;
BITMAP *base3;
BITMAP *base4;

// Variables necesarias para crear el juego:
int retardo=100;
int vidas=3;
int level=1;
int score=0;
bool juegoIniciado=false; // La pelota no se mueve hasta iniciar
bool fin=false; // Cuando ya termina el juego con la tecla esc
bool nuevoNivel=false;
bool enJuego=false;
int dirY=1;
int dirX=-1;
int velocidad=3; // Velocidad de la pelota (en pixeles)
int velocidadIncial=3;  // En realidad es una constante
int fondoN=1; // Nos va a permitir cambiar los fondos de pantalla.
bool muerte=false; // Se ejecuta un sonido, se resta 1 en las vidas, etc...
int suenciaMuerte=1; // Nos va cambiando la imagen de la base, se va viendo más destruida cada vez.
bool musica=true;
bool efectos=true;
bool existeArchivo=false;
int highScore=0;

int baseX=255; // Coordenadas de la base, sólo X porque va a ser fija en Y.
int bolaX=295;
int bolaY=650;

int pant1[63]={1,1,1,1,1,1,1,1,1,
               2,2,2,2,2,2,2,2,2,
               3,3,3,3,3,3,3,3,3,
               4,4,4,4,4,4,4,4,4,
               5,5,5,5,5,5,5,5,5,
               6,6,6,6,6,6,6,6,6,
               7,7,7,7,7,7,7,7,7};

int pant2[63]={1,2,3,4,5,6,7,6,5,
               1,2,3,4,5,6,7,6,5,
               1,2,3,4,5,6,7,6,5,
               1,2,3,4,5,6,7,6,5,
               1,2,3,4,5,6,7,6,5,
               1,2,3,4,5,6,7,6,5,
               1,2,3,4,5,6,7,6,5,};

// Arreglo auxiliar para modificarlos según el nivel:
int mapa[63]; // Mapa de ladrillos

// Es para delimitar nuestra área de movimiento.
int puntaIzq;
int puntaDer;

int colBola; // Movimiento en bola.
int filaBola;
int elemento; // Pendiente

// Arreglo que guarda los pixeles de los ladrillos.
int fila[]={20,50,80,110,140,170,200};

// Declarando las fuentes:
DATAFILE *datfile;
FONT *arialBD;
FONT *arial20;
FONT *orc_a_ex18;

int main(){

    if(inicializo() == 1){
        return 1;
    }

    while(!fin){ // Mientras no sea verdadero el fin.

        // Creamos nuestra pantalla:
        armo_pantalla();

        if(key[KEY_ESC]){ // El juego termina si se preciona ESC
            fin=true;
        }
        if(key[KEY_ENTER] && juegoIniciado==false){
            jugar();
        }

    }

    cout<<"\n\t Fin del juego"<<endl;

}
END_OF_MAIN();

int inicializo(){
    allegro_init(); // Inicializa allegro
    install_keyboard(); // Se instala el teclado
    if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) != 0){
        allegro_message("\n\t Error al iniciar el sonido ",allegro_error);
        return 1;
    }
    datfile=load_datafile("fontsdat.dat");
    orc_a_ex18=(FONT*)datfile[2].dat;
    arialBD=(FONT*)datfile[1].dat; // Traemos los elementos desde el primero
    arial20=(FONT*)datfile[0].dat;

    inicializo_pantalla();
    inicializo_sonidos();
    return 0;
}

void inicializo_pantalla(){
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT,ancho,alto,0,0); //
    buffer=create_bitmap(ancho,alto); // Es como un panel para no ver parpadeos.
    logo=load_bitmap("images/logo.bmp",NULL);
    panel=load_bitmap("images/panel.bmp",NULL);
    recuadro=load_bitmap("images/recuadro.bmp",NULL);
    fondo1=load_bitmap("images/fondo1.bmp",NULL);
    fondo2=load_bitmap("images/fondo2.bmp",NULL);
    fondo3=load_bitmap("images/fondo3.bmp",NULL);
    fondo4=load_bitmap("images/fondo4.bmp",NULL);
    fondo5=load_bitmap("images/fondo5.bmp",NULL);
    lad1=load_bitmap("images/ladrillo1.bmp",NULL);
    lad2=load_bitmap("images/ladrillo2.bmp",NULL);
    lad3=load_bitmap("images/ladrillo3.bmp",NULL);
    lad4=load_bitmap("images/ladrillo4.bmp",NULL);
    lad5=load_bitmap("images/ladrillo5.bmp",NULL);
    lad6=load_bitmap("images/ladrillo6.bmp",NULL);
    lad7=load_bitmap("images/ladrillo7.bmp",NULL);
    ladd=load_bitmap("images/ladrilloduro.bmp",NULL);
    gameover=load_bitmap("images/gameover.bmp",NULL);
    base=load_bitmap("images/base.bmp",NULL);
    base2=load_bitmap("images/base2.bmp",NULL);
    base3=load_bitmap("images/base3.bmp",NULL);
    base4=load_bitmap("images/base4 .bmp",NULL);
}

void inicializo_sonidos(){
    set_volume(230,209);
    // Los midis se pueden ejecutar dentro de un ciclo, varias veces
    // los .wav no, se ejecutan y terminan.
    musica_Inicio=load_midi("sounds/ark.mid");
    musica_Juego=load_midi("sounds/Arkanoid.mid");
    sonido_InicioJuego=load_wav("sounds/InicioJuego.wav");
    sonido_InicioNivel=load_wav("sounds/InicioNivel.wav");
    sonido_LadrilloRoto=load_wav("sounds/LadrilloRoto.wav");
    sonido_RebotePelota=load_wav("sounds/RebotePelota.wav");
    sonido_VidaExtra=load_wav("sounds/VidaExtra.wav");
    sonido_RebotaParedes=load_wav("sounds/rebotaParedes.wav");
    sonido_RebotaBase=load_wav("sounds/reboteBase.wav");
    sonido_VidaPerdida=load_wav("sounds/fallo.wav");
    sonido_GameOver=load_wav("sounds/game-over.wav");
}

void armo_pantalla(){
    clear_to_color(buffer,0x000000);
    draw_sprite(buffer,logo,610,5);
    draw_sprite(buffer,panel,620,140); // Ya que está creado el panel le agregamos la fuentes.

    textprintf_ex(panel,arialBD,130,3,makecol(0,0,0),makecol(0,0,0),"              ");  // Ahora agregamos texto encima del panel. Coordenadas relativas al panel.
    textprintf_ex(panel,arialBD,130,3,makecol(255,0,0),makecol(0,0,0),"%d",level);

    textprintf_ex(panel,arialBD,160,65,makecol(0,0,0),makecol(0,0,0),"              ");  // Ahora agregamos texto encima del panel. Coordenadas relativas al panel.
    textprintf_ex(panel,arialBD,160,65,makecol(255,0,0),makecol(0,0,0),"%d",score);

    textprintf_ex(panel,arialBD,130,130,makecol(0,0,0),makecol(0,0,0),"              ");  // Ahora agregamos texto encima del panel. Coordenadas relativas al panel.
    textprintf_ex(panel,arialBD,130,130,makecol(255,0,0),makecol(0,0,0),"%d",vidas);

    draw_sprite(buffer,recuadro,5,10);
    draw_sprite(buffer,fondo1,11,16); // Arriba del fondo va la base por eso va después:
    draw_sprite(buffer,base,baseX,660);
    circlefill(buffer,bolaX,bolaY,10,makecol(124,250,16));
    // Enviamos el buffer a la pantalla:
    blit(buffer,screen,0,0,0,0,ancho,alto);
}

void jugar(){

    level=1;
    fin=false;

    while(!key[KEY_ESC] && !fin){

        inicializo_nivel(); // Se inicia un nuevo nivel.

        // Se revisa si hay un nuevo nivel, si se no se ha presionado ESC y que las vidas sean mayores que 0.
        while(!nuevoNivel && !key[KEY_ESC] && vidas>0){

            if(key[KEY_SPACE] && enJuego==false){

                // Aquí se va a detener la musica inicial.
                enJuego=true;

            }

            chequeo_base(); // Checa el movimiento

            if(enJuego){
                muestro_ladrillos();
                muevo_bola();
            }

            chequeo_teclas_sonido();
            armo_pantalla();

            if(vidas==0){ // El juego termina cuando las vidas son cero.
                fin=true;
            }

        }

    }

}


void inicializo_nivel(){
    configura_nivel();
    retoma_juego();
    if(efectos){
        play_sample(sonido_InicioNivel,200,150,1000,0);
    }
}

void chequeo_base(){

}

void muestro_ladrillos(){
    int x,y,col;
    int ladrillo_n=0;
    int lad;
    int fila[7]={20,50,80,110,140,170,200};
    for(int i=0;i<63;i++){

    }
}

void muevo_bola(){

}

void chequeo_teclas_sonido(){

    // Si se oprime la tecla DEL se pausa la musica o se reproduce...
    if(key[KEY_DEL]){
        if(musica){
            musica=false;
            midi_pause();
        }else{
            midi_resume();
        }
    }

    // _Los efectos de habilitan o se deshabilitan
    if(key[KEY_TAB]){
        if(efectos){
            efectos=false;
        }else{
            efectos=true;
        }
    }

}

void retoma_juego(){
    baseX=255;
    bolaX=285;
    bolaY=650;
    enJuego=false;
    nuevoNivel=false;
    armo_pantalla();
}

void configura_nivel(){

    // Se configura el arreglo mapa que es el que va a tener los identificadores de ladrillos.
    for(int i=0;i<63;i++){
        if(level==1){
            mapa[i]=pant1[i];
        }
        if(level==2){
            mapa[i]=pant2[i];
        }
        if(level>2){
            mapa[i]=rand()%9; // Aleatorios de 0 a 8
        }
    }

}


