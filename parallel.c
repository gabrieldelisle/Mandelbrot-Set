#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define W 2048
#define H 2048

#define B 4
#define Nmax 256
#define Ncolor 256
#define P 4
//#define DEBUG 1

/**
 *
 * @param d1 real part
 * @param d2 imaginary part
 * @return
 */
int mandelbrot(float d1, float d2) {
    int count = 1;
    // real and imaginary parts
    float z1 = 0;
    float z2 = 0;
    // temp variables to store real and imaginary parts
    float zz1 = 0;
    float zz2 = 0;
    while ((z1 * z1 + z2 * z2 < B) & (count < Nmax)) {
        // store previous z in temp variable
        zz1 = z1;
        zz2 = z2;

        // compute real part
        z1 = zz1 * zz1 - zz2 * zz2 + d1;
        // compute imaginary part
        z2 = 2 * zz1 * zz2 + d2;
        count++;
    }
    return count % Ncolor;
}


int main(int argc, char *argv[]) {
    int rank, size, tag, rc, i, j, t;
    tag = 0;
    MPI_Status status;

    rc = MPI_Init(&argc, &argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float x0 = 0;
    float y0 = 0;



    // we are in the collector process
    if (rank == 0) {
        FILE *fp;
        /* your computations */
        fp = fopen("color.txt", "w");
        // rank of other processes are 1, 2, 3 ... P
        for (t = 0; t < P; t++) {

            // Each process compute W*H/P values
            unsigned char color[W * H / P];
            // we get in the order of the process
            rc = MPI_Recv(color, W * H / P, MPI_UNSIGNED_CHAR, t + 1, tag, MPI_COMM_WORLD, &status);


            // write results to file
            for (j = 0; j < H / P; j++) {
                for (i = 0; i < W; i++)
                    fprintf(fp, "%hhu ", color[i + j * W]);
                fprintf(fp, "\n");
            }
        }
        fclose(fp);

    } else {
        unsigned char color[W * H / P];
        int k;
        // get difference between two pixels on the imaginary plane
        int dx = 2 * B / (W - 1);
        int dy = 2 * B / (H - 1);

        // use 1 based process rank to get offset in x axis
        int x_offset = (rank - 1) * H / P;

        // Along x axis, we only make H / P moves
        for (j = 0; j < H / P; j++) {
            // along y axis we make W moves
            for (i = 0; i < W; i++) {

                color[i + j * W] = mandelbrot((i + x_offset) * dx - B, j * dy - B);

                //printf("point: %d, %d, %f, %f \n", i,k,x0 + b * (2.*i/(W-1) -1), y0 + b * (2.*k/(H-1) -1));

            }
        }
        rc = MPI_Send(color, W * H / P, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);

        // printf("sent ");
        // for (i = 0; i < W*H/P; ++i) {
        // 	printf("%hhu ", color[i]);
        // }
        // printf("\n");
    }

    rc = MPI_Finalize();

    return 0;
}
	