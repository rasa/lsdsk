lsdsk changelog

Version 1.6 - 02 Oct 11

	Fixed crash if domain is null
	Added Release version to zip files
	Added .ico and .rc files to source zip file
	Added shared lib to source zip file (unzip into lsdsk/../shared)
	Added support for ImDisk Virtual Disk Driver (

Version 1.5 - 26 Sep 11

	Updated to Visual Studio 2010 Express
	Added option: -A | --all Display both mounted and unmounted drives
	Added option: -b | --bus Display device bus type (USB, 1394, etc.)
	Fixed crash for 'net use \\host\ipc$' type shares
	Trimed file system value to width of field
	Changed -k to show size suffix as lowercase
	Display volume in name field if volume option not selected, or name is blank

Version 1.4 - 04 Apr 07:

	Better error reporting

Version 1.3 - 12 Jul 06:

	Standardized version information across all programs

Version 1.2 - 08 Apr 06:

	Added option: -e | --exclude d:  Exclude drive d:
	Added option: -i | --include d:  Include drive d: (and exclude all others)
	Added .rc and .ico files
	Improved build scripts
	Updated documentation

Version 1.1 - 01 Mar 06:

    Added option: -d | --domain Display domain and username for network shares
    Added option: -k | --kilobyte Use 1024 for a kilobyte (K) instead of 1000
    Added option: -e | --available Display available drive letters (default is off)
    Fixed used/free/size amounts so they round correctly

Version 1.0 - 18 Feb 06:

	Initial release