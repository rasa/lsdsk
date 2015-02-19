// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

/*

todo:
fix size of 10.0

bit  	bit  	0 or 1
byte 	B 	8 bits
kibibit 	Kibit 	1024 bits
kilobit 	kbit 	1000 bits
kibibyte (binary) 	KiB 	1024 bytes
kilobyte (decimal) 	kB 	1000 bytes
megabit 	Mbit 	1000 kilobits
mebibyte (binary) 	MiB 	1024 kibibytes
megabyte (decimal) 	MB 	1000 kilobytes
gigabit 	Gbit 	1000 megabits
gibibyte (binary) 	GiB 	1024 mebibytes
gigabyte (decimal) 	GB 	1000 megabytes
terabit 	Tbit 	1000 gigabits
tebibyte (binary) 	TiB 	1024 gibibytes
terabyte (decimal) 	TB 	1000 gigabytes
petabit 	Pbit 	1000 terabits
pebibyte (binary) 	PiB 	1024 tebibytes
petabyte (decimal) 	PB 	1000 terabytes
exabit 	Ebit 	1000 petabits
exbibyte (binary) 	EiB 	1024 pebibytes
exabyte (decimal) 	EB 	1000 petabytes

source: http://www.t1shopper.com/tools/calculate/

.to do:
disk space is all wrong
1 - 9.9 > 9.9
10-999  > 999

1.9G is 1G
 10G
% column?

-l | --lowercase        Show drive letters/FAT volume names in lower case
-A | --available-at-end Show available drives as one line at the end

Available: F: G: I: K: Y:

*/

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <stdio.h>
//#include <atlbase.h> // required for Stackwalker
#include <limits.h>
#include <conio.h>
#include <lm.h>
#include <errno.h>

#include <string>
#include <map>
#include <list>

//#include "Stackwalker.h"
//#include "Optimize.h"
#include "SystemMessage.h"
#include "debug.h"
#include "getopt.h"

#include "version.h"
#include "WinIoCtl.h"

#define APPNAME			VER_INTERNAL_NAME
#define APPVERSION		VER_STRING2
#define APPCOPYRIGHT	VER_LEGAL_COPYRIGHT

static char *progname;

static char *short_options = "Aabde:fhi:kmnstuVv?";

static struct option long_options[] = {
	{"all",				no_argument,		0, 'A'},
	{"available",		no_argument,		0, 'a'},
	{"bus",				no_argument,		0, 'b'},
	{"domain",			no_argument,		0, 'd'},
	{"exclude",			required_argument,	0, 'e'},
	{"free",			no_argument,		0, 'f'},
	{"help",			no_argument,		0, 'h'},
	{"include",			required_argument,	0, 'i'},
	{"file_system",		no_argument,		0, 'm'},
	{"name",			no_argument,		0, 'n'},
	{"size",			no_argument,		0, 's'},
	{"type",			no_argument,		0, 't'},
	{"used",			no_argument,		0, 'u'},
	{"volume",			no_argument,		0, 'v'},

	{"A",				no_argument,		0, 'A'},
	{"a",				no_argument,		0, 'a'},
	{"d",				no_argument,		0, 'd'},
	{"e",				no_argument,		0, 'e'},
	{"f",				no_argument,		0, 'f'},
	{"h",				no_argument,		0, 'h'},
	{"i",				no_argument,		0, 'i'},
	{"m",				no_argument,		0, 'm'},
	{"n",				no_argument,		0, 'n'},
	{"s",				no_argument,		0, 's'},
	{"t",				no_argument,		0, 't'},
	{"u",				no_argument,		0, 'u'},
	{"v",				no_argument,		0, 'v'},

	{"avail",			no_argument,		0, 'a'},

	{"kilobyte",		no_argument,		0, 'k'},
	{"kilo",			no_argument,		0, 'k'},
	{"k",				no_argument,		0, 'k'},

	{"filesystem",		no_argument,		0, 'm'},
	{"fs",				no_argument,		0, 'm'},

	{"vol",				no_argument,		0, 'v'},

	{"version",			no_argument,		0, 'V'},
	{"V",				no_argument,		0, 'V'},

	{"?",				no_argument,		0, '?'},
	{NULL,				no_argument,		0, 0}
};

