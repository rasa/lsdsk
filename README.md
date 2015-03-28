# lsdsk [![Flattr this][flatter_png]][flatter]

List mounted disks.

## Usage

````
lsdsk [options]
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
````

## Examples

````batch
C:\>lsdsk
Dr Type      Bus     Volume          Used Free Size FS    Name
-- --------- ------- --------------- ---- ---- ---- ----- -----------------------------
B: RAMDisk           RAMDisk         3.0K 1.0G 1.0G FAT   \Device\RAMDisk
C: Fixed             example_c       7.1G 439M 7.1G NTFS  \Device\HarddiskVolume1
D: Fixed             example_d        45G 2.1G  47G NTFS  \Device\HarddiskVolume3
E: CD-ROM            EXAMPLEVOL      535M   0  535M CDFS
F: Fixed             EXAMPLE_F       288K 131M 131M FAT32 \Device\HarddiskVolume2
G: Removable USB     usb_stick        28M 221M 249M exFAT
H: Removable ATA     domain.com        0   42G  42G EXFS
I: Subst             example_d        45G 2.1G  47G NTFS  D:\a\path\on\d
M: Subst             example_c       7.1G 439M 7.1G NTFS  C:\User\Example\Documents
Y: Network           sharename       123M 456M 579M NTFS  \\servername\sharename (username)
Z: Network           c$              177M 782M 959M NTFS  \\servername\c$ (DOMAIN\user)

C:\>lsdsk -na
Dr Name
-- -----------------------------
B: RAMDisk
C: example_c
D: example_d
E: EXAMPLEVOL
F: EXAMPLE_F
G: usb_stick
I: D:\a\path\on\d
M: C:\User\Example\Documents
Y: \\servername\sharename
Z: \\servername\c$

Free: A: H: J: K: L: N: O: P: Q: R: S: T: U: V: W: X:

C:\>set LSDSK=-na
C:\>lsdsk

(same as above)
````

## Verify a Release

To verify a release, download the .zip, .sha256, and .asc files for the release 
(replacing lsdsk-1.8-win32.zip with the release you are verifying):

````
$ wget https://github.com/rasa/lsdsk/releases/download/v1.8/lsdsk-1.8-win32.zip{,.sha256,.asc}
````

Next, check that sha256sum reports "OK":
````
$ sha256sum -c lsdsk-1.8-win32.zip.sha256
lsdsk-1.8-win32.zip: OK
````

Lastly, check that GPG reports "Good signature":

````
$ gpg --keyserver hkps.pool.sks-keyservers.net --recv-key 0x105a5225b6ab4b22
$ gpg --verify lsdsk-1.8-win32.zip.asc lsdsk-1.8-win32.zip
gpg:                using RSA key 0xFF914F74B4BB6EF3
gpg: Good signature from "Ross Smith II <ross@smithii.com>" [ultimate]
...
````

## Contributing

To contribute to this project, please see [CONTRIBUTING.md](CONTRIBUTING.md).

## Bugs

To view existing bugs, or report a new bug, please see [issues](../../issues).

## Changelog

To view the version history for this project, please see [CHANGELOG.md](CHANGELOG.md).

## License

This project is [MIT licensed](LICENSE).

## Contact

This project was created and is maintained by [Ross Smith II][] [![endorse][endorse_png]][endorse]

Feedback, suggestions, and enhancements are welcome.

[Ross Smith II]: mailto:ross@smithii.com "ross@smithii.com"
[flatter]: https://flattr.com/submit/auto?user_id=rasa&url=https%3A%2F%2Fgithub.com%2Frasa%2Flsdsk
[flatter_png]: http://button.flattr.com/flattr-badge-large.png "Flattr this"
[endorse]: https://coderwall.com/rasa
[endorse_png]: https://api.coderwall.com/rasa/endorsecount.png "endorse"

