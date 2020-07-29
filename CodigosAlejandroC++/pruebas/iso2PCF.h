#include <cmath>

//Structura que define un punto 3D
struct Punto{
    float x;
    float y;
    float z;
};

template <typename TDG1, typename TDG2>
TDG1 distancia(TDG2 x, TDG2 y){
    TDG2 min, max, aprox;
    if (x < 0){
        x*=-1;
    }
    if (y < 0){
        y*=-1;
    }
    
    if (x < y){
        min = x;    
        max = y;
    }else{
        min = y;
        max = x;
    }
    
    aprox = (max*1007) + (min*441);
    //condicion para ajuste y mejora de precisión
    if (max < (min << 4)){ //equivalente a decir que si max < 16*min
        aprox -= max*40; // equivalente a aprox - 5*max/128
    }
    //se añade un 512 para un apropiado redondeo de las cifras
    // se divide entre 1024 con >> 5
    return ((aprox+512) >> 10);
}

// clase 
template <class TDG1, class TDG2>
class iso2PCF{
    // Atributos
    private:
        int num_bins;
        int num_puntos;
        TDG1 d_max;
        Punto *data;
        Punto *rand;
    // Métodos
    public:
        //Constructor de la clase
        iso2PCF(int _num_bins, int _num_puntos, float _d_max, Punto *_data, Punto *_rand){
            num_bins = _num_bins;
            num_puntos = _num_puntos;
            d_max = _d_max;
            data = _data;
            rand = _rand;
        }
        //Sobrecarga del constructor
        iso2PCF(Punto *_data, Punto *_rand){
            num_bins = 1;
            num_puntos = 1;
            d_max = 1;
            data = _data;
            rand = _rand;
        }
        //Sobrecarga del constructor
        iso2PCF(){
            num_bins = 1;
            num_puntos = 1;
            d_max = 1;
            data = NULL;
            rand = NULL;
        }
        void setBins(int _num_bins){
            num_bins =  _num_bins;
        }
        void setDMAX(TDG1 _d_max){
            d_max = _d_max;
        }
        void setData(Punto *_data){
            data = _data;
        }
        void setRand(Punto *_rand){
            rand = _rand;
        }
        void setPuntos(int _NPuntos){
            num_puntos = _NPuntos;
        }
        void histogramasPuros(TDG2 *DD, TDG2 *RR){
            int i,j;
            float dd, rr, s = num_bins/num_puntos, aux;
            for (i = 0; i < num_puntos-1; i++)
            {
                for (j = i+1; j < num_puntos; j++)
                {
                    aux = distancia(data[i].x - data[j].x, data[i].y - data[j].y);
                    dd = distancia(aux, data[i].z - data[j].z);
                    aux = distancia(rand[i].x - rand[j].x, rand[i].y - rand[j].y);
                    rr = distancia(aux, rand[i].z - rand[j].z);
                    if (dd < d_max)
                    {
                        DD[int(dd*s)] += 1;
                    }
                    if (rr < d_max)
                    {
                        RR[int(rr*s)] += 1;
                    }
                    
                }
            }
            
        }
        void histogramasMixtos(TDG2 *DR){
            int i,j;
            float dr, s = num_bins/num_puntos, aux;
            for (i = 0; i < num_puntos; i++)
            {
                for (j = 0; j < num_puntos; j++)
                {
                    aux = distancia(data[i].x - data[j].x, data[i].y - data[j].y);
                    dr = distancia(aux, data[i].z - rand[j].z);
                    if (dr < d_max)
                    {
                        DR[int(dr*s)] += 1;
                    }
                }
            }
        }
        ~iso2PCF();
};