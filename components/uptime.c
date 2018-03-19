/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#elif __OpenBSD__
#include <sys/sysctl.h>
#include <sys/time.h>
#endif

#include "../util.h"

const char *
uptime(void)
{
	int h;
	int m;
	int uptime = 0;
#ifdef __linux__
	struct sysinfo info;

	sysinfo(&info);
	uptime = info.uptime;
#elif __OpenBSD__
	int mib[2];
	size_t size;
	time_t now;
	struct timeval boottime;

	time(&now);

	mib[0] = CTL_KERN;
	mib[1] = KERN_BOOTTIME;

	size = sizeof(boottime);

	if (sysctl(mib, 2, &boottime, &size, NULL, 0) != -1)
		uptime = now - boottime.tv_sec;
	else
		return NULL;
#endif
	h = uptime / 3600;
	m = (uptime - h * 3600) / 60;

	return bprintf("%dh %dm", h, m);
}
