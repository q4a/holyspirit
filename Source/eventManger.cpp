
#include <SDL.h>
#include "prototypes.h"

void EventManager(bool *EventTableau,bool *Clic,coordonnee *positionCamera,coordonnee *positionSouris,long tempsEcoule)
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
   		{
        	case SDL_KEYDOWN: /* Si appui d'une touche */
        	    if(event.key.keysym.sym>0&&event.key.keysym.sym<255)
					EventTableau[event.key.keysym.sym] = 1;
            	break;
        	case SDL_KEYUP: /* Si appui d'une touche */
				if(event.key.keysym.sym>0&&event.key.keysym.sym<255)
					EventTableau[event.key.keysym.sym] = 0;
            	break;
            case SDL_MOUSEMOTION:
                positionSouris->x=event.motion.x;
                positionSouris->y=event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                Clic[event.button.button]=1;
                positionSouris->x=event.button.x;
                positionSouris->y=event.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                Clic[event.button.button]=0;
                positionSouris->x=event.button.x;
                positionSouris->y=event.button.y;
                break;
    	}
	}

    if(positionCamera->y<48)
    positionCamera->y=48;
    if(positionCamera->x<0)
    positionCamera->x=0;

}





