#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <windows.h>
#define ANCHO 450
#define ALTO 720
#define ESCALACUERVO 0.2
#define TAMANYOCUERVOX 512
#define TAMANYOCUERVOY 512
#define TAMANYOESPINASY 39
#define ANCHOESPINAS 150
#define VELOCIDADENX 3.5
#define ANCHORCT 290
#define ALTORCT 140

int numeroAleatorio(int minimo, int maximo){
    srand(time(NULL));
    return minimo + rand() % (maximo + 1 - minimo);
}

std::string enteroaString(int n){
    std::string s;
    char digito[] = "0123456789";

    do{
        s = digito[n % 10] + s;
    }while ((n /= 10) > 0);

    return s;
}

using namespace sf;

// Declaracion de variables a utilizar //
enum{ESPINASABAJO, ESPINASARRIBA, ESPINASIZQIERDA, ESPINASDERECHA};
RenderWindow window(VideoMode(ANCHO, ALTO), "El primero GEEK!");
Texture cuervo, fondo, espinas, corazon, cursor, parallax[4], boton[3], geek;
Sprite sprCuervo, sprFondo, sprEspinas[4], sprCorazon[3], sprCursor, sprParallax[4], sprBoton[3], sprGeek;
Vector2i v2iSeleccionFrame;
Clock reloj, relojColision, relojEventos;
Font fntTipoLetra;
Text txtPuntuacion, txtSaltos;
int direccion = 0, auxiliar = 0, puntuacion = 0, saltos = 0, vida = 3;
float tempy = 0, y = ALTO/2.f, x = ANCHO/2.f, tempx = VELOCIDADENX;
bool gameOver = false, empezar = false;

// Declaracion de funciones //
void setearVariables();
void menu();
void gameLoop();
void actualizar();
void dibujar();
void animarCuervo(Sprite *_sprCuervo, Vector2i frame);

int main(){

    setearVariables();
    menu();
    gameLoop();

    return EXIT_SUCCESS;
}

