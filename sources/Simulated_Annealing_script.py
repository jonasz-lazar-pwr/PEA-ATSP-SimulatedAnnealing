import subprocess
import os
import shlex

# Pobieramy ścieżkę do bieżącego folderu, w którym znajduje się skrypt (Folder_1)
current_folder = os.path.dirname(os.path.realpath(__file__))

# Cofamy się o jeden poziom folderu
parent_folder = os.path.dirname(current_folder)

# Dołączamy ścieżkę do parameters_file i pliku parametry.txt
file_path = os.path.join(parent_folder, 'parameters_files', 'parameters_SA.txt')

# Otwieramy plik z parametrami
with open(file_path, 'r') as file:
    lines = file.readlines()

# Iterujemy przez linie pliku
for line in lines:
    # Usuwamy białe znaki z początku i końca linii
    line = line.strip()

    # Dzielimy linię na parametry, używając shlex.split do uwzględnienia tabulatorów
    parameters = shlex.split(line)

    # Przygotowujemy listę argumentów, pierwszym argumentem jest nazwa programu
    arguments = ['Simulated_Annealing.exe']

    # Dodajemy każdy parametr jako osobny element do listy argumentów
    arguments.extend(parameters)

    # Wywołujemy program C++ przy użyciu subprocess
    subprocess.run(arguments, check=True)
