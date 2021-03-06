/*
Written by Javier Alejandro Acevedo Barroso
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <time.h>

//Constants of the simulation.
#define PI 3.14159265359

//Extreme values for velocity and position.
#define Xmin -1.0
#define Xmax 1.0
#define Vmin -1.0
#define Vmax 1.0
#define scale 1 //scale in order to get better graphics. Better left at 1 after all.

//Grid size.
#define Nx 2048
#define Nv 2048

//Int constants for comparision.
#define toKpc 18
#define toSeconds 14
#define toByear 4
#define toSolarMasses 5
#define toMeters 464
 
#define GAUSS -127
#define JEANS -137
#define BULLET -147

//Relaxation time. 0 means solving the Vlasov equation.
//#define TAU 8972
//#define TAU 15000
#define TAU 0

//Units of the simulation. This particular set corresponds to a galactic scale.
#define mParsecs 35e-3  //How many mpc are equivalent to one spatial unit.
#define solarMases 1e12 //How many solar masses are equivalent to one mass unit.
#define fracT0 4e-3     //What fraction of the age of the universe is equivalent to one time unit.
//#define G 0.959572 //Gravitational constant in this units. It is calculated with sPlots.py
#define G 0.031830 

//Set of units for a galaxy cluster scale.
//#define mParsecs 5
//#define solarMases 1e15
//#define fracT0 2e-1
//#define G 0.2729448134597113


//Arrays
double phase[Nx][Nv] = {0};

double phaseOld[Nx][Nv] = {0};
double phaseTemp[Nx][Nv] = {0};
double *energy;
double *velocity;
double *density;
double *pot;
double *acce;
int initCon;
double totalMass;


//Variables and parameters
int i;
int j;
int k;
int l;
int i2;
int j2;

//Size of the grid
double Lx = Xmax - Xmin;
double Lv = Vmax - Vmin;
double dx = (Xmax-Xmin)*1.0/Nx;
double dv = (Vmax-Vmin)*1.0/Nv;

//Size of a timestep and number of timesteps.
double dt = 0.2; 
int Nt = 150;

//File with parameters of the simulation.
FILE *constants;


//Methods
void printPhase(char *name);
double gaussD(double x, double v, double sx, double sv, double amplitude);
double calDensity();
void printDensity(char *name);
void printConstant(char *name, double value);
double giveDensity(int l);
void potential();
double calcK2(int j2);
double convert(double valor, int unidad);
void calAcceG();
void printAcce(char *name);
double newij(int iin, int jin);
void step();
int mod(int p, int q);
void printPot(char *name);
double jeans(double x, double v, double rho, double sigma, double A, double k);
double feq(int ipos, int jvel);
double feq2(int ipos, int jvel);
double givePos(int ito);
double giveVel(int jto);
double collision(int icol, int jcol, double tau);
void collisionStep();
double newijCol(int iin, int jin);
double bulletC(double x, double v, double sx1, double sx2, double sv, double amplitude1,double amplitude2);

int main()
{
    //dt = dt*dx/dv;

    //Initialization of arrays.
    energy = malloc((sizeof(double)*Nx));
    velocity = malloc((sizeof(double)*Nx));
    density = malloc((sizeof(double)*Nx));
    acce = malloc((sizeof(double)*Nx));
    pot = malloc((sizeof(double)*Nx));
    
	double x;
	double v;
    
    //Choosing what initial conditions to simulate.
    //initCon = GAUSS;
    //initCon = JEANS;
    initCon = BULLET;
    
    //Exporting the parameters of the simulation.
	constants = fopen("./datFiles/constants.dat","w+");
	printConstant("Xmin",Xmin);
	printConstant("Xmax",Xmax);
	printConstant("Vmin",Vmin);
	printConstant("Vmax",Vmax);
	printConstant("Nx",Nx);
	printConstant("Nv",Nv);
	printConstant("Nt", Nt);
    printConstant("InitCon", initCon);
    printConstant("TAU", TAU);
    
    fclose(constants);
    
    /* Parameters of my thesis document
    //Gauss //
    double vSx = 0.06;
    double vSv = 0.06;
    double ampl = 40.0;
    
    //Jeans//
    double rho = 10;
    double sigma = 0.1;
    double A = 0.9999;
    //double A = 0.001;
    double k = 2*PI;
    //double k = 0.5*PI;
    
    //Bullet //
    double vSx1 = 0.04;
    double vSx2 = 0.04;
    double vSvB = 0.06;
    double amplB1 = 30.0;
    double amplB2 = 40.0;
    
    */
    
    
    //Initialization parameters
    //Gauss 
    double vSx = 0.06;
    double vSv = 0.06;
    double ampl = 40.0;
    
    //Jeans2//
    double rho = 0.25/G;
    double A = 0.01;
    double kkj = 0.5;
    double k = 2*(2*PI/Lx); // 2 k_0
    double sigma = 4*PI*G*rho*kkj*kkj/k/k;
    
    //Bullet //
    double vSx1 = 0.04;
    double vSx2 = 0.04;
    double vSvB = 0.06;
    double amplB1 = 30.0;
    double amplB2 = 40.0;
    
    
	for(i=0;i<Nx;i+=1) {
                x = Xmin*1.0+dx*i;
                    for(j=0;j<Nv;j+=1){
                        v = Vmin*1.0+dv*j;
                        if(initCon == GAUSS)
                        {
                            phase[i][j] = gaussD(x,v,vSx,vSv,ampl);
                        }
                        if(initCon == JEANS)
                        {
                            phase[i][j] = jeans(x, v, rho, sigma, A, k);
                        }
                        if(initCon == BULLET)
                        {
                            phase[i][j] = bulletC(x,v,vSx1,vSx2,vSv,amplB1,amplB2);
                        }
                        phaseOld[i][j] = 0;
                        phaseTemp[i][j] = 0;
                    }
			}

		
	//integrates phase space to calculate density. Returns total mass.
	double mass = calDensity();

	//printf("Se simuló %f millones de años con %d pasos de %f millones de años cada uno\n", convert(Nt*dt,toByear)*1000,Nt, convert(dt,toByear)*1000);
    
    //Some execution messages.
    printf("%f million of years were simulated using %d timesteps each of %f million years. \n", convert(Nt*dt,toByear)*1000,Nt,convert(dt,toByear)*1000);
	printf("The total mass was %f times the Milky Way's mass. \n",convert(mass, toSolarMasses)/1e12);

    printf("G es %lf\n", G*1.0);

    //log file with the simulation parameters
	FILE *simInfo = fopen("./images/simInfo.dat","w+");
	
    fprintf(simInfo,"Parameters of the simulation:\n");
	fprintf(simInfo,"x goes from %.2f to %.2f , v goes from %.2f to %.2f\n", Xmin,Xmax,Vmin,Vmax);
    fprintf(simInfo,"The initial conditions were:\n");
	if(initCon == GAUSS)
    {
    
        fprintf(simInfo,"A Gaussian distribution with mean zero with (sx sv A)=\n");
        fprintf(simInfo,"(%.3f %.3f %.3f)\n", vSx, vSv, ampl);        
    }
    if(initCon == JEANS)
    {
    
        fprintf(simInfo,"Jeans instability with (rho sigma A k)=\n");
        fprintf(simInfo,"(%.3f %.3f %.3f %.3f)\n", rho, sigma, A, k);
    }
    if(initCon == BULLET)
    {
        fprintf(simInfo,"A collision of two Gaussian distributions with same velocity dispersion and mean zero.(sx1 sx2 sv A1 A2)=\n");
        fprintf(simInfo,"(%.3f %.3f %.3f %.3f %.3f)\n", vSx1, vSx2, vSvB,amplB1,amplB2);
    }
    
	fprintf(simInfo,"Nt = %d,  dt = %.3f\n", Nt,dt);
    
    //Export of the initial phase space and mass density.
    printPhase("./datFiles/grid0.dat");
	printDensity("./datFiles/density0.dat");
    
    
    //Solving Poisson equation.
    potential();
    
    printPot("./datFiles/potential0.dat");
    
    //acceleration = -grad(Potential)
    calAcceG();
    
    printAcce("./datFiles/acce0.dat");
    
    
   
    
	for(int suprai = 1; suprai<Nt;suprai+=1){
        char *grid = (char*) malloc(200* sizeof(char));
		
		step();
        
        
        if(TAU != 0){
        totalMass = calDensity();
        collisionStep();
        }
        
        totalMass = calDensity();
        
		printf("%d %f\n",suprai,totalMass*100/mass);
        
		sprintf(grid, "./datFiles/density%d.dat", suprai);
		printDensity(grid);

        
		potential();
        
        
        
        sprintf(grid, "./datFiles/potential%d.dat", suprai);
        
		printPot(grid);
        
        
		calAcceG();
        
		sprintf(grid, "./datFiles/acce%d.dat", suprai);
		printAcce(grid);
        sprintf(grid, "./datFiles/grid%d.dat", suprai);
        printPhase(grid);
        free(grid);
                
	}


    
	
	fclose(simInfo);
	return 0;

}

