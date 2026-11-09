

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#define PIN 17

typedef struct {
	unsigned long totalMemory;
	unsigned long freeMemory;
	unsigned long availableMemory;	
} MemoryInfo;

int readMemoryFile(MemoryInfo *memory) {
	FILE *fp = fopen("/proc/meminfo", "r");
	char line[256];
	
	if (fp == NULL) {
		printf("Unable to open /proc/meminfo");
		return 1;
	}
	
	while(fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "MemTotal: %lu kB", &memory->totalMemory) == 1) {
			continue;
		}
		
		if (sscanf(line, "MemFree: %lu kB", &memory->freeMemory) == 1) {
			continue;
		}
		
		if (sscanf(line, "MemAvailable: %lu kB", &memory->availableMemory) == 1) {
			continue;
		}
	}
	
	fclose(fp);
	return 1;
}

int readStorageFile() {
	
	return 1;	
}

const char* checkThreshold(double memoryUsage) {
	const double THRESHOLD = 75.0;
	
	if (memoryUsage > THRESHOLD) {
		return "WARNING";
	}
	
	return "HEALTHY";
}

void printResourceUtilization(double memoryUsage, const char *status, const char *warning) {
	printf("Memory Usage \t System Status \t Warning LED \n");
	printf("%.2f%% \t\t %s \t %s \n", memoryUsage, status, warning);
}

const char* warningLED(double memoryUsage) {
	const char *status = checkThreshold(memoryUsage);
	
	if (status == "HEALTHY") return "OFF";
	
	return "ON";
}

double calculateMemoryPercentageUsage(MemoryInfo *memory) {
	unsigned long memoryUsage = memory->totalMemory - memory->availableMemory;
	return ((double) memoryUsage / memory->totalMemory) * 100.0;
}

int toggleLED(double memoryUsage) {
	if (warningLED(memoryUsage) == "ON") return 1;
	
	return 0;
}

int main(int argc, char **argv)
{
	gpioInitialise();
	gpioSetMode(PIN, PI_OUTPUT);
	MemoryInfo memory = { 0 };
	readMemoryFile(&memory);
	const char *status = checkThreshold(calculateMemoryPercentageUsage(&memory));
	const char *warning = warningLED(calculateMemoryPercentageUsage(&memory));
	printResourceUtilization(calculateMemoryPercentageUsage(&memory), status, warning);
	gpioWrite(PIN, toggleLED(calculateMemoryPercentageUsage(&memory)));
	gpioTerminate();
	return 0;
}
