 #!/bin/bash         

gcc main.cpp graph.cpp analyse_lexical.c analyse_syntaxe.c evaluation.c -lGL -lGLU -lglut -lm -o Grapheur && ./Grapheur