//Exports the phase space grid to text. char name is the name of the output file.
void printPhase(char *name)
{
	FILE *output = fopen(name, "w+");

	for(i=0;i<Nx;i+=1) {
		for(j=1;j<Nv+1;j+=1){ 
          //      printf("ignorarPrimero\n");
			fprintf(output,"%f ", convert(phase[i][Nv-j], toSolarMasses)/convert(1.0,toKpc)/(convert(1.0,toKpc)*3.0857e+19)* convert(1.0,toSeconds)); //Imprime en Masas solares /kpc / (km/s)
            //fprintf(output,"%f ",phase[i][Nv-j]);
        //printf("Error MesagenoIgno\n");
        }
		fprintf(output,"\n");
		//printf("%d\n", i);
			}

	fclose(output);

}

//Exports the density array to text. char name is the name of the output file.
void printDensity(char *name)
{
	FILE *output = fopen(name, "w+");
	for(i=0;i<Nx;i+=1) {
            fprintf(output, "%f\n",convert(density[i],toSolarMasses)/convert(1,toKpc)); //Imprime en Masas solares / kiloparsec.
			}
	fclose(output);
}

//Prints the constante name and value in the parameters file. char name is the name of the parameters, double value is its value.
void printConstant(char *name, double value)
{
    fprintf(constants, "%s", name);
    fprintf(constants, " %f\n", value);
}