struct _opt {
	int drives[26];
	int file_system;
	int free;
	int kilo;
	int name;
	int size;
	int type;
	int used;
	int volume;
	int domain;
	int available;
	int all;
	int bus;
};

const int OPT_FILE_SYSTEM	= 0;
const int OPT_FREE			= 1;
const int OPT_NAME			= 2;
const int OPT_SIZE			= 3;
const int OPT_TYPE			= 4;
const int OPT_USED			= 5;
const int OPT_VOLUME		= 6;
const int OPT_BUS			= 7;

typedef struct _opt Opt;

Opt opt = {
	{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1
	},	// drives[26]
	1,	// file_system
	1,	// free
	0,	// kilo
	1,	// name
	1,	// size
	1,	// type
	1,	// used
	1,	// volume
	1,  // domain
	0,	// available
	0,  // all;
	1,  // bus;
};

static void usage() {
	printf(
"Usage: %s [options]\n"
"Options:\n",
		progname
	);
/*2345678901234567890123456789012345678901234567890123456789012345678901234567890*/
	printf(
"-t | --type        Display type of drive\n"
"-v | --volume      Display volume name of drive\n"
"-u | --used        Display used space on drive\n"
"-f | --free        Display free space remaining on drive\n"
"-s | --size        Display total size of drive\n"
"-m | --file_system Display file system type (NTFS, FAT32, etc.)\n"
"-b | --bus         Display device bus type (USB, 1394, etc.)\n"
"-n | --name        Display network share directory or subst'd directory\n"
"-d | --domain      Display domain and username for network shares\n"
"-e | --exclude d:  Exclude drive d:\n"
"-i | --include d:  Include drive d: (and exclude all others)\n"
"-k | --kilobyte    Use 1024 for a kilobyte (K) instead of 1000\n"
"-a | --available   Display available drive letters afterwards\n"
"-A | --all         Display both mounted and unmounted drives\n"
"-V | --version     Show version and copyright information and quit\n"
"-? | --help        Show this help message and quit\n"
);
}

char *last_error() {
  int last_error = GetLastError();
  static char buf[2048];
  if (FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    last_error,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &buf,
    sizeof(buf),
    NULL
    ) == 0) {
      _snprintf(buf, sizeof(buf), "Unknown Windows error %d", last_error);
  }
  return buf;
}

/* @todo move to shared lib */
/* per http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+basename */
static char* basename(char* s) {
	char* rv;

	if (!s || !*s)
		return ".";

	rv = s + strlen(s) - 1;

	do {
		if (*rv == '\\' || *rv == '/')
			return rv + 1;
		--rv;
	} while (rv >= s);

	return s;
}

class DriveInfo {
public:
	std::string letter;
	std::string root;
	std::string type;
	std::string bus;
	std::string volume;
	std::string name;
	std::string file_system;

	ULARGE_INTEGER FreeBytesAvailable;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;
};

DriveInfo drive[26];

const int DRIVE_OTHER = -1;
const int DRIVE_SUBST = -2;

static void reset_options() {
	static int option_seen = 0;

	if (option_seen)
		return;

	++option_seen;

	opt.file_system = 0;
	opt.free = 0;
	opt.name = 0;
	opt.size = 0;
	opt.type = 0;
	opt.used = 0;
	opt.volume = 0;
	opt.domain = 0;
	opt.available = 0;
	opt.all = 0;
	opt.bus = 0;
}

/*

//#include <windows.h>
#include <setupapi.h>
//#include <stdio.h>
#include <devguid.h>
#include <regstr.h>

int test()
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;

    // Create a HDEVINFO with all present devices.
    hDevInfo = SetupDiGetClassDevs(NULL,
        0, // Enumerator
        0,
        DIGCF_PRESENT | DIGCF_ALLCLASSES );

    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        // Insert error handling here.
        return 1;
    }

    // Enumerate through all devices in Set.

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,
        &DeviceInfoData);i++)
    {
        DWORD DataT;
        LPTSTR buffer = NULL;
        DWORD buffersize = 0;

        //
        // Call function with null to begin with,
        // then use the returned buffer size
        // to Alloc the buffer. Keep calling until
        // success or an unknown failure.
        //
        while (!SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
            SPDRP_DRIVER,
			// SPDRP_DEVICEDESC,
            &DataT,
            (PBYTE)buffer,
            buffersize,
            &buffersize))
        {
            if (GetLastError() ==
                ERROR_INSUFFICIENT_BUFFER)
            {
                // Change the buffer size.
                if (buffer) LocalFree(buffer);
                buffer = (LPTSTR) LocalAlloc(LPTR,buffersize);
            }
            else
            {
                // Insert error handling here.
                break;
            }
        }

        printf("Device: %s\n",buffer);

        if (buffer) LocalFree(buffer);
    }

    if ( GetLastError()!=NO_ERROR &&
         GetLastError()!=ERROR_NO_MORE_ITEMS )
    {
        // Insert error handling here.
        return 1;
    }

    //  Cleanup
    SetupDiDestroyDeviceInfoList(hDevInfo);

    return 0;
}

*/

