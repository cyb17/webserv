#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    // Chemin du répertoire à lire
    const char *chemin_repertoire = "./webSite";

    // Ouvrir le répertoire
    DIR *dir = opendir(chemin_repertoire);
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // Lire les entrées du répertoire
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("Nom de fichier: %s\n", entry->d_name);
    }

    // Fermer le répertoire
    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
