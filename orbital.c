#include "orbital.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ORBITAL_SPHERE_TEST(float *vertices, int point_count) {
    // color 1
    const float r1 = 0.0f, g1 = 1.0f, b1 = 1.0f;
    // color 2
    const float r2 = 1.0f, g2 = 0.0f, b2 = 0.5f;
    for (int i = 0; i < point_count; i++) {
        float r = 1;
        
        float doublePI = 2.0f * M_PI;
        
        float theta = ((float)rand() / RAND_MAX) * M_PI;
        float phi =  ((float)rand() / RAND_MAX) * doublePI; 
        float t = theta / M_PI;
        float x = r * sinf(theta) * cosf(phi);
        float y = r * cosf(theta);
        float z = r * sinf(theta) * sinf(phi);
        vertices[i * 6] = x;
        vertices[i * 6 + 1] = y;
        vertices[i * 6 + 2] = z;
        // colors
        vertices[i * 6 + 3] = r1 * (1.0f - t) + r2 * t;
        vertices[i * 6 + 4] = g1 * (1.0f - t) + g2 * t;
        vertices[i * 6 + 5] = b1 * (1.0f - t) + b2 * t;
    }
}

float rand01() {
    return (float)rand() / RAND_MAX;
}

float clamp01(float x) {
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x;
}

float smoothstep(float edge0, float edge1, float x) {
    x = clamp01((x - edge0)) / (edge1 - edge0);
    return x * x * (3.0f - 2.0f * x);
}

float mix_float(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}

void APPLY_DENSITY_HIGHLIGHT(
    float *r_col,
    float *g_col,
    float *b_col,
    double density,
    double max_density
) {
    if (max_density <= 0.0) {
        return;
    }

float ratio = (float)(density / max_density);
ratio = clamp01(ratio);

const float highlight_start = 0.75f;
const float highlight_full = 0.88f;

float highlight = smoothstep(highlight_start, highlight_full, ratio);

const float hr = 1.0f;
const float hg = 1.0f;
const float hb = 0.0f;

*r_col = mix_float(*r_col, hr, highlight);
*g_col = mix_float(*g_col, hg, highlight);
*b_col = mix_float(*b_col, hb, highlight);
}

double ASSOC_LEGENDRE(int l, int m, float x) {

    if (l < 0 || m < 0 || m > l) {
        return NAN;
    }

    if (x < -1.0 || x > 1.0) {
        return NAN;
    }

    double pmm = 1.0; // start
    
    if (m > 0) {
        double cosxinsin = sqrt((1.0 - x) * (1.0 + x));
        double factorial = 1.0;
        // from 1 to m
        for (int i = 1; i <= m; i++) {
            pmm = pmm * (-factorial * cosxinsin);
            factorial += 2.0;
        }
    }

    if (l == m) {
        return pmm;
    }

    double pmmp1 = x * (2 * m + 1) * pmm;

    if (l == m + 1) {
        return pmmp1;
    }

    double ahyet = 0.0;
    for (int ll = m + 2; ll <= l; ll++) {
        ahyet = ((2 * ll - 1) * x * pmmp1 - (ll + m - 1) * pmm) / (double)(ll - m);
        pmm = pmmp1;
        pmmp1 = ahyet;
    }
    
    return ahyet;
}

double ASSOC_LAGUERRE(int k, int alpha, float x) {

    if (k < 0 || alpha < 0)  {
        return NAN;
    }

    double v0 = 1.0;
    double v1;

    if (k == 0) {
        return v0;
    }

    v1 = 1.0 + alpha - x;
    if (k == 1) {
        return v1; 
    }

    double vj = 0.0;
    for (int j = 2; j <= k; j++) {
        vj = (((2.0 * j - 1.0 + (double)alpha - x) * v1) - ((j - 1.0 + (double)alpha) * v0)) / (double)j;
        v0 = v1;
        v1 = vj;
    }
    return vj;
}

double CALCDENSITY(int n, int l, int m, 
    double r, double theta, double phi) {
    int k = n - l - 1;
    int alpha = 2 * l + 1;
    double cos_theta = cos(theta);
    
    double laguerre = ASSOC_LAGUERRE(k, alpha, r);
    double legendre = ASSOC_LEGENDRE(l, abs(m), cos_theta);
    double azimuth;
        if (m > 0) {
            azimuth = cos(m * phi);
        } else if (m < 0) {
            azimuth = sin(abs(m) * phi);
        } else {
            azimuth = 1.0;
        }
    double decay = exp(-r / (double)n);
    double radial_scale = pow(r, l);

    double psi = decay * radial_scale * azimuth * legendre * laguerre;

    return psi * psi;

}

double FIND_MACSIM(int n, int l, int m) {
    double max = 0.0;
    double max_r = (double)(n * n + 5 * n);

    for (int i = 0; i < 5000; i++) {
        double r = rand01() * max_r;
        double theta = rand01() * M_PI;
        double phi = rand01() * (2.0f * M_PI);
        double d = CALCDENSITY(n, l, m, r, theta, phi);

        if (d > max) { 
            max = d;
        }
    }
    
    return (max == 0.0) ? 0.0001 : max * 1.05;
}

