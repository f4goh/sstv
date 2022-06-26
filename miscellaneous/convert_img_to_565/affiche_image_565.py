# Créé par anthony, le 25/06/2022 en Python 3.7
from PIL import Image

LARGEUR=320
HAUTEUR=240

def lire_image_565(nom):
    fichier=open(nom,"rb")
    data=fichier.read()
    print(len(data))
    fichier.close()
    return data


def affiche_hexa(data):
    cpt=0
    for n in range(0,len(data),2):
        msb=data[n]
        lsb=data[n+1]
        word=msb*256+lsb
        print(hex(word),end="")
        print(',',end="")
        cpt+=1
        if cpt==32:
            print()
            cpt=0


def conversion_img_565(nom_fic,header_fic):
    fichier=open(header_fic,"w")
    img=Image.open(nom_fic)
    largeur,hauteur=img.size
    cpt=0
    fichier.write("const static word imageTest["+str(largeur)+"*"+str(hauteur)+"] PROGMEM = {\n")
    for y in range(hauteur):
        for x in range(largeur):
            r,v,b=img.getpixel((x,y))
            r=(r>>3)<<11
            v=(v>>2)<<5
            b=b>>3
            word=r|v|b
            fichier.write(hex(word))
            #print(hex(word),end="")
            #print(',',end="")
            fichier.write(',')
            cpt+=1
            if cpt==40:
                fichier.write('\n')
                #print()
                cpt=0
    fichier.write("};\n")
    fichier.close()
    img.show()




def affiche_image(data):
    idx=0
    img=Image.new('RGB',(LARGEUR,HAUTEUR),(0,0,0))
    for y in range(HAUTEUR):
        for x in range(LARGEUR):
            msb=data[idx]
            lsb=data[idx+1]
            idx+=2
            pixel=msb*256+lsb
            r=(pixel>>11)
            v=(pixel>>5) & 0x3f
            b=pixel & 0x1f
            r=r<<3
            v=v<<2
            b=b<<3
            img.putpixel((x,y),(r,v,b))
    img.show()
    img.save("essai14.png")




#data=lire_image_565("photo14.565")
#affiche_image(data)
#affiche_hexa(data)

#conversion_img_565("ballon.png","image.h")
conversion_img_565("ligneMire640.png","mire.h")