void setearVariables(){

    window.setFramerateLimit(60);

	cuervo.loadFromFile("Cuervo/Cuervo.png");
	fondo.loadFromFile("Fondo/Fondo.png");
    espinas.loadFromFile("Espinas/Espinas.png");
    corazon.loadFromFile("Corazon/heart.png");
    cursor.loadFromFile("Cursor/Rojo.png");
    geek.loadFromFile("Geek/Geek.png");
    for(int i = 0; i < 3; i++){
        std::string s = "Boton/";
        s += char(49 + i);
        boton[i].loadFromFile(s + ".png");
    }
    for(int i = 0; i < 4; i++){
        std::string s = "Parallax/";
        s += char(49 + i);
        parallax[i].loadFromFile(s + ".png");
        parallax[i].setRepeated(true);
    }

    sprCuervo.setTexture(cuervo);
	v2iSeleccionFrame.x = 0;
	v2iSeleccionFrame.y = 0;
	animarCuervo(&sprCuervo, v2iSeleccionFrame);
	sprCuervo.scale(ESCALACUERVO, ESCALACUERVO);
    sprCuervo.setPosition((ANCHO/2.f) - (256 * ESCALACUERVO), (ALTO/2.f) - (256 * ESCALACUERVO));

    sprFondo.setTexture(fondo);
    sprFondo.setOrigin(ANCHO/2.f, ALTO/2.f);
    sprFondo.setPosition(ANCHO/2.f, ALTO/2.f);

    sprEspinas[ESPINASABAJO].setTexture(espinas);
    sprEspinas[ESPINASABAJO].setOrigin(ANCHO/2.f, TAMANYOESPINASY/2.f);
    sprEspinas[ESPINASABAJO].setPosition(ANCHO/2.f, ALTO-(TAMANYOESPINASY/2.f));

    sprEspinas[ESPINASARRIBA].setTexture(espinas);
    sprEspinas[ESPINASARRIBA].setOrigin(ANCHO/2.f, TAMANYOESPINASY/2.f);
    sprEspinas[ESPINASARRIBA].setPosition(ANCHO/2.f, TAMANYOESPINASY/2.f);
    sprEspinas[ESPINASARRIBA].setRotation(180.f);

    IntRect rctEspinas(0, 0, ANCHOESPINAS, TAMANYOESPINASY);
    sprEspinas[ESPINASIZQIERDA].setTexture(espinas);
    sprEspinas[ESPINASIZQIERDA].setTextureRect(rctEspinas);
    sprEspinas[ESPINASIZQIERDA].setOrigin(ANCHOESPINAS/2.f, TAMANYOESPINASY/2.f);
    sprEspinas[ESPINASIZQIERDA].setRotation(90.f);
    sprEspinas[ESPINASIZQIERDA].setPosition(TAMANYOESPINASY/2.f, ALTO/2.f);

    sprEspinas[ESPINASDERECHA].setTexture(espinas);
    sprEspinas[ESPINASDERECHA].setTextureRect(rctEspinas);
    sprEspinas[ESPINASDERECHA].setOrigin(ANCHOESPINAS/2.f, TAMANYOESPINASY/2.f);
    sprEspinas[ESPINASDERECHA].setRotation(270.f);
    sprEspinas[ESPINASDERECHA].setPosition(ANCHO-(TAMANYOESPINASY/2.f), ALTO/2.f);

    sprCorazon[0].setTexture(corazon);
    sprCorazon[0].scale(0.2, 0.2);
    sprCorazon[0].setPosition((ANCHO / 13) * 5, (ALTO / 34) * 2);

    sprCorazon[1].setTexture(corazon);
    sprCorazon[1].scale(0.2, 0.2);
    sprCorazon[1].setPosition((ANCHO / 13) * 6, (ALTO / 34) * 2);

    sprCorazon[2].setTexture(corazon);
    sprCorazon[2].scale(0.2, 0.2);
    sprCorazon[2].setPosition((ANCHO / 13) * 7, (ALTO / 34) * 2);

    sprCursor.setTexture(cursor);
    sprCursor.scale(0.6, 0.6);
    sprCursor.setPosition(ANCHO/2, ALTO/2);

    sprGeek.setTexture(geek);
    sprGeek.setOrigin(geek.getSize().x/2, geek.getSize().y);
    sprGeek.setScale(0.4, 0.4);
    sprGeek.setPosition(ANCHO/2, 150);

    for(int i = 0; i < 3; i++){
        sprBoton[i].setTexture(boton[i]);
        sprBoton[i].setOrigin(boton[i].getSize().x/2, boton[i].getSize().y/2);
    }

    sprBoton[0].setPosition(ANCHO/2, ALTO/2);
    sprBoton[1].setPosition(ANCHO/2, (ALTO/4) + (((ALTO/2) - (ALTO/4)) / 2));
    sprBoton[2].setPosition(ANCHO/2, ALTO/4);

    for(int i = 0; i < 4; i++)
        sprParallax[i].setTexture(parallax[i]);

    fntTipoLetra.loadFromFile("Fuente/itlivesintheswamp.ttf");

    txtPuntuacion.setFont(fntTipoLetra);
    txtPuntuacion.setPosition((ANCHO / 5) * 4, (ALTO / 34));
    txtPuntuacion.setCharacterSize(60);
    txtPuntuacion.setFillColor(Color::White);
    txtPuntuacion.setString(enteroaString(0));

    txtSaltos.setFont(fntTipoLetra);
    txtSaltos.setPosition(60, (ALTO / 34));
    txtSaltos.setCharacterSize(60);
    txtSaltos.setFillColor(Color::White);
    txtSaltos.setString(enteroaString(0));

}

