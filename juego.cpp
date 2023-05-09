
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

#define ANCHO 796
#define ALTURA 447

// Prototipos de las funciones:
void iniciar_menu();
int iniciar_juego();
void iniciar_pantalla();
void iniciar_sonidos();
void armar_pantalla();
void jugar();

// Objetos para la imagenes (Variables Global):
BITMAP *buffer; // Es como un mantel de mesa.
//BITMAP *logo;
BITMAP *panel;
BITMAP *fondo;
BITMAP *raton;
BITMAP *porta1;
BITMAP *porta2;
BITMAP *runner;

// Objetos referenciados para musica y efectos:
SAMPLE *musica_menu;
SAMPLE *musica_juego;

// Variables primitivas del juego:
int nivel=1;
int score=0;
int vidas=3;
bool fin = false;
bool elJuegoHaIniciado=false;
bool click=false;
bool enMenu=true; // Variable que indica cuando acaba el menú.

// Declarando objetos para fuentes:
DATAFILE *archivodat;
FONT *arialBold20;
FONT *arialReg20;
FONT *arialRounded20;

int main(){

    if( iniciar_juego() == 1 ){
        return 1;
    }

    iniciar_menu(); // Función que agrega el menú al inicio del juego.

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
    return 0;
}
END_OF_MAIN();

void iniciar_menu(){

    play_sample(musica_menu,200,150,1000,3);

    while(enMenu){

        if(mouse_x>618 && mouse_x<766 && mouse_y>198 && mouse_y<346){
            blit(porta2,buffer,0,0,0,0,796,447); // Fondo con circulo amarillo.
            if(mouse_b & 1){ // Botones del mouse. 1(izquierdo) y 2(derecho)
                enMenu=false;
            }
        }else{
            blit(porta1,buffer,0,0,0,0,796,447); // Fondo con circulo blanco.
        }

        // Función que vuelve transparente el color rosa.
        masked_blit(raton,buffer,0,0,mouse_x,mouse_y,30,30);
        blit(buffer,screen,0,0,0,0,ANCHO,ALTURA);

    }

    stop_sample(musica_menu);

}

// Definciones de funciones:
int iniciar_juego(){

    allegro_init(); // Decimos que vamos a usar Allegro.
    install_keyboard(); // Para poder usar el teclado.
    install_mouse(); // Iniciar el mouse.

    if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) != 0){
        allegro_message("\n\t Error al iniciar sonido... ",allegro_error);
        return 1;
    }

    // Inicializando archivo dat y fuentes.
    archivodat=load_datafile("fuentesdat.dat");
    // Queda pendiente revisar.
    arialRounded20=(FONT*)archivodat[2].dat;
    arialReg20=(FONT*)archivodat[1].dat;
    arialBold20=(FONT*)archivodat[0].dat;

    iniciar_pantalla();
    iniciar_sonidos();
    // TODO: iniciar sonido
    return 0;

}

void iniciar_pantalla(){

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT,ANCHO,ALTURA,0,0);

    // Incializando objetos:
    buffer = create_bitmap(ANCHO,ALTURA);

    //logo = load_bitmap("images/logo.bmp",NULL);
    panel = load_bitmap("images/panel.bmp",NULL);
    //recuadro = load_bitmap("images/recuadro.bmp",NULL);
    fondo = load_bitmap("images/publico.bmp",NULL);
    raton = load_bitmap("images/cursor.bmp",NULL);

    // Cargo imagenes del ratón y del mouse:
    raton = load_bitmap("images/cursor2.bmp",NULL);
    porta1 = load_bitmap("images/porta1.bmp",NULL);
    porta2 = load_bitmap("images/porta2.bmp",NULL);

    runner = load_bitmap("images/runner1.bmp",NULL); // Corredor.

}

void iniciar_sonidos(){

    musica_menu = load_sample("sounds/diamond_race.wav"); // probar
    musica_juego = load_sample("sounds/night_runner.wav");

}

void armar_pantalla(){

    // Cada que hay una vista diferente de la pantalla
    clear_to_color(buffer,BLACK);

    // Ponemos las imagenes sobre el buffer (bitmap)
    //draw_sprite(buffer,logo,610,5);
    draw_sprite(buffer,fondo,16,16);
    draw_sprite(buffer,panel,16,300);

    // Una vez dibujado agregado el panel al buffer
    // Agregamos las fuentes:

    textprintf_ex(panel,arialBold20,146,12,makecol(0,0,0),makecol(0,0,0),"    ");
    textprintf_ex(panel,arialBold20,146,12,makecol(255,0,0),makecol(0,0,0),"%d",nivel);

    textprintf_ex(panel,arialBold20,156,52,makecol(0,0,0),makecol(0,0,0),"    ");
    textprintf_ex(panel,arialBold20,156,52,makecol(255,0,0),makecol(0,0,0),"%d",score);

    textprintf_ex(panel,arialBold20,146,92,makecol(0,0,0),makecol(0,0,0),"    ");
    textprintf_ex(panel,arialBold20,146,92,makecol(255,0,0),makecol(0,0,0),"%d",vidas);

    //draw_sprite(buffer,recuadro,16,12);


    // Colocamos el buffer sobre la pantalla.
    blit(buffer,screen,0,0,0,0,ANCHO,ALTURA);

}

void jugar(){

    play_sample(musica_juego,200,150,1000,3);

    draw_sprite(fondo,runner,33,132);

}
