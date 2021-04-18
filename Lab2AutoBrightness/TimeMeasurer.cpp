#include "TimeMeasurer.h"

void onThreadsCount()
{
	const char* filenames[] =
	{
		"images/orig/river-640.ppm",
		"images/orig/river-640.pgm",
		"images/orig/river-1920.ppm",
		"images/orig/river-1920.pgm"
	};

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