//Returns the value of a Gaussian distribution given by x,v, sigma x (sx), sigma v (sv), and amplitude A.
double gaussD(double x, double v, double sx, double sv, double amplitude)
{
	double ex = -x*x/(2.0*sx*sx)-v*v/(2.0*sv*sv);
	return amplitude*exp(ex);
}

//Returns the value of a bimodal Gaussian distribution given by x,v, sigma x1 (sx1), sigma x2 (sx2), sigma v (sv), and amplitudes amplitude1 and amplitude2. The peaks are separated by 0.4 units of space.
double bulletC(double x, double v, double sx1, double sx2, double sv, double amplitude1,double amplitude2)
{
	double ex1 = -(x-0.40)*(x-0.40)/(2.0*sx1*sx1)-v*v/(2.0*sv*sv);
    double ex2 = -(x+0.40)*(x+0.40)/(2.0*sx2*sx2)-v*v/(2.0*sv*sv);
	return amplitude1*exp(ex1)+amplitude2*exp(ex2);

}

//Returns the value corresponding to a Jeans instability for a pair (x,v), with parameters density (rho), sigma v (sv), an amplitude (0<A<=1) and a k.
double jeans(double x, double v, double rho, double sigma, double A, double k)
{
 return rho*pow(2*PI*sigma,-0.5)*exp(-v*v/(2*sigma))*(1.0+A*cos(k*x));
}


//Calcula la densidad, la velocidad macroscópica (u) y la energía libre (e) y los carga en los correspondientes arreglos.
//Integrates the phase space with regards to velocity in order to obtain density, average velocity (u) and local free energy (e).
double calDensity()
{
	double mass = 0;
	for(i = 0;i<Nx;i+=1){
		density[i]=0;
        velocity[i] = 0;
        energy[i] = 0;
		for(j=0;j<Nv;j+=1){
				density[i] += phase[i][j]*dv;
                velocity[i] += phase[i][j]*dv*giveVel(j);
        }
        for(j=0;j<Nv;j+=1){
            energy[i] += phase[i][j]*dv*pow(giveVel(j) - velocity[i], 2)/2.0;
        }
        velocity[i] = velocity[i] / density[i];
        energy[i] = energy[i] / density[i];
		mass += density[i]*dx;
		}
	return mass;
}

