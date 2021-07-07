#include "headers/Physical space.h"

int scan_sphere_data(FILE* input_file, Sphere* array_of_spheres){

    int counter = 0;

    while (fgetc_without_space(input_file) != '!'){

        counter++;
        fseek(input_file, -1, SEEK_CUR);

        array_of_spheres = (Sphere*)realloc(array_of_spheres, counter * sizeof(Sphere));
        array_of_spheres[counter - 1] = Sphere(input_file);
    }

    return counter;
}

int scan_flat_data(FILE* input_file, Flatness* array_of_flats){

    int counter = 0;

    while (fgetc_without_space(input_file) != '!'){
        
        counter++;
        fseek(input_file, -1, SEEK_CUR);

        array_of_flats = (Flatness*)realloc(array_of_flats, counter * sizeof(Flatness));
        array_of_flats[counter - 1] = Flatness(input_file);
    }

    return counter;
}


int scan_elastic_inter(FILE* input_file, Elastic_interaction* arr_elastic_inter, Sphere* array_of_spheres, int num_sphers){

    int counter = 0;

    while (fgetc_without_space(input_file) != '!'){

        counter++;
        fseek(input_file, -1, SEEK_CUR);

        arr_elastic_inter = (Elastic_interaction*)realloc(arr_elastic_inter, counter * sizeof(Elastic_interaction));
        arr_elastic_inter[counter - 1] = Elastic_interaction(input_file, array_of_spheres, num_sphers);//надо дописать так чтобы присваивал только безошибочный код
    }
    
    return counter;
}

Space::Space(FILE* input_file){

    number_of_fields = 0;
    //прописывание полей
    phys_fields = NULL;
    //конец прописывания

    assert(fgetc_without_space(input_file) == '!');
    array_of_spheres = NULL;
    number_of_spheres = scan_sphere_data(input_file, array_of_spheres);

    assert(fgetc_without_space(input_file) == '!');
    array_of_flats = NULL;
    number_of_flats = scan_flat_data(input_file, array_of_flats);

    assert(fgetc_without_space(input_file) == '!');
    arr_elastic_inter = NULL;
    number_of_elastic_inter = scan_elastic_inter(input_file, arr_elastic_inter, array_of_spheres, number_of_spheres);

}