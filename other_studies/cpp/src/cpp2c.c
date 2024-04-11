#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf */

/* class PublicTransport */
static int s_count = 0; /* static member of PublicTransport */

typedef struct
{
    void *vptr;
    int m_license_plate;
} PublicTransport;

typedef struct
{
    void (*dtor_ptr)(PublicTransport *const this_);
    void (*display_ptr)(PublicTransport *const this_);
} PublicTransport_v_table;

void PublicTransport_dtor(PublicTransport *const this_) /* implicit virtul because the inheritance */
{
    --s_count;

    printf("PublicTransport::Dtor()%d\n", this_->m_license_plate);
}

void PublicTransport_display(PublicTransport *const this_) /* virtual member function */
{
    printf("PublicTransport::display(): %d\n", this_->m_license_plate);   
}

PublicTransport_v_table pt_vtable = {&PublicTransport_dtor, &PublicTransport_display};

void PublicTransport_ctor(PublicTransport *const this_)
{
    this_->vptr = &pt_vtable; /* implicit */
    this_->m_license_plate = ++s_count;

    printf("PublicTransport::Ctor()%d\n", this_->m_license_plate);
}

void PublicTransport_cctor(PublicTransport *const this_, const PublicTransport *other_)
{
    this_->vptr = &pt_vtable; /* implicit */
    this_->m_license_plate = ++s_count;

    printf("PublicTransport::CCtor() %d\n", this_->m_license_plate);
}

void PublicTransport_print_count(void) /* static member function */
{
    printf("s_count: %d\n", s_count);
}

int PublicTransport_get_ID(PublicTransport *const this_) /* protected member function */
{
    return this_->m_license_plate;
}


/* class Minibus : public PublicTransport */
typedef struct
{
    PublicTransport base;
    int m_numSeats;
} Minibus;

typedef struct
{
    void (*dtor_ptr)(Minibus *const this_);
    void (*display_ptr)(Minibus *const this_);
    void (*wash_ptr)(Minibus *const this_, int minutes_);
} Minibus_v_table;

void Minibus_dtor(Minibus *const this_) /* implicit virtul because the inheritance */
{
    printf("Minibus::Dtor()\n");

    PublicTransport_dtor(&this_->base); /* implicit */
}

void Minibus_display(Minibus *const this_) /* implicit virtul because the inheritance */
{
    printf("Minibus::display() ID:%d", PublicTransport_get_ID(&this_->base));
    printf(" num seats:%d\n", this_->m_numSeats);
}

void Minibus_wash(Minibus *const this_, int minutes_) /* explicit virtual member function */
{
    printf("Minibus::wash(%d) ID:%d\n", minutes_, PublicTransport_get_ID(&this_->base));    
}

Minibus_v_table m_vptr = {&Minibus_dtor, &Minibus_display, &Minibus_wash};

void Minibus_ctor(Minibus *const this_)
{
    PublicTransport_ctor(&this_->base); /* implicit */
    this_->base.vptr = &m_vptr; /* implicit */

    this_->m_numSeats = 20;

    printf("Minibus::Ctor()\n");
}

void Minibus_cctor(Minibus *const this_, const Minibus *other_)
{
    PublicTransport_cctor(&this_->base, &other_->base);
    this_->base.vptr = &m_vptr; /* implicit */

    this_->m_numSeats = other_->m_numSeats;

    printf("Minibus::CCtor()\n");
}


/* class ArmyMinibus : public Minibus */
typedef struct
{
    Minibus base;
} ArmyMinibus;

typedef struct
{
    void (*dtor_ptr)(ArmyMinibus *const this_);
    void (*display_ptr)(Minibus *const this_);
    void (*wash_ptr)(Minibus *const this_, int minutes_);
} ArmyMinibus_v_table;

void ArmyMinibus_dtor(ArmyMinibus *const this_) /* implicit virtul because the inheritance */
{
    printf("ArmyMinibus::Dtor()\n");

    Minibus_dtor(&this_->base); /* implicit */
}

ArmyMinibus_v_table am_vtable = {&ArmyMinibus_dtor, &Minibus_display, &Minibus_wash};

void ArmyMinibus_ctor(ArmyMinibus *const this_)
{
    Minibus_ctor(&this_->base); /* implicit */
    this_->base.base.vptr = &am_vtable; /* implicit */

    printf("ArmyMinibus::Ctor()\n");
}

void ArmyMinibus_cctor(ArmyMinibus *const this_, const ArmyMinibus *other_)
{
    Minibus_cctor(&this_->base, &other_->base);
    this_->base.base.vptr = &am_vtable; /* implicit */

    printf("ArmyMinibus::CCtor()\n");
}


