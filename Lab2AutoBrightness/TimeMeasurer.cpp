#include "TimeMeasurer.h"

const char* filenames[] =
{
	"images/orig/river-640.ppm",
	"images/orig/river-640.pgm",
	"images/orig/river-1920.ppm",
	"images/orig/river-1920.pgm"
};

void onThreadsCount()
{
	printf("Filename,ThreadsCount,Time (ms)\n");
	
	Timer timer;
	for (int iFilename = 0; iFilename < 4; iFilename++)
	{
		const char* currentFilename = filenames[iFilename];
		NetpbmImage* image = NetpbmImage::read(currentFilename);

		for (int iThreadsCount = 1; iThreadsCount <= 16; iThreadsCount++)
		{
			omp_set_num_threads(iThreadsCount);

			timer.start();
			image->autoBrightness();
			timer.stop();
			float measuredMs = timer.getMs();

			printf("%s,%i,%f\n", currentFilename, iThreadsCount, measuredMs);
		}
	}
}

void onSchedules()
{
	printf("Static\n");
	printf("Filename,ChunkSize,Time (ms)\n");
	
	Timer timer;
	for (int iFilename = 0; iFilename < 4; iFilename++)
	{
		const char* currentFilename = filenames[iFilename];
		NetpbmImage* image = NetpbmImage::read(currentFilename);
		
		for (int iChunkSize = 0; iChunkSize <= 10; iChunkSize++)
		{
			int chunkSize = (int)pow(2, iChunkSize);
			timer.start();
			image->autoBrightnessScheduleTest(chunkSize);
			timer.stop();
			float measuredMs = timer.getMs();

			printf("%s,%i,%f\n", currentFilename, chunkSize, measuredMs);
		}
	}
}

void onOmpOnOff()
{
	printf("Filename,Omp,Time (ms)\n");

	omp_set_num_threads(1);

	Timer timer;
	for (int iFilename = 0; iFilename < 4; iFilename++)
	{
		const char* currentFilename = filenames[iFilename];
		NetpbmImage* image = NetpbmImage::read(currentFilename);

		for (int isOmp = 0; isOmp <= 1; isOmp++)
		{
			timer.start();
			if (isOmp)
				image->autoBrightness();
			else
				image->autoBrightnessST();
			timer.stop();
			float measuredMs = timer.getMs();

			printf("%s,%s,%f\n", currentFilename, isOmp ? "on" : "off", measuredMs);
		}
	}
}

void onManualDiffSchedules()
{
	printf("Filename,QueueSchedule,ScalingSchedule,Time (ms)\n");

	const char* queueSchedule = "guided";
	const char* scalingSchedule = "dynamic32";

	Timer timer;
	for (int iFilename = 0; iFilename < 4; iFilename++)
	{
		const char* currentFilename = filenames[iFilename];
		NetpbmImage* image = NetpbmImage::read(currentFilename);

		timer.start();
		image->autoBrightness();
		timer.stop();
		float measuredMs = timer.getMs();

		printf("%s,%s,%s,%f\n", currentFilename, queueSchedule, scalingSchedule, measuredMs);
	}
}
