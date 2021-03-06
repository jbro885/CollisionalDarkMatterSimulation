# -*- coding: utf-8 -*-
"""


@author: Javier Alejandro Acevedo Barroso
Script de Python para comparar versión colisional con no colisional.

"""
import numpy as np
#import pyqtgraph as pg
import matplotlib.pyplot as plt
import scipy as sc
import matplotlib.ticker as ticker
from matplotlib import rcParams
rcParams.update({'figure.autolayout': True})
#plt.rcParams['image.cmap'] = 'nipy_spectral'
plt.rcParams['image.cmap'] = 'seismic'

constantes = np.loadtxt("col/constants.dat", usecols = 1)
x = np.linspace(constantes[0], constantes[1], int(constantes[4]))  
TAU = int(constantes[8])
#inF = np.loadtxt("inF.dat")
#outF = np.loadtxt("outF0.dat")
#outF1 = np.loadtxt("outF1.dat")
#oI = np.loadtxt("oI.dat")
#oR = np.loadtxt("oR.dat")
#acce = np.loadtxt("acce.dat")
BULLET = -147
JEANS = -137
GAUSS = -127
def fmt(x, pos):
    #a = '{:2}'.format(x)
    a = "{0:.0f}".format(x)
    return r'${:2} \% $'.format(a)
#        
#plt.pcolormesh(dat)



dt = 0.4
velUnit = 621 #m/s
estUnit = 35 #kpc
potUnit = 385962691092 #J/kg
acceUnit = 3.5737451e-13 #km/s²
fsize = 16

figu = plt.gcf()
dpII = 150
ran1 = [0,1]
ran2 = list(range(2,int(constantes[6]),2))
ran3 = ran1+ran2
for i in ran3:
    col = np.loadtxt("./col/grid{:d}.dat".format(i)).T
    nocol = np.loadtxt("./nocol/grid{:d}.dat".format(i)).T
    scol = sum(sum(col))
    snocol = sum(sum(nocol))
    col = col/sum(sum(col))
    nocol = nocol/sum(sum(nocol))
#    col = col/np.max(col)
#    nocol = nocol/np.max(nocol)
    average = (col + nocol)
#    ii = (np.abs(average) ==0)
    dif = (nocol-col)
    
#    dif[~ii] = (-dif[~ii]*100)/average[~ii]
#    dif[ii] = 0
    
    #dif[ii] = 0
    #dif[~ii] = dif[~ii]/col[~ii]
    print(np.max(np.abs(dif)))
#    dif[np.abs(dif) <= 0.0000003] = 0
    plt.imshow(-dif/np.max(dif)*100, extent=[constantes[0],constantes[1],constantes[2],constantes[3]], aspect='auto')
#    print(constantes)
    if(int(constantes[7]) == GAUSS):
        plt.title("Gauss percentage difference $t =$ {:.2f} ut".format(i*dt),fontsize=fsize)
        #plt.clim(-0.0025,0.0025)
        plt.clim(-100,100)
        plt.xlim(constantes[2]/2,constantes[3]/2)
        plt.ylim(constantes[2]/2,constantes[3]/2)
    if(int(constantes[7]) == BULLET):
        plt.title("Percentage difference $t =$ {:.2f} ut".format(i*dt),fontsize=fsize)
        plt.xlim(constantes[2],constantes[3])
        plt.ylim(constantes[2]/2,constantes[3]/2)
        plt.clim(-100,100)
     #   plt.clim(-0.0025,0.0025)
#    plt.title("Gauss Comparison ($\\tau =$ {:d}) - ($\\tau = \\infty$)".format(TAU), fontsize=fsize)
    plt.yticks(plt.yticks()[0], [str(np.round(t*velUnit)) for t in plt.yticks()[0]])
    plt.ylabel("Velocity [km/s]",fontsize=fsize)
    plt.xticks(plt.xticks()[0], [str(np.round(t*estUnit)) for t in plt.xticks()[0]])
    plt.xlabel("Position [kpc]",fontsize=fsize)
    #cbar = plt.colorbar(format=ticker.FuncFormatter(fmt))
    #cbar = plt.colorbar(format = '%.0f%%')
#    cbar=plt.colorbar()
    #cbar.set_ticks([0, .2, .4, .6, .8, 1])
    #cbar.set_ticklabels(['0', '20%','40%','60%', '80%', '100%'])
#    cbar.set_label("Percentage difference",fontsize=fsize)
    plt.savefig("./dif/phase{:d}.png".format(i), dpi=dpII)
    plt.clf()



#------------------------------------

    dcol = np.loadtxt("./col/density{:d}.dat".format(i))
    dnocol = np.loadtxt("./nocol/density{:d}.dat".format(i))

    
    dcol = dcol/sum(dcol)
    dnocol = dnocol/sum(dnocol)
#    ddif = -(dnocol - dcol)*100*100


    daverage = (dcol + dnocol)
