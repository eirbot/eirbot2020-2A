#ifndef VECTEUR_2D
#define VECTEUR_2D
/**
 * Un vecteur 2 dimensions est défini par deux flottants.
 */
class Vecteur2D
{
    public:
        Vecteur2D(float x, float y);
        float x();
        float y();
        /** Translate le vecteur par (tx, ty) */
        void translation(float tx, float ty);

    private:
        float vec_x;
        float vec_y;
};


/** Détermine l'angle entre l'axe Ox et le vecteur (point_x, point_y) */
float angle(float point_x, float point_y);

#endif
