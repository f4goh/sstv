/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bitmap.h
 * Author: ale
 *
 * Created on 6 octobre 2021, 16:16
 */

#ifndef BITMAP_H
#define BITMAP_H

#pragma pack(2)

typedef struct
{
    unsigned char signature[2];
    unsigned int tailleFichier;
    unsigned int reserve;
    unsigned int adresseImg;

} enTeteFichierBmp;//14

typedef struct
{
    unsigned int tailleEnTete;
    unsigned int largeur;
    unsigned int hauteur;
    unsigned short int nbPlan;
    unsigned short int bitParPixel;
    unsigned int typeCompression;
    unsigned int tailleImage;
    unsigned int resolutionHorizontale;
    unsigned int resolutionVerticale;
    unsigned int nbCouleursUtilises;
    unsigned int nbCouleursImportantes;
} enTeteImageBmp;//40

typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
}couleur;//4


#endif /* BITMAP_H */

