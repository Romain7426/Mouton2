#include "global.h"
#include "ivresse.h"
#include "opengl.h"
#include "son.h"


static unsigned int ivresse_anim_i = 0;
static real ivre_animation_indice = 0.0;

#define ivresse_anim_i_max 500

void ivresse_activer(void)
{
    if(ivresse_anim_i == 0) 
    {  
        son_jouer(SON_IVRESSE);
        ivresse_anim_i = ivresse_anim_i_max;   
    }
}


static real fact_plateau_central(real f)
{
    if(f < 0.1)
        return f * 10.0;
    else if (f > 0.9)
        return (1.0 - (f - 0.9) * 10.0);
    else
        return 1.0;   
}
void ivresse_boucle_debut(void)
{
    if(ivresse_anim_i > 0)
    {
        real f = ((real) (ivresse_anim_i_max - ivresse_anim_i)) / ((real) ivresse_anim_i_max);
        real fact = fact_plateau_central(f);
                      
        repere_homothetie(1.0 + fact * 0.1 * sin(ivre_animation_indice),
		      1.0 + 0.1 * fact * cos(ivre_animation_indice), 1.0);              
        ivresse_anim_i--; 
        ivre_animation_indice += 0.05f;               
    }   
}

void ivresse_boucle_fin(void)
{
    if(ivresse_anim_i > 0) 
    {
         ivresse_anim_i--; 
         repere_precedent();  
    }
}



