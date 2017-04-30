#include <iostream>
#include <stdio.h>
#include "mpi.h"
#include "utils.h"

using namespace std;

float func(float x)
{
	return x;
}

float trap(float a, float b, int n, float h)
{
	float integral;
	float x;
	int i;
	x = a;	
	integral = (f2(a) + f2(b) )/2.0;
	for(i = 0; i < n-1; i++)
	{
		x = x + h;
		integral = integral + f2(x);	
	}
	return integral*h;
}

int main()
{

	/*
 	Lower bound = a
	Upper bound = b
	number of steps = n
	defined function in utils.cpp
 	*/
	float a = 0.0;
	float b = 1.0;
	int n = 10000;
	
	MPI_Init(NULL, NULL);
	
	int numproc;
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);

        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	float h = (b-a)/n;
	int local_n = n/numproc;
	float local_a = a + rank*local_n*h;
	float local_b = local_a + local_n*h;
	float integral;
	float total;	
	
	integral = trap(local_a, local_b, local_n, h);
	printf("Local a = %f, local b = %f, integral = %f  in proc %d \n", local_a, local_b, integral, rank);
	MPI_Reduce(&integral, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Answer is %f \n", total);
		
	}


	return 0;

}
