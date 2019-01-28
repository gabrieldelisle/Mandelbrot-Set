#include <stdio.h>
//#include <mpi.h>
#include <math.h>
#define W 2048
#define H 2048

#define b 4
#define Nmax 256
#define Ncolor 64


int mandelbrot(float d1, float d2){
	int count = 1;
	float z1 = 0; float z2 = 0;
	float zz1 = 0; float zz2 = 0;
	while ((z1*z1 + z2*z2 < b) & (count < Nmax)){
		zz1 = z1; zz2 = z2;
		z1 = zz1*zz1 - zz2*zz2 + d1;
		z2 = 2*zz1*zz2 + d2;
		count++;
	}
	return count % Ncolor;
}


int main(int argc, char const *argv[])
{
	unsigned char color[W*H];
	float x0 = 0; float y0 = 0;
	float zoom = 2;

	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
		   color[i+j*W] = mandelbrot(x0 + zoom * (2.*i/(W-1) -1), y0 + zoom * (2.*j/(H-1) -1));
		}
	}
	
	FILE *fp;
	/* your computations */
	fp = fopen("color.txt","w");
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++)
		   fprintf(fp, "%hhu ", color[i+j*W]);
		fprintf(fp, "\n");
	}
	fclose(fp);  

		
	return 0;
}
	