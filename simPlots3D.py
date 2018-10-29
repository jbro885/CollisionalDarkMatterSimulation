#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 15 12:32:13 2018

@author: Javier Alejandro Acevedo Barroso
Script de Python para la visualización de la simulación en 3D.
"""
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from matplotlib import rcParams
rcParams.update({'figure.autolayout': True})
dpiT = 300

def fastShow(image, title="none",clim=None):
    plt.clf()
    plt.imshow(image)
    if(clim != None):
        plt.clim(clim[0],clim[1])
    cbar = plt.colorbar()
    plt.title(title)	
    plt.savefig("./images/"+title+".png",dpi=dpiT)


def fmt(x, pos):
    a, b = '{:.4e}'.format(x).split('e')
    b = int(b)
    return r'${} \times 10^{{{}}}$'.format(a, b)

constantes = np.loadtxt("./datFiles/constants.dat", usecols = 1)
#print(constantes)
tamano = int(constantes[14])
Nt = int(constantes[-1])



densidadXY0 = np.loadtxt('./datFiles/densXY0.dat').T
densidadYZ0  = np.loadtxt('./datFiles/densYZ0.dat').T
densidadXZ0  = np.loadtxt('./datFiles/densXZ0.dat').T

#densidadXY1 = np.loadtxt('./datFiles/densXY1.dat').T
#densidadYZ1  = np.loadtxt('./datFiles/densYZ1.dat').T
#densidadXZ1  = np.loadtxt('./datFiles/densXZ1.dat').T


fastShow(densidadXY0, "XY0",clim=[0,0.018])
fastShow(densidadYZ0, "YZ0")
fastShow(densidadXZ0, "XZ0")

#fastShow(densidadXY1, "XY1")
#fastShow(densidadYZ1, "YZ1")
#fastShow(densidadXZ1, "XZ1")
#densidadXY = np.loadtxt("./datFiles/densXY{:d}.dat".format(i)).T
#plt.figure()
imagenes = tamano//8
for i in range(0,tamano,tamano//imagenes):
    potXY0 = np.loadtxt("./datFiles/pot0XY{:d}.dat".format(i)).T
    #potXY1 = np.loadtxt("./datFiles/pot1XY{:d}.dat".format(i)).T
    #diff = potXY0 - potXY1
    accexXY0 = np.loadtxt("./datFiles/accex0XY{:d}.dat".format(i)).T
    #accexXY1 = np.loadtxt("./datFiles/accex1XY{:d}.dat".format(i)).T
    #diff1 = accexXY0 -accexXY1
    fastShow( potXY0 , "potXY{:d}".format(i) )
    fastShow( accexXY0 , "accex0XY{:d}".format(i) )
   # fastShow( accexXY1 , "accex1XY{:d}".format(i) )
    

for i in range(1,Nt):
	

	densidadXY = np.loadtxt('./datFiles/densXY{:d}.dat'.format(i)).T
	#densidadYZ = np.loadtxt('./datFiles/densYZ{:d}.dat'.format(i)).T
	#densidadXZ = np.loadtxt('./datFiles/densXZ{:d}.dat'.format(i)).T

	fastShow(densidadXY, "XY{:d}".format(i), clim=[0,0.018])
	#fastShow(densidadYZ, "YZ{:d}".format(i))
	#fastShow(densidadXZ, "XZ{:d}".format(i))






















