
#include<allegro.h> // Librería principal de allegro
#include<string> // Librería para cadenas
#include<conio.h> // Entradas y salidas de consola.
#include<sstream> // Uso de la pantalla.
#include<iostream> // Input y output de la pantalla.
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<winalleg.h>
#include<fstream>

using namespace std;

// TODO: Crear una paleta de colores (max 5 o 6).
#define BLACK 0x000000

#define ANCHO 1024
#define ALTURA 740

// Prototipos de las funciones:
int iniciar_juego();
void iniciar_pantalla();
void armar_pantalla();
void jugar();

// Objetos para la imagenes (Variables Global):
BITMAP *buffer; // Es como un mantel de mesa.
BITMAP *logo;
BITMAP *panel;
BITMAP *recuadro;
BITMAP *fondo;

// Variables primitivas del juego:
bool fin = false;
bool elJuegoHaIniciado=false;

int main(){

    if( iniciar_juego() == 1 ){
        return 1;
    }

    // Sí todo sale bien, inicia el juego.
    while(!fin){ // Mientras no sea verdadero el fin.

        // Creamos nuestra pantalla.
        armar_pantalla();

        if(key[KEY_ESC]){
            fin=true;
        }

        if(key[KEY_ENTER] && elJuegoHaIniciado==false){
            jugar();
        }

    }

    cout<<"\n\t El juego ha terminado "<<endl;

}
END_OF_MAIN();

// Definciones de funciones:
int iniciar_juego(){

    allegro_init(); // Decimos que vamos a usar Allegro.
    install_keyboard(); // Para poder usar el teclado.
    // install_mouse();
    if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) != 0){
        allegro_message("\n\t Error al iniciar sonido... ",allegro_error);
        return 1;
    }

    iniciar_pantalla();
    // TODO: iniciar sonido
    return 0;

}

void iniciar_pantalla(){

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT,ANCHO,ALTURA,0,0);

    // Incializando objetos:
    buffer = create_bitmap(ANCHO,ALTURA);
    logo = load_bitmap("images/logo.bmp",NULL);
    panel = load_bitmap("images/panel.bmp",NULL);
    recuadro = load_bitmap("images/recuadro.bmp",NULL);
    fondo = load_bitmap("images/fondo1.bmp",NULL);

}

void armar_pantalla(){

    // Cada que hay una vista diferente de la pantalla
    clear_to_color(buffer,BLACK);

    // Ponemos las imagenes sobre el buffer (bitmap)
    draw_sprite(buffer,logo,610,5);
    draw_sprite(buffer,panel,620,140);
    draw_sprite(buffer,recuadro,5,10);
    draw_sprite(buffer,fondo,11,16);

    // Colocamos el buffer sobre la pantalla.
    blit(buffer,screen,0,0,0,0,ANCHO,ALTURA);

}

void jugar(){

}
