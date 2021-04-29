#include<iostream>
#include<vector>
#include<fstream>

struct Alumno {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    void print(){
        printCodigo();
        printNombre();
        printApellidos();
        printCarrera();
        std::cout<<std::endl;
    }

    void printCodigo(){
        for(int i = 0; i < 5;i++){
            std::cout<<codigo[i];
        }
    }

    void printNombre(){
        for(int i = 0; i < 11;i++){
            std::cout<<nombre[i];
        }
    }

    void printApellidos(){
        for(int i = 0; i < 20;i++){
            std::cout<<apellidos[i];
        }
    }

    void printCarrera(){
        for(int i = 0; i < 15;i++){
            std::cout<<carrera[i];
        }
    }
};

class FixedRecord {
private:
    std::vector<Alumno> recordVec;
    std::string fileName;

public:
    FixedRecord(std::string nombreArchivo):fileName(nombreArchivo){
        std::ifstream file;
        std::string line;
        Alumno* alumn;
        int i;
        file.open(nombreArchivo);
        while(getline(file,line)){
            alumn = new Alumno;
            i = 0;
            while(i < 5){
                alumn->codigo[i] = line[i];
                i++;
            }
            while(i < 16){
                alumn->nombre[i-5] = line[i];
                i++;
            }
            while(i < 36){
                alumn->apellidos[i-16] = line[i];
                i++;
            }
            while(i <51){
                alumn->carrera[i-36] = line[i];
                i++;
            }
            add(*alumn);
        }
        file.close();
    }

    std::vector<Alumno> load(){
        for(auto it = recordVec.begin(); it != recordVec.end(); it++){
            it->print();
        }
        return recordVec;
    }

    void add(Alumno record){
        recordVec.push_back(record);
    }

    Alumno readRecord(int pos){
        return recordVec[pos];
    }
};

int main() {
    auto FR = FixedRecord("datos1.txt");
    FR.load();
    std::cout<<"\nRegistro [9]:"<<std::endl;
    FR.readRecord(9).print();
    return 0;
}