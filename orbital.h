#ifndef ORBITAL_H
#define ORBITAL_H

typedef struct {
    float r;          
    float theta;      
    float phi;        
    float phi_speed;  
    float r_col, g_col, b_col; 
} Particle;

void ORBITAL_1S(float *vertices, int point_count);
void ORBITAL_2P(float *vertices, int point_count);
void ORBITAL_SPHERE_TEST(float *vertices, int point_count);
double ASSOC_LEGENDRE(int l, int m, float x);
double ASSOC_LAGUERRE(int k, int alpha, float x);
double CALCDENSITY(int n, int l, int m, double r, double theta, double phi);
void BUILD_ORBITAL(float *vertices, int point_count, int n, int l, int m);
void INIT_ORBITAL_PARTICLES(Particle *particles, int point_count, int n, int l, int m);
void INIT_ORBITAL_PARTICLES_DIF(Particle *particles, int point_count, int n, int l, int m);

#endif