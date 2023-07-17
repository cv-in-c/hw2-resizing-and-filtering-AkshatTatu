#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if(x<0)
    x = 0;
    else if(x>=im.w)
    x = im.w-1;
    if(y<0)
    y = 0;
    else if(y>=im.h)
    y = im.h-1;
    int n = x + y*im.w +c*im.w*im.h;
    return im.data[n];
}   

void set_pixel(image im, int x, int y, int c, float v)
{
    if(x<0||x>im.w||y<0||y>im.h||c<0||c>im.c)
    return;
    int m= x + y*im.w + c*im.w*im.h;
    im.data[m] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    int l=im.w*im.h*im.c;
    for(int i=0; i<l; i++)
    copy.data[i] = im.data[i];
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int i=0; i<im.w*im.h; i++){
        float r = im.data[i];
        float g = im.data[i+im.h*im.w];
        float b = im.data[i+2*im.h*im.w];
        float o = (0.299*r + 0.587*g + 0.144*b);
        gray.data[i]=o;
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int i=0; i<im.w; i++){
        for(int j=0; j<im.h; j++){
            int p = i + j*im.w + c*im.h*im.w;
            im.data[p]+=v;
        }
    }
}

void clamp_image(image im)
{
    for(int i=0; i<im.h*im.w*im.c; i++){
        if(im.data[i]<0)
        im.data[i] = 0;
        else if(im.data[i]>1)
        im.data[i] = 1;
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for(int i=0; i<im.h*im.w; i++)
    {
        float r = im.data[i];
        float g = im.data[i + im.h*im.w];
        float b = im.data[i + 2*im.h*im.w];
        float v = three_way_max(r,g,b);
        float m = three_way_min(r,g,b);
        float p= v-m;
        float s,h,h0=0;
        if(r==0 && g==0 && b==0)
        s=0;
        else
        s=p/v;

        if (p==0)
        h0 = 0;
        else if(v==r)
        h0 = (g-b)/p;
        else if(v==g) 
        h0 = (g-b)/p+2;
        else if(v==b)
        h0 = (r-g)/p+4;
        
        if(h0<0)
        h = (h0/6)+1;
        else
        h = (h0/6);
        im.data[i] = h;
        im.data[i + im.h*im.w] = s;
        im.data[i + 2*im.h*im.w] = v;
    }
}

void hsv_to_rgb(image im)
{
    for(int i=0; i<im.h*im.w; i++)
    {
        float h = im.data[i];
        float s = im.data[i + im.h*im.w];
        float v = im.data[i + 2*im.h*im.w];
        float hue_sector, hue_fraction, l, m, n, r, g, b;
        hue_sector = floor(h * 6);
        hue_fraction = (h * 6) - hue_sector;
        l = v * (1 - s);
        m = v * (1 - hue_fraction * s);
        n = v * (1 - (1 - hue_fraction) * s);
    
    switch ((int)hue_sector) {
        case 0:
            r = v;
            g = n;
            b = l;
            break;
        case 1:
            r = m;
            g = v;
            b = l;
            break;
        case 2:
            r = l;
            g = v;
            b = n;
            break;
        case 3:
            r = l;
            g = m;
            b = v;
            break;
        case 4:
            r = n;
            g = l;
            b = v;
            break;
        default:
            r = v;
            g = l;
            b = m;
        break;
    }
     r = im.data[i];
     g = im.data[i + im.h*im.w];
     b = im.data[i + 2*im.h*im.w];
    }
}
