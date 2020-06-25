#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <filesystem>
#include <experimental\filesystem>
#include "Config.h"
#include <string>
#include <sstream>
#include <vector>

/* My first attempt at a useful C++ program (for myself). */
/* Most of these functions were found online. Sadly, I don't have enough skill in C++ to create my own :\ */
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

namespace fs = std::experimental::filesystem;

void printError(std::string const &error)
{
	std::cout << "ERROR: " << error << "\n";
	getchar();
}

std::string royReplace(std::string &s, const std::string &toReplace, const std::string &replaceWith)
{
	return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}

	return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

int main(int iArgs, char* argv[])
{
	/* Configuration Strings. */
	std::string sSourceDir;
	std::string sLinkDir;
	std::string sIgnoreFiles;

	/* Mixed. */
	std::string sLinkName = "link1";

	/* Command Line Variables. */
	bool bRemoveFiles = false;
	bool bForce = false;
	std::string sConfigFile = "config.ini";
	bool bLinkNameCmdLine = false;

	for (int i = 1; i < iArgs; ++i)
	{
		/* Check for the --name option. */
		if (std::string(argv[i]) == "--name")
		{
			bLinkNameCmdLine = true;

			if (i + 1 < iArgs)
			{
				sLinkName = argv[++i];
			}
			else
			{
				std::cout << "--name requires one argument.";
				getchar();
				getchar();
				return 0;
			}
		}

		/* Check for the --config option. */
		if (std::string(argv[i]) == "--config")
		{
			if (i + 1 < iArgs)
			{
				sConfigFile = argv[++i];
			}
		}

		/* Check for the -remove option. */
		if (std::string(argv[i]) == "-remove")
		{
			bRemoveFiles = true;
		}

		/* Check for the -force option. */
		if (std::string(argv[i]) == "-force")
		{
			bForce = true;
		}
	}

	ConfigNameSpace::ConfigFile cfg(sConfigFile);

	if (!cfg.keyExists("sourcedir"))
	{
		printError("Could not find \"sourcepath\" key in config file.");
		return 0;
	}

	if (!cfg.keyExists("linkdir"))
	{
		printError("Could not find \"linkpath\" key in config file.");
		return 0;
	}

	/* Get values from the configuration file. */
	sSourceDir = cfg.getValueOfKey<std::string>("sourcedir", "source/");
	sLinkDir = cfg.getValueOfKey<std::string>("linkdir", "links/");
	sIgnoreFiles = cfg.getValueOfKey<std::string>("ignorefiles", "");


	if (!bLinkNameCmdLine)
	{
		std::cout << "Enter in a link name: ";
		std::cin >> sLinkName;
	}

	std::cout << "Source Dir: " << sSourceDir << "\n";
	std::cout << "Link Dir: " << sLinkDir << "\n";
	std::cout << "Link Name: " << sLinkName << "\n";
	std::cout << "Remove Files: " << ((bRemoveFiles) ? "Yes." : "No.") << "\n\n";

	if (!bForce)
	{
		std::cout << "Please confirm the above and hit enter..." << "\n";
		system("pause 2> nul");
	}

	/* Compile the ignore files array. */
	std::vector<std::string> vIgnoreFiles = split(sIgnoreFiles, ',');

	std::string sCmd = "";

	/* Create the folders needed! */
	sCmd = "mkdir " + sLinkDir + "\\" + sLinkName + " 2> nul";
	system(sCmd.c_str());

	if (!bRemoveFiles)
	{
		std::string sFolder;

		for (fs::recursive_directory_iterator i(sSourceDir), end; i != end; ++i)
		{
			if (is_directory(i->path()))
			{
				sFolder = royReplace(SSTR(i->path()), sSourceDir, "");

				sCmd = "mkdir " + sLinkDir + sLinkName + "\\" + sFolder + " 2> nul";
				system(sCmd.c_str());
				std::cout << "Created folder " << sFolder << "\n";
			}
		}

		std::cout << "\n\n";
	}

	/* Create the link! */
	std::string sFile;
	for (fs::recursive_directory_iterator i(sSourceDir), end; i != end; ++i)
	{
		if (!is_directory(i->path()))
		{
			/* This is a crappy solution, but it's the only thing I got right now. */
			sFile = royReplace(SSTR(i->path()), sSourceDir, "");

			/* Make sure the file isn't on the ignore list. */
			bool bIgnore = false;

			for (auto i : vIgnoreFiles)
			{
				if (sFile.compare(SSTR(i)) == 0)
				{
					std::cout << sFile << " is on the ignore list. Ignoring..." << "\n";
					bIgnore = true;
				}
			}

			if (bIgnore)
			{
				continue;
			}
			
			if (bRemoveFiles)
			{
				sCmd = "DEL /F /Q " + sLinkDir + sLinkName + "\\" + sFile;
				std::cout << "Removed file " << sLinkDir << sLinkName << "\\" << sFile << "\n";
			}
			else
			{
				sCmd = "mklink /H " + sLinkDir + sLinkName + "\\" + sFile + " " + sSourceDir + sFile;
			}
			//std::cout << sCmd << "\n\n\n";
			system(sCmd.c_str());
		}
	}

	if (!bForce)
	{
		getchar();
		getchar();
	}

	return 0;
}