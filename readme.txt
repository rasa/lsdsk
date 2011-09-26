lsdsk - Version 1.5 - Sep 26 2011
Copyright (c) 2005-2011 Ross Smith II (http://smithii.com) All Rights Reserved

------------------------------------------------------------------------------

Usage: lsdsk [options]
Options:
-t | --type        Display type of drive
-v | --volume      Display volume name of drive
-u | --used        Display used space on drive
-f | --free        Display free space remaining on drive
-s | --size        Display total size of drive
-m | --file_system Display file system type (NTFS, FAT32, etc.)
-b | --bus         Display device bus type (USB, 1394, etc.)
-n | --name        Display network share directory or subst'd directory
-d | --domain      Display domain and username for network shares
-e | --exclude d:  Exclude drive d:
-i | --include d:  Include drive d: (and exclude all others)
-k | --kilobyte    Use 1024 for a kilobyte (K) instead of 1000
-a | --available   Display available drive letters afterwards
-A | --all         Display both mounted and unmounted drives
-V | --version     Show version and copyright information and quit
-? | --help        Show this help message and quit
------------------------------------------------------------------------------

Examples:

C:\>lsdsk
Dr Type      Volume          Used Free Size FS    Name
-- --------- --------------- ---- ---- ---- ----- -----------------------------
B: RAMDisk   RAMDisk         3.0K 1.0G 1.0G FAT   \Device\RAMDisk
C: Fixed     example_c       7.1G 439M 7.1G NTFS  \Device\HarddiskVolume1
D: Fixed     example_d        45G 2.1G  47G NTFS  \Device\HarddiskVolume3
E: CD-ROM    EXAMPLEVOL      535M   0  535M CDFS
F: Fixed     EXAMPLE_F       288K 131M 131M FAT32 \Device\HarddiskVolume2
G: Removable usb_stick        28M 221M 249M exFAT
I: Subst     example_d        45G 2.1G  47G NTFS  D:\a\path\on\d
M: Subst     example_c       7.1G 439M 7.1G NTFS  C:\User\Example\Documents
Y: Network   sharename       123M 456M 579M NTFS  \\servername\sharename (username)
Z: Network   c$              177M 782M 959M NTFS  \\servername\c$ (DOMAIN\user)

C:\>lsdsk -tna -e b
Dr Type      Name
-- --------- -----------------------------
C: Fixed     \Device\HarddiskVolume1
D: Fixed     \Device\HarddiskVolume3
E: CD-ROM
F: Fixed     \Device\HarddiskVolume2
G: Removable
I: Subst     D:\a\path\on\d
M: Subst     C:\Documents and Settings\ex\My Documents
Y: Network   \\servername\sharename (username)
Z: Network   \\servername\c$ (DOMAIN\user)

Free: A: H: J: K: L: N: O: P: Q: R: S: T: U: V: W: X:

------------------------------------------------------------------------------

C:\>set LSDSK=-tna -e b
C:\>lsdsk

(same as above)

------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

$Id$
