#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 11 15:55:35 2018

@author: Javier Alejandro Acevedo Barroso
Script de Python para la visualización de la simulación en 2D.
"""
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from matplotlib import rcParams
rcParams.update({'figure.autolayout': True})



def fmt(x, pos):
    a, b = '{:.1e}'.format(x).split('e')
    b = int(b)
    return r'${} \times 10^{{{}}}$'.format(a, b)

constantes = np.loadtxt("constants.dat", usecols = 1)
TAU = int(constantes[8])

#x = np.linspace(constantes[0], constantes[1], int(constantes[4]))  

densidadTheo = np.loadtxt('./datFiles/density0theo.dat')
potTheo = np.loadtxt('./datFiles/potential0.dat')
potReal = np.loadtxt('./datFiles/potential1.dat')

plt.imshow(densidadTheo)
cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))

plt.figure()
plt.imshow(potTheo)
cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))


plt.figure()
plt.imshow(potReal)
cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))


#accex = np.loadtxt('./datFiles/acce0x.dat').T
#accey = np.loadtxt('./datFiles/acce0y.dat').T
#
#gtheox = np.loadtxt('./datFiles/gtheox.dat').T
#gtheoy = np.loadtxt('./datFiles/gtheoy.dat').T

#lapl = (-4*potTheo[0:128,0:128]+potTheo[-1:127,1:129]+potTheo[1:129,1:129]+potTheo[-1:127,-1:127]+potTheo[-1:127,1:129])

def darX(inx):
    return -1.0+2.0/128*inx

def darY(iny):
    return -1.0+2.0/128*iny


potT = potTheo.T-potTheo
i,j = 3,3
stencil = np.array([potTheo[i+1,j+1],potTheo[i-1,j+1],potTheo[i+1,j-1],potTheo[i-1,j-1]])
stencil2 = np.array([potTheo[i,j-1],potTheo[i,j+1],potTheo[i+1,j],potTheo[i-1,j]])
stencil3 = np.array([potTheo[i+1,j],potTheo[i,j+1]])
print(i,j,potTheo[i+1,j+1],potTheo[i-1,j+1],potTheo[i+1,j-1],potTheo[i-1,j-1], stencil.sum()-4*potTheo[i,j])
#def rad(x,y):
#    return x*x+y*y
#x = np.linspace(-1,1,128)
#y = np.linspace(-1,1,128)
#
#values = np.zeros((128,128))
#values = rad(x,y)

#ratax = accex/gtheox
#ratay = accey/gtheoy
#
#plt.figure()
#plt.imshow(accey)
#plt.imshow(accex)
#plt.imshow(gtheoy)
#cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#plt.figure()
#plt.imshow(accey)
#cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#plt.figure()
#plt.imshow(ratax)
#cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#plt.figure()
#plt.imshow(np.log(ratay))
#cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))


#
    
#cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#def giveDens(i,n):
#    cosa = ['a','b']
#    cosa[0] = './datFiles/density{:d}.dat'.format(i)
#    cosa[1] = './images/density{:d}.png'.format(i)
#    return cosa[n]
#
#def giveGridX(i,n):
#    cosa = ['a','b']
#    cosa[0] = './datFiles/gridx{:d}.dat'.format(i)
#    cosa[1] = './images/gridx{:d}.png'.format(i)
#    return cosa[n]
#
#def giveGridY(i,n):
#    cosa = ['a','b']
#    cosa[0] = './datFiles/gridy{:d}.dat'.format(i)
#    cosa[1] = './images/gridy{:d}.png'.format(i)
#    return cosa[n]
#
#
#dpi = 300
#
#for i in range(15):
#    dens = np.loadtxt(giveDens(i,0)).T
#    h0 = plt.figure()
#    plt.imshow(dens)
#    cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#    plt.title('Densidad t = %d' %(i))
#    cbar.set_clim(0,4e-2)
#    plt.savefig(giveDens(i,1),dpi=dpi)
#
#
#for i in range(15):
#    phasex = np.loadtxt(giveGridX(i,0)).T
#    h0 = plt.figure()
#    plt.imshow(phasex)
#    cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#    plt.title('phaseX t = %d' %(i))
#    plt.savefig(giveGridX(i,1),dpi=dpi)
#
#for i in range(15):
#    phasey = np.loadtxt(giveGridY(i,0)).T
#    h0 = plt.figure()
#    plt.imshow(phasey)
#    cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
#    plt.clim(0,1e-4)
#    plt.title('phaseY t = %d' %(i))
#    plt.savefig(giveGridY(i,1),dpi=dpi)
#
#
#
#
#
#for i in range(128):
#    print(-1.0+i*2/127)


























