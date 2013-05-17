#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <mpi.h>

int findmin(float* arr, int start, int end)
{
	int ind;
	float minVal = 10000;
	for (int i = start; i < end; i++)
		if (arr[i] < minVal)
		{
			ind = i;
			minVal = arr[i];
		}
	return ind;
}

int main(int argc, char** argv)
{
	int n = 10;
	float a[] = {1, 2, 3, 4, 5, 6, 7, -1, 9, 10};
	
	/*
	unsigned long n = 1000;
    float * a = new float[n];
    int fd = open("/dev/random", O_RDONLY);
    read(fd, a, n);
	*/
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	// Print off a hello world message
	printf("Hello world from processor %s, rank %d"
         " out of %d processors\n",
         processor_name, world_rank, world_size);

	if (world_rank == 0)
	{
		int buf[] = {n/2, n};
		MPI_Send(buf, 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
		int myInd = findmin(a, 0, n/2);
		MPI_Recv(buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
		int otherInd = buf[0];
		if (a[otherInd] < a[myInd])
			myInd = otherInd;
		printf("Min's element position %d\n", myInd);
		printf("Min's element: %f\n", a[myInd]);
	}
	else if (world_rank == 1)
	{
		int* buf = new int[2];
		MPI_Recv(buf, 2, MPI_INT, 0, 0, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
		int minInd = findmin(a, buf[0], buf[1]);
		MPI_Send(&minInd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	// Finalize the MPI environment.
	MPI_Finalize();
}
