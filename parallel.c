#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define M 2048 // Rows
#define N 2048 // Columns
#define B 2 // Threshold 
#define x0 0.25 // X axis coordinate in the center
#define y0 0.0 // Y axis coordinate in the center 
#define zoom 1.0/256 // width (or height) of the picture

#define Nmax 256 // Number of colors
#define P 128// Number of processes -1


int mandelbrot(float d1, float d2) {
	int count = 0;
	float z1 = 0; float z2 = 0;
	float zz1 = 0; float zz2 = 0;

	// the loops keep running until its norm is higher than the threshold or the maximum number of iterations is reached
	while ((z1*z1 + z2*z2 < B*B) & (count < Nmax)){
		// z = z^2 + d 
		zz1 = z1; zz2 = z2;
		z1 = zz1*zz1 - zz2*zz2 + d1;
		z2 = 2*zz1*zz2 + d2;
		count++;
	}
	return count;
}


int main(int argc, char *argv[]) {
	int rank, size, tag, rc, i,j,t;
	tag = 0;
	
	// Init MPI
	MPI_Status status;

	rc = MPI_Init(&argc, &argv);
	rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	if (rank == 0) {
		// root process collect computed pixels and write it in the file
		FILE *fp;
		fp = fopen("color.txt","w");
		for (t = 0; t < P; t++) {

			unsigned char color[M*N/P];
			rc = MPI_Recv(color, M*N/P, MPI_UNSIGNED_CHAR, t+1, tag, MPI_COMM_WORLD, &status);
			
			for (j = 0; j < N/P; j++) {
				for (i = 0; i < M; i++)
				   fprintf(fp, "%hhu ", color[i+j*M]);
				fprintf(fp, "\n");
			}
		}
		fclose(fp);  
		
	}
	else{
		// each of the P other process compute a part of the picture
		unsigned char color[M*N/P];
		int k;
		for (j = 0; j < N/P; j++) {
			for (i = 0; i < M; i++) {
				k = (rank-1)*N/P+j;
				// coordinates of each pixel
				float d1 = x0 + zoom * (2.*i/(M-1) -1);
				float d2 = y0 + zoom * (2.*k/(N-1) -1);
            color[i+j*M] = mandelbrot(d1, d2);
			}
		}
		rc = MPI_Send(color, M*N/P, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);

	}
	rc = MPI_Finalize();

		
	return 0;
}