//Solves the Poisson equation using Fourier Method. Updates the potential array.
void potential()
{

    fftw_complex *in, *out, *inR, *mem;
    in=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*Nx);
    out=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*Nx);
    inR=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*Nx);
    mem=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*Nx);


    fftw_plan pIda;
    pIda = fftw_plan_dft_1d(Nx, in, out,FFTW_FORWARD, FFTW_MEASURE);

    //Cargar densidad en in y borra out:
    for(i=0;i<Nx;i+=1){

        in[i] = giveDensity(i) - totalMass/(Xmax-Xmin);
        inR[i] = -1.0;
        out[i] = 0;
    }
    fftw_execute(pIda);

    //Guarda out en mem.
    for(i=0;i<Nx;i+=1){
        mem[i] = out[i];
    }

    pIda = fftw_plan_dft_1d(Nx, out, inR, FFTW_BACKWARD, FFTW_MEASURE); //Se debe usar el mismo plan sí o sí al parecer.
    
    //Devuelve carga a out Î(Chi).
    out[0] = -4*PI*G*mem[0];
    for(i=1;i<Nx;i+=1){
      out[i] = -4.0*PI*G*mem[i]*calcK2(i);
    //out[i] = mem[i]; //Descomentar esta línea para obtener la distribucion original.
    }
    fftw_execute(pIda);

    
    for(i=0;i<Nx;i+=1){
        pot[i] = creal(inR[i]/Nx);
    }
}

//K**2 using pseudospectral approximation.
double calcK2(int j2)
{
    double rta;
    if( ( (j2 == 0)   )  ){
        return 0;
    }
    if(j2<Nx/2){
        rta = 2*PI*j2/(Xmax-Xmin);
    }
    if(j2>=Nx/2){
        rta = -2*PI*(Nx-j2)/(Xmax-Xmin);
    }    
    return pow(1.0/rta,2);
}


//Returns density in position l from the density array.
double giveDensity(int l)
{
    double rta = density[l];
    return rta;
}

//Converts from simulation units  (u.m, u.s, u.t) to (solar masses, meters, billion of years or seconds)
double convert(double value, int unit )
{
    double conx0 = 3.0857e+22; //a megaparsec in meters.
    double cont0 = 13.772*1000000000; //Age of the universe in years.
    double cont0s = cont0*365.24*24*60*60; //age of the universe in seconds

    if(unit == toSolarMasses){
        return value * solarMases;
    }
    if(unit == toKpc){
        return value * mParsecs * 1000.0; 
    }
    if(unit == toByear){
        return value*13.772*fracT0;
    }
    if(unit == toSeconds){
        return value*cont0s*fracT0;
    }
    if(unit == toMeters){
        return value * mParsecs * conx0;
    }
    return -1;
}

//Differentiates the potential and load the acceleration on the acce array.
void calAcceG()
{
    for(i = 0; i<Nx ; i +=1){
    acce[i] =  (-pot[mod(i-2,Nx)] + 8*pot[mod(i-1,Nx)]-8*pot[mod(i+1,Nx)]+pot[mod(i+2,Nx)])/(12*dx);
    }
}

//Prints acce array on a file named "name" in kpc/(million year)^2.
void printAcce(char *name)
{
	FILE *output = fopen(name, "w+");
	for(i=0;i<Nx;i+=1) {
            fprintf(output, "%f\n",convert(acce[i], toKpc)/pow(convert(1.0, toByear)*1000,2)); 
			}
	fclose(output);
}

//Prints pot array on a file named "name" in J/kg.
void printPot(char *name)
{
	FILE *output = fopen(name, "w+");
	for(i=0;i<Nx;i+=1) {
            fprintf(output, "%f\n",pow(convert(pot[i], toMeters)/convert(1.0, toSeconds),2)/pot[i]); //Imprime potential en J/kg
			}
	fclose(output);
}


