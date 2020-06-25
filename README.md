# Description
**Warning** - This application was made in 2016. I am importing this project to GitHub from GFL's GitLab server.

A small C++ program that automatically **hard links** all files and subfolders in a directory. I use this program on my Windows Home Server (2008) to make game servers. Since it hard links the files, they don't take up any extra hard drive space.

# Why Hard Link Directories/Files?
* Takes up less space - E.g. Having all the servers in one game **linked** to **one** installation base is much better than having each server have its own installation (~14GB each for CS:GO).
* If you want to update a game like CS:GO, all you have to do is update the one installation base for the game. Although, you must relink the files that have been changed in the update. I have a update.bat file (in the tools folder) that automatically relinks all the installation files for each game server after updating the game.

# Command Line
* **--config <configfile>** - Specify a configuration file (default is config.ini).
* **--name <linkName>** - The link name. If this isn't specified, you will be asked when the program starts up.
* **-remove** - Removes the hard linked files (useful for having to relink the files).
* **-force** - The program will never "pause" (excluding if you must specify a link name at the beginning).