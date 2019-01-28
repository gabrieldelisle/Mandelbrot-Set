#include <stdio.h>
#include <mpi.h>
#include <math.h>
#define M 256
#define N 256

#define B 4
#define Nmax 256
#define Ncolor 256
#define P 16
//#define DEBUG 1


int mandelbrot(float d1, float d2){
	int count = 1;
	float z1 = 0; float z2 = 0;
	float zz1 = 0; float zz2 = 0;
	while ((z1*z1 + z2*z2 < B) & (count < Nmax)){
		zz1 = z1; zz2 = z2;
		z1 = zz1*zz1 - zz2*zz2 + d1;
		z2 = 2*zz1*zz2 + d2;
		count++;
	}
	return count % Ncolor;
}


int main(int argc, char *argv[])
{
	int rank, size, tag, rc, i,j,t;
	tag = 0;
	MPI_Status status;

	rc = MPI_Init(&argc, &argv);
	rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	float x0 = 0; float y0 = 0;
	float zoom = 2;
	

	if (rank == 0) {
		FILE *fp;
		/* your computations */
		fp = fopen("color.txt","w");
		for (t = 0; t < P; t++) {

			unsigned char color[M*N/P];
			rc = MPI_Recv(color, M*N/P, MPI_UNSIGNED_CHAR, t+1, tag, MPI_COMM_WORLD, &status);
			
			// printf("received ");
			// for (i = 0; i < M*N/P; ++i) {
			// 	printf("%hhu ", color[i]);
			// }
			// printf("\n");


			for (j = 0; j < N/P; j++) {
				for (i = 0; i < M; i++)
				   fprintf(fp, "%hhu ", color[i+j*M]);
				fprintf(fp, "\n");
			}
		}
		fclose(fp);  
		
	}
	else{
		unsigned char color[M*N/P];
		int k;
		for (j = 0; j < N/P; j++) {
			for (i = 0; i < M; i++) {
				k = (rank-1)*N/P+j;
			   color[i+j*M] = mandelbrot(x0 + zoom * (2.*i/(M-1) -1), y0 + zoom * (2.*k/(N-1) -1));

				//printf("point: %d, %d, %f, %f \n", i,k,x0 + zoom * (2.*i/(M-1) -1), y0 + zoom * (2.*k/(N-1) -1));

			}
		}
		rc = MPI_Send(color, M*N/P, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
		
		// printf("sent ");
		// for (i = 0; i < M*N/P; ++i) {
		// 	printf("%hhu ", color[i]);
		// }
		// printf("\n");
	}

		
	return 0;
}
	