#    ii = (np.abs(daverage) < 1e-9)
    ddif = dnocol-dcol
#    ddif[~ii] = (-ddif[~ii]*100)/daverage[~ii]
#    ddif[ii] = 0
    
#    ddif[ii] = 0
#    ddif[~ii] = ddif[~ii]/dcol[~ii]
#    ddif[np.abs(ddif) <= 0.000003] = 0
    
#    for aq in range(2047):
#        if(dnocol[aq]>0):
#            ddif[aq] = (dnocol[aq]-dcol[aq]/dnocol[aq])*100
#        else:
#            ddif[aq]= 0
    
#    plt.plot(x,ddif*100)
    plt.plot(x,dnocol, label = 'Collisionless', color = 'blue')
    plt.plot(x,dcol, label = r'$\tau$ = '+str(constantes[8]), color = 'red')
    plt.legend()
    
    
    
    #plt.plot((0, 0), (-1, 1), 'k-')
#    plt.title("Density Comparison $(\\tau = 0$) - ($\\tau =$ {:d})".format(TAU), fontsize=fsize)

    plt.xticks(plt.xticks()[0], [str(t*estUnit) for t in plt.xticks()[0]])
    plt.xlabel("Position [kpc]",fontsize=fsize)
    plt.xlim(-1.1,1.1)

    if(int(constantes[7]) == GAUSS):	
        plt.title("Gauss percentage difference in density", fontsize=fsize)
        plt.ylabel("Percentage difference",fontsize=fsize)
        #plt.ylim(-35, 30)
    plt.ylim(0,0.01)
    plt.savefig("./dif/density{:d}.png".format(i), dpi=dpII)
    plt.clf()
    
    
#----------------------------------
    
    
    #pcol = np.loadtxt("./col/potential{:d}.dat".format(i))
    #pnocol = np.loadtxt("./nocol/potential{:d}.dat".format(i))
#
 #   pcol = pcol/sum(pcol)
  #  pnocol = pnocol/sum(pnocol)
   # pdif = (pnocol - pcol)*100
    
    
 #   plt.plot(x,pdif)
  #  plt.xticks(plt.xticks()[0], [str(t*200) for t in plt.xticks()[0]])
#    plt.xlabel("Position [kpc]")
 #   plt.ylabel("Porcentual difference")
  #  plt.ylim(-0.0006, 0.001)
   # plt.title("Potential Comparison $(\\tau = 0$) - $\\tau =$ {:d}".format(TAU))
    #plt.savefig("./dif/potential{:d}.png".format(i), dpi=dpII)
    #plt.clf()


#-------------------------------------------------
    
    
 #   acol = np.loadtxt("./col/acce{:d}.dat".format(i))
#    anocol = np.loadtxt("./nocol/acce{:d}.dat".format(i))
#    acol = acol/sum(acol)
#    anocol = anocol/sum(anocol)
#    adif = (anocol - acol)*100
#    plt.plot(x,adif)
#    plt.xticks(plt.xticks()[0], [str(t*200) for t in plt.xticks()[0]])
#    plt.xlabel("Position [kpc]")
#    plt.ylabel("Porcentual difference")
#    plt.title("Acceleration Difference $(\\tau = 0$) - $\\tau =$ {:d}".format(TAU))
#    plt.savefig("./dif/acce{:d}.png".format(i), dpi=dpII)
#    plt.clf()




#-------------------------------------





#    potential = np.loadtxt("./datFiles/potential{:d}.dat".format(i))
#    plt.plot(x,potential)
#    plt.savefig("./images/potential{:d}.png".format(i))
#    plt.clf()
#    acce = np.loadtxt("./datFiles/acce{:d}.dat".format(i))
#    plt.plot(x,acce)
#    plt.savefig("./images/acce{:d}.png".format(i))
#    plt.clf()
    
#xf = np.linspace(0,1-1/constantes[4],int(constantes[4])) #Espacio de frecuencias senoidal
#plt.plot(x,density)
#plt.savefig("densidad.png")
#plt.figure()
#sns.distplot(density, kde=False, rug=True);

#plt.plot(x,inF)
#plt.plot(x,oR, color = 'black')
#plt.savefig("potencial.png")
#plt.scatter(x,density, color ='g')

#Verifica que el arreglo final luego de un loop sea igual al inicial.
#diffPorc = sum(np.abs(oR -density)/density)*100
#print diffPorc
#print "La diferencia porcentual entre el arreglo original y el arreglo un loop de fourier después es: "+ str(np.floor(diffPorc*10000)/10000)+"%"


#h = plt.figure()
#plt.plot(outF)
#plt.savefig("Fourier.png")
#h = plt.figure()
#plt.plot(x,oR)
#plt.savefig("Potencial.png")
#h = plt.figure()
#plt.plot(x,acce)
#plt.savefig("Aceleracion.png")

#simps(simps(z, y), x)