/* class Taxi: public PublicTransport */
typedef struct
{
    PublicTransport base;
} Taxi;

typedef struct
{
    void (*dtor_ptr)(Taxi *const this_);
    void (*display_ptr)(Taxi *const this_);
} Taxi_v_table;

void Taxi_dtor(Taxi *const this_)
{
    printf("Taxi::Dtor()\n");

    PublicTransport_dtor(&this_->base); /* implicit */
}

void Taxi_display(Taxi *const this_) /* implicit virtul because the inheritance */
{
    printf("Taxi::display() ID:%d\n", PublicTransport_get_ID(&this_->base));
}

Taxi_v_table t_vtable = {&Taxi_dtor, &Taxi_display};

void Taxi_ctor(Taxi *const this_) /* implicit virtul because the inheritance */
{
    PublicTransport_ctor(&this_->base); /* implicit */
    this_->base.vptr = &t_vtable; /* implicit */

    printf("Taxi::Ctor()\n");
}

void Taxi_cctor(Taxi *const this_, const Taxi *other_)
{
    PublicTransport_cctor(&this_->base, &other_->base);
    this_->base.vptr = &t_vtable; /* implicit */

    printf("Taxi::CCtor()\n");
}

static int max_funcI(const int *i1, const int *i2) /* generate template func for int */
{
    return ((*i1 > *i2) ? *i1 : *i2);
}


/* class SpecialTaxi : public Taxi */
typedef struct
{
    Taxi base;
} SpecialTaxi;

typedef struct
{
    void (*dtor_ptr)(SpecialTaxi *const this_);
    void (*display_ptr)(SpecialTaxi *const this_);  
} SpecialTaxi_v_table;

void SpecialTaxi_dtor(SpecialTaxi *const this_) /* implicit virtul because the inheritance */
{
    printf("SpecialTaxi::Dtor()\n");

    Taxi_dtor(&this_->base);
}

void SpecialTaxi_display(SpecialTaxi *const this_) /* implicit virtul because the inheritance */
{
    printf("SpecialTaxi::display() ID:%d\n", PublicTransport_get_ID(&this_->base.base));
}

SpecialTaxi_v_table st_vtable = {&SpecialTaxi_dtor, &SpecialTaxi_display};