std::string int64tostr(ULARGE_INTEGER ui, int kilo) {
	static std::string s = "";
	static char buf[1024];
	static char *suffixes = " KMGTPEZY";

	const unsigned __int64 KILOBYTE = 1000ui64;
	const unsigned __int64 KIBIBYTE = 1024ui64;

	unsigned __int64 kilobyte = kilo ? KIBIBYTE : KILOBYTE;

	double kd = (signed) kilobyte;

	int indexes = (int) strlen(suffixes);

	for (int index = 0; index < indexes; ++index) {
/*
<1M		->  k
1-9M	=>	1.0M - 9.9M
10-999M	=>  10M-999M
*/
		if (ui.QuadPart < KILOBYTE) {
			char suffix = suffixes[index];
			if (kilo) {
				suffix = tolower(suffix);
			}
			unsigned int n = (unsigned int) ui.QuadPart;
			sprintf(buf, "%3d%c", n, suffix);
			s = buf;
			return s;
		}

		if (ui.QuadPart < KILOBYTE * 10) {
			char suffix = suffixes[index + 1];
			if (kilo) {
				suffix = tolower(suffix);
			}
			unsigned int n = (unsigned int) ui.QuadPart;
			double d = n;
//printf("\nn=%d d=%15.5f\n", n, d);
			d /= 1000.0 + 0.49999999999;
//printf("\nn=%d d=%15.5f\n", n, d);
			sprintf(buf, "%3.1f%c", d, suffix);
			s = buf;
			return s;
		}

		ui.QuadPart /= kilobyte;
	}

	return "";
}

static void set_option(int *o, char *optarg) {
	static int seen_option = 0;

	if (!seen_option) {
		++seen_option;
		opt.file_system = 0;
		opt.free = 0;
		opt.name = 0;
		opt.size = 0;
		opt.type = 0;
		opt.used = 0;
		opt.volume = 0;
		opt.domain = 0;
		opt.available = 0;
		opt.all = 0;
		opt.bus = 0;
	}
	*o = 1;
}

/* move to list.h */

struct _list {
	void			*data;
	struct _list	*next;
};

typedef struct _list List;

/* move to list.c */

static void error(char *fmt, ...) {
	va_list args;

	fprintf(stderr, "%s: ", progname);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	if (errno) {
		fprintf(stderr, ": ");
		fprintf(stderr, strerror(errno));
	}
	fprintf(stderr, "\n");
	fflush(stderr);
	va_end(args);
	if (errno)
		exit(errno);
}

static inline void *xalloc(int size) {
	void *p = malloc(size);
	if (!p) {
		error("out of memory");
		exit(ENOMEM);
	}

	return p;
}

static List *list_new(void *data) {
	List *node = (List *) xalloc(sizeof(List));

	node->data = data;
	node->next = NULL;

	return node;
}

static List *list_last(List *node) {
	if (!node)
		return NULL;

	while (node->next)
		node = node->next;

	return node;
}

static List *list_append(List **head, void *data) {
	List *node = list_new(data);

	if (*head == NULL)
		*head = node;
	else {
		List *tail = list_last(*head);
		tail->next = node;
	}

	return node;
}

static void list_free(List *node) {
	while (node) {
		List *next = node->next;
		free(node);
		node = next;
	}
}

/* @todo move to shared lib */

static char* str_trim(char *p) {
	char *pend;

	while (isspace(*p))
		++p;
	if (*p) {
		pend = p + strlen(p) - 1;
		while (isspace(*pend))
			--pend;
		++pend;
		*pend = '\0';
	}

	return p;
}

static int process_options(int argc, char **argv);

