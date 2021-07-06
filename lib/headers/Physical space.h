#ifndef PHYS_SPACE
#define PHYS_SPACE

#include "Objects.h"

typedef struct struct_elastic_inter{

    const Sphere& fir_obj;
    const Sphere& sec_obj;

    float koef_k;
    float normal_distance;

} elastic_interaction;

//-----------------------------------------------------------
class Space{

private: 

    Sphere* array_of_spheres;

    Flatness* array_of_flats;
    
    elastic_interaction* arr_elastic_inter;

    Field* phys_fields;   //массив полей (руками прописываем поля в конструкторе)

    int number_of_spheres;

    int number_of_flats;

    int number_of_param;

    int number_of_fields;

    int number_of_elastic_inter;

public:

    Space(FILE* input_file);        //в файле описаны характеристики сфер, плоскостей и взаимодействий

    Space(const Space&) = delete;

    Space& operator = (Space&&) = delete;

    Space& operator = (const Space&) = delete;

    ~Space();

    friend class Field;

    friend class Physics_engine;
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Physics_engine{                 //тут было наследование от Space

private:

    Space& cur_space;

    float delta_t;

public:

    Physics_engine(Space& space, float delta_time);

    ~Physics_engine();
    
    void calc_offset();                  //сдвигает все что надо

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


//-----------------------------------------------------------
class Collision_engine{

private:

    Space& cur_space;

public:

    void calc_collisions();

    int probability_of_collis(Sphere& fir_sphere, Sphere& sec_sphere); //условие удара помимо расстояния еще и направлнеие вектора скорости
                                                                         //(так можно избежать множественных ударов)
    int probability_of_collis(Sphere& cur_sphere, Flatness& cur_flat);

    void sphere_sphere_collis(Sphere& fir_sphere, Sphere& sec_sphere);

    void flat_sphere_collis(Flatness& cur_flat, Sphere& cur_sphere);
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Field{

public:

    Matrix return_force(Sphere& cur_obj); //возвращает силу
};
//-----------------------------------------------------------

#endif
