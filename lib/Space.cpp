#include "headers/Physical space.h"

int scan_sphere_data(FILE* input_file, Sphere* array_of_spheres){

    int counter = 0;

    while (my_getc(input_file) != '!'){

        counter++;

        array_of_spheres = (Sphere*)realloc(array_of_spheres, counter * sizeof(Sphere));
        array_of_spheres[counter - 1] = Sphere(input_file);
    }

    fgetc(input_file);

    return counter;
}

int scan_flat_data(FILE* input_file, Flatness* array_of_flats){

    int counter = 0;

    while (my_getc(input_file) != '!'){
        
        counter++;

        array_of_flats = (Flatness*)realloc(array_of_flats, counter * sizeof(Flatness));
        array_of_flats[counter - 1] = Flatness(input_file);
    }

    fgetc(input_file);

    return counter;
    
}

Space::Space(FILE* input_file){

    number_of_param = 0;
    number_of_fields = 0;
    //прописывание полей
    phys_fields = NULL;
    //конец прописывания

    assert(my_getc(input_file) == '!');
    fgetc(input_file);
    array_of_spheres = NULL;
    number_of_spheres = scan_sphere_data(input_file, array_of_spheres);

    assert(my_getc(input_file) == '!');
    fgetc(input_file);
    array_of_flats = NULL;
    number_of_flats = scan_flat_data(input_file, array_of_flats);
}