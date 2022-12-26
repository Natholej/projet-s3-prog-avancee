#include "SDL_graphique.h"


void update_data(monde_t* monde){
    //Affichage graphique des obstacles du niveau
    for (int j=0; j<monde->niveau.nbObstacle; j++){
        SDL_RenderCopy(monde->ecran, monde->niveau.tabObstacle[j].TextureObstacle, NULL, &monde->niveau.tabObstacle[j].SpriteGraphique[0]);
        deplacementObstacle(&monde->niveau.tabObstacle[j]);
        if (sprites_collide(monde->joueur.SpriteGraphique, monde->niveau.tabObstacle[j].SpriteGraphique)){
            monde->niveau.tabObstacle = chargerniveau(monde->niveau.numero, monde->ecran, &monde->niveau.nbObstacle);
        }
    }
    JumpJoueur(&monde->joueur.jump, &monde->joueur, &monde->joueur.compteurJump, &monde->joueur.sensJump);
}


int main(int argc, char *argv[])
{

monde_t monde;

monde.fin = false;
monde.pause = true;


if(SDL_Init(SDL_INIT_VIDEO) < 0){ // Initialisation de la SDL
    printf("Erreur d’initialisation de la SDL: %s",SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
}


// Créer la fenêtre
monde.fenetre = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
if(monde.fenetre == NULL){ // En cas d’erreur
    printf("Erreur de la creation d’une fenetre: %s",SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
}


// Mettre en place un contexte de rendu de l’écran
monde.ecran = SDL_CreateRenderer(monde.fenetre, -1, SDL_RENDERER_ACCELERATED);

//****Chargement image
//FOND
monde.fond = charger_image("../fond.bmp", monde.ecran);
//MENU (test à mettre dans une fonction)
monde.menu = charger_image_transparente("../Menu.bmp", monde.ecran, 0, 255, 0);
//FLECHE MENU
monde.curseur = charger_image("../curseur.bmp", monde.ecran);
SDL_Rect SpGraphique[1];
SpGraphique[0].x = 600;
SpGraphique[0].y = 50;
SpGraphique[0].w = 50;
SpGraphique[0].h = 50;

//*****SPRITE JOUEUR
init_joueur(&monde.joueur);
monde.joueur.JoueurSprite = charger_image_transparente("../sprites.bmp", monde.ecran, 0, 255, 255); //Compteur pour réduire fréquence du mouvement du sprite du joueur


//**********OBSTACLE

monde.niveau.numero = 1;
monde.niveau.tabObstacle = chargerniveau(monde.niveau.numero, monde.ecran, &monde.niveau.nbObstacle);


// Boucle principale
while(!monde.fin){
    SDL_RenderClear(monde.ecran); //Clear la cible actuelle
    SDL_RenderCopy(monde.ecran, monde.fond, NULL, NULL); //Copie la texture et la met sur le renderer


    if (!monde.pause){
        update_data(&monde);
    } else{
        monde.joueur.SpriteFichier[0].y = 400/6;
        SDL_RenderCopy(monde.ecran, monde.menu, NULL, NULL); 
        SDL_RenderCopy(monde.ecran, monde.curseur, NULL, &SpGraphique[0]); 
    }

    //Animation du sprite Joueur
    if (monde.joueur.animation == true){
        monde.joueur.compteurSprite = animationJoueur(monde.joueur.compteurSprite, &monde.joueur.SpriteGraphique[0], &monde.joueur.SpriteFichier[0]); //Stockage de la valeur du compteur à chaque itération pour actualisation du sprite
    }

    SDL_RenderCopy(monde.ecran, monde.joueur.JoueurSprite, &monde.joueur.SpriteFichier[0], &monde.joueur.SpriteGraphique[0]);


    SDL_PollEvent( &monde.evenements );
    handle_events(&monde.evenements, &monde.fin, &monde.joueur, &monde.pause, &monde.choix);

    //Update
    SDL_RenderPresent(monde.ecran);
}
// Quitter SDL
SDL_Quit();
SDL_DestroyTexture(monde.fond);
SDL_DestroyWindow(monde.fenetre);
return 0;
}