void SpecialTaxi_ctor(SpecialTaxi *const this_)
{
    Taxi_ctor(&this_->base); /* implicit */
    this_->base.base.vptr = &st_vtable; /* implicit */

    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi_cctor(SpecialTaxi *const this_, const SpecialTaxi *other_)
{
    Taxi_cctor(&this_->base, &other_->base);
    this_->base.base.vptr = &st_vtable; /* implicit */

    printf("SpecialTaxi::CCtor()\n");
}


/* class PublicConvoy : public PublicTransport */
typedef struct
{
    PublicTransport base;
    PublicTransport *m_pt1;
    PublicTransport *m_pt2;
    Minibus m_m;
    Taxi m_t;
} PublicConvoy;

typedef struct
{
    void (*dtor_ptr)(PublicConvoy *const this_);
    void (*display_ptr)(PublicConvoy *const this_); 
} PublicConvoy_v_table;

void PublicConvoy_dtor(PublicConvoy *const this_) /* implicit virtul because the inheritance */
{
    Minibus_dtor((Minibus *)this_->m_pt1);
    free(this_->m_pt1);

    Taxi_dtor((Taxi *)this_->m_pt2);
    free(this_->m_pt2);

    Taxi_dtor(&this_->m_t);
    Minibus_dtor(&this_->m_m);
    PublicTransport_dtor(&this_->base);
}

void PublicConvoy_display(PublicConvoy *const this_) /* implicit virtul because the inheritance */
{
    ((PublicTransport_v_table *)(this_->m_pt1->vptr))->display_ptr(this_->m_pt1);
    ((PublicTransport_v_table *)(this_->m_pt2->vptr))->display_ptr(this_->m_pt2);
    ((Minibus_v_table *)(this_->m_m.base.vptr))->display_ptr(&this_->m_m);
    ((Taxi_v_table *)(this_->m_t.base.vptr))->display_ptr(&this_->m_t);
}

PublicConvoy_v_table pc_vtable = {&PublicConvoy_dtor, &PublicConvoy_display};

void PublicConvoy_ctor(PublicConvoy *const this_)
{
    PublicTransport_ctor(&this_->base); /* implicit */

    this_->m_pt1 = (PublicTransport *)malloc(sizeof(Minibus));
    Minibus_ctor((Minibus *)this_->m_pt1);

    this_->m_pt2 = (PublicTransport *)malloc(sizeof(Taxi));
    Taxi_ctor((Taxi *)this_->m_pt2);

    Minibus_ctor(&this_->m_m); /* implicit */
    Taxi_ctor(&this_->m_t); /* implicit */
  
    this_->base.vptr = &pc_vtable; /* implicit */
}

void PublicConvoy_cctor(PublicConvoy *const this_, const PublicConvoy *other_)
{
    PublicTransport_cctor(&this_->base, &other_->base);

    this_->m_pt1 = (PublicTransport *)malloc(sizeof(Minibus));
    Minibus_cctor((Minibus *)this_->m_pt1, (Minibus *)other_->m_pt1);

    this_->m_pt2 = (PublicTransport *)malloc(sizeof(Taxi));
    Taxi_cctor((Taxi *)this_->m_pt2, (Taxi *)other_->m_pt2);

    Minibus_cctor(&this_->m_m, &other_->m_m);
    Taxi_cctor(&this_->m_t, &other_->m_t);

    this_->base.vptr = &pc_vtable; /* implicit */
}


/* free functions */
void print_infoPT(PublicTransport *a_) /* my TODO: *const ? */
{
    ((PublicTransport_v_table *)(a_->vptr))->display_ptr(a_);
}

void print_infoV(void)
{
    PublicTransport_print_count();
}

void print_infoM(Minibus *m_) /* my TODO: *const ? */
{
    ((Minibus_v_table *)(m_->base.vptr))->wash_ptr(m_, 3);   
}

PublicTransport print_infoI(int i_)
{
    Minibus ret;
    Minibus_ctor(&ret); /* implicit */
    printf("print_info(int i)\n");
    ((Minibus_v_table *)(ret.base.vptr))->display_ptr(&ret);

    PublicTransport ret_copy; /* implicit */
    PublicTransport_cctor(&ret_copy, &ret.base); /* implicit */
    Minibus_dtor(&ret); /* implicit */

    return ret_copy;
}

void taxi_displayFree(Taxi *s_)
{
    ((Taxi_v_table *)(s_->base.vptr))->display_ptr(s_);  
}

int main()
{
    /* Minibus m; */
    Minibus m;
    Minibus_ctor(&m);

    /* print_info(m); */
    print_infoM(&m);

    /* print_info(3).display(); */
    PublicTransport func_ret = print_infoI(3);
    ((PublicTransport_v_table *)(func_ret.vptr))->display_ptr(&func_ret);
    ((PublicTransport_v_table *)(func_ret.vptr))->dtor_ptr(&func_ret);

    /* PublicTransport *array[] = { new Minibus(), new Taxi(), new Minibus() }; */
    PublicTransport *array[] = {malloc(sizeof(Minibus)),
                                malloc(sizeof(Taxi)),
                                malloc(sizeof(Minibus))};
    
    Minibus_ctor((Minibus *)array[0]);
    Taxi_ctor((Taxi *)array[1]);
    Minibus_ctor((Minibus *)array[2]);

    /* for (int i = 0; i < 3; ++i) {
        array[i]->display();
    } */
    ((Minibus_v_table *)(array[0]->vptr))->display_ptr((Minibus *)array[0]);
    ((Taxi_v_table *)(array[1]->vptr))->display_ptr((Taxi *)array[1]);
    ((Minibus_v_table *)(((Minibus *)(array[2]))->base.vptr))->display_ptr((Minibus *)array[2]);

    /* for (int i = 0; i < 3; ++i) {
        delete array[i];
    } */
    ((Minibus_v_table *)(array[0]->vptr))->dtor_ptr((Minibus *)array[0]);
    free(array[0]);

    ((Taxi_v_table *)(array[1]->vptr))->dtor_ptr((Taxi *)array[1]);
    free(array[1]);

    ((Minibus_v_table *)(((Minibus *)(array[2]))->base.vptr))->dtor_ptr((Minibus *)array[2]);
    free(array[2]);

    /* PublicTransport arr2[] = { Minibus(), Taxi(), PublicTransport() }; */
    PublicTransport arr2[3];

    Minibus m_temporary;
    Minibus_ctor(&m_temporary);

    PublicTransport m_temporary_copy;
    PublicTransport_cctor(&m_temporary_copy, &m_temporary.base);

    arr2[0] = m_temporary_copy;

    ((Minibus_v_table *)(m_temporary.base.vptr))->dtor_ptr(&m_temporary); 


    Taxi t_temporary;
    Taxi_ctor(&t_temporary);

    PublicTransport t_temporary_copy;
    PublicTransport_cctor(&t_temporary_copy, &t_temporary.base);

    arr2[1] = t_temporary_copy;

    ((Taxi_v_table *)(t_temporary.base.vptr))->dtor_ptr(&t_temporary); 


    PublicTransport pt_temporary;
    PublicTransport_ctor(&pt_temporary);

    arr2[2] = pt_temporary;

    /* for (int i = 0; i < 3; ++i) {
        arr2[i].display();
    } */
    int i = 0;
    for (i = 0; i < 3; ++i)
    {
        ((PublicTransport_v_table *)(arr2[i].vptr))->display_ptr(arr2 + i);
    }

    /* print_info(arr2[0]); */
    print_infoPT(arr2);

    /* PublicTransport::print_count(); */
    PublicTransport_print_count();

    /* Minibus m2; */
    Minibus m2;
    Minibus_ctor(&m2);

    /* m2.print_count(); */
    PublicTransport_print_count();

    /* Minibus arr3[4]; */
    Minibus arr3[4];

    for (i = 0; i < 4; ++i)
    {
       Minibus_ctor(arr3 + i); 
    }

    /* Taxi *arr4 = new Taxi[4]; */
    Taxi *arr4 = malloc(4*sizeof(Taxi));

    for (i = 0; i < 4; ++i)
    {
        Taxi_ctor(&arr4[i]);
    }

    /* delete[] arr4; */
    for (i = 3; i >= 0; --i)
    {
        ((Taxi_v_table *)(arr4[i].base.vptr))->dtor_ptr(&arr4[i]);
    }
    free(arr4);

    /* std::cout << max_func(1, 2) << "\n"; */
    int i1 = 1;
    int i2 = 2;

    printf("%d\n", max_funcI(&i1, &i2));

    /* std::cout << max_func<int>(1, 2.0f)<< "\n"; */
    i2 = 2.0;
    printf("%d\n", max_funcI(&i1, &i2));

    /* SpecialTaxi st; */
    SpecialTaxi st;
    SpecialTaxi_ctor(&st);

    /* taxi_display(st); */
    Taxi st_copy;

    Taxi_cctor(&st_copy, &st.base);
    taxi_displayFree(&st_copy);
    ((Taxi_v_table *)(t_temporary.base.vptr))->dtor_ptr(&st_copy);     


    /* PublicConvoy *ts1 = new PublicConvoy(); */
    PublicConvoy *ts1 = malloc(sizeof(PublicConvoy));
    PublicConvoy_ctor(ts1);

    /* PublicConvoy *ts2 = new PublicConvoy(*ts1); */
    PublicConvoy *ts2 = malloc(sizeof(PublicConvoy));
    PublicConvoy_cctor(ts2, ts1);

    /* ts1->display(); */
    ((PublicConvoy_v_table *)(ts1->base.vptr))->display_ptr(ts1);

    /* ts2->display(); */
    ((PublicConvoy_v_table *)(ts2->base.vptr))->display_ptr(ts2);

    /* delete ts1; */
    ((PublicConvoy_v_table *)(ts1->base.vptr))->dtor_ptr(ts1); 
    free(ts1);

    /* ts2->display(); */
    ((PublicConvoy_v_table *)(ts2->base.vptr))->display_ptr(ts2);

    /* delete ts2; */
    ((PublicConvoy_v_table *)(ts2->base.vptr))->dtor_ptr(ts2); 
    free(ts2);

    /* ArmyMinibus* army_minibus = new ArmyMinibus; */
    ArmyMinibus *army_minibus = malloc(sizeof(ArmyMinibus));
    ArmyMinibus_ctor(army_minibus);

    /* army_minibus->display(); */
    ((ArmyMinibus_v_table *)(army_minibus->base.base.vptr))->display_ptr(&army_minibus->base);

    /* army_minibus->wash(5); */
    ((ArmyMinibus_v_table *)(army_minibus->base.base.vptr))->wash_ptr(&army_minibus->base, 5);

    /* delete army_minibus; */
    ((ArmyMinibus_v_table *)(army_minibus->base.base.vptr))->dtor_ptr(army_minibus); 
    free(army_minibus);

    /* c translate for implicit dtors */
    ((SpecialTaxi_v_table *)(st.base.base.vptr))->dtor_ptr(&st); 

    for (i = 3; i >= 0; --i)
    {
        ((Minibus_v_table *)(arr3[i].base.vptr))->dtor_ptr(arr3 + i); 
    }

    ((Minibus_v_table *)(m2.base.vptr))->dtor_ptr(&m2); 

    for (i = 2; i >= 0; --i)
    {
        ((PublicTransport_v_table *)(arr2[i].vptr))->dtor_ptr(arr2 + i);
    }

    ((Minibus_v_table *)(m.base.vptr))->dtor_ptr(&m);


    return 0;
}