void BUILD_ORBITAL(float *vertices, int point_count, int n, int l, int m) {
// color 1
    const float r1 = 0.0f, g1 = 1.0f, b1 = 1.0f;
// color 2
    const float r2 = 1.0f, g2 = 0.0f, b2 = 0.5f;

    double max_density = FIND_MACSIM(n, l, m);
    double max_r = (double)(n * n + 5 * n);

    int i = 0;
    while (i < point_count) {
        double r = rand01() * max_r;
        double theta = acos(2.0 * rand01() - 1.0);
        double phi = rand01() * (2.0f * M_PI);
        double pdensity = CALCDENSITY(n, l, m, r, theta, phi);
        
        if (rand01() < (pdensity / max_density)) {
            float t = (float)(r / max_r);
            double x = r * sin(theta) * cos(phi);
            double y = r * cos(theta);
            double z = r * sin(theta) * sin(phi);
            
            float scale = 1.2f / (float)max_r;

            vertices[i * 6] = (float)x * scale;
            vertices[i * 6 + 1] = (float)y * scale;
            vertices[i * 6 + 2] = (float)z * scale;
            //colors
            vertices[i * 6 + 3] = r1 * (1.0f - t) + r2 * t;
            vertices[i * 6 + 4] = g1 * (1.0f - t) + g2 * t;
            vertices[i * 6 + 5] = b1 * (1.0f - t) + b2 * t;

            i++;
        }
    }
}

void INIT_ORBITAL_PARTICLES(Particle *particles, int point_count, int n, int l, int m) {
// color 1
    const float r1 = 0.0f, g1 = 1.0f, b1 = 1.0f;
// color 2
    const float r2 = 1.0f, g2 = 0.0f, b2 = 0.5f;

    int abs_m = abs(m);
    double max_density = FIND_MACSIM(n, l, m);
    double max_r = (double)(n * n + 5 * n);

    int i = 0;
    while (i < point_count) {
        double r = rand01() * max_r;
        double theta = acos(2.0 * rand01() - 1.0);
        double phi = rand01() * (2.0f * M_PI);
        double pdensity = CALCDENSITY(n, l, m, r, theta, phi);

    if (rand01() < (pdensity / max_density)) {
        float t = (float)(r / max_r);

        particles[i].r = (float)r;
        particles[i].theta = (float)theta;
        particles[i].phi = (float)phi;

        particles[i].phi_speed = 0.0005f; // normal speed
        // particles[i].phi_speed = 0.00001f + (((float)rand() / RAND_MAX) * 0.0025f); //random speed

        float cr = r1 * (1.0f - t) + r2 * t;
        float cg = g1 * (1.0f - t) + g2 * t;
        float cb = b1 * (1.0f - t) + b2 * t;

        APPLY_DENSITY_HIGHLIGHT(&cr, &cg, &cb, pdensity, max_density);

        particles[i].r_col = cr;
        particles[i].g_col = cg;
        particles[i].b_col = cb;

        i++;
    }
}
}

void INIT_ORBITAL_PARTICLES_DIF(Particle *particles, int point_count, int n, int l, int m) {
// color 1
    const float r1 = 0.0f, g1 = 1.0f, b1 = 1.0f;
// color 2
    const float r2 = 1.0f, g2 = 0.0f, b2 = 0.5f;

    int abs_m = abs(m);
    double max_density = FIND_MACSIM(n, l, m);
    double max_r = (double)(n * n + 5 * n);

    int i = 0;
    while (i < point_count) {
        double r = rand01() * max_r;
        double theta = acos(2.0 * rand01() - 1.0);
        double phi = rand01() * (2.0f * M_PI);

        int k = n - l - 1;
        int alpha = 2 * l + 1;
        double cos_theta = cos(theta);
        double laguerre = ASSOC_LAGUERRE(k, alpha, r);
        double legendre = ASSOC_LEGENDRE(l, abs_m, cos_theta);
        double azimuth;
        if (m > 0) {
            azimuth = cos(m * phi);
        } else if (m < 0) {
            azimuth = sin(abs(m) * phi);
        } else {
            azimuth = 1.0;
        }
        double decay = exp(-r / (double)n);
        double radial_scale = pow(r, l);

        double psi = decay * radial_scale * azimuth * legendre * laguerre;
        double pdensity = CALCDENSITY(n, l, m, r, theta, phi);
        float t = (float)(r / max_r);
        float fade = 1.0f - 0.5f * t;

    if (rand01() < (pdensity / max_density)) {
        particles[i].r = (float)r;
        particles[i].theta = (float)theta;
        particles[i].phi = (float)phi;

        particles[i].phi_speed = 0.0005f; // speed
        float cr, cg, cb;

        if (psi >= 0.0) {
            cr = r1 * fade;
            cg = g1 * fade;
            cb = b1 * fade;
        } else {
            cr = r2 * fade;
            cg = g2 * fade;
            cb = b2 * fade;
        }

        APPLY_DENSITY_HIGHLIGHT(&cr, &cg, &cb, pdensity, max_density);

        particles[i].r_col = cr;
        particles[i].g_col = cg;
        particles[i].b_col = cb;

        i++;
        }
    }
}