void menu(){

    float contador[4] = {0, 0, 0};
    bool ok = false;
    Vector2i posicionMouse = Mouse::getPosition(window);
    sprCuervo.setScale(0.4, 0.4);

    window.setMouseCursorVisible(false);

    do{

        Event e;
        while(window.pollEvent(e)){
            if (e.type == Event::Closed){
                window.close();
                exit(EXIT_SUCCESS);
            }
        }

        if(Mouse::isButtonPressed(Mouse::Left)){
            if(sprCursor.getGlobalBounds().intersects(sprBoton[0].getGlobalBounds()))
                ok = true;
            if(sprCursor.getGlobalBounds().intersects(sprBoton[1].getGlobalBounds())){
                system("START /max iexplore.exe https://www.facebook.com/Juegos-Geek-1890472260968691");
            }
            if(sprCursor.getGlobalBounds().intersects(sprBoton[2].getGlobalBounds())){
                system("START /max iexplore.exe paypal.me/LMarquezMendez");
            }
        }

        posicionMouse = Mouse::getPosition(window);
        sprCursor.setPosition(Vector2f(posicionMouse));

        IntRect rctRectangulo1(contador[0], 0, ANCHO, ALTO), rctRectangulo2(contador[1], 0, ANCHO, ALTO), rctRectangulo3(contador[2], 0, ANCHO, ALTO);
        sprParallax[0].setTextureRect(rctRectangulo1);
        sprParallax[1].setTextureRect(rctRectangulo2);
        sprParallax[2].setTextureRect(rctRectangulo3);

        contador[0] += 1;
        contador[1] += 0.5;
        contador[2] += 0.1;

    sprCuervo.setPosition((ANCHO/2) -256 * 0.4, ALTO - 250);
    Time tiempo;
    tiempo = reloj.getElapsedTime();
    if(tiempo.asMilliseconds() >= 0){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 50){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 100){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 150){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 200){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 250){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 300){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 350){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 400){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 450){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 500){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 550){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 600){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 650){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 700){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 750){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 800){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 850){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 900){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 950){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1000){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1050){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1100){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1150){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1200){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 0;
        reloj.restart();
    }
    animarCuervo(&sprCuervo, v2iSeleccionFrame);

        window.clear(Color::Black);
        for(int i = 3; i >= 0; i--)
            window.draw(sprParallax[i]);
        for(int i = 0; i < 3; i++)
            window.draw(sprBoton[i]);
        window.draw(sprGeek);
        window.draw(sprCuervo);
        window.draw(sprCursor);
        window.display();
    }while(!ok);

    sprCuervo.setScale(ESCALACUERVO, ESCALACUERVO);
    sprCuervo.setOrigin(TAMANYOCUERVOX / 2, TAMANYOCUERVOY / 2);
    sprCuervo.setPosition(ANCHO / 2, ALTO / 2);

}

void gameLoop(){
    while (!gameOver){

        Event e;

        while(window.pollEvent(e)){

            if (e.type == Event::Closed){
                window.close();
                exit(EXIT_SUCCESS);
            }

            if(e.key.code == Keyboard::Space){
                Time tiempoEventos = relojEventos.getElapsedTime();
                if(tiempoEventos.asMilliseconds() >= 180){ // Cada 150 milisegundos detecta la tecla espacio
                    saltos++;
                    relojEventos.restart();
                }
                empezar = true;
                tempy = -13.f;
                reloj.restart();
            }

        }

        dibujar();
        if(empezar){
            actualizar();
        }

    }
}

