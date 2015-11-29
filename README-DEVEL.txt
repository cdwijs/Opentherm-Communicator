Developers guide windows 7:
)Install kicad from here:
http://iut-tice.ujf-grenoble.fr/cao/
http://iut-tice.ujf-grenoble.fr/cao/KiCad_stable-2013.07.07-BZR4022_Win_full_version.exe
)Install tortoisegit:
http://code.google.com/p/tortoisegit/wiki/Download
http://download.tortoisegit.org/tgit/1.8.7.0/TortoiseGit-1.8.7.0-64bit.msi
)install mplab X or mplab 8.x
http://www.microchip.com/pagehandler/en-us/family/mplabx/
http://www.microchip.com/mplabx-ide-windows-installer
)get the opentherm-communicator files from sourceforge.
Right click on an empty folder, git clone...
URL: git://git.code.sf.net/p/ot-communicator/code [Web]
[]depth []recursive []clone into bare repo []no checkout
[]branch []origin name
[] load putty key   [...]
[] from SVN repository

output:
"
git.exe clone      --progress -v  "git://git.code.sf.net/p/ot-communicator/code" "U:\Users\cedric\Cedric\Memories\2014-ot-communicator\code"

Cloning into 'U:\Users\cedric\Cedric\Memories\2014-ot-communicator\code'...
remote: Counting objects: 212, done.
Receiving objects: 100% (212/212), 210.24 KiB | 184.00 KiB/s, done.
Resolving deltas: 100% (88/88), done.
remote: Compressing objects: 100% (202/202), done.
remote: Total 212 (delta 88), reused 0 (delta 0)

Success (9267 ms @ 3/8/2014 9:52:19 AM)
"
Now the following folder structure should have been created:
code
|Pcb
||Prototyping_pcb
|Source
||Pic 18
|||opentherm-test1.X

To alter the PCB, start kicad, and choose open-> code\Pcb\Prototyping_pcb\ot-communicator.pro

