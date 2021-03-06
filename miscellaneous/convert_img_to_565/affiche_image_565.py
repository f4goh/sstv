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



"""
YUV422 looks like YUYV, where this is definition for 2 pixels,
so it become Y0 U0 Y1 V0, where Y0U0V0 is for pixel1 and Y1U0V0 for pixel2
"""

def test(coul):
    if coul>255:
        return 255
    if coul<0:
        return 0
    return int(coul)

def pixelYUV2RGB(Y, U, V):
    RY=V
    BY=U
    #R = test(Y + 1.4075 * (V - 128))
    #G = test(Y - 0.3455 * (U - 128) - (0.7169 * (V - 128)))
    #B = test(Y + 1.7790 * (U - 128))
    R = test(0.003906 * ((298.082 * (Y - 16.0)) + (408.583 * (RY - 128.0))))
    G = test(0.003906 * ((298.082 * (Y - 16.0)) + (-100.291 * (BY - 128.0)) + (-208.12 * (RY - 128.0))))
    B = test(0.003906 * ((298.082 * (Y - 16.0)) + (516.411 * (BY - 128.0))))

    return R,G,B

def pixelRGB2YUV(R, G, B):
    Y = test(16.0 + (.003906 * ((65.738 * R) + (129.057 * G) + (25.064 * B))))
    RY = test(128.0 + (.003906 * ((112.439 * R) + (-94.154 * G) + (-18.285 * B))))
    BY = test(128.0 + (.003906 * ((-37.945 * R) + (-74.494 * G) + (112.439 * B))))
    V=RY
    U=BY
    return Y,U,V



def conversion_img_YUV(nom_fic,header_fic):
    fichier=open(header_fic,"w")
    img=Image.open(nom_fic)
    largeur,hauteur=img.size
    cpt=0
    buf=[]
    fichier.write("const static word imageTest["+str(largeur)+"*"+str(hauteur)+"] PROGMEM = {\n")
    for y in range(hauteur):
        for x in range(largeur//2):
            r0,v0,b0=img.getpixel((x*2,y))
            r1,v1,b1=img.getpixel((x*2+1,y))
            Y0,U0,V0=pixelRGB2YUV(r0,v0,b0)
            Y1,U1,V1=pixelRGB2YUV(r1,v1,b1)
            U=(U0+U1)//2
            V=(V0+V1)//2
            word1=Y0*256+U
            word2=Y1*256+V
            buf.append(word1)
            buf.append(word2)
            fichier.write(hex(word1))
            fichier.write(',')
            fichier.write(hex(word2))
            fichier.write(',')
            cpt+=1
            if cpt==40:
                fichier.write('\n')
                #print()
                cpt=0
    fichier.write("};\n")
    fichier.close()
    #img.show()
    return buf




def affiche_image_yuv(data):
    idx=0
    img=Image.new('RGB',(LARGEUR,HAUTEUR),(0,0,0))
    for y in range(HAUTEUR):
        for x in range(LARGEUR//2):
            Y0=data[idx]
            U=data[idx+1] #BY
            Y1=data[idx+2]
            V=data[idx+3] #RY
            idx+=4
            coulP1=pixelYUV2RGB(Y0,U,V)
            coulP2=pixelYUV2RGB(Y1,U,V)
            img.putpixel((x*2,y),coulP1)
            img.putpixel((x*2+1,y),coulP2)
    img.show()
    img.save("essai14.png")

def affiche_mire_yuv(data):
    img=Image.new('RGB',(320,240),(0,0,0))
    for y in range(HAUTEUR):
        idx=0
        for x in range(LARGEUR//2):
            Y0=data[idx]>>8
            U=data[idx] & 0xff #BY
            Y1=data[idx+1]>>8
            V=data[idx+1] & 0xff #RY
            idx+=2
            coulP1=pixelYUV2RGB(Y0,U,V)
            coulP2=pixelYUV2RGB(Y1,U,V)
            img.putpixel((x*2,y),coulP1)
            img.putpixel((x*2+1,y),coulP2)
    img.show()
    img.save("mire_test.png")



#data=lire_image_565("photo1.yuv")
#affiche_image_yuv(data)
#affiche_hexa(data)

#conversion_img_565("ballon.png","image.h")
buf=conversion_img_YUV("ligneMire320.png","mire_yuv.h")
affiche_mire_yuv(buf)