void actualizar(){

    sprCuervo.setPosition(x, y);
    tempy += 0.5;
    y += tempy;
    if(y < 0)
        y = 0;
    if(y > ALTO)
        y = ALTO;
    x += tempx;
    Time tiempo = reloj.getElapsedTime();
    if(tiempo.asMilliseconds() >= 0){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 50){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 100){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 150){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 0;
    }
    if(tiempo.asMilliseconds() >= 200){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 250){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 300){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 350){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 400){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 1;
    }
    if(tiempo.asMilliseconds() >= 450){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 500){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 550){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 600){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 650){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 2;
    }
    if(tiempo.asMilliseconds() >= 700){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 750){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 800){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 850){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 900){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 3;
    }
    if(tiempo.asMilliseconds() >= 950){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1000){
        v2iSeleccionFrame.x = 1;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1050){
        v2iSeleccionFrame.x = 2;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1100){
        v2iSeleccionFrame.x = 3;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1150){
        v2iSeleccionFrame.x = 4;
        v2iSeleccionFrame.y = 4;
    }
    if(tiempo.asMilliseconds() >= 1200){
        v2iSeleccionFrame.x = 0;
        v2iSeleccionFrame.y = 0;
    }
    animarCuervo(&sprCuervo, v2iSeleccionFrame);

    if(x <= 25){ // Si el cuervo toca el borde izquierdo

        puntuacion++;
        tempx *= -1; // Para cambiar de signo
        if(numeroAleatorio(1, 2) == 1){ // Arriba
            auxiliar = numeroAleatorio(1, int(sprEspinas[ESPINASDERECHA].getPosition().y) - (ANCHOESPINAS / 2));
            direccion = 1;
        }
        else{ // Abajo
            auxiliar = numeroAleatorio(1, int(ALTO - sprEspinas[ESPINASDERECHA].getPosition().y) - (ANCHOESPINAS / 2));
            direccion = 2;
        }

    }

    if(x >= ANCHO - 25){

        puntuacion++;
        tempx *= -1.f; // Para cambiar de signo
        if(numeroAleatorio(1, 2) == 1){ // Arriba
            auxiliar = numeroAleatorio(1, int(sprEspinas[ESPINASIZQIERDA].getPosition().y) - (ANCHOESPINAS / 2));
            direccion = 3;
        }
        else{ // Abajo
            auxiliar = numeroAleatorio(1, int(ALTO - sprEspinas[ESPINASIZQIERDA].getPosition().y) - (ANCHOESPINAS / 2));
            direccion = 4;
        }

    }

    txtPuntuacion.setString(enteroaString(puntuacion));
    txtSaltos.setString(enteroaString(saltos));

    switch(direccion){
        case 1:
            if(auxiliar > 0){ // Mientras que auxiliar sea positivo restarle 5 a la posicion actual
                sprEspinas[ESPINASDERECHA].move(0, -5);
                auxiliar -= 5;
            }
            break;

        case 2:
            if(auxiliar > 0){
                sprEspinas[ESPINASDERECHA].move(0, 5);
                auxiliar -= 5;
            }
            break;

        case 3:
            if(auxiliar > 0){
                sprEspinas[ESPINASIZQIERDA].move(0, -5);
                auxiliar -= 5;
            }
            break;

        case 4:
            if(auxiliar > 0){
                sprEspinas[ESPINASIZQIERDA].move(0, 5);
                auxiliar -= 5;
            }
            break;

        default:
            break;
    }

    Time tiempoColision = relojColision.getElapsedTime();
    if(tiempoColision.asMilliseconds() >= 700){ // Cada 300 milisegundos detecta colision

        if(sprCuervo.getGlobalBounds().intersects(sprEspinas[ESPINASIZQIERDA].getGlobalBounds()) ||
           sprCuervo.getGlobalBounds().intersects(sprEspinas[ESPINASABAJO].getGlobalBounds())    ||
           sprCuervo.getGlobalBounds().intersects(sprEspinas[ESPINASARRIBA].getGlobalBounds())   ||
           sprCuervo.getGlobalBounds().intersects(sprEspinas[ESPINASDERECHA].getGlobalBounds())){

            vida--;

        }
        relojColision.restart();

    }
    if(vida <= 0)
        gameOver = true;

}

void dibujar(){

    window.clear(Color::Black);
    window.draw(sprFondo);
    window.draw(sprEspinas[ESPINASABAJO]);
    window.draw(sprEspinas[ESPINASARRIBA]);
    window.draw(sprEspinas[ESPINASIZQIERDA]);
    window.draw(sprEspinas[ESPINASDERECHA]);
    window.draw(txtPuntuacion);
    window.draw(txtSaltos);
    if(vida == 3){
        window.draw(sprCorazon[0]);
        window.draw(sprCorazon[1]);
        window.draw(sprCorazon[2]);
    }
    if(vida == 2){
        window.draw(sprCorazon[0]);
        window.draw(sprCorazon[1]);
    }
    if(vida == 1)
        window.draw(sprCorazon[0]);
    window.draw(sprCuervo);
    window.display();

}

void animarCuervo(Sprite *_sprCuervo, Vector2i frame){

    if(tempx > 0){
        IntRect rectangulo(frame.x * TAMANYOCUERVOX, frame.y * TAMANYOCUERVOY, TAMANYOCUERVOX, TAMANYOCUERVOY);
        _sprCuervo->setTextureRect(rectangulo);
    }
    else{
        IntRect rectangulo(++frame.x * TAMANYOCUERVOX, frame.y * TAMANYOCUERVOY, -TAMANYOCUERVOX, TAMANYOCUERVOY);
        _sprCuervo->setTextureRect(rectangulo);
    }

}
