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

// Prototipos de las funciones.
int inicializo();
void inicializo_pantalla();
void inicializo_sonidos();
void armo_pantalla();
void jugar();

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

int mapa[63];

// Es para delimitar nuestra área de movimiento.
int puntaIzq;
int puntaDer;

int colBola; // Movimiento en bola.
int filaBola;
int elemento; // Pendiente

// Arreglo que guarda los pixeles de los ladrillos.
int fila[]={20,50,80,110,140,170,200};

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
    draw_sprite(buffer,panel,620,140);
    draw_sprite(buffer,recuadro,5,10);
    draw_sprite(buffer,fondo1,11,16); // Arriba del fondo va la base por eso va después:
    draw_sprite(buffer,base,baseX,660);
    circlefill(buffer,bolaX,bolaY,10,makecol(124,250,16));
    // Enviamos el buffer a la pantalla:
    blit(buffer,screen,0,0,0,0,ancho,alto);
}

void jugar(){
    // TODO: programar como jugar...
}


