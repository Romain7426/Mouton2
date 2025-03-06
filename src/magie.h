#ifndef MAGIE_H
#define MAGIE_H

/*
 * L'API d'une magie est plutôt rudimentaire:
 * utiliser/lancer!
 *
 * À part cela, il faut comptabiliser le nombre de pm utilisés,
 * les effets sur la victime (gain pv, perte pv, gain pm, perte pm, 
 * effet sur épée, épée plus forte, plus faible, etc.)
 * plus l'animation
 * animation qui peut être multiple (ref SOM)
 *  - animation de l'élémentale invoqué
 *  - animation du sort en lui-même
 *  - animation sur la victime (elle rentre dans un état qui la font devenir jaune, brûler, etc.)
 * On pourrait imaginer des effets le lanceur (la magie n'est pas safe-use)
 *
 * Bref, à l'utilisation, on a
 *  - le lanceur (voire plusieurs...)
 *  - la victime (voire plusieurs...)
 *  - l'effet sur le lanceur
 *  - l'effet sur la victime
 *  - l'animation (il faut créer des instances multiples 
 *                 car la même magie peut être utilisé simultanément
 *                 et l'animation se déroule sur plusieurs tours de boucle;
 *                 ou alors imaginer un autre système, par exemple, le lanceur
 *                 passe dans un certain état et en fait l'instance de la magie est créée dans le lanceur
 *                 oui j'aime bien cette méthode car bien sûr on suppose qu'un être ne peut 
 *                 lancer qu'une magie à la fois)
 * 
 * Pour l'instant, on suppose un lanceur et une victime
 * (sinon gestion de liste ou alors comme SOM, un ou tout le monde)
 *
 * L'animation est un automate qui a des effets sur le graphisme du jeu,
 * sur les transitions, les actions ont effet le lanceur, la victime
 * et le reste de la carte.
 * 
 */

typedef void (* magie_lanceur_effet_t)(objet_physique_t * lanceur);
typedef void (* magie_victime_effet_t)(objet_physique_t * victime);
struct magie_animation_etat_t;
typedef void (* magie_animation_t)(magie_animation_etat_t * etat);

struct magie_t {
  const char * nom;
  const char * description;

  texture_t * inventaire_image;
  
  magie_lanceur_effet_t lanceur_effet;
  magie_victime_effet_t victime_effet;

  magie_animation_t animation;
};

struct magie_animation_etat_t {
  // automate

  //
  objet_physique_t * lanceur;
  objet_physique_t * victime;
  carte_t * carte;
};

// l'image de l'inventaire n'est pas `const'
// TODO c un peu embêtant ça, va falloir y réfléchir
//   l'important d'un "copy", c qu'on puisse l'utiliser indépendemment de l'original
//   dont le "const" dans le paramètre d'un "copy"
//   en l'occurrence, eu égard à l'API mise à disposition,
//   la seule chose qu'on fasse est un ++
//   et comme on ne manipule la texture qu'au travers de ses méthodes,
//   il n'y a pas de risque de faire n'importe quoi
//   la texture pourra être bien utilisée de façon indépendante de la première
//   il faut voir que la structure ne contient que des informations pour afficher l'image
//   donc en plus toute modification des données internes de la texture est complètement idiot
//   bref on fait le chgt
// le seul problème potentiel serait que GCC fasse quelque supposition plus ou moins idiote à ce sujet
// et que cela engendre des incohérences 
//  De toutes les façon, c'est déjà cela qui est fait quand on crée une texture déjà chargée en mémoire
extern const magie_t * const magie_total_annihilation;

extern void magie_init(void);
extern void magie_end(void);

extern const char * magie_nom(const magie_t * magie);
extern const char * magie_description(const magie_t * magie);
extern const texture_t * magie_inventaire_image(const magie_t * magie);
//extern void magie_utiliser(const magie_t * magie, objet_physique_t * lanceur, objet_physique_t * victime, carte_t * carte);
extern void magie_utiliser(const magie_t * magie, objet_physique_t * lanceur, objet_physique_t * victime, jeu_t * jeu);


#endif /* MAGIE_H */
