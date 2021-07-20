#ifndef PHYS_SPACE
#define PHYS_SPACE

#include "Objects.h"

//-----------------------------------------------------------
class Field{

public:

    virtual Matrix return_force(Sphere& cur_obj); //возвращает силу
};
//-----------------------------------------------------------

//-----------------------------------------------------------
class Elastic_interaction{

private:

    Sphere* fir_obj;
    Sphere* sec_obj;

    float koef_k;
    float normal_distance;

public:

    Elastic_interaction(FILE* input_file, Sphere* array_of_spheres, int number_of_spheres);

    Elastic_interaction(const Elastic_interaction& old_obj) = delete;

    Elastic_interaction(Elastic_interaction&& old_obj);

    ~Elastic_interaction();
    
    Elastic_interaction& operator = (Elastic_interaction&& rv);

    Elastic_interaction& operator = (const Elastic_interaction&) = delete;

    Matrix cur_force_fir_sphere();

    Matrix cur_force_sec_sphere();

    void get_info(FILE* output_file);

    friend int scan_elastic_inter(FILE* input_file, Elastic_interaction*& arr_elastic_inter, Sphere* array_of_spheres, int num_sphers);

    friend class Physics_engine;
};
//-----------------------------------------------------------

//-----------------------------------------------------------
class Space{

private: 

    Sphere* array_of_spheres;

    Flatness* array_of_flats;
    
    Elastic_interaction* arr_elastic_inter;

    Field* phys_fields;   //массив полей (руками прописываем поля в конструкторе)

    int number_of_spheres;
    int number_of_flats;
    int number_of_elastic_inter;
    int number_of_fields;

public:

    Space(FILE* input_file);        //в файле описаны характеристики сфер, плоскостей и взаимодействий

    Space(const Space&) = delete;

    Space(Space&&) = delete;
    
    ~Space();

    Space& operator = (Space&&) = delete;

    Space& operator = (const Space&) = delete;

    void get_info(FILE* output_file);

    friend class Field;

    friend class Physics_engine;
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Physics_engine{                 //тут было наследование от Space

private:

    Space& cur_space;
    
    int* spheres_that_bumped;

public:

    Physics_engine(Space& space);

    ~Physics_engine();

    void call_phys_engine(float delta_t);

    void call_collision_engine(float delta_t);
    
    void shift_objects(float delta_t);                  //сдвигает все что надо

    Matrix get_force(Sphere& cur_sphere);//просчитывает 

    friend class Space;
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Rendering_engine{  //тут было наследование от Space

private:

    Space& cur_space;

    //открытое окно для отрисовки 

public:

    //в конструктор подается уже открытое окно 
    //функция отрисовки кадра 
    //смена позиции наблюдателя
    //?? пока плохо разобрался с отрисовкой
};
//-----------------------------------------------------------

#endif