//Calculates the new position of an element of the phase space grid due the free streaming. The new positions are loaded on (i2,j2).
double newij(int iin, int jin)
{
        double x = Xmin*1.0+dx*iin; //Inicialización
        double v = acce[iin]*dt;
        double dj = v/dv;
        dj = (int)dj;
        j2 = jin+dj;

        if(j2 < 0 || j2 >= Nv) return -1;
        v = Vmin*1.0+dv*j2;
        x = v*dt;
        double di = x/dx*scale;
        di = (int) di;

        i2 = iin + di;
        i2 = mod(i2,Nx);
    return 0;
}

//Performs a streaming step. Updates the phase space (phase). (k,i) corresponds to x, (j,l) corresponds to v.
void step()
{
	for(k = 0; k<Nx; k++){
		for(l= 0; l<Nv; l++){
			if(newij(k,l) ==0){
				phaseTemp[i2][j2] += phase[k][l];
			}
		}
	}

	for(i = 0; i<Nx; i++){
		for(j= 0; j<Nv; j++){
			phase[i][j] = phaseTemp[i][j];
			phaseTemp[i][j] = 0;
		}
	}
}

//Performs a collisional step. Updates the phase space (phase). (k,i) corresponds to x, (j,l) corresponds to v.
void collisionStep()
{
    	for(k = 0; k<Nx; k++){
		for(l= 0; l<Nv; l++){
			if(newijCol(k,l) ==0){
                phaseTemp[i2][l] += collision(k,l,TAU) + phase[k][l] ;//+ dt*feq2(k,l)*acce[k]*(giveVel(l)-velocity[k])/energy[k];
			}
		}
	}

	for(i = 0; i<Nx; i++){
		for(j= 0; j<Nv; j++){
			phase[i][j] = phaseTemp[i][j];
			phaseTemp[i][j] = 0;
		}
	}
}

//Calcula el cambio en x. Ignora j.
//Calculates the new position of an element of the phase space grid due the collisions. The new positions are loaded on (i2,j2).
double newijCol(int iin, int jin)
{
        double x = Xmin*1.0+dx*iin; //Inicialización


        double v = acce[iin]*dt;
        j2 = jin; 

        if(j2 < 0 || j2 >= Nv) return -1;
        v = giveVel(j2);
        
        x = v*dt*scale;
        double di = x/dx;
        di = (int) di;

        i2 = iin + di;
        i2 = mod(i2,Nx);
    return 0;
}


//Calculates the collisional contribution on the phase space for a given Tau.
double collision(int icol, int jcol, double tau)
{
    if(TAU==0) return 0;
    double df = (feq(icol,jcol) - phase[icol][jcol])/(1.0*tau);    
    return df;
}

//Calculates the equilibrium distribution. Maxwell-Boltzmann equilibrium.
double feq(int ipos, int jvel)
{
    double ex = -1.0*pow(giveVel(jvel)-velocity[ipos],2)/(2.0*energy[ipos]);
    double other = density[ipos] / sqrt(2.0*PI*energy[ipos]);
    return other * exp(ex);    
}

//Calculates the equilibrium distribution. Low Mach number approximation.
double feq2(int ipos, int jvel)
{
    double ex = -1.0*pow(giveVel(jvel),2)/(2.0*energy[ipos]);
    double other = density[ipos] / sqrt(2*PI*energy[ipos]);
    double lowMach = 1.0 + giveVel(jvel)*velocity[ipos]/energy[ipos] + pow(giveVel(jvel)*velocity[ipos],2)/(2.0*energy[ipos]) - pow(velocity[ipos],2)/(2.0*energy[ipos]);
    return other * exp(ex)* lowMach;
}

//Returns the coordinate for the array element ito.
double givePos(int ito)
{
    return Xmin*1.0+dx*ito;
}

//Returns the coordinate for the array element jto.
double giveVel(int jto)
{
    return Vmin*1.0+dv*jto;
}


//modulus function. The range is from 0 to q-1.
int mod(int p, int q)
{
	p = p%q;
	if(p<0){
		 return p+q;
	}
	return p;

}

















