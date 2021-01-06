#include <stdio.h>
#include <stdlib.h>
//#include <errno.h>
#include <sys/utsname.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define BUFFER 100
#define PATH 64

char *getDest(char *, const int, const int);


void calcTime(int modVar)
{

	int day, hour, min, sec, modupTime;

	day = (modVar) / (24 * 60 * 60); 
	min = (modVar) / 60; 
	sec = (modVar) % 60; 
	hour = (modVar) / (60 * 60);

	printf(" %d : %d : %d : %d\n", day, hour, min, sec);

}


int main(int argc, char *argv[])
{
	struct timeval tv;
	time_t t;	
	struct tm *info;

	gettimeofday(&tv, NULL);
	t = tv.tv_sec;

	char *buffer;
	double upTime, idle;
	char c1,c2;

	int modupTime, moduTime, modsysTime, modidleTime, modbTime;

	int uTime, idleTime, systemTime, CS, bTime, procNum;
	int memTotal, memAvail, loadAvg;
	int diskWr, diskRe;

	int sampleRate, totalTime;			// Argc set variables

	char repTypeName[16];
	
	strcpy(repTypeName, "Standard");
	if (argc > 1)
       		{
		sscanf(argv[1], "%c%c", &c1, &c2);
			if (c1 != '-') 
			{
				fprintf(stderr, "usage: observer [-s][-l int dur]\n");
				exit(1);
			}
			if (c2 == 's') 
			{
				//reportType = SHORT;
				strcpy(repTypeName, "Short");
			}
			if (c2 == 'l') 
			{
				//reportType = LONG;
				strcpy(repTypeName, "Long");
					sampleRate = atoi(argv[2]);
					totalTime = atoi(argv[3]);
			}
		}

	if (argc == 1) 
		{
		printf("\n");
		printf("Part B ..... \n");

		printf("\n");
			//sscanf(argv[1], "%c%c", &c1, &c2);

		buffer = getDest("cpuinfo", 4, 13); 		// LINE #4!!
		printf("CPU info: %s\n", buffer);
		free(buffer);

		printf("\n");
						
		buffer = getDest("sys/kernel/osrelease", 0, 0);
		printf("Version: Linux %s\n", buffer);
		free(buffer);
						
		printf("\n");

		buffer = getDest("uptime", 0, 0);
		sscanf(buffer, "%lf", &upTime);
		free(buffer);


			modupTime = (int) upTime; 
			printf("Last booted on:  \n");
			calcTime(modupTime);
			
			

		printf("\n");

		info = localtime(&t);
		printf("%s", asctime (info));
		
		printf("\n");

		buffer = getDest("sys/kernel/hostname", 0, 0);
		printf("Hostname:  %s\n", buffer);
		free(buffer);

		printf("\n");
						
		}

	if((argc == 2)) // (argc == 1))
	{
	

		printf("\n");

		printf("Part C .... \n");
	
		printf("\n");

			buffer = getDest("stat", 0, 5);
			sscanf(buffer, "%d", &uTime);
			free(buffer);

			printf("Time cpu has been in user mode: \n"); 
			moduTime = (int) uTime;
			calcTime(moduTime);

				printf("\n");

			buffer = getDest("stat", 0, 21);
			sscanf(buffer, "%d", &systemTime);
			free(buffer);

			printf("Time cpu has been is system mode: \n");
			modsysTime = (int) systemTime; 
			calcTime(modsysTime);

				printf("\n");


			buffer = getDest("stat", 0, 29);
			sscanf(buffer, "%d", &idleTime);
			free(buffer);

			printf("Time cpu has been idle for:  \n");
			modidleTime = (int) idleTime; 
			calcTime(modidleTime);

				printf("\n");

			buffer = getDest("diskstats", 25, 16);		// Field #1 sda
			sscanf(buffer, "%d", &diskWr);
			printf("Number of disk writes:  %d\n", diskWr);
			free(buffer);


				printf("\n");

			buffer = getDest("diskstats", 25, 53);		// Field #5 sda
			sscanf(buffer, "%d", &diskRe);
			printf("Number of disk reads:   %d\n", diskRe);
			free(buffer);


				printf("\n");

			//buffer = getDest("stat", 6, 6);			// ctxt
			//sscanf(buffer, "%d", &CS);
			//printf("Number of context switches:   %d\n", CS);
			//free(buffer);

			buffer = getDest("stat", 6, 0);
			printf("Number of context switches: %s\n", buffer);
			free(buffer);

				printf("\n");

			buffer = getDest("stat", 7, 5);			// btime
			sscanf(buffer, "%d", &bTime);
			free(buffer);
			
				modbTime = (int) bTime; 
				printf("Time when system was last booted:  \n");
				calcTime(modbTime);

				printf("\n");

			buffer = getDest("stat", 8, 10);			// processes
			sscanf(buffer, "%d", &procNum);
			printf("Number of processes created since last boot:   %d\n", procNum);
			free(buffer);



				printf("\n");
	}

	
	if((argc == 4)) // (argc == 1))
	{

		printf("Part D .... \n");
		//sampleRate = atoi(argv[2]);
		//totalTime = atoi(argv[3]);

		printf("\n");

		printf("Sample rate set to: %d\n", sampleRate);
		printf("Duration set to: %d\n", totalTime);

			printf("\n");
			
			buffer = getDest("meminfo", 0, 17);		
			sscanf(buffer, "%d", &memTotal);
			printf("Amount of memory configured:  %d\n", memTotal);
			free(buffer);
		
			printf("\n");

			buffer = getDest("meminfo", 1, 17);		
			sscanf(buffer, "%d", &memAvail);
			printf("Amount of memory currently available:  %d\n", memAvail);
			free(buffer);
	
			printf("\n");


			for(int i = 0; i < totalTime; i++)
				{
				
				sleep(sampleRate);
				buffer = getDest("loadavg", 0, 0);
				printf("Load average:  %s\n", buffer);
				free(buffer);
				//sleep(sampleRate);
				//printf("Testing ...\n");
				}
				
		
	}


		info = localtime(&t);
		printf("Report Type: %s 	Time: %s",repTypeName, asctime (info));

		buffer = getDest("sys/kernel/hostname", 0, 0);
		printf("Hostname:  %s\n", buffer);
		free(buffer);


       	return 0;

}

char *getDest(char *file, const int getLine, const int chLen)
	{
		int x, y, z;

		FILE *myProc;

		char lineC;
		char *buffer;
		char path[PATH] = "/proc/";		// Call to PROC
		strcat(path, file);			// Combine PROC with passed Path



		myProc = fopen(path, "r");

			for(x = 0; x < getLine; x++)		// Get line in file destination
			{
				do
				{

					lineC = getc(myProc);

				}while(lineC != '\n');

			}

			for(z = 0; z < chLen; z++)		// Use buffer to display number characters
				
				lineC = getc(myProc); 

				z  = 0;
				lineC = '\0';

				buffer = (char *) malloc(BUFFER);	//Allocate MEM
			
					do
					{

					lineC = getc(myProc);
					buffer[z++] = lineC;	// Fill Buffer

					}while(lineC != '\n');

				//lineC = '\0';
	
				buffer[z-1] = '\0';		
				fclose(myProc);

		return buffer;
		}
