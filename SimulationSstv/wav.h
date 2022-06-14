/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   wav.h
 * Author: ale
 *
 * Created on 25 septembre 2021, 17:35
 */

#ifndef WAV_H
#define WAV_H

/* Formats audios
 * (je ne connais que le PCM ...) */
enum F_AUDIO {
    PCM = 1
};
 
/* Taille de l'en-tête subTaille1 */
enum T_AUDIO {
    T_PCM = 16
};
 
/* Canaux :
 *  Mono = 1 seule enceinte
 *  Stéréo = 2 enceintes
 *  GDC = 3 enceintes (gauche, droite, centre)
 *  GD_GD = 4 enceintes (avant gauche et droite, arrière gauche et droite)
 *  GDCS = 5 enceintes (GDC + Surround)
 *  All = 6 enceintes (GDC, centre gauche, centre droit, surround) */
enum CAN {
    MONO = 1,
    STEREO = 2,
    GDC = 3,
    GD_GD = 4,
    GDCS = 5,
    ALL = 6
};
 
/* Fréquence d'échantillonnage
 * Valeurs en hz*/
enum F_ECH {
    F_8K = 8000,
    F_11K = 11025,
    F_22K = 22050,
    F_41K = 44100,
    F_48K = 48000, /* Encodage d'un CD audio  */
    F_96K = 96000
};
 
/* Bits par échantillon */
enum BPE {
    BPE_8 = 8,
    BPE_16 = 16,
    BPE_24 = 24,
    BPE_32 = 32
};
 
#define T_ENTETE 44
 
/* L'en-tête fait 44 octets.
 * Il ne faut pas oublier d'enlever sizeof(void*) si
 * on utilise sizeof sur notre structure. */
typedef struct {
    /* Le mot "RIFF" */
    char riff[4];
    /* La taille du fichier - 8
     * Ou subTaille2 + 44 - 8 */
    int32_t taille;
    /* Le mot "WAVE" */
    char wave[4];
 
    /* Le mot "fmt " */
    char fmt[4];
    /* Taille du header jusqu'à "data" */
    int32_t subTaille1;
    /* Format du fichier */
    int16_t formatAudio;
    /* Nombres de canaux */
    int16_t nombreCanaux;
    /* Fréquence d'échantillonnage */
    int32_t freqEch;
    /* ByteRate
     * Nombre d'ocets par seconde
     ** Calcul :
     *===> freqEch * nombreCanaux * bitsParEch / 8 */
    int32_t ByteRate;
    /* Alignement
     * Nombre d'octets par échantillon
     * Englobe tous les canaux !
     ** Calcul :
     *===> nombreCanaux * bitsParEch / 8 */
    int16_t align;
    /* Bits par échantillon */
    int16_t bitsParEch;
 
    /* Le mot "data" et la
     * taille des données */
    char Ndata[4];
    /* Taille des données */
    int32_t subTaille2;
 
    /* A allouer dynamiquement.
     * Contiendra les données */
    //void *data;
    //enregistrement au fur et a mesure (non utilisé ici)
}wave;

#endif /* WAV_H */