#define COMMENT_DELIMETER "//"
#define TOKEN_DELIMETERS " \t"

static int process_envvar(char *envvar) {
	List *argv_list = NULL;
	int argc = 1;
	char **argv;
	List *node;
	char **p;

	char *pstart;
	char *pend;
	char *token;

	pstart = getenv(envvar);

	if (!pstart || !*pstart) {
		return 0;
	}

	pend = strstr(pstart, COMMENT_DELIMETER);
	if (pend)
		*pend = '\0';

	pstart = str_trim(pstart);

	if (strlen(pstart) == 0)
		return 0;

	token = strtok(pstart, TOKEN_DELIMETERS);

	while (token != NULL) {
		token = str_trim(token);

		if (strlen(token)) {
			++argc;
			list_append(&argv_list, (void *) token);
		}

		token = strtok(NULL, TOKEN_DELIMETERS);
	}

	if (argc == 1)
		return 0;

	argv = (char **) xalloc((argc + 1) * sizeof(char *));
	p = argv;
	*p++ = progname;
	node = argv_list;
	while (node) {
		*p++ = (char *) node->data;
		node = node->next;
	}
	*p = NULL;

	process_options(argc, argv);

	free(argv);

	list_free(argv_list);

	return 0;
}

int process_options(int argc, char **argv) {
	static int seen_include = 0;
	char *cpu;
	int i;

	opterr = 0;
	optind = 1;
	optreset = 1;

	int dr = 0;

	while (1) {
		int c;
		int option_index = 0;

		if (optind < argc && argv[optind] && argv[optind][0] == '/')
			argv[optind][0] = '-';

		c = getopt_long(argc, argv, short_options, long_options, &option_index);

		if (opterr) {
			usage();
			exit(1);
		}

		if (c == -1)
			break;

		switch (c) {
			case 'a':
				opt.available = 1;
				break;

			case 'A':
				opt.all = 1;
				break;

			case 'b':
				set_option(&opt.bus, optarg);
				break;

			case 'd':
				set_option(&opt.domain, optarg);
				break;

			case 'e':
				if (!optarg || !*optarg) {
					usage();
					exit(1);
				}
				dr = tolower(*optarg) - 'a';
				if (dr < 0 || dr > 25) {
					usage();
					exit(1);
				}
				opt.drives[dr] = 0;
				break;

			case 'f':
				set_option(&opt.free, optarg);
				break;

			case 'i':
				if (!optarg || !*optarg) {
					usage();
					exit(1);
				}
				if (!seen_include) {
					++seen_include;
					for (i = 0; i < 26; ++i) {
						opt.drives[i] = 0;
					}
				}
				dr = tolower(*optarg) - 'a';
				if (dr < 0 || dr > 25) {
					usage();
					exit(1);
				}
				opt.drives[dr] = 1;
				break;


			case 'k':
				opt.kilo = 1;
				break;

			case 'm':
				set_option(&opt.file_system, optarg);
				break;

			case 'n':
				set_option(&opt.name, optarg);
				break;

			case 's':
				set_option(&opt.size, optarg);
				break;

			case 't':
				set_option(&opt.type, optarg);
				break;

			case 'u':
				set_option(&opt.used, optarg);
				break;

			case 'v':
				set_option(&opt.volume, optarg);
				break;

			case 'V': // version
#if _WIN64 || __amd64__ || __X86_64__
				cpu = "64 bit";
#else
				cpu = "32 bit";
#endif
				printf("%s - Version %s - %s (%s version)\n", APPNAME, APPVERSION, __DATE__, cpu);
#ifdef _DEBUG
				printf("*** Debug Build ***\n");
#endif

				printf(APPCOPYRIGHT "\n\n");

				printf(
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n");

				exit(0);
				break;

			case '?': // help
				usage();
				exit(0);
				break;

			default:
				usage();
				exit(1);
		}
	}

	return 0;
}

int main(int argc, char **argv) {
#ifdef _DEBUG_ALLOC
    InitAllocCheck();
#endif
	unsigned int i;

	SetErrorMode(SEM_NOOPENFILEERRORBOX);

	progname = basename(argv[0]);

	unsigned int len = (unsigned int) strlen(progname);
	if (len > 4 && _stricmp(progname + len - 4, ".exe") == 0)
		progname[len - 4] = '\0';

	process_envvar("LSDSK");

	process_options(argc, argv);
	std::map<int,std::string> types;

	types[DRIVE_UNKNOWN]		= "Unknown";
	types[DRIVE_NO_ROOT_DIR]	= "Unmounted";
	types[DRIVE_REMOVABLE]		= "Removable";
	types[DRIVE_FIXED]			= "Fixed";
	types[DRIVE_REMOTE]			= "Network";
	types[DRIVE_CDROM]			= "CD-ROM";
	types[DRIVE_RAMDISK]		= "RAMDisk";
	types[DRIVE_OTHER]			= "Other";
	types[DRIVE_SUBST]			= "Subst";

	std::map<int, std::string> busTypes;

    busTypes[BusTypeUnknown]	= "Unknown";
    busTypes[BusTypeScsi]		= "SCSI";
    busTypes[BusTypeAtapi]		= "ATAPI";
    busTypes[BusTypeAta]		= "ATA";
    busTypes[BusType1394]		= "1394";
    busTypes[BusTypeSsa]		= "SSA";
    busTypes[BusTypeFibre]		= "Fibre";
    busTypes[BusTypeUsb]		= "USB";
    busTypes[BusTypeRAID]		= "RAID";
    busTypes[BusTypeiScsi]		= "iSCSI";
    busTypes[BusTypeSas]		= "SAS";
    busTypes[BusTypeSata]		= "SATA";
#ifdef BusTypeSd
    busTypes[BusTypeSd]			= "SD";
#endif
#ifdef BusTypeMmc
    busTypes[BusTypeMmc]		= "MMC";
#endif
#ifdef BusTypeVirtual
    busTypes[BusTypeVirtual]	= "Virtual";
#endif
#ifdef BusTypeFileBackedVirtual
    busTypes[BusTypeFileBackedVirtual] = "vFile";
#endif
#ifdef BusTypeMax
    busTypes[BusTypeMax] = "Max";
#endif

	unsigned int width[8];

// 9 + 15 + 4 + 4 + 4 + 5 + 20 + 7 = 79

	width[OPT_TYPE]			= 9;
	width[OPT_VOLUME]		= 15;
	width[OPT_USED]			= 4;
	width[OPT_FREE]			= 4;
	width[OPT_SIZE]			= 4;
	width[OPT_FILE_SYSTEM]	= 5;
	width[OPT_NAME]			= 20;
	width[OPT_BUS]			= 7;

	printf("Dr");

	if (opt.type) {
		printf(" ");
		printf("%-*s", width[OPT_TYPE], "Type");
	}

	if (opt.bus) {
		printf(" ");
		printf("%-*s", width[OPT_BUS], "Bus");
	}

	if (opt.volume) {
		printf(" ");
		printf("%-*s", width[OPT_VOLUME], "Volume");
	}

	if (opt.used) {
		printf(" ");
		printf("%*s", width[OPT_USED], "Used");
	}

	if (opt.free) {
		printf(" ");
		printf("%*s", width[OPT_FREE], "Free");
	}

	if (opt.size) {
		printf(" ");
		printf("%*s", width[OPT_SIZE], "Size");
	}

	if (opt.file_system) {
		printf(" ");
		printf("%-*s", width[OPT_FILE_SYSTEM], "FS");
	}

	if (opt.name) {
		printf(" ");
		printf("%-*s", width[OPT_NAME], "Name");
	}

	printf("\n");

	printf("--");

	if (opt.type) {
		printf(" ");
		for (i = 0; i < width[OPT_TYPE]; ++i)
			printf("-");
	}

	if (opt.bus) {
		printf(" ");
		for (i = 0; i < width[OPT_BUS]; ++i)
			printf("-");
	}

	if (opt.volume) {
		printf(" ");
		for (i = 0; i < width[OPT_VOLUME]; ++i)
			printf("-");
	}

	if (opt.used) {
		printf(" ");
		for (i = 0; i < width[OPT_USED]; ++i)
			printf("-");
	}

	if (opt.free) {
		printf(" ");
		for (i = 0; i < width[OPT_FREE]; ++i)
			printf("-");
	}

	if (opt.size) {
		printf(" ");
		for (i = 0; i < width[OPT_SIZE]; ++i)
			printf("-");
	}

	if (opt.file_system) {
		printf(" ");
		for (i = 0; i < width[OPT_FILE_SYSTEM]; ++i)
			printf("-");
	}

	if (opt.name) {
		printf(" ");
		for (i = 0; i < width[OPT_NAME]; ++i)
			printf("-");
	}

	printf("\n");

	for (i = 0; i < 26; ++i) {
		char c = 'A' + i;
		drive[i].letter = c;
		drive[i].root = drive[i].letter + ":\\";
		drive[i].type = "";
		drive[i].bus = "";
		drive[i].volume = "";
		drive[i].name = "";
		drive[i].file_system = "";
		drive[i].FreeBytesAvailable.QuadPart = 0ui64;
		drive[i].TotalNumberOfBytes.QuadPart = 0ui64;
		drive[i].TotalNumberOfFreeBytes.QuadPart = 0ui64;
	}

	typedef std::list <std::string> StringList;

	StringList freelist;

	while (1) {
	//	USE_INFO_2 ui2[26];

		unsigned char *BufPtr = NULL;
		DWORD EntriesRead = 0;
		DWORD TotalEntries = 0;
		DWORD ResumeHandle = NULL;

		NET_API_STATUS rv3 = NetUseEnum(
			NULL,					// LMSTR UncServerName,
			2,						// DWORD Level,
			&BufPtr,
			MAX_PREFERRED_LENGTH,	// DWORD PreferedMaximumSize,
			&EntriesRead,
			&TotalEntries,
			&ResumeHandle
		);
		if (rv3 != NERR_Success) {
			fprintf(stderr, "%s\n", last_error());
			if (BufPtr) {
				NetApiBufferFree(BufPtr);
			}
			break;
		}

		unsigned char *p = BufPtr;

		for (i = 0; i < EntriesRead; ++i) {
			PUSE_INFO_2 ui = (USE_INFO_2*) p;
			char arg[32767];
			int len = WideCharToMultiByte(CP_ACP, 0, ui->ui2_local, -1, arg, sizeof(arg), NULL, NULL);
			char dri = toupper(arg[0]);
			if (dri >= 'A') {
				int index = dri - 'A';

	//			ui2[index].ui2_local = ui->ui2_local;
	//			ui2[index].ui2_remote = ui->ui2_remote;
	//			ui2[index].ui2_username = ui->ui2_username;
	//			ui2[index].ui2_domainname = ui->ui2_domainname;

				char *remote = (char*) malloc(wcslen(ui->ui2_remote) * sizeof(char));
				len = WideCharToMultiByte(CP_ACP, 0, ui->ui2_remote, -1, remote, sizeof(arg), NULL, NULL);
				drive[index].name = remote;

				if (opt.domain) {
					drive[index].name += " (";

					if (ui->ui2_domainname) {
						char *domainname = (char*) malloc(wcslen(ui->ui2_domainname) * sizeof(char));
						len = WideCharToMultiByte(CP_ACP, 0, ui->ui2_domainname, -1, domainname, sizeof(arg), NULL, NULL);
						if (strstr(drive[index].name.c_str(), domainname) == 0)
							drive[index].name = drive[index].name + domainname + "\\";
					}

					char *username = (char*) malloc(wcslen(ui->ui2_username) * sizeof(char));
					len = WideCharToMultiByte(CP_ACP, 0, ui->ui2_username, -1, username, sizeof(arg), NULL, NULL);
					drive[index].name = drive[index].name + username;

					drive[index].name = drive[index].name + ")";
				}
			}
			p += sizeof(USE_INFO_2);
		}

		if (BufPtr) {
			NetApiBufferFree(BufPtr);
		}


		if (!ResumeHandle)
			break;
	}

//	NetApiBufferFree(&BufPtr);

	for (i = 0; i < 26; ++i) {
		if (opt.drives[i] == 0)
			continue;
		char buf[1024];
		std::string dr = drive[i].letter + ":";
		int rv2;

		UINT rv = GetDriveType(drive[i].root.c_str());
		switch (rv) {
			case DRIVE_UNKNOWN: // 	The drive type cannot be determined.
			case DRIVE_NO_ROOT_DIR: // 	The root path is invalid, for example, no volume is mounted at the path.
			case DRIVE_REMOVABLE: // 	The drive is a type that has removable media, for example, a floppy drive or removable hard disk.
			case DRIVE_CDROM: // 	The drive is a CD-ROM drive.
			case DRIVE_RAMDISK: // 	The drive is a RAM disk.
				drive[i].type = types[rv];
				break;
			case DRIVE_REMOTE: // 	The drive is a remote (network) drive.
			case DRIVE_FIXED: // 	The drive is a type that cannot be removed, for example, a fixed hard drive.
				drive[i].type = types[rv];

				rv2 = QueryDosDevice(dr.c_str(), buf, sizeof(buf));
				if (rv2) {
					if (strstr(buf, "RAM") || strstr(buf, "Ram") || strstr(buf, "ImDisk")) {
						drive[i].type = types[DRIVE_RAMDISK];
					}
					if (drive[i].name.length() == 0) {
						drive[i].name = buf;
						if (drive[i].name.substr(0, 3) == "\\??") {
							drive[i].name = drive[i].name.substr(4);
						}
						std::string lanman = "\\Device\\LanmanRedirector";
						int j = drive[i].name.find(lanman);
						if (j >= 0) {
							std::string p = drive[i].name.substr(lanman.length() + 1);
							int k = p.find("\\");
							if (k) {
								p = "\\" + p.substr(k);
							}
							drive[i].name = p;
						}
					}
					if (strcspn(drive[i].name.c_str(), ":") == 1) {
						drive[i].type = types[DRIVE_SUBST];
					}
				}
				break;
			default:
				drive[i].type = types[DRIVE_OTHER];
		}

		if (rv == DRIVE_NO_ROOT_DIR) {
			if (opt.available) {
				freelist.push_back(dr);
//				printf("%-2s (free)\n", dr.c_str());
			}
			if (!opt.all) {
				continue;
			}
		}

		if (drive[i].type == types[DRIVE_REMOVABLE]) {
			// see http://msdn.microsoft.com/en-us/library/aa364939.aspx
			// szPath without trailing backslash like
			//  "\\\\.\\X:"
			//  "\\\\\?\\Volume{433619ed-c6ea-11d9-a3b2-806d6172696f}
			//  "\\\\.\\PhysicalDrive0"

			std::string szPath = "\\\\.\\" + dr;

			HANDLE hDevice = CreateFile(szPath.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
										NULL, OPEN_EXISTING, 0, NULL);

			if ( hDevice != INVALID_HANDLE_VALUE ) {

				DWORD dwOutBytes = 0;           // IOCTL output length
#ifdef STORAGE_PROPERTY_QUERY
				STORAGE_PROPERTY_QUERY Query;   // input param for query

				// specify the query type
				Query.PropertyId = StorageDeviceProperty;
				Query.QueryType = PropertyStandardQuery;

				char OutBuf[1024] = {0};  // good enough, usually about 100 bytes
				PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)OutBuf;
				pDevDesc->Size = sizeof(OutBuf);

				// Query using IOCTL_STORAGE_QUERY_PROPERTY
				BOOL res = DeviceIoControl(hDevice,                     // device handle
							 IOCTL_STORAGE_QUERY_PROPERTY,             // info of device property
							 &Query, sizeof(STORAGE_PROPERTY_QUERY),  // input data buffer
							 pDevDesc, pDevDesc->Size,               // output data buffer
							 &dwOutBytes,                           // out's length
							 (LPOVERLAPPED)NULL);

				CloseHandle(hDevice);

				if ( res ) {
					// here we are
					if (pDevDesc->BusType) {
						drive[i].bus = busTypes[pDevDesc->BusType];
					}
				}
#endif
			}
		}

		printf("%-2s", dr.c_str());

		if (opt.type) {
			printf(" ");
			printf("%-*s", width[OPT_TYPE], drive[i].type.c_str());
		}

		if (rv == DRIVE_NO_ROOT_DIR) {
			if (!opt.all) {
				printf("\n");
				continue;
			}
		}

		char VolumeNameBuffer[MAX_PATH + 1];;
		DWORD VolumeSerialNumber;
		DWORD MaximumComponentLength;
		DWORD FileSystemFlags;
		char FileSystemNameBuffer[MAX_PATH + 1];

		BOOL b = GetVolumeInformation(
			drive[i].root.c_str(),
			VolumeNameBuffer,
			sizeof(VolumeNameBuffer),
			&VolumeSerialNumber,
			&MaximumComponentLength,
			&FileSystemFlags,
			FileSystemNameBuffer,
			sizeof(FileSystemNameBuffer)
		);

		if (b) {
			drive[i].volume = str_trim(VolumeNameBuffer);
			drive[i].file_system = str_trim(FileSystemNameBuffer);
			if (drive[i].file_system.length() > width[OPT_FILE_SYSTEM]) {
				drive[i].file_system = drive[i].file_system.substr(0, width[OPT_FILE_SYSTEM]);
			}
		}

		LPCTSTR lpDirectoryName = drive[i].root.c_str();

		ULARGE_INTEGER FreeBytesAvailable;
		ULARGE_INTEGER TotalNumberOfBytes;
		ULARGE_INTEGER TotalNumberOfFreeBytes;

		FreeBytesAvailable.QuadPart = 0UL;
		TotalNumberOfBytes.QuadPart = 0UL;
		TotalNumberOfFreeBytes.QuadPart = 0UL;

/*
		HANDLE hnd = CreateFile(
lpDirectoryName,
0, // dwDesiredAccess,
7, // dwShareMode,
NULL, // SecurityAttributes,
OPEN_EXISTING, // dwFlagsAndAttributes
0, // dwFlagsAndAttributes,
NULL // hTemplateFile
);
*/

		bool getFreeSpace = drive[i].type != types[DRIVE_CDROM] || drive[i].volume.length() > 0;

		if (getFreeSpace) {
			b = GetDiskFreeSpaceEx(
				lpDirectoryName,
				&FreeBytesAvailable,
				&TotalNumberOfBytes,
				&TotalNumberOfFreeBytes
			);

			if (b) {
				drive[i].FreeBytesAvailable = FreeBytesAvailable;
				drive[i].TotalNumberOfBytes = TotalNumberOfBytes;
				drive[i].TotalNumberOfFreeBytes = TotalNumberOfFreeBytes;
			}
		}

		if (opt.bus) {
			printf(" ");
			printf("%-*s", width[OPT_BUS], drive[i].bus.c_str());
		}

		if (opt.volume) {
			printf(" ");
			printf("%-*s", width[OPT_VOLUME], drive[i].volume.c_str());
		}

		if (opt.used) {
			printf(" ");
			ULARGE_INTEGER used;
			used.QuadPart = drive[i].TotalNumberOfBytes.QuadPart - drive[i].TotalNumberOfFreeBytes.QuadPart;
			std::string s = int64tostr(used, opt.kilo);
			if (!getFreeSpace) {
				s = "";
			}
			printf("%-*s", width[OPT_USED], s.c_str());
		}

		if (opt.free) {
			printf(" ");
			std::string s = int64tostr(drive[i].TotalNumberOfFreeBytes, opt.kilo);
			if (!getFreeSpace) {
				s = "";
			}
			printf("%-*s", width[OPT_FREE], s.c_str());
		}

		if (opt.size) {
			printf(" ");
			std::string s = int64tostr(drive[i].TotalNumberOfBytes, opt.kilo);
			if (!getFreeSpace) {
				s = "";
			}
			printf("%-*s", width[OPT_SIZE], s.c_str());
		}

		if (opt.file_system) {
			printf(" ");
			printf("%-*s", width[OPT_FILE_SYSTEM], drive[i].file_system.c_str());
		}

		if (opt.name) {
			printf(" ");
			if (!opt.volume && drive[i].name.find("\\Device") == 0 && drive[i].volume.length() > 0) {
				printf("%-*s", width[OPT_NAME], drive[i].volume.c_str());
			} else if (drive[i].name.length() > 0) {
				printf("%-*s", width[OPT_NAME], drive[i].name.c_str());
			} else if (!opt.volume && drive[i].volume.length() > 0) {
				printf("%-*s", width[OPT_NAME], drive[i].volume.c_str());
			} else if (!opt.type) {
				printf("%-*s", width[OPT_NAME], drive[i].type.c_str());
			}
		}

		printf("\n");
	}

	if (opt.available) {
		printf("\n");
		printf("Free: ");
		i = 0;
		for (StringList::iterator it = freelist.begin(); it != freelist.end(); ++it) {
			if (i++)
				printf(" ");
			printf("%s", ((std::string)*it).c_str());
		}
		printf("\n");
	}

//	_getch();
    return 0;
}
