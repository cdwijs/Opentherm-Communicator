Create a github account, and fork the repository
** windows **
Getting the source and contribute changes
Developers guide (Windows 7)

================================================
Create a github account, and fork the repository
================================================
Go to github.com, create an account, and login.
Search for Opentherm-Communicator, and goto the repository.
https://github.com/cdwijs/Opentherm-Communicator
Next click "fork" at the top-left, then click fork.
Copy the "HTTPS clone URL" line at the right side, it should be something like this:
https://github.com/username/Opentherm-Communicator.git

===================================================
Getting the source and contribute changes (Windows)
===================================================
Download and install tortoisegit from here:
https://code.google.com/p/tortoisegit/wiki/Download

create a new folder somewhere on your hard disk.
right click the folder, choose "Git Clone..."
paste the "HTTPS clone URL" line into the line "URL:" it should be something like this:
https://github.com/username/Opentherm-Communicator.git
click OK. the repository is now cloned into the folder on your hard disk.

When you have edited files, right click on the folder, choose commit -> master.
You will be asked to enter information why you made the changes. This information is then stored locally on your computer.
Next, publish the changes you have made to github by rightclick, and push. You will be asked for your github username and password. Now the changes are transferred to your repository on github.

Finally, login to github, navigate to your fork of CatanScoreboard, and click "Pull request", and then click "Create pull request"
Provide a meaningful summary and description, then click "Create pull request". Now the developers are notified of your changes.

============================
Developers guide (Windows 7)
============================
)Install kicad from here:
http://iut-tice.ujf-grenoble.fr/cao/
http://iut-tice.ujf-grenoble.fr/cao/KiCad_stable-2013.07.07-BZR4022_Win_full_version.exe
)Install tortoisegit:
http://code.google.com/p/tortoisegit/wiki/Download
http://download.tortoisegit.org/tgit/1.8.7.0/TortoiseGit-1.8.7.0-64bit.msi
)install mplab X (or mplab 8.x)
http://www.microchip.com/pagehandler/en-us/family/mplabx/
http://www.microchip.com/mplabx-ide-windows-installer
)get the opentherm-communicator files from github.
Right click on an empty folder, git clone...
URL: https://github.com/cdwijs/Opentherm-Communicator.git [Web]
[]depth []recursive []clone into bare repo []no checkout
[]branch []origin name
[] load putty key   [...]
[] from SVN repository

output:
"
git.exe clone     --progress -v  "https://github.com/cdwijs/Opentherm-Communicator.git" "C:\Documents and Settings\Administrator\Desktop\ot-nov2005\test-werkmap\Opentherm-Communicator"

Cloning into 'C:\Documents and Settings\Administrator\Desktop\ot-nov2005\test-werkmap\Opentherm-Communicator'...
POST git-upload-pack (200 bytes)
remote: Counting objects: 241, done.
remote: Compressing objects: 100% (125/125), done.
Receiving objects: 100% (241/241), 217.03 KiB | 59 KiB/s, done.
Resolving deltas: 100% (111/111), done.
remote: Total 241 (delta 111), reused 236 (delta 106), pack-reused 0

Success (6328 ms @ 11/29/2015 3:05:17 PM)

"
Now the following folder structure should have been created:
Opentherm-Communicator
|Pcb
||Prototyping_pcb
|Source
||Pic 18
|||opentherm-test1.X

To alter the PCB, start kicad, and choose open-> code\Pcb\Prototyping_pcb\ot-communicator.pro

