#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int	deleteFolderRecursive (const std::string& dirPath)
{
	DIR* dir;
	if ((dir = opendir(dirPath.c_str())) == NULL)
		return (403);
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
        std::string tmp(entry->d_name);
		if (tmp.compare(".") == 0 || tmp.compare("..") == 0)
			continue ;
		tmp = dirPath + "/" + entry->d_name;
		struct stat buff;
		if (stat(tmp.c_str(), &buff) == 0)
		{
			if (S_ISDIR(buff.st_mode))
			{
				int code = deleteFolderRecursive(tmp);
				if (code != 200)
					return (closedir(dir), code);
			}
			else
			{
				if (remove(tmp.c_str()) != 0)
					return (closedir(dir), 500);
			}
		}
	}
	closedir(dir);
	if (rmdir(dirPath.c_str()) == -1)
		return (500);
	return (200);
}

int main() {
    // Chemin du répertoire à lire
    std::string chemin_repertoire = "./example";

    int code = deleteFolderRecursive(chemin_repertoire);
    if (code != 200)
        return (std::cout << "Fail\n", EXIT_FAILURE);
    std::cout << "Success\n";
    return EXIT_SUCCESS;
}
